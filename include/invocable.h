#pragma once

#include "regular.h"

namespace elements {

template <typename F, typename... Args>
concept Invocable =
    requires(F&& f, Args&&... args) {
        std::invoke(fw<F>(f), fw<Args>(args)...);
    };

template <typename F, typename... Args>
concept Regular_invocable = Invocable<F, Args...>;

template <typename F, typename... Args>
concept Operation =
    Regular_invocable<F, Args...> and
    sizeof...(Args) > 0 and
    Same_as<Return_type<F, Args...>, Decay<std::tuple_element_t<0, std::tuple<Args...>>>>;

template <typename T, Operation<T, T> Op>
struct transpose_op
{
    Op op;

    explicit constexpr
    transpose_op(Op op_) : op{op_} {}

    constexpr auto
    operator()(T const& x, T const& y) -> T
    {
        return invoke(op, y, x);
    }
};

template <typename F, typename... Args>
concept Predicate =
    Regular_invocable<F, Args...> &&
    Boolean_testable<Return_type<F, Args...>>;

template <typename T, Predicate<T> P>
struct negation
{
    P pred;

    constexpr
    negation(P pred_) : pred{pred_} {}

    constexpr auto
    operator()(T const& x) -> bool
    {
        return !invoke(pred, x);
    }
};

template <typename R, typename T, typename U>
concept Relation =
    Predicate<R, T, T> and
    Predicate<R, U, U> and
    Predicate<R, T, U> and
    Predicate<R, U, T>;

template <typename T, typename U, Relation<T, U> R>
struct identity_relation
{
    R rel;

    explicit constexpr
    identity_relation(R r) : rel{r} {}

    constexpr auto
    operator()(T const& x, U const& y) -> bool
    {
        return invoke(rel, x, y);
    }
};

template <typename T, typename U, Relation<T, U> R>
struct complement
{
    R rel;

    explicit constexpr
    complement(R r) : rel{r} {}

    constexpr auto
    operator()(T const& x, U const& y) -> bool
    {
        return !invoke(rel, x, y);
    }
};

template <typename T, typename U, Relation<T, U> R>
struct converse
{
    R rel;

    explicit constexpr
    converse(R r) : rel{r} {}

    constexpr auto
    operator()(T const& x, U const& y) -> bool
    {
        return invoke(rel, y, x);
    }
};

template <typename T, typename U, Relation<T, U> R>
struct complement_of_converse
{
    R rel;

    explicit constexpr
    complement_of_converse(R r) : rel{r} {}

    constexpr auto
    operator()(T const& x, U const& y) -> bool
    {
        return !invoke(rel, y, x);
    }
};

}
