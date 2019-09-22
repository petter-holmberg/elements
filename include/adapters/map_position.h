#pragma once

#include "concepts/position.h"

namespace elements {

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Codomain<F>>
struct map_position
{
    P pos;
    std::function<Codomain<F>(Input_type<F, 0>)> fun;

    constexpr
    map_position(P const& pos_, F fun_)
        : pos{pos_}
        , fun{mv(fun_)}
    {}
};

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Codomain<F>>
struct value_type_t<map_position<P, F>>
{
    using type = Value_type<P>;
};

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Codomain<F>>
constexpr auto
precedes(map_position<P, F> const& pos0, P const& pos1) -> bool
{
    return pos0.pos != pos1;
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Codomain<F>>
constexpr auto
precedes(P const& pos0, map_position<P, F> const& pos1) -> bool
{
    return !(precedes(pos1, pos0));
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Codomain<F>>
constexpr void
increment(map_position<P, F>& pos)
{
    increment(pos.pos);
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Codomain<F>>
constexpr auto
successor(map_position<P, F> const& pos) -> map_position<P, F>
{
    return map_position{successor(pos.pos)};
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Codomain<F>>
constexpr void
store(map_position<P, F>& pos, Domain<F> const& x)
{
    store(at(pos.pos), pos.fun(x));
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Codomain<F>>
constexpr void
store(map_position<P, F>& pos, Domain<F>&& x)
{
    store(at(pos.pos), pos.fun(fw<Domain<F>>(x)));
}

template <Unary_function F>
struct map_sink
{
    std::function<Codomain<F>(Input_type<F, 0>)> fun;

    explicit constexpr
    map_sink(F fun_)
        : fun{mv(fun_)}
    {}

    template <Position P>
    requires
        Storable<P> and
        Same<Decay<Value_type<P>>, Codomain<F>>
    constexpr auto
    operator()(P const& pos)
    {
        return map_position{pos, fun};
    }
};

}
