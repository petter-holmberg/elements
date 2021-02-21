#pragma once

#include "algebra.h"

namespace elements {

template <Integral I>
constexpr void
increment(I& x)
{
    ++x;
}

template <Integral I>
constexpr void
decrement(I& x)
{
    --x;
}

template <Integral I>
constexpr auto
twice(I const& a) -> I
{
    return a + a;
}

template <Integral I>
constexpr auto
half(I const& a) -> I
{
    return a >> I{1};
}

template <Integral I>
constexpr auto
binary_scale_up(I const& a, I const& k) -> I
{
    return a << k;
}

template <Integral I>
constexpr auto
binary_scale_down(I const& a, I const& k) -> I
{
    return a >> k;
}

template <Integral I>
constexpr auto
is_positive(I const& a) -> bool
{
    return I{0} < a;
}

template <Integral I>
constexpr auto
is_negative(I const& a) -> bool
{
    return a < I{0};
}

template <Integral I>
constexpr auto
is_zero(I const& a) -> bool
{
    return a == Zero<I>;
}

template <Integral I>
constexpr auto
is_one(I const& a) -> bool
{
    return a == I{1};
}

template <Integral I>
constexpr auto
is_even(I const& a) -> bool
{
    return (a bitand I{1}) == I{0};
}

template <Integral I>
constexpr auto
is_odd(I const& a) -> bool
{
    return (a bitand I{1}) != I{0};
}

template <Integral I>
constexpr auto
precedes(I const& i0, I const& i1) -> bool
{
    return i0 < i1;
}

template <typename, typename...>
struct distance_type_t;

template <typename T, typename... Us>
using Distance_type = typename distance_type_t<T, Us...>::type;

template <Integral I>
struct distance_type_t<I>
{
    using type = Unsigned_type<I>;
};

template <typename I>
concept Integer =
    requires (I a) {
        increment(a);
        decrement(a);
        { twice(a) } -> Same_as<I>;
        { half(a) } -> Same_as<I>;
        { binary_scale_up(a, a) } -> Same_as<I>;
        { binary_scale_down(a, a) } -> Same_as<I>;
        { is_positive(a) } -> Boolean_testable;
        { is_negative(a) } -> Boolean_testable;
        { is_zero(a) } -> Boolean_testable;
        { is_one(a) } -> Boolean_testable;
        { is_even(a) } -> Boolean_testable;
        { is_odd(a) } -> Boolean_testable;
    };

template <Integer I, typename T, Operation<T, T> Op>
constexpr auto
power_left_associated(T a, I n, Op op) -> T
//[[expects: is_positive(n)]]
{
    if (n == One<I>) return a;
    return invoke(op, power_left_associated(a, n - One<I>, op), a);
}

template <Integer I, typename T, Operation<T, T> Op>
constexpr auto
power_right_associated(T a, I n, Op op) -> T
//[[expects: is_positive(n)]]
{
    if (n == One<I>) return a;
    return invoke(op, a, power_right_associated(a, n - One<I>, op));
}

template <Integer I, typename Op, Semigroup<Op> S>
constexpr auto
power_accumulate_semigroup(S r, S a, I n, Op op) -> S
//[[expects: axiom_Semigroup(a, a, a, op) and is_positive(n)]]
{
    while (true) {
        if (is_odd(n)) {
            r = invoke(op, r, a);
            if (is_one(n)) return r;
        }
        a = invoke(op, a, a);
        n = half(n);
    }
}

template <Integer I, typename Op, Semigroup<Op> S>
constexpr auto
power_semigroup(S a, I n, Op op) -> S
//[[expects: axiom_Semigroup(a, a, a, op) and is_positive(n)]]
{
    while (is_even(n)) {
        a = invoke(op, a, a);
        n = half(n);
    }
    if (is_one(n)) return a;
    return power_accumulate_semigroup(a, invoke(op, a, a), half(n - One<I>), op);
}

template <Integer I, typename Op, Monoid<Op> M>
constexpr auto
power_monoid(M a, I n, Op op) -> M
//[[expects: axiom_Monoid(a, a, a, op) and !is_negative(n)]]
{
    if (is_zero(n)) return Identity_element<M, Op>;
    return power_semigroup(a, n, op);
}

template <Integer I, typename Op, Group<Op> G>
constexpr auto
power_group(G a, I n, Op op) -> G
//[[expects: axiom_Group(a, a, a, op)]]
{
    if (is_negative(n)) {
        n = negative{}(n);
        a = inverse_operation<G, Op>{}(a);
    }
    return power_monoid(a, n, op);
}

}
