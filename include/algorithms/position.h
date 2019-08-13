#pragma once

#include "concepts/regular.h"
#include "concepts/position.h"
#include "type_functions/position.h"

#include "integer.h"

namespace elements {

// Access

template <typename T>
constexpr auto
pointer_to(T& x) -> Pointer_type<T>
{
    return &x;
}

template <typename L>
requires Movable<Decay<L>>
constexpr auto
load(L const& x) -> Value_type<L> const&
{
    return x;
}

template <typename T>
requires Movable<Decay<T>>
constexpr auto
load(Pointer_type<T> x) -> T const&
{
    return *x;
}

template <Movable T>
constexpr void
store(T& x, Value_type<T> const& v)
{
    x = v;
}

template <Movable T>
constexpr void
store(T& x, Value_type<T>&& v)
{
    x = std::forward<Value_type<T>>(v);
}

template <Movable T>
constexpr void
store(Pointer_type<T> x, Value_type<T> const& v)
{
    *x = v;
}

template <Movable T>
constexpr void
store(Pointer_type<T> x, Value_type<T>&& v)
{
    *x = std::forward<Value_type<T>>(v);
}

template <Movable T>
constexpr auto
at(T& x) -> T&
{
    return x;
}

template <Movable T>
constexpr auto
at(T const& x) -> T const&
{
    return x;
}

template <Movable T>
constexpr auto
at(Pointer_type<T> x) -> T&
{
    return *x;
}

template <Movable T>
constexpr auto
at(Pointer_type<T const> x) -> T const&
{
    return *x;
}

// Linear traversal

template <typename P>
requires Movable<Decay<P>>
constexpr void
increment(P& x)
{
    ++x;
}

template <typename P>
requires Movable<Decay<P>>
constexpr void
increment(Pointer_type<P>& x)
{
    x = x + One<Difference_type<Pointer_type<P>>>;
}

template <typename P>
requires Movable<Decay<P>>
constexpr void
decrement(P& x)
{
    --x;
}

template <typename P>
requires Movable<Decay<P>>
constexpr void
decrement(Pointer_type<P>& x)
{
    x = x - One<Difference_type<Pointer_type<P>>>;
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr auto
successor(T x) -> T
{
    increment(x);
    return x;
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
predecessor(T x) -> T
{
    decrement(x);
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

// Ranges

template <Position P, Movable L>
constexpr auto
precedes(P const&, L const&) -> bool
{
    return false;
}

template <Binary_integer I>
constexpr auto
precedes(I const& i0, I const& i1) -> bool
{
    return i0 < i1;
}

template <typename T>
requires Movable<Decay<T>>
constexpr auto
precedes(Pointer_type<T> const& pos0, Pointer_type<T> const& pos1) -> bool
{
    return pos0 != pos1;
}

template <Position P, Limit<P> L>
constexpr auto
operator-(L lim, P pos) -> Difference_type<P>
//[[expects axiom: is_bounded_range(pos, lim)]]
{
    auto n(Zero<Difference_type<P>>);
    while (precedes(pos, lim)) {
        increment(n);
        increment(pos);
    }
    return n;
}

template <Position P>
constexpr auto
operator+(P pos, Difference_type<P> dist) -> P
//[[expects axiom: dist >= Zero<Difference_type<P>>]]
//[[expects axiom: is_weak_range(pos, dist)]]
{
    while (!is_zero(dist)) {
        increment(pos);
        decrement(dist);
    }
    return pos;
}

template <Position P>
constexpr auto
operator+(Difference_type<P> dist, P pos) -> P
//[[expects axiom: dist >= Zero<Difference_type<P>>]]
//[[expects axiom: is_weak_range(pos, dist)]]
{
    return pos + dist;
}

template <Bidirectional_position P>
constexpr auto
operator-(P pos, Difference_type<P> dist) -> P
//[[expects axiom: dist >= Zero<Difference_type<P>>]]
//[[expects axiom first: is_weak_range(first, dist)]]
//[[expects axiom first: pos = first + dist]]
{
    while (!is_zero(dist)) {
        decrement(pos);
        decrement(dist);
    }
    return pos;
}

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

}
