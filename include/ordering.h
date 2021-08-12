#pragma once

#include "invocable.h"

namespace elements {

// 2 values

template <typename T, Relation<T, T> R>
constexpr auto
select_0_2(T const& a, T const& b, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return b;
    return a;
}

template <typename T, Relation<T, T> R>
constexpr auto
select_0_2(T& a, T& b, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return b;
    return a;
}

template <typename T, Relation<T, T> R>
constexpr void
select_0_2_accumulate(T& a, T const& b, R rel)
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) a = b;
}

template <typename T0, typename T1>
requires Default_totally_ordered_with<T0, T1>
constexpr decltype(auto)
min(T0&& a, T1&& b)
{
    return select_0_2(fw<T0>(a), fw<T1>(b), lt{});
}

template <typename T0, typename T1>
requires Default_totally_ordered_with<T0, T1>
constexpr void
min_accumulate(T0& a, T1 const& b)
{
    select_0_2_accumulate(a, b, lt{});
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

template <typename T, Relation<T, T> R>
constexpr void
select_1_2_accumulate(T& a, T const& b, R rel)
//[[expects axiom: weak_ordering(rel)]]
{
    if (!invoke(rel, b, a)) a = b;
}

template <typename T0, typename T1>
requires Default_totally_ordered_with<T0, T1>
constexpr decltype(auto)
max(T0&& a, T1&& b)
{
    return select_1_2(fw<T0>(a), fw<T1>(b), lt{});
}

template <typename T0, typename T1>
requires Default_totally_ordered_with<T0, T1>
constexpr void
max_accumulate(T0& a, T1 const& b)
{
    select_1_2_accumulate(a, b, lt{});
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

template <typename T0, typename T1, typename T2>
requires
    Default_totally_ordered_with<T0, T1> and
    Default_totally_ordered_with<T0, T2> and
    Default_totally_ordered_with<T1, T2>
constexpr decltype(auto)
min(T0&& a, T1&& b, T2&& c)
{
    return select_0_3(fw<T0>(a), fw<T1>(b), fw<T2>(c), lt{});
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
select_1_3_ac(T const& a, T const& b, T const& c, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
//[[expects: select_0_1(a, c, rel) == a]]
{
    if (invoke(rel, b, a)) return a;
    return select_0_2(b, c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_3_ac(T& a, T& b, T& c, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
//[[expects: select_0_1(a, c, rel) == a]]
{
    if (invoke(rel, b, a)) return a;
    return select_0_2(b, c, rel);
}

template <typename T0, typename T1, typename T2>
requires Default_totally_ordered_with<T1, T2>
constexpr decltype(auto)
clamp(T0&& a, T1&& b, T2&& c)
//[[expects axiom: weak_ordering(rel)]]
//[[expects: select_0_1(a, c, lt{}) == a]]
{
    return select_1_3_ac(fw<T0>(a), fw<T1>(b), fw<T2>(c), lt{});
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_3(T const& a, T const& b, T const& c, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return select_1_3_ab(b, a, c, rel);
    return select_1_3_ab(a, b, c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_3(T& a, T& b, T& c, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return select_1_3_ab(b, a, c, rel);
    return select_1_3_ab(a, b, c, rel);
}

template <typename T0, typename T1, typename T2>
requires Default_totally_ordered_with<T0, T2>
constexpr decltype(auto)
median(T0&& a, T1&& b, T2&& c)
{
    return select_1_3(fw<T0>(a), fw<T1>(b), fw<T2>(c), lt{});
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

template <typename T0, typename T1, typename T2>
requires
    Default_totally_ordered_with<T0, T1> and
    Default_totally_ordered_with<T0, T2> and
    Default_totally_ordered_with<T1, T2>
constexpr decltype(auto)
max(T0&& a, T1&& b, T2&& c)
{
    return select_2_3(fw<T0>(a), fw<T1>(b), fw<T2>(c), lt{});
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_4_ab_cd(T const& a, T const& b, T const& c, T const& d, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, c, a)) return select_0_2(a, d, rel);
    return select_0_2(b, c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_4_ab_cd(T& a, T& b, T& c, T& d, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, c, a)) return select_0_2(a, d, rel);
    return select_0_2(b, c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_2_4_ab_cd(T const& a, T const& b, T const& c, T const& d, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, d, b)) return select_1_2(a, d, rel);
    return select_1_2(b, c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_2_4_ab_cd(T& a, T& b, T& c, T& d, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, d, b)) return select_1_2(a, d, rel);
    return select_1_2(b, c, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_4_ab(T const& a, T const& b, T const& c, T const& d, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, d, c)) return select_1_4_ab_cd(a, b, d, c, rel);
    return select_1_4_ab_cd(a, b, c, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_4_ab(T& a, T& b, T& c, T& d, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, d, c)) return select_1_4_ab_cd(a, b, d, c, rel);
    return select_1_4_ab_cd(a, b, c, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_2_4_cd(T const& a, T const& b, T const& c, T const& d, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return select_2_4_ab_cd(b, a, c, d, rel);
    return select_2_4_ab_cd(a, b, c, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_2_4_cd(T& a, T& b, T& c, T& d, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return select_2_4_ab_cd(b, a, c, d, rel);
    return select_2_4_ab_cd(a, b, c, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_0_4(T const& a, T const& b, T const& c, T const& d, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return select_0_3(b, c, d, rel);
    return select_0_3(a, c, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_0_4(T& a, T& b, T& c, T& d, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return select_0_3(b, c, d, rel);
    return select_0_3(a, c, d, rel);
}

template <typename T0, typename T1, typename T2, typename T3>
requires
    Default_totally_ordered_with<T0, T1> and
    Default_totally_ordered_with<T0, T2> and
    Default_totally_ordered_with<T0, T3> and
    Default_totally_ordered_with<T1, T2> and
    Default_totally_ordered_with<T1, T3> and
    Default_totally_ordered_with<T2, T3>
constexpr decltype(auto)
min(T0&& a, T1&& b, T2&& c, T3&& d)
{
    return select_0_4(fw<T0>(a), fw<T1>(b), fw<T2>(c), fw<T3>(d), lt{});
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_4(T const& a, T const& b, T const& c, T const& d, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return select_1_4_ab(b, a, c, d, rel);
    return select_1_4_ab(a, b, c, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_1_4(T& a, T& b, T& c, T& d, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, b, a)) return select_1_4_ab(b, a, c, d, rel);
    return select_1_4_ab(a, b, c, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_2_4(T const& a, T const& b, T const& c, T const& d, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, d, c)) return select_2_4_cd(a, b, d, c, rel);
    return select_2_4_cd(a, b, c, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_2_4(T& a, T& b, T& c, T& d, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, d, c)) return select_2_4_cd(a, b, d, c, rel);
    return select_2_4_cd(a, b, c, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_3_4(T const& a, T const& b, T const& c, T const& d, R rel) -> T const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, d, c)) return select_2_3(a, b, c, rel);
    return select_2_3(a, b, d, rel);
}

template <typename T, Relation<T, T> R>
constexpr auto
select_3_4(T& a, T& b, T& c, T& d, R rel) -> T&
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, d, c)) return select_2_3(a, b, c, rel);
    return select_2_3(a, b, d, rel);
}

template <typename T0, typename T1, typename T2, typename T3>
requires
    Default_totally_ordered_with<T0, T1> and
    Default_totally_ordered_with<T0, T2> and
    Default_totally_ordered_with<T0, T3> and
    Default_totally_ordered_with<T1, T2> and
    Default_totally_ordered_with<T1, T3> and
    Default_totally_ordered_with<T2, T3>
constexpr decltype(auto)
max(T0&& a, T1&& b, T2&& c, T3&& d)
{
    return select_3_4(fw<T0>(a), fw<T1>(b), fw<T2>(c), fw<T3>(d), lt{});
}

}
