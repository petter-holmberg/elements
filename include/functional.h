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

template <typename T, Unary_function F>
constexpr auto
fmap(T&& x, F fun) -> decltype(x.fmap(fun))
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

template <Mutable_range R, Unary_function F>
requires Convertible_to<Value_type<R>, Domain<F>>
constexpr auto
operator&(R& x, F fun) -> Codomain<F>
{
    return flat_map(first(x), limit(x), fun);
}

template <typename T, Unary_function F>
constexpr auto
operator&(T&& x, F fun) -> Codomain<F>
{
    return x.flat_map(fun);
}

template <typename T, Unary_function... Fs>
constexpr Monad auto
chain(T x, Fs... funcs)
{
    return (x & ... & funcs);
}

template <Cursor C, Unary_predicate P>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Domain<P>>
struct filter_cursor
{
    C cur{};
    std::function<Codomain<P>(Input_type<P, 0>)> pred;

    constexpr
    filter_cursor(C cur_, P pred_)
        : cur{cur_}
        , pred{mv(pred_)}
    {}
};

template <Cursor C, Unary_predicate P>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Domain<P>>
struct value_type_t<filter_cursor<C, P>>
{
    using type = Value_type<C>;
};

template <Cursor C, Unary_predicate P>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Domain<P>>
constexpr auto
precedes(filter_cursor<C, P> const& cur0, C const& cur1) -> bool
{
    return cur0.cur != cur1;
}

template <Cursor C, Unary_predicate P>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Domain<P>>
constexpr auto
precedes(C const& cur0, filter_cursor<C, P> const& cur1) -> bool
{
    return !(precedes(cur1, cur0));
}

template <Cursor C, Unary_predicate P>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Domain<P>>
constexpr void
increment(filter_cursor<C, P>&)
{}

template <Cursor C, Unary_predicate P>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Domain<P>>
constexpr auto
successor(filter_cursor<C, P> const& cur) -> filter_cursor<C, P>
{
    return filter_cursor{successor(cur.cur)};
}

template <Cursor C, Unary_predicate P>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Domain<P>>
constexpr void
store(filter_cursor<C, P>& cur, Value_type<C> const& x)
{
    if (cur.pred(x)) {
        store(at(cur.cur), x);
        increment(cur.cur);
    }
}

template <Cursor C, Unary_predicate P>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Domain<P>>
constexpr void
store(filter_cursor<C, P>& cur, Value_type<C>&& x)
{
    if (cur.pred(x)) {
        store(at(cur.cur), fw<Value_type<C>>(x));
        increment(cur.cur);
    }
}

template <Unary_predicate P>
struct filter_sink
{
    std::function<Codomain<P>(Input_type<P, 0>)> pred;

    explicit constexpr
    filter_sink(P pred_)
        : pred{mv(pred_)}
    {}

    template <Cursor C>
    requires
        Storable<C> and
        Same_as<Decay<Value_type<C>>, Domain<P>>
    constexpr auto
    operator()(C cur)
    {
        return filter_cursor{cur, pred};
    }
};

template <Cursor C, Unary_function F>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Codomain<F>>
struct map_cursor
{
    C cur;
    std::function<Codomain<F>(Input_type<F, 0>)> fun;

    constexpr
    map_cursor(C cur_, F fun_)
        : cur{cur_}
        , fun{mv(fun_)}
    {}
};

template <Cursor C, Unary_function F>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Codomain<F>>
struct value_type_t<map_cursor<C, F>>
{
    using type = Value_type<C>;
};

template <Cursor C, Unary_function F>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Codomain<F>>
constexpr auto
precedes(map_cursor<C, F> const& cur0, C const& cur1) -> bool
{
    return cur0.cur != cur1;
}

template <Cursor C, Unary_function F>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Codomain<F>>
constexpr auto
precedes(C const& cur0, map_cursor<C, F> const& cur1) -> bool
{
    return !(precedes(cur1, cur0));
}

template <Cursor C, Unary_function F>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Codomain<F>>
constexpr void
increment(map_cursor<C, F>& cur)
{
    increment(cur.cur);
}

template <Cursor C, Unary_function F>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Codomain<F>>
constexpr auto
successor(map_cursor<C, F> const& cur) -> map_cursor<C, F>
{
    return map_cursor{successor(cur.cur)};
}

template <Cursor C, Unary_function F>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Codomain<F>>
constexpr void
store(map_cursor<C, F>& cur, Domain<F> const& x)
{
    store(at(cur.cur), cur.fun(x));
}

template <Cursor C, Unary_function F>
requires
    Storable<C> and
    Same_as<Decay<Value_type<C>>, Codomain<F>>
constexpr void
store(map_cursor<C, F>& cur, Domain<F>&& x)
{
    store(at(cur.cur), cur.fun(fw<Domain<F>>(x)));
}

template <Unary_function F>
struct map_sink
{
    std::function<Codomain<F>(Input_type<F, 0>)> fun;

    explicit constexpr
    map_sink(F fun_)
        : fun{mv(fun_)}
    {}

    template <Cursor C>
    requires
        Storable<C> and
        Same_as<Decay<Value_type<C>>, Codomain<F>>
    constexpr auto
    operator()(C cur)
    {
        return map_cursor{cur, fun};
    }
};

}
