#pragma once

#include "algorithms/position.h"
#include "concepts/position.h"

namespace elements {

template <Position P>
struct counted_position
{
    P pos{};
    Difference_type<P> count = Zero<Difference_type<P>>;

    constexpr
    counted_position() = default;

    explicit constexpr
    counted_position(P const& pos_, Difference_type<P> const& count_ = Zero<Difference_type<P>>)
        : pos{pos_}
        , count{count_}
    {}
};


template <Position P>
struct value_type_t<counted_position<P>>
{
    using type = Value_type<P>;
};

template <Position P>
struct difference_type_t<counted_position<P>>
{
    using type = Difference_type<P>;
};

template <Position P>
struct underlying_type_t<counted_position<P>>
{
    using type = counted_position<Underlying_type<P>>;
};

template <Position P>
constexpr auto
operator==(counted_position<P> const& x, counted_position<P> const& y) -> bool
{
    return x.pos == y.pos and x.count == y.count;
}

template <Position P>
constexpr void
increment(counted_position<P>& pos)
{
    increment(pos.pos);
    increment(pos.count);
}

template <Bidirectional_position P>
constexpr void
decrement(counted_position<P>& pos)
{
    decrement(pos.pos);
    decrement(pos.count);
}

template <Position P>
constexpr auto
successor(counted_position<P> const& pos) -> counted_position<P>
{
    return counted_position<P>{successor(pos.pos), successor(pos.count)};
}

template <Bidirectional_position P>
constexpr auto
predecessor(counted_position<P> const& pos) -> counted_position<P>
{
    return counted_position<P>{predecessor(pos.pos), predecessor(pos.count)};
}

template <Position P>
requires Loadable<P>
constexpr auto
load(counted_position<P> const& pos) -> Value_type<P> const&
{
    return load(pos.pos);
}

template <Position P>
requires Storable<P>
constexpr void
store(counted_position<P>& pos, Value_type<P> const& value)
{
    store(pos.pos, value);
}

template <Position P>
requires Storable<P>
constexpr void
store(counted_position<P>& pos, Value_type<P>&& value)
{
    store(pos.pos, std::forward<Value_type<P>>(value));
}

template <Position P>
requires Mutable<P>
constexpr auto
at(counted_position<P>& pos) -> Value_type<P>&
{
    return at(pos.pos);
}

template <Position P, Regular C>
requires Equality_comparable_with<Difference_type<P>, C>
constexpr auto
precedes(counted_position<P> const& pos, C const& count) -> bool
{
    return pos.count != count;
}

}
