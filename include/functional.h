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

template <typename T, typename F>
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

template <Mutable_range R, Regular_invocable<Value_type<R>> F>
constexpr auto
operator&(R& x, F fun) -> Return_type<F, Value_type<R>>
{
    return flat_map(first(x), limit(x), fun);
}

template <typename T, Regular_invocable<T> F>
constexpr auto
operator&(T&& x, F fun) -> Return_type<F, T>
{
    return x.flat_map(fun);
}

template <typename T, typename... Fs>
constexpr Monad auto
chain(T x, Fs... funcs)
{
    return (x & ... & funcs);
}

template <Cursor C, typename P>
requires Storable<C>
struct filter_cursor
{
    C cur{};
    P pred;

    constexpr
    filter_cursor(C cur_, P pred_)
        : cur{cur_}
        , pred{mv(pred_)}
    {}
};

template <Cursor C, typename P>
requires Storable<C>
struct value_type_t<filter_cursor<C, P>>
{
    using type = Value_type<C>;
};

template <Cursor C, typename P>
requires Storable<C>
constexpr auto
precedes(filter_cursor<C, P> const& cur0, C const& cur1) -> bool
{
    return cur0.cur != cur1;
}

template <Cursor C, typename P>
requires Storable<C>
constexpr auto
precedes(C const& cur0, filter_cursor<C, P> const& cur1) -> bool
{
    return !(precedes(cur1, cur0));
}

template <Cursor C, typename P>
requires Storable<C>
constexpr void
increment(filter_cursor<C, P>&)
{}

template <Cursor C, typename P>
requires Storable<C>
constexpr auto
successor(filter_cursor<C, P> const& cur) -> filter_cursor<C, P>
{
    return filter_cursor{successor(cur.cur)};
}

template <Cursor C, typename P>
requires Storable<C>
constexpr void
store(filter_cursor<C, P>& cur, Value_type<C> const& x)
{
    if (cur.pred(x)) {
        store(at(cur.cur), x);
        increment(cur.cur);
    }
}

template <Cursor C, typename P>
requires Storable<C>
constexpr void
store(filter_cursor<C, P>& cur, Value_type<C>&& x)
{
    if (cur.pred(x)) {
        store(at(cur.cur), fw<Value_type<C>>(x));
        increment(cur.cur);
    }
}

template <typename P>
struct filter_sink
{
    P pred;

    explicit constexpr
    filter_sink(P pred_)
        : pred{mv(pred_)}
    {}

    template <Cursor C>
    requires Storable<C>
    constexpr auto
    operator()(C cur)
    {
        return filter_cursor{cur, pred};
    }
};

template <Cursor C, typename F>
requires Storable<C>
struct map_cursor
{
    C cur;
    F fun;

    constexpr
    map_cursor(C cur_, F fun_)
        : cur{cur_}
        , fun{mv(fun_)}
    {}
};

template <Cursor C, typename F>
requires Storable<C>
struct value_type_t<map_cursor<C, F>>
{
    using type = Value_type<C>;
};

template <Cursor C, typename F>
requires Storable<C>
constexpr auto
precedes(map_cursor<C, F> const& cur0, C const& cur1) -> bool
{
    return cur0.cur != cur1;
}

template <Cursor C, typename F>
requires Storable<C>
constexpr auto
precedes(C const& cur0, map_cursor<C, F> const& cur1) -> bool
{
    return !(precedes(cur1, cur0));
}

template <Cursor C, typename F>
requires Storable<C>
constexpr void
increment(map_cursor<C, F>& cur)
{
    increment(cur.cur);
}

template <Cursor C, typename F>
requires Storable<C>
constexpr auto
successor(map_cursor<C, F> const& cur) -> map_cursor<C, F>
{
    return map_cursor{successor(cur.cur)};
}

template <Cursor C, typename F, typename T>
requires Storable<C>
constexpr void
store(map_cursor<C, F>& cur, T const& x)
{
    store(at(cur.cur), cur.fun(x));
}

template <Cursor C, typename F, typename T>
requires Storable<C>
constexpr void
store(map_cursor<C, F>& cur, T&& x)
{
    store(at(cur.cur), cur.fun(fw<T>(x)));
}

template <typename F>
struct map_sink
{
    F fun;

    explicit constexpr
    map_sink(F fun_)
        : fun{mv(fun_)}
    {}

    template <Cursor C>
    requires Storable<C>
    constexpr auto
    operator()(C cur)
    {
        return map_cursor{cur, fun};
    }
};

}
