#pragma once

#include <type_traits>

#include "regular.h"
#include "invocable.h"
#include "concepts/regular.h"
#include "type_functions/position.h"

namespace elements {

// Access

template <typename T>
requires Constructible<Decay<T>>
constexpr auto
load(T const& x) -> Value_type<T> const&;

template <typename T>
requires Constructible<Decay<T>>
constexpr auto
load(Pointer_type<T> x) -> T const&;

template <typename L>
concept Loadable =
    //Constructible<L> and
    requires (L const& x) {
        { load(x) } -> Value_type<L> const&;
    };

template <typename T>
constexpr void
store(T& x, Value_type<T> const& v);

template <typename T>
constexpr void
store(T& x, Value_type<T>&& v);

template <typename T>
constexpr void
store(Pointer_type<T> x, Value_type<T> const& v);

template <typename T>
constexpr void
store(Pointer_type<T> x, Value_type<T>&& v);

template <typename S>
concept Storable =
    //Constructible<S> and
    requires (S& x, Value_type<S>&& v) {
        store(x, v);
    };

template <Constructible T>
constexpr auto
at(T& x) -> T&;

template <Constructible T>
constexpr auto
at(T const& x) -> T const&;

template <Constructible T>
constexpr auto
at(Pointer_type<T> x) -> T&;

template <Constructible T>
constexpr auto
at(Pointer_type<T const> x) -> T const&;

template <typename M>
concept Mutable =
    Loadable<M> and
    Storable<M> and
    requires (M& x) {
        { at(x) } -> Value_type<M>&;
    } and
    requires (M const& x) {
        { at(x) } -> Value_type<M> const&;
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
concept Forward_position =
    Regular<P> and
    Position<P>;

template <typename P>
concept Bidirectional_position =
    Forward_position<P> and
    requires (P& x) {
        decrement(x);
    };

// Ranges

template <Position P, Movable L>
constexpr auto
precedes(P const&, L const&) -> bool;

template <typename L, typename P>
concept Limit =
    Regular<L> and
    Position<P> and
    requires (P const& pos, L const& lim) {
        { precedes(pos, lim) } -> bool;
    };

template <typename R>
concept Range =
    requires (R&& x) {
        { first(x) } -> Position_type<R>;
        limit(x);
    } and
    requires (R x, Size_type<R> i) {
        { is_empty(x) } -> bool;
        { size(x) } -> Size_type<R>;
        { x[i] } -> Value_type<R> const&;
        { x[i] } -> Value_type<R>&;
        // axiom {
        //     is_empty(x) == limit(x) - first(x);
        // }
        // axiom {
        //     size(x) == limit(x) - first(x);
        // }
    };

template <Range>
struct front;

template <Range>
struct back;

template <typename R>
concept Mutable_range =
    Range<R>;
    // and
    //Mutable<Position_type<R>>;

template <typename S>
concept Sequence =
    Range<S> and
    Default_totally_ordered<S>;

template <typename T>
concept Dynamic_sequence =
    Sequence<T> and
    requires (back<T> t, Value_type<T> x) {
        { insert(t, x) } -> back<T>;
        { erase(t) } -> back<T>;
    };

}
