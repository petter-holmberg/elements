#pragma once

#include "algorithms/position.h"
#include "concepts/position.h"

namespace elements {

template <Bidirectional_position P>
struct reverse_position
{
    P pos{};

    constexpr
    reverse_position() = default;

    explicit constexpr
    reverse_position(P const& pos_) : pos{pos_} {}
};

template <Bidirectional_position P>
struct value_type_t<reverse_position<P>>
{
    using type = Value_type<P>;
};

template <Bidirectional_position P>
struct difference_type_t<reverse_position<P>>
{
    using type = Difference_type<P>;
};

template <Bidirectional_position P>
constexpr auto
operator==(reverse_position<P> const& x, reverse_position<P> const& y) -> bool
{
    return x.pos == y.pos;
}

template <Bidirectional_position P>
constexpr void
increment(reverse_position<P>& pos)
{
    decrement(pos.pos);
}

template <Bidirectional_position P>
constexpr void
decrement(reverse_position<P>& pos)
{
    increment(pos.pos);
}

template <Bidirectional_position P>
constexpr auto
successor(reverse_position<P> const& pos) -> reverse_position<P>
{
    return reverse_position<P>{predecessor(pos.pos)};
}

template <Bidirectional_position P>
constexpr auto
predecessor(reverse_position<P> const& pos) -> reverse_position<P>
{
    return reverse_position<P>{successor(pos.pos)};
}

template <Bidirectional_position P>
requires Loadable<P>
constexpr auto
load(reverse_position<P> const& pos) -> Value_type<P> const&
{
    return load(predecessor(pos.pos));
}

template <Bidirectional_position P>
requires Storable<P>
constexpr void
store(reverse_position<P>& pos, Value_type<P> const& value)
{
    store(predecessor(pos.pos), value);
}

template <Bidirectional_position P>
requires Storable<P>
constexpr void
store(reverse_position<P>& pos, Value_type<P>&& value)
{
    store(pos.pos, fw<Value_type<P>>(value));
}

template <Bidirectional_position P>
requires Mutable<P>
constexpr auto
at(reverse_position<P> const& pos) -> Value_type<P> const&
{
    return at(predecessor(pos.pos));
}

template <Bidirectional_position P>
requires Mutable<P>
constexpr auto
at(reverse_position<P>& pos) -> Value_type<P>&
{
    return at(predecessor(pos.pos));
}

template <Bidirectional_position P>
constexpr auto
precedes(reverse_position<P> const& pos0, reverse_position<P> const& pos1) -> bool
{
    return pos0.pos != pos1.pos;
}

}
