#pragma once

#include "concepts/regular.h"

namespace elements {

template <Unary_predicate P>
struct negation
{
    P pred;

    constexpr
    negation(P pred_) : pred{pred_} {}

    constexpr auto
    operator()(Domain<P> const& x) -> bool
    {
        return !pred(x);
    }
};

template <Relation R>
struct identity_relation
{
    R rel;

    explicit constexpr
    identity_relation(R r) : rel{r} {}

    constexpr auto
    operator()(Domain<R> const& x, Domain<R> const& y) -> bool
    {
        return rel(x, y);
    }
};

template <Relation R>
struct complement
{
    R rel;

    explicit constexpr
    complement(R r) : rel{r} {}

    constexpr auto
    operator()(Domain<R> const& x, Domain<R> const& y) -> bool
    {
        return !rel(x, y);
    }
};

template <Relation R>
struct converse
{
    R rel;

    explicit constexpr
    converse(R r) : rel{r} {}

    constexpr auto
    operator()(Domain<R> const& x, Domain<R> const& y) -> bool
    {
        return rel(y, x);
    }
};

template <Relation R>
struct complement_of_converse
{
    R rel;

    explicit constexpr
    complement_of_converse(R r) : rel{r} {}

    constexpr auto
    operator()(Domain<R> const& x, Domain<R> const& y) -> bool
    {
        return !rel(y, x);
    }
};

}
