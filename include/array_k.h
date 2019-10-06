#pragma once

#include "copy.h"
#include "functional.h"
#include "lexicographical.h"
#include "map.h"

namespace elements {

template <Semiregular T, pointer_diff k>
requires 0 < k and k <= std::numeric_limits<pointer_diff>::max() / sizeof(T)
struct array_k
{
    T data[static_cast<std::size_t>(k)];

    constexpr
    array_k()
        : data{}
    {}

    constexpr
    array_k(std::initializer_list<T> x)
        //[[expects: x.size() == k]]
        : data{}
    {
        copy(std::begin(x), std::end(x), data);
    }

    constexpr auto
    operator[](pointer_diff i) const -> T const&
    {
        return data[i];
    }

    constexpr auto
    operator[](pointer_diff i) -> T&
    {
        return data[i];
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<Decay<T>, Codomain<Fun>>
    constexpr auto
    map(Fun fun) -> array_k<T, k>&
    {
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> array_k<Codomain<Fun>, k>
    {
        using elements::map;
        array_k<Codomain<Fun>, k> x;
        map(first(at(this)), limit(at(this)), first(x), fun);
        return x;
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> array_k<Value_type<Codomain<Fun>>, k * Size<Codomain<Fun>>>
    {
        array_k<Value_type<Codomain<Fun>>, k * Size<Codomain<Fun>>> x;
        auto src = first(at(this));
        auto lim = limit(at(this));
        auto dst = first(x);
        while (precedes(src, lim)) {
            auto y = fun(load(src));
            dst = copy(first(y), limit(y), dst);
            increment(src);
        }
        return x;
    }
};

template <Semiregular T, pointer_diff k>
struct value_type_t<array_k<T, k>>
{
    using type = T;
};

template <Semiregular T, pointer_diff k>
struct position_type_t<array_k<T, k>>
{
    using type = Pointer_type<T>;
};

template <Semiregular T, pointer_diff k>
struct position_type_t<array_k<T, k> const>
{
    using type = Pointer_type<T const>;
};

template <Semiregular T, pointer_diff k>
struct size_type_t<array_k<T, k>>
{
    using type = Difference_type<Pointer_type<T>>;
    static constexpr auto value = k;
};

template <Regular T, pointer_diff k>
constexpr auto
operator==(array_k<T, k> const& x, array_k<T, k> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T, pointer_diff k>
constexpr auto
operator<(array_k<T, k> const& x, array_k<T, k> const& y) -> bool
{
    return less_range(x, y);
}

template <typename T, pointer_diff k>
constexpr auto
first(array_k<T, k> const& x) -> Pointer_type<T const>
{
    return pointer_to(x.data[0]);
}

template <typename T, pointer_diff k>
constexpr auto
first(array_k<T, k>& x) -> Pointer_type<T>
{
    return pointer_to(x.data[0]);
}

template <typename T, pointer_diff k>
constexpr auto
limit(array_k<T, k> const& x) -> Pointer_type<T const>
{
    return first(x) + k;
}

template <typename T, pointer_diff k>
constexpr auto
limit(array_k<T, k>& x) -> Pointer_type<T>
{
    return first(x) + k;
}

template <typename T, pointer_diff k>
constexpr auto
last(array_k<T, k>& x) -> Pointer_type<T>
{
    return limit(x) - 1;
}

template <typename T, pointer_diff k>
constexpr auto
is_empty(array_k<T, k> const&) -> bool
{
    return false;
}

template <typename T, pointer_diff k>
constexpr auto
size(array_k<T, k> const&) -> Size_type<array_k<T, k>>
{
    return k;
}

}
