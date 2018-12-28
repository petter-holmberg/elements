#pragma once

#include "concepts/algebra.h"
#include "type_functions/algebra.h"

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
    static constexpr A value = Zero<A>;
};

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
    static constexpr M value = One<M>;
};

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

}
