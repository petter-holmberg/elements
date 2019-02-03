#pragma once

#include <type_traits>

#include "regular.h"
#include "type_functions/position.h"
#include "algorithms/integer.h"

namespace elements {

// Access

template <typename T>
constexpr auto
pointer_to(T& x) -> Pointer_type<T>
{
    return &x;
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr auto
load(T const& x) -> T const&
{
    return x;
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr auto
load(Pointer_type<T> x) -> T const&
{
    return *x;
}

template <Semiregular T>
constexpr void
store(T& x, Value_type<T> const& v)
{
    x = v;
}

template <Semiregular T>
constexpr void
store(Pointer_type<T> x, Value_type<T> const& v)
{
    *x = v;
}

template <Semiregular T>
constexpr auto
at(T& x) -> T&
{
    return x;
}

template <Semiregular T>
constexpr auto
at(Pointer_type<T> x) -> T&
{
    return *x;
}

// Linear traversal

template <typename T>
requires Semiregular<Decay<T>>
constexpr void
increment(Pointer_type<T>& x)
{
    x = x + Distance_type<Pointer_type<T>>{1};
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr void
decrement(Pointer_type<T>& x)
{
    x = x - Distance_type<Pointer_type<T>>{1};
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr auto
successor(Pointer_type<T> x) -> Pointer_type<T>
{
    increment(x);
    return x;
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr auto
predecessor(Pointer_type<T> x) -> Pointer_type<T>
{
    decrement(x);
    return x;
}

}

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

// Access and traversal

template <typename P>
concept Loadable_position =
    Loadable<P> and
    Position<P>;

template <typename P>
concept Storable_position =
    Storable<P> and
    Position<P>;

template <typename P>
concept Mutable_position =
    Mutable<P> and
    Position<P>;

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

template <typename S>
concept Sequence =
    Range<S> and
    Totally_ordered<S> and
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
struct front
{
    Pointer_type<S> sequence;

    constexpr front(S& sequence_)
        : sequence{&sequence_}
    {}
};

template <Sequence S>
struct back
{
    Pointer_type<S> sequence;

    back(S& sequence_)
        : sequence{&sequence_}
    {}
};

template <typename T>
concept Dynamic_sequence =
    Sequence<T> and
    requires (back<T> t, Value_type<T> x) {
        { insert(t, x) } -> back<T>;
        { erase(t) } -> back<T>;
    };

}
