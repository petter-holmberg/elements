#pragma once

#include "algorithms/position.h"
#include "concepts/position.h"

namespace elements {

template <Position P>
requires Loadable<P>
struct loadable_position
{
    P pos;

    explicit loadable_position(P pos_) : pos{pos_} {}

    loadable_position(loadable_position const&) = delete;

    loadable_position(loadable_position&&) = default;

    loadable_position& operator=(loadable_position const&) = delete;

    loadable_position& operator=(loadable_position&&) = default;

    ~loadable_position() = default;
};

template <Position P>
requires Loadable<P>
struct value_type_t<loadable_position<P>>
{
    using type = Value_type<P>;
};

template <Position P>
requires Loadable<P>
struct difference_type_t<loadable_position<P>>
{
    using type = Difference_type<P>;
};

template <Position P>
requires Loadable<P>
struct underlying_type_t<loadable_position<P>>
{
    using type = Underlying_type<P>;
};

template <Position P>
constexpr void
increment(loadable_position<P>& pos)
{
    increment(pos.pos);
}

template <Position P>
constexpr void
decrement(loadable_position<P>& pos)
{
    decrement(pos.pos);
}

template <Position P>
constexpr auto
successor(loadable_position<P> const& pos) -> loadable_position<P>
{
    return loadable_position<P>(successor(pos.pos));
}

template <Position P>
constexpr auto
predecessor(loadable_position<P> const& pos) -> loadable_position<P>
{
    return loadable_position<P>(predecessor(pos.pos));
}

template <Position P>
requires Loadable<P>
constexpr auto
load(loadable_position<P> const& pos) -> Value_type<P> const&
{
    return load(pos.pos);
}

template <Position P>
constexpr auto
precedes(loadable_position<P> const& pos0, loadable_position<P> const& pos1) -> bool
{
    return pos0.pos != pos1.pos;
}

}
