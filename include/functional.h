#pragma once

#include "flat_map.h"
#include "invocable.h"
#include "range.h"

namespace elements {

template <typename T>
concept Functor =
    Movable<T> and
    requires (T&& x, Value_type<T>(&fun)(Value_type<T>)) {
        fmap(fw<T>(x), fun);
    };

template <typename T, Unary_function Fun>
constexpr auto
fmap(T&& x, Fun fun) -> decltype(x.fmap(fun))
{
    return x.fmap(fun);
}

template <typename T>
constexpr auto
unit(Value_type<T>&& x) -> T
{
    return T{fw<Value_type<T>>(x)};
}

template <typename M>
concept Monad =
    requires (M m, Value_type<M>&& x, M(&fun)(Value_type<M>)) {
        { unit<M>(fw<Value_type<M>>(x)) } -> Same_as<M>;
        m & fun;
    };

template <Mutable_range R, Unary_function Fun>
requires Convertible_to<Value_type<R>, Domain<Fun>>
constexpr auto
operator&(R& x, Fun fun) -> Codomain<Fun>
{
    return flat_map(first(x), limit(x), fun);
}

template <typename T, Unary_function Fun>
constexpr auto
operator&(T&& x, Fun fun) -> Codomain<Fun>
{
    return x.flat_map(fun);
}

template <typename T, Unary_function... Fs>
constexpr Monad auto
chain(T x, Fs... funcs)
{
    return (x & ... & funcs);
}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
struct filter_position
{
    P pos{};
    std::function<Codomain<U>(Input_type<U, 0>)> pred;

    constexpr
    filter_position(P const& pos_, U pred_)
        : pos{pos_}
        , pred{mv(pred_)}
    {}
};

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
struct value_type_t<filter_position<P, U>>
{
    using type = Value_type<P>;
};

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr auto
precedes(filter_position<P, U> const& pos0, P const& pos1) -> bool
{
    return pos0.pos != pos1;
}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr auto
precedes(P const& pos0, filter_position<P, U> const& pos1) -> bool
{
    return !(precedes(pos1, pos0));
}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr void
increment(filter_position<P, U>&)
{}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr auto
successor(filter_position<P, U> const& pos) -> filter_position<P, U>
{
    return filter_position{successor(pos.pos)};
}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr void
store(filter_position<P, U>& pos, Value_type<P> const& x)
{
    if (pos.pred(x)) {
        store(at(pos.pos), x);
        increment(pos.pos);
    }
}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr void
store(filter_position<P, U>& pos, Value_type<P>&& x)
{
    if (pos.pred(x)) {
        store(at(pos.pos), fw<Value_type<P>>(x));
        increment(pos.pos);
    }
}

template <Unary_predicate U>
struct filter_sink
{
    std::function<Codomain<U>(Input_type<U, 0>)> pred;

    explicit constexpr
    filter_sink(U pred_)
        : pred{mv(pred_)}
    {}

    template <Position P>
    requires
        Storable<P> and
        Same_as<Decay<Value_type<P>>, Domain<U>>
    constexpr auto
    operator()(P const& pos)
    {
        return filter_position{pos, pred};
    }
};

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Codomain<F>>
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
    Same_as<Decay<Value_type<P>>, Codomain<F>>
struct value_type_t<map_position<P, F>>
{
    using type = Value_type<P>;
};

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Codomain<F>>
constexpr auto
precedes(map_position<P, F> const& pos0, P const& pos1) -> bool
{
    return pos0.pos != pos1;
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Codomain<F>>
constexpr auto
precedes(P const& pos0, map_position<P, F> const& pos1) -> bool
{
    return !(precedes(pos1, pos0));
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Codomain<F>>
constexpr void
increment(map_position<P, F>& pos)
{
    increment(pos.pos);
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Codomain<F>>
constexpr auto
successor(map_position<P, F> const& pos) -> map_position<P, F>
{
    return map_position{successor(pos.pos)};
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Codomain<F>>
constexpr void
store(map_position<P, F>& pos, Domain<F> const& x)
{
    store(at(pos.pos), pos.fun(x));
}

template <Position P, Unary_function F>
requires
    Storable<P> and
    Same_as<Decay<Value_type<P>>, Codomain<F>>
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
        Same_as<Decay<Value_type<P>>, Codomain<F>>
    constexpr auto
    operator()(P const& pos)
    {
        return map_position{pos, fun};
    }
};

}
