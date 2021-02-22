#pragma once

#include "array_k.h"
#include "integer.h"

namespace elements {

template <Forward_cursor C, Limit<C> L, Operation<Value_type<C>, Value_type<C>> Op>
requires Mutable<C>
constexpr auto
add_to_counter(C cur, L lim, Op op, Value_type<C> x, Value_type<C> const& zero) -> Value_type<C>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    if (x == zero) return zero;
    while (precedes(cur, lim)) {
        if (load(cur) == zero) {
            store(cur, x);
            return zero;
        }
        store(x, op(load(cur), x));
        store(cur, zero);
        increment(cur);
    }
    return x;
}

template <Semiregular T, Operation<T, T> Op, pointer_diff k = 64>
struct binary_counter
{
    Op op;
    T zero;
    array_k<T, k> data;
    pointer_diff size;

    constexpr
    binary_counter(Op op_, T const& zero_)
        : op(op_)
        , zero(zero_)
        , data()
        , size(Zero<pointer_diff>)
    //[[expects axiom: partially_associative(op)]]
    {}

    constexpr void
    operator()(T const& x)
    //[[expects: size(at(this)) < k]]
    {
        auto carry(add_to_counter(first(data), first(data) + size, op, x, zero));
        if (carry != zero) {
            store(at(first(data) + size), carry);
            increment(size);
        }
    }
};

template <Semiregular T, Operation<T, T> Op, pointer_diff k>
struct value_type_t<binary_counter<T, Op, k>>
{
    using type = array_k<T, k>;
};

template <Semiregular T, Operation<T, T> Op, pointer_diff k>
struct size_type_t<binary_counter<T, Op, k>>
{
    using type = pointer_diff;
    static constexpr auto value = k;
};

template <Semiregular T, Operation<T, T> Op, pointer_diff k>
constexpr auto
first(binary_counter<T, Op, k>& x) -> Cursor_type<Value_type<binary_counter<T, Op, k>>>
{
    return first(x.data);
}

template <Semiregular T, Operation<T, T> Op, pointer_diff k>
constexpr auto
first(binary_counter<T, Op, k> const& x) -> Cursor_type<Value_type<binary_counter<T, Op, k>> const>
{
    return first(x.data);
}

template <Semiregular T, Operation<T, T> Op, pointer_diff k>
constexpr auto
limit(binary_counter<T, Op, k>& x) -> Cursor_type<Value_type<binary_counter<T, Op, k>>>
{
    return first(x) + size(x);
}

template <Semiregular T, Operation<T, T> Op, pointer_diff k>
constexpr auto
limit(binary_counter<T, Op, k> const& x) -> Cursor_type<Value_type<binary_counter<T, Op, k>> const>
{
    return first(x) + size(x);
}

template <Semiregular T, Operation<T, T> Op, pointer_diff k>
constexpr auto
is_empty(binary_counter<T, Op, k> const& x) noexcept -> bool
{
    return x.size != Zero<pointer_diff>;
}

template <Semiregular T, Operation<T, T> Op, pointer_diff k>
constexpr auto
size(binary_counter<T, Op, k> const& x) noexcept -> Size_type<binary_counter<T, Op, k>>
{
    return x.size;
}

}
