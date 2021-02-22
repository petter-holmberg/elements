#pragma once

#include "integer.h"
#include "invocable.h"

namespace elements {

template <typename F, typename Arg>
requires Operation<F, Arg>
struct distance_type_t<F, Arg>
{
    using type = Distance_type<Arg>;
};

template <typename F, typename Arg>
concept Transformation =
    Operation<F, Arg> and
    Integer<Distance_type<F, Arg>>;

template <typename F, typename Arg>
requires Invocable<F, Arg&> and Same_as<Return_type<F, Arg&>, void>
struct distance_type_t<F, Arg>
{
    using type = Distance_type<Arg>;
};

template <typename F, typename Arg>
concept Action =
    Invocable<F, Arg&> and
    Same_as<Return_type<F, Arg&>, void> and
    Integer<Distance_type<F, Arg>>;

template <typename F, typename Arg, typename N>
requires Transformation<F, Arg> and Integer<N>
constexpr auto
power_unary(Arg x, N n, F fun) -> Arg
//[[expects axiom: "n >= 0 and for all i in n, f^i(x) is defined"]]
{
    while (n != Zero<N>) {
        decrement(n);
        x = invoke(fun, x);
    }
    return x;
}

template <typename F, typename Arg, typename N>
requires Action<F, Arg> and Integer<N>
constexpr auto
power_unary(Arg x, N n, F fun) -> Arg
//[[expects axiom: "n >= 0 and for all i in n, f^i(x) is defined"]]
{
    while (n != Zero<N>) {
        decrement(n);
        invoke(fun, x);
    }
    return x;
}

template <typename F, typename Arg>
requires Transformation<F, Arg>
constexpr auto
distance(Arg x, Arg y, F fun) -> Distance_type<F, Arg>
//[[expects axiom: "y is reachable from x under fun"]]
{
    auto n{Zero<Distance_type<F, Arg>>};
    while (x != y) {
        x = invoke(fun, x);
        n = n + One<Distance_type<F, Arg>>;
    }
    return n;
}

template <typename F, typename Arg>
requires Action<F, Arg>
constexpr auto
distance(Arg x, Arg y, F fun) -> Distance_type<F, Arg>
//[[expects axiom: "y is reachable from x under fun"]]
{
    auto n{Zero<Distance_type<F, Arg>>};
    while (x != y) {
        invoke(fun, x);
        n = n + One<Distance_type<F, Arg>>;
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
    if (!invoke(pred, x)) return x;
    Arg slow{x};
    Arg fast{invoke(fun, x)};
    while (fast != slow) {
        slow = invoke(fun, slow);
        if (!invoke(pred, fast)) return fast;
        fast = invoke(fun, fast);
        if (!invoke(pred, fast)) return fast;
        fast = invoke(fun, fast);
    }
    return fast;
}

template <typename F, typename P, typename Arg>
requires Action<F, Arg> and Predicate<P, Arg>
constexpr auto
collision_point(Arg const& x, F fun, P pred) -> Arg
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
//[[ensures ret: ret is terminal point or collision point"]]
{
    if (!invoke(pred, x)) return x;
    Arg slow{x};
    Arg fast{x};
    invoke(fun, fast);
    while (fast != slow) {
        invoke(fun, slow);
        if (!invoke(pred, fast)) return fast;
        invoke(fun, fast);
        if (!invoke(pred, fast)) return fast;
        invoke(fun, fast);
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
    Arg fast{invoke(fun, x)};
    while (fast != slow) {
        slow = invoke(fun, slow);
        fast = invoke(fun, fast);
        fast = invoke(fun, fast);
    }
    return fast;
}

template <typename F, typename Arg>
requires Action<F, Arg>
constexpr auto
collision_point_nonterminating_orbit(Arg const& x, F fun) -> Arg
//[[expects axiom: "transformation is total or orbit is nonterminating for x"]]
//[[ensures ret: ret is collision point"]]
{
    Arg slow{x};
    Arg fast{x};
    invoke(fun, fast);
    while (fast != slow) {
        invoke(fun, slow);
        invoke(fun, fast);
        invoke(fun, fast);
    }
    return fast;
}

template <typename F, typename P, typename Arg>
requires (Transformation<F, Arg> or Action<F, Arg>) and Predicate<P, Arg>
constexpr auto
is_terminating(Arg const& x, F fun, P pred) -> bool
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    return !invoke(pred, collision_point(x, fun, pred));
}

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
is_circular(Arg const& x, F fun, P pred) -> bool
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    Arg y{collision_point(x, fun, pred)};
    return invoke(pred, y) && x == invoke(fun, y);
}

template <typename F, typename P, typename Arg>
requires Action<F, Arg> and Predicate<P, Arg>
constexpr auto
is_circular(Arg const& x, F fun, P pred) -> bool
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    Arg y{collision_point(x, fun, pred)};
    if (invoke(pred, y)) {
        invoke(fun, y);
        return x == y;
    } else {
        return false;
    }
}

template <typename F, typename Arg>
requires Transformation<F, Arg>
constexpr auto
is_circular_nonterminating_orbit(Arg const& x, F fun) -> bool
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    return x == invoke(fun, collision_point_nonterminating_orbit(x, fun));
}

template <typename F, typename Arg>
requires Action<F, Arg>
constexpr auto
is_circular_nonterminating_orbit(Arg const& x, F fun) -> bool
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    auto y{collision_point_nonterminating_orbit(x, fun)};
    invoke(fun, y);
    return x == y;
}

template <typename F, typename Arg>
requires Transformation<F, Arg>
constexpr auto
convergent_point(Arg x0, Arg x1, F fun) -> Arg
//[[expects axiom: "there exists an n in Distance_type<F, Arg> such that n >= 0 and func^n(x0) = func^n(x1)"]]
{
    while (x0 != x1) {
        x0 = invoke(fun, x0);
        x1 = invoke(fun, x1);
    }
    return x1;
}

template <typename F, typename Arg>
requires Action<F, Arg>
constexpr auto
convergent_point(Arg x0, Arg x1, F fun) -> Arg
//[[expects axiom: "there exists an n in Distance_type<F, Arg> such that n >= 0 and func^n(x0) = func^n(x1)"]]
{
    while (x0 != x1) {
        invoke(fun, x0);
        invoke(fun, x1);
    }
    return x1;
}

template <typename F, typename Arg>
requires Transformation<F, Arg> or Action<F, Arg>
constexpr auto
convergent_point_guarded(Arg x0, Arg x1, Arg y, F fun) -> Arg
//[[expects axiom: "y is reachable from x0 under fun and y is reachable from x1 under fun"]]
{
    auto d0{distance(x0, y, fun)};
    auto d1{distance(x1, y, fun)};
    if (d0 < d1) {
        x1 = power_unary(x1, d1 - d0, fun);
    } else if (d1 < d0) {
        x0 = power_unary(x0, d0 - d1, fun);
    }
    return convergent_point(x0, x1, fun);
}

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
connection_point(Arg const& x, F fun, P pred) -> Arg
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    Arg y{collision_point(x, fun, pred)};
    if (!invoke(pred, y)) return y;
    return convergent_point(x, invoke(fun, y), fun);
}

template <typename F, typename P, typename Arg>
requires Action<F, Arg> and Predicate<P, Arg>
constexpr auto
connection_point(Arg const& x, F fun, P pred) -> Arg
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    Arg y{collision_point(x, fun, pred)};
    if (!invoke(pred, y)) return y;
    invoke(fun, y);
    return convergent_point(x, y, fun);
}

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
connection_point_nonterminating_orbit(Arg const& x, F fun, P pred) -> Arg
{
    return convergent_point(x, invoke(fun, collision_point_nonterminating_orbit(x, fun)), fun);
}

template <typename F, typename P, typename Arg>
requires Action<F, Arg> and Predicate<P, Arg>
constexpr auto
connection_point_nonterminating_orbit(Arg const& x, F fun, P pred) -> Arg
{
    auto y{collision_point_nonterminating_orbit(x, fun)};
    invoke(fun, y);
    return convergent_point(x, y, fun);
}

template <typename F, typename Arg>
requires Transformation<F, Arg> or Action<F, Arg>
struct orbit
{
    Distance_type<F, Arg> handle_distance;
    Distance_type<F, Arg> cycle_distance;
    Arg connection;
};

template <typename F, typename P, typename Arg>
requires Transformation<F, Arg> and Predicate<P, Arg>
constexpr auto
orbit_structure(Arg const& x, F fun, P pred) -> orbit<F, Arg>
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    auto connection{connection_point(x, fun, pred)};
    Distance_type<F, Arg> cycle_distance{0};
    if (invoke(pred, connection)) cycle_distance = distance(invoke(fun, connection), connection, fun);
    return {distance(x, connection, fun), cycle_distance, connection};
}

template <typename F, typename P, typename Arg>
requires Action<F, Arg> and Predicate<P, Arg>
constexpr auto
orbit_structure(Arg const& x, F fun, P pred) -> orbit<F, Arg>
//[[expects axiom: "pred(x) <=> fun(x) is defined"]]
{
    auto connection{connection_point(x, fun, pred)};
    auto cycle_distance{Zero<Distance_type<F, Arg>>};
    auto y{connection};
    invoke(fun, y);
    if (invoke(pred, connection)) cycle_distance = distance(y, connection, fun);
    return {distance(x, connection, fun), cycle_distance, connection};
}

template <typename F, typename Arg>
requires Transformation<F, Arg>
constexpr auto
orbit_structure_nonterminating_orbit(Arg const& x, F fun) -> orbit<F, Arg>
{
    auto connection{connection_point_nonterminating_orbit(x, fun)};
    return {distance(x, connection, fun), distance(invoke(fun, connection), connection, fun), connection};
}

template <typename F, typename Arg>
requires Action<F, Arg>
constexpr auto
orbit_structure_nonterminating_orbit(Arg const& x, F fun) -> orbit<F, Arg>
{
    auto connection{connection_point_nonterminating_orbit(x, fun)};
    auto y{connection};
    invoke(fun, y);
    return {distance(x, connection, fun), distance(y, connection, fun), connection};
}

}
