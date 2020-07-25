#pragma once

#include "invocable.h"
#include "cursor.h"

namespace elements {

template <typename T0, typename T1 = T0>
struct pair
{
    T0 m0;
    T1 m1;

    constexpr
    pair()
        : m0()
        , m1()
    {}

    constexpr
    pair(T0 member0, T1 member1)
        : m0(mv(member0))
        , m1(mv(member1))
    {}

    template <Unary_function F>
    requires
        Same_as<T0, T1> and
        Same_as<Decay<T0>, Domain<F>> and
        Same_as<Decay<T0>, Codomain<F>>
    constexpr auto
    fmap(F fun) -> pair<T0>&
    {
        m0 = invoke(fun, mv(m0));
        m1 = invoke(fun, mv(m1));
        return *this;
    }

    template <Unary_function F>
    requires
        Same_as<T0, T1> and
        Same_as<Decay<T0>, Domain<F>>
    constexpr auto
    fmap(F fun) const -> pair<Codomain<F>>
    {
        return {invoke(fun, m0), invoke(fun, m1)};
    }
};

template <typename T>
struct value_type_t<pair<T, T>>
{
    using type = T;
};

template <typename, int>
struct element_type_t;

template <typename T, int i>
using Element_type = typename element_type_t<T, i>::type;

template <typename T0, typename T1>
struct element_type_t<pair<T0, T1>, 0>
{
    using type = T0;
};

template <typename T0, typename T1>
struct element_type_t<pair<T0, T1>, 1>
{
    using type = T1;
};

template <typename T0, typename T1>
requires
    Regular<Remove_const<T0>> and
    Regular<Remove_const<T1>>
constexpr auto
operator==(pair<T0, T1> const& x, pair<T0, T1> const& y) -> bool
{
    return x.m0 == y.m0 and x.m1 == y.m1;
}

template <typename T0, typename T1>
requires
    Totally_ordered<Remove_const<T0>> and
    Totally_ordered<Remove_const<T1>>
constexpr auto
operator<(pair<T0, T1> const& x, pair<T0, T1> const& y) -> bool
{
    return x.m0 < y.m0 or (!(y.m0 < x.m0) and x.m1 < y.m1);
}

template<int32_t>
struct pair_get;

template <>
struct pair_get<0>
{
    template <typename T0, typename T1>
    static constexpr auto
    get(pair<T0, T1>& x) noexcept -> T0&
    {
        return x.m0;
    }

    template <typename T0, typename T1>
    static constexpr auto
    get_const(pair<T0, T1> const& x) noexcept -> T0 const&
    {
        return x.m0;
    }

    template <typename T0, typename T1>
    static constexpr auto
    get_move(pair<T0, T1>&& x) noexcept -> T0&&
    {
        return std::forward<T0>(x.m0);
    }

    template <typename T0, typename T1>
    static constexpr auto
    get_move_const(pair<T0, T1> const&& x) noexcept -> T0 const&&
    {
        return std::forward<const T0>(x.m0);
    }
};

template <>
struct pair_get<1>
{
    template <typename T0, typename T1>
    static constexpr auto
    get(pair<T0, T1>& x) noexcept -> T1&
    {
        return x.m1;
    }

    template <typename T0, typename T1>
    static constexpr auto
    get_const(pair<T0, T1> const& x) noexcept -> T1 const&
    {
        return x.m1;
    }

    template <typename T0, typename T1>
    static constexpr auto
    get_move(pair<T0, T1>&& x) noexcept -> T1&&
    {
        return std::forward<T1>(x.m1);
    }

    template <typename T0, typename T1>
    static constexpr auto
    get_move_const(pair<T0, T1> const&& x) noexcept -> T1 const&&
    {
        return std::forward<const T1>(x.m1);
    }
};

template <int i, typename T0, typename T1>
constexpr auto
get(pair<T0, T1>& x) noexcept -> Element_type<pair<T0, T1>, i>&
{
    return pair_get<i>::get(x);
}

template <int i, typename T0, typename T1>
constexpr auto
get(pair<T0, T1> const& x) noexcept -> Element_type<pair<T0, T1>, i> const&
{
    return pair_get<i>::get_const(x);
}

template <int i, typename T0, typename T1>
constexpr auto
get(pair<T0, T1>&& x) noexcept -> Element_type<pair<T0, T1>, i>&&
{
    return pair_get<i>::get_move(mv(x));
}

template <int i, typename T0, typename T1>
constexpr auto
get(pair<T0, T1> const&& x) noexcept -> Element_type<pair<T0, T1>, i> const&&
{
    return pair_get<i>::get_move_const(mv(x));
}

template <typename T0, typename T1>
constexpr auto
get(pair<T0, T1>& x) noexcept -> T0&
{
    return x.m0;
}

template <typename T0, typename T1>
constexpr auto
get(pair<T0, T1> const& x) noexcept -> T0 const&
{
    return x.m0;
}

template <typename T0, typename T1>
constexpr auto
get(pair<T0, T1>&& x) noexcept -> T0&&
{
    return mv(x.m0);
}

template <typename T0, typename T1>
constexpr auto
get(pair<T0, T1> const&& x) noexcept -> T0 const&&
{
    return mv(x.m0);
}

template <typename T0, typename T1>
constexpr auto
get(pair<T1, T0>& x) noexcept -> T0&
{
    return x.m1;
}

template <typename T0, typename T1>
constexpr auto
get(pair<T1, T0> const& x) noexcept -> T0 const&
{
    return x.m1;
}

template <typename T0, typename T1>
constexpr auto
get(pair<T1, T0>&& x) noexcept -> T0&&
{
    return mv(x.m1);
}

template <typename T0, typename T1>
constexpr auto
get(pair<T1, T0> const&& x) noexcept -> T0 const&&
{
    return mv(x.m1);
}

}
