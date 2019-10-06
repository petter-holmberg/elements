#pragma once

#include "algebra.h"
#include "ordering.h"

namespace elements {

template <typename S>
concept Ordered_additive_semigroup =
    Additive_semigroup<S> and
    Totally_ordered<S>;
    // axiom natural_total_order {
    //     a < b => a + c < b + c;
    // }

template <typename M>
concept Ordered_additive_monoid =
    Ordered_additive_semigroup<M> and
    Additive_monoid<M>;

template <typename G>
concept Ordered_additive_group =
    Ordered_additive_semigroup<G> and
    Additive_monoid<G>;

template <Ordered_additive_group G>
constexpr auto
abs(G const& a) -> G
{
    if (a < G{0}) return -a;
    else return a;
}

template <typename R>
concept Ordered_ring =
    Ring<R> and
    Totally_ordered<R>;

template <typename I>
concept Ordered_integral_domain =
    Integral_domain<I> and
    Totally_ordered<I>;

}
