#pragma once

#include <type_traits>

#include "regular.h"
#include "invocable.h"
#include "type_functions/position.h"

namespace elements {

// Access

template <typename L>
requires Movable<Decay<L>>
constexpr auto
load(L const& x) -> Value_type<L> const&;

template <typename T>
requires Movable<Decay<T>>
constexpr auto
load(Pointer_type<T> x) -> T const&;

template <typename L>
concept Loadable =
    Movable<L> and
    requires (L const& x) {
        { load(x) } -> Value_type<L> const&;
    };

template <Semiregular T>
constexpr void
store(T& x, Value_type<T> const& v);

template <Semiregular T>
constexpr void
store(Pointer_type<T> x, Value_type<T> const& v);

template <typename S>
concept Storable =
    Regular<S> and
    requires (S& x, Value_type<S>&& v) {
        store(x, v);
    };

// Linear traversal

template <typename P>
requires Movable<Decay<P>>
constexpr void
increment(P& x);

template <typename P>
requires Movable<Decay<P>>
constexpr void
increment(Pointer_type<P>& x);

template <typename P>
requires Movable<Decay<P>>
constexpr void
decrement(P& x);

template <typename P>
requires Movable<Decay<P>>
constexpr void
decrement(Pointer_type<P>& x);

template <typename P>
concept Position =
    Movable<P> and
    requires (P& x) {
        increment(x);
    };

template <typename P>
concept Bidirectional_position =
    Position<P> and
    requires (P& x) {
        decrement(x);
    };

// Access and linear traversal

template <typename P>
concept Loadable_position =
    Loadable<P> and
    Position<P>;

template <typename P>
concept Storable_position =
    Storable<P> and
    Position<P>;

template <typename P>
concept Loadable_bidirectional_position =
    Loadable<P> and
    Bidirectional_position<P>;

template <typename P>
concept Storable_bidirectional_position =
    Storable<P> and
    Bidirectional_position<P>;

template <typename P>
concept Mutable_bidirectional_position =
    Loadable<P> and
    Storable<P> and
    Bidirectional_position<P>;

// Ranges

template <Position P, Movable L>
constexpr auto
precedes(P const&, L const&) -> bool;

template <typename L, typename P>
concept Limit =
    Position<P> and
    requires (P const& pos, L const& lim) {
        { precedes(pos, lim) } -> bool;
    };

template <typename R>
concept Range =
    requires (R&& x) {
        { first(x) } -> Position_type<R>;
        limit(x);
    };

template <typename R>
concept Mutable_range =
    Range<R>;
    // and
    //Mutable<Position_type<R>>;

template <typename S>
concept Sequence =
    Range<S> and
    Default_totally_ordered<S> and
    requires (S x, Size_type<S> i) {
        { is_empty(x) } -> bool;
        { size(x) } -> Size_type<S>;
        { x[i] } -> Value_type<S> const&;
        { x[i] } -> Value_type<S>&;
        // axiom {
        //     is_empty(x) == limit(x) - first(x);
        // }
        // axiom {
        //     size(x) == limit(x) - first(x);
        // }
    };

template <Sequence S>
struct front;

template <Sequence S>
struct back;

template <typename T>
concept Dynamic_sequence =
    Sequence<T> and
    requires (back<T> t, Value_type<T> x) {
        { insert(t, x) } -> back<T>;
        { erase(t) } -> back<T>;
    };

}
