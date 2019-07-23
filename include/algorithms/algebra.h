#pragma once

#include "concepts/algebra.h"
#include "position.h"
#include "map.h"
#include "reduce.h"
#include "swap.h"

namespace elements {

// Additive semigroups

template <Additive_semigroup A>
struct sum
{
    constexpr auto
    operator()(A const& a, A const& b) const -> A
    {
        return a + b;
    }
};

template <Additive_semigroup A>
constexpr auto
operator+=(A& a, A const& b) -> A&
{
    a = a + b;
    return a;
}

// Multiplicative semigroups

template <Multiplicative_semigroup M>
struct product
{
    constexpr auto
    operator()(M const& a, M const& b) const -> M
    {
        return a * b;
    }
};

template <Multiplicative_semigroup M>
constexpr auto
operator*=(M& a, M const& b) -> M&
{
    a = a * b;
    return a;
}

// Additive monoids

template <Additive_semigroup A>
struct identity_element_t<A, sum<A>>
{
    static A const value;
};

template <Additive_semigroup A>
A const identity_element_t<A, sum<A>>::value = Zero<A>;

template <Additive_monoid A>
constexpr auto
operator+(A& a) -> A&
{
    return a;
}

template <Additive_monoid A>
struct positive
{
    constexpr auto
    operator()(A const& a) const -> A
    {
        return +a;
    }
};

template <Additive_monoid A>
struct negative
{
    constexpr auto
    operator()(A const& a) const -> A
    {
        return -a;
    }
};

template <Additive_monoid A>
struct inverse_operation<A, sum<A>>
{
    constexpr auto
    operator()(A const& a) const -> A
    {
        return negative<A>()(a);
    }
};

// Multiplicative monoids

template <Multiplicative_semigroup M>
struct identity_element_t<M, product<M>>
{
    static M const value;
};

template <Multiplicative_semigroup M>
M const identity_element_t<M, product<M>>::value = One<M>;

template <Multiplicative_monoid M>
struct reciprocal
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return One<M> / a;
    }
};

template <Multiplicative_monoid M>
struct inverse_operation<M, product<M>>
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return reciprocal<M>()(a);
    }
};

// Additive groups

template <Additive_group A>
struct difference
{
    constexpr auto
    operator()(A const& a, A const& b) const -> A
    {
        return a - b;
    }
};

template <Additive_group T>
constexpr auto
operator-=(T& x, T const& y) -> T&
{
    x = x - y;
    return x;
}

// Multiplicative groups

template <Multiplicative_group M>
struct quotient
{
    constexpr auto
    operator()(M const& a, M const& b) const -> M
    {
        return a / b;
    }
};

template <typename T>
constexpr auto
operator/=(T& x, T const& y) -> T&
{
    x = x / y;
    return x;
}

// Semirings

template <
    typename S0,
    Limit<S0> L0,
    typename S1,
    typename D,
    typename S_add_op,
    typename S_mul_op>
requires
    Loadable<S0> and Position<S0> and
    Loadable<S0> and Position<S1> and
    Storable<D> and Forward_position<D> and
    Semiring<Decay<Value_type<S0>>, S_add_op, S_mul_op> and
    Semiring<Decay<Value_type<S1>>, S_add_op, S_mul_op> and
    Semiring<Decay<Value_type<D>>, S_add_op, S_mul_op>
constexpr auto
inner_product(S0 src0, L0 lim0, S1 src1, D dst, S_add_op add_op, S_mul_op mul_op) -> Value_type<D>
{
    return reduce(
        dst,
        map(mv(src0), mv(lim0), mv(src1), dst, mul_op),
        add_op,
        Zero<Value_type<D>>);
}

// Linear algebra

template <typename V, typename S = Value_type<V>>
requires Right_semimodule<V, S>
constexpr auto
operator*=(V& v, S const& a) -> V&
{
    v = v * a;
    return v;
}

template <typename V, typename S = Value_type<V>>
requires Vector_space<V, S>
constexpr auto
operator-(V const& v, V const& w) -> V
{
    return v + (-w);
}

template <typename V, typename S = Value_type<V>>
requires Vector_space<V>
constexpr auto
operator/(V const& v, Value_type<V> const& a) -> V
{
    return (One<Value_type<V>> / a) * v;
}

template <typename V, typename S = Value_type<V>>
requires Vector_space<V>
constexpr auto
operator/=(V const& v, Value_type<V> const& a) -> V
{
    v = v / a;
    return v;
}

template <typename P, typename V = Difference_type<P>, typename S = Value_type<V>>
requires Affine_space<P>
constexpr auto
operator+=(P& p, V const& v) -> P&
{
    p = p + v;
    return p;
}

template <typename P, typename V = Difference_type<P>, typename S = Value_type<V>>
requires Affine_space<P>
constexpr auto
operator-=(P& p, V const& v) -> P&
{
    p = p - v;
    return p;
}

// Combinatorics

template <Binary_integer I>
constexpr auto
choose(I n, I k) -> I
{
    if (n < k) return Zero<I>;
    n = n - k;
    if (n < k) swap(n, k);
    I r{One<I>};
    I i{Zero<I>};
    while (i < k) {
        increment(i);
        increment(n);
        r = r * n;
        r = r / i;
    }
    return r;
}

}
