#pragma once

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include "algorithms/for_each.h"
#include "algorithms/position.h"
#include "algorithms/regular.h"
#include "algorithms/swap.h"
#include "data_structures/array.h"
#include "data_structures/array_double_ended.h"
#include "data_structures/pair.h"

namespace elements {

struct notification_queue
{
    bool is_done = false;
    std::mutex mutex{};
    std::condition_variable ready{};
    array_double_ended<std::function<void()>> queue{};

    using lock_type = std::unique_lock<std::mutex>;

    template <Invocable T>
    bool try_push(T&& task)
    {
        {
            lock_type lock{mutex, std::try_to_lock};
            if (!lock) return false;
            emplace(queue, fw<T>(task));
        }
        ready.notify_one();
        return true;
    }

    template <Invocable T>
    void push(T&& task)
    {
        {
            lock_type lock{mutex};
            emplace(queue, fw<T>(task));
        }
        ready.notify_one();
    }

    bool try_pop(std::function<void()>& task)
    {
        lock_type lock{mutex, std::try_to_lock};
        if (!lock or is_empty(queue)) return false;
        task = mv(at(first(queue)));
        pop_first(queue);
        return true;
    }

    bool pop(std::function<void()>& task)
    {
        lock_type lock{mutex};
        while (is_empty(queue) and !is_done) ready.wait(lock);
        if (is_empty(queue)) return false;
        task = mv(at(first(queue)));
        pop_first(queue);
        return true;
    }

    void done()
    {
        {
            lock_type lock{mutex};
            is_done = true;
        }
        ready.notify_all();
    }
};

struct task_system
{
    const uint32_t n_cores;
    uint32_t n_task_stealing_attempts;
    array<std::thread> threads{};
    std::vector<notification_queue> queues{n_cores};
    std::atomic<uint32_t> atomic_index = 0;

    explicit task_system(
        uint32_t n_queues = std::thread::hardware_concurrency(),
        uint32_t n_task_stealing_attempts_ = 64
    )
        : n_cores{n_queues}
        , n_task_stealing_attempts{n_task_stealing_attempts_}
    {
        reserve(threads, n_cores);
        for (uint32_t n = 0; n != n_cores; ++n) {
            emplace(threads, [&, n]{ run(n); });
        }
    }

    ~task_system()
    {
        auto done = [](notification_queue& queue){ queue.done(); };
        for_each(std::begin(queues), std::end(queues), done);
        auto join = [](std::thread& thread){ thread.join(); };
        for_each(first(threads), limit(threads), join);
    }

    void run(uint32_t i)
    {
        while (true) {
            std::function<void()> task;
            for (uint32_t n = 0; n != n_cores * n_task_stealing_attempts; ++n) {
                if (queues[(i + n) % n_cores].try_pop(task)) break;
            }
            if (!task and !queues[i].pop(task)) break;
            task();
        }
    }

    template <Invocable T>
    void async(T&& task)
    {
        auto i = atomic_index++;
        for (uint32_t n = 0; n != n_cores * n_task_stealing_attempts; ++n) {
            if (queues[(i + n) % n_cores].try_push(fw<T>(task))) return;
        }
        queues[i % n_cores].push(fw<T>(task));
    }
};

/**************************************************************************************************/

//TODO

static task_system global_task_system;

template <typename R>
struct shared_base
{
    std::vector<R> _r{}; // optional
    std::mutex mutex{};
    std::condition_variable _ready{};
    array<std::function<void()>> _then{};

    using lock_type = std::unique_lock<std::mutex>;

    virtual ~shared_base()
    {}

    void set(R&& r)
    {
        array<std::function<void()>> then;
        {
            lock_type lock{mutex};
            _r.push_back(mv(r));
            swap(_then, then);
        }
        _ready.notify_all();
        auto pos = first(_then);
        while (precedes(pos, limit(_then))) {
            global_task_system.async(mv(at(pos)));
            increment(pos);
        }
    }

    template <Invocable Fun>
    void fmap(Fun&& fun)
    {
        bool resolved{false};
        {
            lock_type lock{mutex};
            if (_r.empty()) push(_then, fw<Fun>(fun));
            else resolved = true;
        }
        if (resolved) global_task_system.async(mv(fun));
    }

    auto get() -> R const&
    {
        lock_type lock{mutex};
        while (_r.empty()) _ready.wait(lock);
        return _r.back();
    }
};

template <typename>
struct shared;

template <typename Res, typename... Args>
struct shared<Res(Args...)> : shared_base<Res>
{
    std::function<Res(Args...)> fun;

    template <typename F>
    shared(F&& fun_)
        : fun(fw<F>(fun_))
    {}

    template <typename... Args_>
    void operator()(Args_&&... args)
    {
        this->set(fun(fw<Args_>(args)...));
        fun = nullptr;
    }
};

template <typename>
struct packaged_task;

template <typename>
struct future;

template <Invocable Task, Invocable Fun>
auto package(Fun&& fun) -> pair<packaged_task<Task>, future<Codomain<Task>>>;

template <typename Res>
struct future
{
    std::shared_ptr<shared_base<Res>> header;

    explicit future(std::shared_ptr<shared_base<Res>> pos)
        : header(mv(pos))
    {}

    future() = default;

    template <Invocable Fun>
    auto fmap(Fun&& fun)
    {
        auto pack = package<Return_type<Fun(Res)>()>([header_ = header, fun_ = fw<Fun>(fun)](){
            return fun_(header_->_r.back());
        });
        header->fmap(mv(pack.first));
        return get<1>(pack);
    }

    auto get() const -> Res const&
    {
        return header->get();
    }
};

template <typename Fun, typename... Args>
struct packaged_task<Fun(Args...)>
{
    std::weak_ptr<shared<Fun(Args...)>> wp;

    explicit packaged_task(std::weak_ptr<shared<Fun(Args...)>> wp_)
        : wp(mv(wp_))
    {}

    packaged_task() = default;

    template <typename... Args_>
    void operator()(Args_&&... args) const
    {
        auto sp = wp.lock();
        if (sp) (*sp)(fw<Args_>(args)...);
    }
};

template <Invocable Task, Invocable Fun>
auto package(Fun&& fun) -> pair<packaged_task<Task>, future<Codomain<Task>>>
{
    auto pack = std::make_shared<shared<Task>>(fw<Fun>(fun));
    return {packaged_task<Task>(pack), future<Codomain<Task>>(pack)};
}

template <typename Fun, typename... Args>
auto async(Fun&& fun, Args&&... args)
{
    auto pack = package<Return_type<Fun, Args...>()>(std::bind(fw<Fun>(fun), fw<Args>(args)...));
    global_task_system.async(mv(get<0>(pack)));
    return get<1>(pack);
}

}
