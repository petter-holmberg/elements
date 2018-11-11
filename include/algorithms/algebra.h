#pragma once

#include "concepts/algebra.h"
#include "type_functions/algebra.h"

namespace elements {

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
struct identity_element_t<A, sum<A>>
{
    static constexpr A value = zero<A>;
};

template <Additive_monoid A>
struct inverse_operation<A, sum<A>>
{
    constexpr auto
    operator()(A const& a) const -> A
    {
        return -a;
    }
};

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
struct identity_element_t<M, product<M>>
{
    static constexpr M value = one<M>;
};

template <Multiplicative_monoid M>
struct reciprocal
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return one<M> / a;
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

}
