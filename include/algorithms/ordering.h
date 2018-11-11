#pragma once

#include "concepts/invocable.h"
#include "concepts/regular.h"

namespace elements {

// 2 values

template <Relation R>
constexpr auto
select_0_2(
    domain<R>& a,
    domain<R>& b,
    R rel) -> domain<R>&
//[[expects axiom: weak_ordering(rel)]]
{
    if (rel(b, a)) return b;
    return a;
}

template <Relation R>
constexpr auto
select_0_2(
    domain<R> const& a,
    domain<R> const& b,
    R rel) -> domain<R> const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (rel(b, a)) return b;
    return a;
}

template <Default_totally_ordered T>
constexpr auto
min(
    T const& a,
    T const& b) -> T const&
{
    return select_0_2(a, b, less<T>{});
}

template <Default_totally_ordered T>
constexpr auto
min(
    T& a,
    T& b) -> T&
{
    return select_0_2(a, b, less<T>{});
}

template <Relation R>
constexpr auto
select_1_2(
    domain<R> const& a,
    domain<R> const& b,
    R rel) -> domain<R> const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (rel(b, a)) return a;
    return b;
}

template <Relation R>
constexpr auto
select_1_2(
    domain<R>& a,
    domain<R>& b,
    R rel) -> domain<R>&
//[[expects axiom: weak_ordering(rel)]]
{
    if (rel(b, a)) return a;
    return b;
}

template <Default_totally_ordered T>
constexpr auto
max(
    T const& a,
    T const& b) -> T const&
{
    return select_1_2(a, b, less<T>{});
}

template <Default_totally_ordered T>
constexpr auto
max(
    T& a,
    T& b) -> T const&
{
    return select_1_2(a, b, less<T>{});
}

// 3 values

template <Relation R>
constexpr auto
select_0_3(
    domain<R> const& a,
    domain<R> const& b,
    domain<R> const& c,
    R rel) -> domain<R> const&
//[[expects axiom: weak_ordering(rel)]]
{
    return select_0_2(select_0_2(a, b, rel), c, rel);
}

template <Relation R>
constexpr auto
select_0_3(
    domain<R>& a,
    domain<R>& b,
    domain<R>& c,
    R rel) -> domain<R>&
//[[expects axiom: weak_ordering(rel)]]
{
    return select_0_2(select_0_2(a, b, rel), c, rel);
}

template <Default_totally_ordered T>
constexpr auto
min(
    T const& a,
    T const& b,
    T const& c) -> T const&
{
    return select_0_3(a, b, c, less<T>{});
}

template <Default_totally_ordered T>
constexpr auto
min(
    T& a,
    T& b,
    T& c) -> T&
{
    return select_0_3(a, b, c, less<T>{});
}

template <Relation R>
constexpr auto
select_1_3_ab(
    domain<R> const& a,
    domain<R> const& b,
    domain<R> const& c,
    R rel) -> domain<R> const&
//[[expects axiom: weak_ordering(rel)]]
//[[expects: select_0_1(a, b, rel) == a]]
{
    if (!rel(c, b)) return b;
    return select_1_2(a, c, rel);
}

template <Relation R>
constexpr auto
select_1_3_ab(
    domain<R>& a,
    domain<R>& b,
    domain<R>& c,
    R rel) -> domain<R>&
//[[expects axiom: weak_ordering(rel)]]
//[[expects: select_0_1(a, b, rel) == a]]
{
    if (!rel(c, b)) return b;
    return select_1_2(a, c, rel);
}

template <Relation R>
constexpr auto
select_1_3(
    domain<R> const& a,
    domain<R> const& b,
    domain<R> const& c,
    R rel) -> domain<R> const&
//[[expects axiom: weak_ordering(rel)]]
{
    if (rel(b, a))
        return select_1_3_ab(b, a, c, rel);
    else
        return select_1_3_ab(a, b, c, rel);
}

template <Relation R>
constexpr auto
select_1_3(
    domain<R>& a,
    domain<R>& b,
    domain<R>& c,
    R rel) -> domain<R>&
//[[expects axiom: weak_ordering(rel)]]
{
    if (rel(b, a))
        return select_1_3_ab(b, a, c, rel);
    else
        return select_1_3_ab(a, b, c, rel);
}

template <Default_totally_ordered T>
constexpr auto
median(
    T const& a,
    T const& b,
    T const& c) -> T const&
{
    return select_1_3(a, b, c, less<T>{});
}

template <Default_totally_ordered T>
constexpr auto
median(
    T& a,
    T& b,
    T& c) -> T&
{
    return select_1_3(a, b, c, less<T>{});
}

template <Relation R>
constexpr auto
select_2_3(
    domain<R> const& a,
    domain<R> const& b,
    domain<R> const& c,
    R rel) -> domain<R> const&
//[[expects axiom: weak_ordering(rel)]]
{
    return select_1_2(select_1_2(a, b, rel), c, rel);
}

template <Relation R>
constexpr auto
select_2_3(
    domain<R>& a,
    domain<R>& b,
    domain<R>& c,
    R rel) -> domain<R>&
//[[expects axiom: weak_ordering(rel)]]
{
    return select_1_2(select_1_2(a, b, rel), c, rel);
}

template <Default_totally_ordered T>
constexpr auto
max(
    T const& a,
    T const& b,
    T const& c) -> T const&
{
    return select_2_3(a, b, c, less<T>{});
}

template <Default_totally_ordered T>
constexpr auto
max(
    T& a,
    T& b,
    T& c) -> T&
{
    return select_2_3(a, b, c, less<T>{});
}

}
