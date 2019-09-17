#pragma once

#include "concepts/position.h"

namespace elements {

template <Position P, Unary_function F>
requires Storable<P>
struct map_position
{
    P pos;
    F fun;

    constexpr
    map_position(P const& pos_, F fun_)
        : pos{pos_}
        , fun{fun_}
    {}
};

template <Position P, Unary_function F>
requires Storable<P>
struct value_type_t<map_position<P, F>>
{
    using type = Value_type<P>;
};

template <Position P, Unary_function F>
requires Storable<P>
constexpr auto
precedes(map_position<P, F> const& pos0, P const& pos1) -> bool
{
    return pos0.pos != pos1;
}

template <Position P, Unary_function F>
requires Storable<P>
constexpr auto
precedes(P const& pos0, map_position<P, F> const& pos1) -> bool
{
    return !(precedes(pos1, pos0));
}

template <Position P, Unary_function F>
requires Storable<P>
constexpr void
increment(map_position<P, F>& pos)
{
    increment(pos.pos);
}

template <Forward_position P, Unary_function F>
requires Storable<P>
constexpr void
decrement(map_position<P, F>& pos)
{
    decrement(pos.pos);
}

template <Position P, Unary_function F>
requires Storable<P>
constexpr auto
successor(map_position<P, F> const& pos) -> map_position<P, F>
{
    return map_position{successor(pos.pos)};
}

template <Forward_position P, Unary_function F>
requires Storable<P>
constexpr auto
predecessor(map_position<P, F> const& pos) -> map_position<P, F>
{
    return map_position{predecessor(pos.pos)};
}

template <Position P, Unary_function F>
requires Storable<P>
constexpr void
store(map_position<P, F>& pos, Value_type<P> const& x)
{
    store(at(pos.pos), pos.fun(x));
}

template <Position P, Unary_function F>
requires Storable<P>
constexpr void
store(map_position<P, F>& pos, Value_type<P>&& x)
{
    store(at(pos.pos), pos.fun(fw<Value_type<P>>(x)));
}

template <Unary_function F>
struct map_sink
{
    F fun;

    explicit constexpr
    map_sink(F fun_)
        : fun{fun_}
    {}

    template <Position P>
    requires Storable<P>
    constexpr auto
    operator()(P const& pos)
    {
        return map_position{pos, fun};
    }
};

}
