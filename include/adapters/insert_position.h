#pragma once

#include "concepts/position.h"

namespace elements {

template <Position P>
requires Storable<P>
struct insert_position
{
    P pos;

    explicit constexpr
    insert_position(P const& pos_)
        : pos{pos_}
    {}
};

template <Position P>
requires Storable<P>
struct value_type_t<insert_position<P>>
{
    using type = Value_type<P>;
};

template <Position P>
requires Storable<P>
constexpr auto
precedes(insert_position<P> const& pos0, P const& pos1) -> bool
{
    return pos0.pos != pos1;
}

template <Position P>
requires Storable<P>
constexpr auto
precedes(P const& pos0, insert_position<P> const& pos1) -> bool
{
    return !(precedes(pos1, pos0));
}

template <Position P>
requires Storable<P>
constexpr void
increment(insert_position<P>&)
{}

template <Position P>
requires Storable<P>
constexpr auto
successor(insert_position<P> const& pos) -> insert_position<P>
{
    return insert_position{successor(pos.pos)};
}

template <Position P>
requires Storable<P>
constexpr void
store(insert_position<P>& pos, Value_type<P> const& x)
{
    pos.pos = insert(pos.pos, x);
}

template <Position P>
requires Storable<P>
constexpr void
store(insert_position<P>& pos, Value_type<P>&& x)
{
    pos.pos = insert(pos.pos, fw<Value_type<P>>(x));
}

struct insert_sink
{
    template <Position P>
    requires Storable<P>
    constexpr auto
    operator()(P const& pos)
    {
        return insert_position{pos};
    }
};

}
