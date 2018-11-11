#pragma once

#include "algebra.h"
#include "ordering.h"

namespace elements {

// Ordered algebraic structures

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

}
