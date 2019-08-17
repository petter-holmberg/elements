#pragma once

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include "algorithms/for_each.h"
#include "algorithms/position.h"

#define Function typename

namespace elements {

struct notification_queue
{
    bool is_done{false};
    std::mutex mutex{};
    std::condition_variable ready{};
    std::deque<std::function<void()>> queue{};

    using lock_type = std::unique_lock<std::mutex>;

    template <typename F>
    bool try_push(F&& task)
    {
        {
            lock_type lock{mutex, std::try_to_lock};
            if (!lock) return false;
            queue.emplace_back(std::forward<F>(task));
        }
        ready.notify_one();
        return true;
    }

    template <typename F>
    void push(F&& task)
    {
        {
            lock_type lock{mutex};
            queue.emplace_back(std::forward<F>(task));
        }
        ready.notify_one();
    }

    bool try_pop(std::function<void()>& task)
    {
        lock_type lock{mutex, std::try_to_lock};
        if (!lock or queue.empty()) return false;
        task = std::move(queue.front());
        queue.pop_front();
        return true;
    }

    bool pop(std::function<void()>& task)
    {
        lock_type lock{mutex};
        while (queue.empty() and !is_done) ready.wait(lock);
        if (queue.empty()) return false;
        task = std::move(queue.front());
        queue.pop_front();
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
    std::atomic<uint32_t> atomic_index{0};

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

    explicit task_system(
        uint32_t n_queues = std::thread::hardware_concurrency(),
        uint32_t n_task_stealing_attempts_ = 64
    )
        : n_cores{n_queues}
        , n_task_stealing_attempts{n_task_stealing_attempts_}
    {
        reserve(threads, n_cores);
        uint32_t n = 0;
        while (n != n_cores) {
            push(threads, [&, n]{ run(n); });
            increment(n);
        }
    }

    ~task_system()
    {
        auto done = [](notification_queue& queue){ queue.done(); };
        for_each(std::begin(queues), std::end(queues), done);
        auto join = [](std::thread& thread){ thread.join(); };
        for_each(first(threads), limit(threads), join);
    }

    template <typename F>
    void async(F&& task)
    {
        auto i = atomic_index++;
        for (uint32_t n = 0; n != n_cores * n_task_stealing_attempts; ++n) {
            if (queues[(i + n) % n_cores].try_push(std::forward<F>(task))) return;
        }

        queues[i % n_cores].push(std::forward<F>(task));
    }
};

/**************************************************************************************************/

//TODO

static task_system _system;

template <typename R>
struct shared_base {
    std::vector<R> _r{}; // optional
    std::mutex _mutex{};
    std::condition_variable _ready{};
    std::vector<std::function<void()>> _then{};

    using lock_t = std::unique_lock<std::mutex>;

    virtual ~shared_base() { }

    void set(R&& r) {
        std::vector<std::function<void()>> then;
        {
            lock_t lock{_mutex};
            _r.push_back(std::move(r));
            std::swap(_then, then);
        }
        _ready.notify_all();
        for (const auto& f : then) _system.async(std::move(f));
    }

    template <typename F>
    void then(F&& f) {
        bool resolved{false};
        {
            lock_t lock{_mutex};
            if (_r.empty()) _then.push_back(std::forward<F>(f));
            else resolved = true;
        }
        if (resolved) _system.async(std::move(f));
    }

    const R& get() {
        lock_t lock{_mutex};
        while (_r.empty()) _ready.wait(lock);
        return _r.back();
    }
};

template <typename> struct shared; // not defined

template <typename R, typename... Args>
struct shared<R(Args...)> : shared_base<R> {
    std::function<R(Args...)> _f;

    template<typename F>
    shared(F&& f) : _f(std::forward<F>(f)) { }

    template <typename... A>
    void operator()(A&&... args) {
        this->set(_f(std::forward<A>(args)...));
        _f = nullptr;
    }
};

template <typename> class packaged_task; //not defined
template <typename> class future;

template <typename S, typename F>
auto package(F&& f) -> std::pair<packaged_task<S>, future<Codomain<S>>>;

template <typename R>
class future {
    std::shared_ptr<shared_base<R>> _p;

    template <typename S, typename F>
    friend auto package(F&& f) -> std::pair<packaged_task<S>, future<Codomain<S>>>;

    explicit future(std::shared_ptr<shared_base<R>> p) : _p(std::move(p)) { }
 public:
    future() = default;

    template <typename F>
    auto then(F&& f) {
        auto pack = package<std::result_of_t<F(R)>()>([p = _p, f = std::forward<F>(f)](){
            return f(p->_r.back());
        });
        _p->then(std::move(pack.first));
        return pack.second;
    }

    const R& get() const { return _p->get(); }
};

template<typename R, typename ...Args >
class packaged_task<R (Args...)> {
    std::weak_ptr<shared<R(Args...)>> _p;

    template <typename S, typename F>
    friend auto package(F&& f) -> std::pair<packaged_task<S>, future<Codomain<S>>>;

    explicit packaged_task(std::weak_ptr<shared<R(Args...)>> p) : _p(move(p)) { }

 public:
    packaged_task() = default;

    template <typename... A>
    void operator()(A&&... args) const {
        auto p = _p.lock();
        if (p) (*p)(std::forward<A>(args)...);
    }
};

template <typename S, typename F>
auto package(F&& f) -> std::pair<packaged_task<S>, future<Codomain<S>>> {
    auto p = std::make_shared<shared<S>>(std::forward<F>(f));
    return std::make_pair(packaged_task<S>(p), future<Codomain<S>>(p));
}

template <typename F, typename ...Args>
auto async(F&& f, Args&&... args)
{
    using result_type = std::result_of_t<F (Args...)>;
//    using packaged_type = packaged_task<result_type()>;

    auto pack = package<result_type()>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    _system.async(std::move(std::get<0>(pack)));
    return std::get<1>(pack);
}

}
