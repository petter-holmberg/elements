#pragma once

#include "cursor.h"
#include "integer.h"
#include "invocable.h"

namespace elements {

template <typename T, Integral Arg>
requires Operation<T, Arg>
struct distance_type_t<T, Arg>
{
    using type = Distance_type<Arg>;
};

template <typename F, typename Arg>
concept Transformation =
    Operation<F, Arg> and
    Integer<Distance_type<F, Arg>>;

template <typename F, typename Arg>
requires Transformation<F, Arg>
constexpr auto
distance(Arg x, Arg y, F fun) -> Distance_type<F, Arg>
//[[expects axiom: "y is reachable from x under fun"]]
{
    Distance_type<F, Arg> n{0};
    while (x != y)
    {
        store(x, fun(x));
        store(n, n + Distance_type<F, Arg>{1});
    }
    return n;
}

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
collision_point(Arg const& x, F fun, P pred) -> Arg
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
//[[ensures ret: ret is terminal point or collision point"]]
{
    if (!pred(x)) return x;
    Arg slow{x};
    Arg fast{fun(x)};
    while (fast != slow)
    {
        store(slow, fun(slow));
        if (!pred(fast)) return fast;
        store(fast, fun(fast));
        if (!pred(fast)) return fast;
        store(fast, fun(fast));
    }
    return fast;
}

template <typename F, typename Arg>
requires Transformation<F, Arg>
constexpr auto
collision_point_nonterminating_orbit(Arg const& x, F fun) -> Arg
//[[expects axiom: "transformation is total or orbit is nonterminating for x"]]
//[[ensures ret: ret is collision point"]]
{
    Arg slow{x};
    Arg fast{fun(x)};
    while (fast != slow)
    {
        store(slow, fun(slow));
        store(fast, fun(fast));
        store(fast, fun(fast));
    }
    return fast;
}

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
is_terminating(Arg const& x, F fun, P pred) -> bool
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    return !pred(collision_point(x, fun, pred));
}

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
is_circular(Arg const& x, F fun, P pred) -> bool
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    Arg y{collision_point(x, fun, pred)};
    return pred(y) && x == fun(y);
}

template <typename F, typename Arg>
requires Transformation<F, Arg>
constexpr auto
is_circular_nonterminating_orbit(Arg const& x, F fun) -> bool
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    return x == fun(collision_point_nonterminating_orbit(x, fun));
}

template <typename F, typename Arg>
requires Transformation<F, Arg>
constexpr auto
convergent_point(Arg x0, Arg x1, F fun) -> Arg
{
    while (x0 != x1)
    {
        x0 = fun(x0);
        x1 = fun(x1);
    }
    return x1;
}

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
connection_point(Arg const& x, F fun, P pred) -> Arg
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    Arg y{collision_point(x, fun, pred)};
    if (!pred(y)) return y;
    return convergent_point(x, fun(y), fun);
}

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
connection_point_nonterminating_orbit(Arg const& x, F fun, P pred) -> Arg
{
    return convergent_point(x, fun(collision_point_nonterminating_orbit(x, fun)), fun);
}

template <typename F, typename Arg>
requires Transformation<F, Arg>
struct orbit
{
    Distance_type<F, Arg> handle_distance;
    Distance_type<F, Arg> cycle_distance;
    Return_type<F, Arg> connection;
};

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
orbit_structure(Arg const& x, F fun, P pred) -> orbit<F, Arg>
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    auto connection{connection_point(x, fun, pred)};
    Distance_type<F, Arg> cycle_distance{0};
    if (pred(connection)) cycle_distance = distance(fun(connection), connection, fun);
    return {distance(x, connection, fun), cycle_distance, connection};
}

template <typename F, typename Arg>
requires Transformation<F, Arg>
constexpr auto
orbit_structure_nonterminating_orbit(Arg const& x, F fun) -> orbit<F, Arg>
{
    auto connection{connection_point_nonterminating_orbit(x, fun)};
    return {distance(x, connection, fun), distance(fun(connection), connection, fun), connection};
}

}
