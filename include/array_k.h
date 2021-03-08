#pragma once

#include "copy.h"
#include "fill.h"
#include "functional.h"
#include "lexicographical.h"
#include "map.h"

namespace elements {

template <Semiregular T, pointer_diff k>
requires (0 < k) and (k <= std::numeric_limits<pointer_diff>::max() / sizeof(T))
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
        using elements::copy;
        copy(first(x), limit(x), data);
    }

    explicit constexpr
    array_k(T const& x)
        : data{}
    {
        fill(data, data + k, x);
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
};

template <Semiregular T, pointer_diff k>
struct value_type_t<array_k<T, k>>
{
    using type = T;
};

template <Semiregular T, pointer_diff k>
struct cursor_type_t<array_k<T, k>>
{
    using type = Pointer_type<T>;
};

template <Semiregular T, pointer_diff k>
struct cursor_type_t<array_k<T, k> const>
{
    using type = Pointer_type<T const>;
};

template <Semiregular T, pointer_diff k>
struct size_type_t<array_k<T, k>>
{
    using type = Difference_type<Pointer_type<T>>;
    static constexpr auto value = k;
};

template <Semiregular T, pointer_diff k>
struct functor_t<array_k<T, k>>
{
    using constructor_type = array_k<T, k>;

    template <Operation<T> Op>
    static constexpr auto
    fmap(array_k<T, k>& x, Op op) -> array_k<T, k>&
    {
        using elements::copy;
        copy(first(x), limit(x), map_sink{op}(first(x)));
        return x;
    }

    template <Regular_invocable<T> F>
    static constexpr auto
    fmap(array_k<T, k>&& x, F fun) -> array_k<Return_type<F, T>, k>
    {
        using elements::map;
        array_k<Return_type<F, T>, k> y;
        map(first(x), limit(x), first(y), fun);
        return y;
    }
};

template <Semiregular T, pointer_diff k>
struct monad_t<array_k<T, k>>
{
    template <Regular_invocable<T> F>
    static constexpr auto
    bind(array_k<T, k> const& x, F fun) -> array_k<Value_type<Return_type<F, T>>, k * Size<Return_type<F, T>>>
    {
        array_k<Value_type<Return_type<F, T>>, k * Size<Return_type<F, T>>> y;
        auto src = first(x);
        auto lim = limit(x);
        auto dst = first(y);
        while (precedes(src, lim)) {
            auto z{fun(load(src))};
            dst = copy(first(z), limit(z), dst);
            increment(src);
        }
        return y;
    }
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
