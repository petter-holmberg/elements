#pragma once

#include "intrinsics.h"
#include "swap.h"

namespace elements {

template <typename S>
struct locked_stack
{
    S stack;
    mutex m;

    locked_stack() = default;

    locked_stack(locked_stack&& x)
    {
        scoped_lock l{x.m};
        stack = mv(x.stack);
        x.stack = {};
    }

    locked_stack& operator=(locked_stack&& x)
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            scoped_lock<decltype(m)> l{m, x.m};
            erase_all(stack);
            swap(stack, x.stack);
        }
        return at(this);
    }
};

template <typename S>
struct value_type_t<locked_stack<S>>
{
    using type = Value_type<S>;
};

template <typename S>
struct size_type_t<locked_stack<S>>
{
    using type = Size_type<S>;
};

enum stack_error
{
    stack_is_locked,
    stack_is_empty
};

template <typename S>
constexpr auto
try_push(locked_stack<S>& s, Value_type<S> const& x) -> bool
{
    unique_lock l{s.m, try_lock};
    if (!l) return false;
    emplace(s.stack, x);
    return true;
}

template <typename S>
constexpr auto
try_push(locked_stack<S>& s, Value_type<S>&& x) -> bool
{
    unique_lock l{s.m, try_lock};
    if (!l) return false;
    emplace(s.stack, fw<Value_type<S>>(x));
    return true;
}

template <typename S>
constexpr void
push(locked_stack<S>& s, Value_type<S> const& x)
{
    scoped_lock l{s.m};
    emplace(s.stack, mv(x));
}

template <typename S>
constexpr void
push(locked_stack<S>& s, Value_type<S>&& x)
{
    scoped_lock l{s.m};
    emplace(s.stack, fw<Value_type<S>>(x));
}

template <typename S>
constexpr auto
try_pop(locked_stack<S>& s, Value_type<S>& x) -> bool
{
    unique_lock l{s.m, try_lock};
    if (!l or is_empty(s.stack)) return false;
    x = mv(at(predecessor(limit(s.stack))));
    pop(s.stack);
    return true;
}

}
