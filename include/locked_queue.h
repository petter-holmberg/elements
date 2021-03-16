#pragma once

#include "intrinsics.h"
#include "swap.h"

namespace elements {

template <typename Q>
struct locked_queue
{
    Q queue;
    mutex m;
    condition_variable cv;

    locked_queue() = default;

    locked_queue(locked_queue&& x)
    {
        scoped_lock l{x.m};
        queue = mv(x.queue);
        x.queue = {};
    }

    locked_queue& operator=(locked_queue&& x)
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            scoped_lock<mutex> l{m, x.m};
            erase_all(queue);
            swap(queue, x.queue);
        }
        return at(this);
    }
};

template <typename Q>
struct value_type_t<locked_queue<Q>>
{
    using type = Value_type<Q>;
};

template <typename Q>
struct size_type_t<locked_queue<Q>>
{
    using type = Size_type<Q>;
};

template <typename Q>
constexpr auto
try_push(locked_queue<Q>& q, Value_type<Q> const& x) -> bool
{
    {
        unique_lock l{q.m, try_lock};
        if (!l) return false;
        emplace(q.queue, x);
    }
    notify_one(x.cv);
    return true;
}

template <typename Q>
constexpr auto
try_push(locked_queue<Q>& q, Value_type<Q>&& x) -> bool
{
    {
        unique_lock l{q.m, try_lock};
        if (!l) return false;
        emplace(q.queue, fw<Value_type<Q>>(x));
    }
    notify_one(x.cv);
    return true;
}

template <typename Q>
constexpr void
push(locked_queue<Q>& q, Value_type<Q>&& x)
{
    {
        scoped_lock l{q.m};
        emplace(q.queue, fw<Value_type<Q>>(x));
    }
    notify_one(q.cv);
}

template <typename Q>
constexpr void
push(locked_queue<Q>& q, Value_type<Q> const& x)
{
    {
        scoped_lock l{q.m};
        emplace(q.queue, x);
    }
    notify_one(q.cv);
}

template <typename Q>
constexpr auto
try_pop(locked_queue<Q>& q, Value_type<Q>& x) -> bool
{
    unique_lock l{q.m, try_lock};
    if (!l or is_empty(q.queue)) return false;
    x = mv(at(first(q.queue)));
    pop_first(q.queue);
    return true;
}

template <typename Q>
constexpr void
pop(locked_queue<Q>& q, Value_type<Q>& x)
{
    unique_lock l{q.m};
    wait(q.cv, l, [&q](){ return !is_empty(q.queue); });
    x = mv(at(first(q.queue)));
    pop_first(q.queue);
}

}
