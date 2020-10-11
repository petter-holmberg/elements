#pragma once

#include "invocable.h"

namespace elements {

// 2 values

template <typename T, Relation<T, T> R>
constexpr auto
select_0_2(T& a, T& b, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return b;
    return a;
}

template <typename T, Relation<T, T> R>
constexpr auto
select_0_2(T const& a, T const& b, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return b;
    return a;
}

template <Default_totally_ordered T>
constexpr auto
min(T const& a, T const& b) -> T const&
{
    return select_0_2(a, b, lt{});
}

template <Default_totally_ordered T>
constexpr auto
min(T& a, T& b) -> T&
{
    return select_0_2(a, b, lt{});
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_2(T const& a, T const& b, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return a;
    return b;
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_2(T& a, T& b, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return a;
    return b;
}

template <Default_totally_ordered T>
constexpr auto
max(T const& a, T const& b) -> T const&
{
    return select_1_2(a, b, lt{});
}

template <Default_totally_ordered T>
constexpr auto
max(T& a, T& b) -> T const&
{
    return select_1_2(a, b, lt{});
}

// 3 values

template <typename T, Relation<T, T> R>
constexpr auto
select_0_3(T const& a, T const& b, T const& c, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    return select_0_2(select_0_2(a, b, rel), c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_0_3(T& a, T& b, T& c, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    return select_0_2(select_0_2(a, b, rel), c, rel);
}

template <Default_totally_ordered T>
constexpr auto
min(T const& a, T const& b, T const& c) -> T const&
{
    return select_0_3(a, b, c, lt{});
}

template <Default_totally_ordered T>
constexpr auto
min(T& a, T& b, T& c) -> T&
{
    return select_0_3(a, b, c, lt{});
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_3_ab(T const& a, T const& b, T const& c, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
//[[expects: select_0_1(a, b, rel) == a]]
{
    if (!invoke(rel, c, b)) return b;
    return select_1_2(a, c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_3_ab(T& a, T& b, T& c, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
//[[expects: select_0_1(a, b, rel) == a]]
{
    if (!invoke(rel, c, b)) return b;
    return select_1_2(a, c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_3(T const& a, T const& b, T const& c, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a))
        return select_1_3_ab(b, a, c, rel);
    else
        return select_1_3_ab(a, b, c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_3(T& a, T& b, T& c, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a))
        return select_1_3_ab(b, a, c, rel);
    else
        return select_1_3_ab(a, b, c, rel);
}

template <Default_totally_ordered T>
constexpr auto
median(T const& a, T const& b, T const& c) -> T const&
{
    return select_1_3(a, b, c, lt{});
}

template <Default_totally_ordered T>
constexpr auto
median(T& a, T& b, T& c) -> T&
{
    return select_1_3(a, b, c, lt{});
}

template <typename T, Relation<T, T> R>
constexpr auto
select_2_3(T const& a, T const& b, T const& c, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    return select_1_2(select_1_2(a, b, rel), c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_2_3(T& a, T& b, T& c, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    return select_1_2(select_1_2(a, b, rel), c, rel);
}

template <Default_totally_ordered T>
constexpr auto
max(T const& a, T const& b, T const& c) -> T const&
{
    return select_2_3(a, b, c, lt{});
}

template <Default_totally_ordered T>
constexpr auto
max(T& a, T& b, T& c) -> T&
{
    return select_2_3(a, b, c, lt{});
}

}
