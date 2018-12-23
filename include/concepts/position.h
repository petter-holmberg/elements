#pragma once

#include "regular.h"

namespace elements {

// Access

template <typename L>
concept Loadable =
    Totally_ordered<L> and
    requires (L const& x) {
        { load(x) } -> Value_type<L>;
    };

template <typename S>
concept Storable =
    Regular<S> and
    requires (S& x, Value_type<S>&& v) {
        store(x, v);
    };

template <typename M>
concept Mutable =
    Loadable<M> and
    Storable<M> and
    requires (M& x) {
        { at(x) } -> Value_type<M>&;
    };

// Linear traversal

template <typename P>
concept Position =
    Totally_ordered<P> and
    requires (P& x) {
        increment(x);
    };

template <typename P>
concept Forward_position =
    Position<P> and
    Is_forward_position<P>;

template <typename P>
concept Indexed_position =
    Forward_position<P> and
    requires (P x, Distance_type<P> n) {
        { x + n } -> P;
        { x - x } -> Distance_type<P>;
    };

template <typename P>
concept Bidirectional_position =
    Forward_position<P> and
    requires (P& x) {
        decrement(x);
    };

template <typename P>
concept Random_access_position =
    Indexed_position<P> and
    Bidirectional_position<P> and
    Affine_space<P> and
    Totally_ordered<P>;

template <typename P>
concept Contiguous_position =
    Random_access_position<P> and
    Is_contiguously_addressable<P>;

// Ranges

template <typename P, typename L>
concept Limit =
    Position<P> and
    Weakly_equality_comparable<P, L>;

template <typename R>
concept Range =
    requires (R&& x) {
        { first(x) } -> Position_type<R>;
        limit(x);
    };

template <typename R>
concept Loadable_range =
    Range<R> and
    Loadable<Position_type<R>>;

template <typename R>
concept Storable_range =
    Range<R> and
    Storable<Position_type<R>>;

template <typename R>
concept Mutable_range =
    Range<R> and
    Mutable<Position_type<R>>;

}
