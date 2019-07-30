#pragma once

#include "concepts/invocable.h"
#include "concepts/position.h"
#include "data_structures/array_k.h"

namespace elements {

template <Forward_position P, Limit<P> L, Binary_operation Op>
requires
    Mutable<P> and
    Same<Decay<Value_type<P>>, Domain<Op>>
constexpr auto
add_to_counter(P pos, L lim, Op op, Domain<Op> x, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    if (x == zero) return zero;
    while (precedes(pos, lim)) {
        if (load(pos) == zero) {
            store(pos, x);
            return zero;
        }
        store(x, op(load(pos), x));
        store(pos, zero);
        increment(pos);
    }
    return x;
}

template <Binary_operation Op, pointer_diff k = 64>
struct binary_counter
{
    Op op;
    Domain<Op> zero;
    array_k<Domain<Op>, k> data;
    pointer_diff size;

    constexpr
    binary_counter(Op op_, Domain<Op> const& zero_)
        : op(op_)
        , zero(zero_)
        , data()
        , size(Zero<pointer_diff>)
    //[[expects axiom: partially_associative(op)]]
    {}

    constexpr void
    operator()(Domain<Op> const& x)
    //[[expects: size(at(this)) < k]]
    {
        auto carry(add_to_counter(first(data), first(data) + size, op, x, zero));
        if (carry != zero) {
            store(at(first(data) + size), carry);
            increment(size);
        }
    }
};

template <Binary_operation Op, pointer_diff k>
struct value_type_t<binary_counter<Op, k>>
{
    using type = array_k<Domain<Op>, k>;
};

template <Binary_operation Op, pointer_diff k>
struct size_type_t<binary_counter<Op, k>>
{
    using type = pointer_diff;
    static constexpr auto value = k;
};

template <Binary_operation Op, pointer_diff k>
constexpr auto
first(binary_counter<Op, k>& x) -> Position_type<Value_type<binary_counter<Op, k>>>
{
    return first(x.data);
}

template <Binary_operation Op, pointer_diff k>
constexpr auto
first(binary_counter<Op, k> const& x) -> Position_type<Value_type<binary_counter<Op, k>> const>
{
    return first(x.data);
}

template <Binary_operation Op, pointer_diff k>
constexpr auto
limit(binary_counter<Op, k>& x) -> Position_type<Value_type<binary_counter<Op, k>>>
{
    return first(x) + size(x);
}

template <Binary_operation Op, pointer_diff k>
constexpr auto
limit(binary_counter<Op, k> const& x) -> Position_type<Value_type<binary_counter<Op, k>> const>
{
    return first(x) + size(x);
}

template <Binary_operation Op, pointer_diff k>
constexpr auto
is_empty(binary_counter<Op, k> const& x) noexcept -> bool
{
    return x.size != Zero<pointer_diff>;
}

template <Binary_operation Op, pointer_diff k>
constexpr auto
size(binary_counter<Op, k> const& x) noexcept -> Size_type<binary_counter<Op, k>>
{
    return x.size;
}

}
