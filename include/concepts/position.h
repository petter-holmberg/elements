#pragma once

#include "regular.h"

namespace elements {

// Access

template <typename L>
concept Loadable =
    Regular<decay<L>> and
    requires (decay<L> const& x) {
        { load(x) } -> value_type<L>;
    };

template <typename S>
concept Storable =
    Regular<S> and
    requires (S& x, value_type<S>&& v) {
        store(x, v);
    };

template <typename M>
concept Mutable =
    Loadable<M> and
    Storable<M> and
    requires (M& x) {
        { at(x) } -> value_type<M>&;
    };

// Linear traversal

template <typename P>
concept Position =
    Regular<P> and
    requires (P& x) {
        increment(x);
    };

template <typename>
constexpr bool is_forward_position = false;

template <typename P>
concept Forward_position =
    Position<P> and
    is_forward_position<P>;

template <typename>
struct distance_type_t;

template <typename T>
requires Semiregular<decay<T>>
using distance_type = typename distance_type_t<T>::type;

template <typename P>
concept Indexed_position =
    Forward_position<P> and
    requires (P x, distance_type<P> n) {
        { x + n } -> P;
        { x - x } -> distance_type<P>;
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

template <typename>
constexpr bool is_contiguously_addressable = false;

template <typename P>
concept Contiguous_position =
    Random_access_position<P> and
    is_contiguously_addressable<P>;

// Ranges

template <typename P, typename L>
concept Limit =
    Position<P> and
    Weakly_equality_comparable<P, L>;

template <typename T>
requires Semiregular<decay<T>>
struct position_type_t;

template <typename T>
requires Semiregular<decay<T>>
using position_type = typename position_type_t<T>::type;

template <typename R>
concept Range =
    requires (R&& x) {
        { first(x) } -> position_type<R>;
        limit(x);
    };

}
