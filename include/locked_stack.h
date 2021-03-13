#pragma once

#include "intrinsics.h"
#include "result.h"

namespace elements {

struct empty_locked_stack {};

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

template <typename S>
constexpr void
push(locked_stack<S>& s, Value_type<locked_stack<S>> x)
{
    scoped_lock l{s.m};
    emplace(s.stack, mv(x));
}

template <typename S>
constexpr auto
pop(locked_stack<S>& s) -> result<Value_type<locked_stack<S>>, empty_locked_stack>
{
    scoped_lock l{s.m};
    if (is_empty(s.stack)) return fail<Value_type<locked_stack<S>>, empty_locked_stack>({});
    auto res{load(predecessor(limit(s.stack)))};
    pop(s.stack);
    return result<Value_type<locked_stack<S>>, empty_locked_stack>{res};
}

}
