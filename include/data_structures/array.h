#pragma once

#include <cstddef>
#include <initializer_list>
#include <limits>

#include "algorithms/algebra.h"
#include "algorithms/copy.h"
#include "algorithms/lexicographical.h"
#include "algorithms/map.h"
#include "algorithms/ordering.h"
#include "algorithms/position.h"
#include "algorithms/swap.h"
#include "concepts/invocable.h"
#include "concepts/position.h"
#include "concepts/regular.h"

namespace elements {

template <Range T>
constexpr auto
linearizable_equal(T const& x, T const& y) -> bool
{
    return lexicographical_equal(first(x), limit(x), first(y), limit(y));
}

template <Range T>
constexpr auto
linearizable_less(T const& x, T const& y) -> bool
{
    return lexicographical_less(first(x), limit(x), first(y), limit(y));
}

template <typename T>
requires Semiregular<Remove_cv<T>>
struct array_prefix
{
    Pointer_type<T> limit;
    Pointer_type<T> limit_of_storage;
    T x;
};

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr auto
allocate_array(Difference_type<Pointer_type<T>> n) -> Pointer_type<array_prefix<T>>
{
    using P = Pointer_type<array_prefix<T>>;
    if (is_zero(n)) return P{nullptr};
    P remote{reinterpret_cast<Pointer_type<array_prefix<T>>>(
        std::malloc(
            sizeof(array_prefix<T>) +
            static_cast<std::size_t>(
                (predecessor(n) * static_cast<pointer_diff>(sizeof(T))))))};
    Pointer_type<T> first{pointer_to(at(remote).x)};
    at(remote).limit = first;
    at(remote).limit_of_storage = first + n;
    return remote;
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr void
deallocate_array(Pointer_type<array_prefix<T>> prefix)
{
    std::free(prefix);
}

template <typename T>
requires Semiregular<Remove_cv<T>>
struct array
{
    Pointer_type<array_prefix<T>> header{nullptr};

    constexpr
    array() = default;

    constexpr
    array(array const& x)
        : header{allocate_array<T>(size(x))}
    {
        at(header).limit = copy(first(x), limit(x), first(at(this)));
    }

    constexpr
    array(array&& x)
    {
        header = x.header;
        x.header = Pointer_type<array_prefix<T>>{nullptr};
    }

    constexpr
    array(std::initializer_list<T> x)
        : header{allocate_array<T>(Size_type<array<T>>{static_cast<pointer_diff>(std::size(x))})}
    {
        at(header).limit = copy(std::begin(x), std::end(x), first(at(this)));
    }

    constexpr
    array(Size_type<array<T>> capacity)
        : header{allocate_array<T>(capacity)}
    {}

    constexpr
    array(Size_type<array<T>> size, Size_type<array<T>> capacity, T const& x)
        : header{allocate_array<T>(capacity)}
    {
        while (!is_zero(size)) { push(at(this), x); decrement(size); }
    }

    constexpr auto
    operator=(array const& x) -> array&
    {
        array temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(array&& x) -> array&
    {
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(header, x.header);
        }
        return at(this);
    }

    ~array()
    {
        erase_all(at(this));
    }

    constexpr auto
    operator[](pointer_diff i) -> T&
    {
        return at(first(at(this)) + i);
    }

    constexpr auto
    operator[](pointer_diff i) const -> T const&
    {
        return at(first(at(this)) + i);
    }

    template <Unary_function Fun>
    requires
        Same<Decay<T>, Domain<Fun>> and
        Same<Decay<T>, Codomain<Fun>>
    constexpr auto
    map(Fun fun) -> array<T>&
    {
        using elements::map;
        map(first(*this), limit(*this), first(*this), fun);
        return *this;
    }

    template <Unary_function Fun>
    requires Same<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> array<Codomain<Fun>>
    {
        using elements::map;
        array<Codomain<Fun>> x;
        reserve(x, size(*this));
        map(first(*this), limit(*this), first(x), fun);
        at(x.header).limit = at(x.header).limit_of_storage;
        return x;
    }

    template <Unary_function Fun>
    requires Same<Decay<T>, Domain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> Codomain<Fun>
    {
        Codomain<Fun> x;
        auto src = first(*this);
        auto lim = limit(*this);
        while (precedes(src, lim)) {
            auto y = fun(load(src));
            reserve(x, size(x) + size(y));
            auto dst = first(x) + size(x);
            copy(first(y), limit(y), dst);
            at(x.header).limit = at(x.header).limit_of_storage;
            increment(src);
        }
        return x;
    }
};

template <typename T>
requires Semiregular<Remove_cv<T>>
struct underlying_type_t<array<T>>
{
    using type = struct { Pointer_type<array_prefix<T>> header; };
};

template <typename T>
requires Semiregular<Remove_cv<T>>
struct value_type_t<array<T>>
{
    using type = T;
};

template <typename T>
requires Semiregular<Remove_cv<T>>
struct position_type_t<array<T>>
{
    using type = Pointer_type<T>;
};

template <typename T>
requires Semiregular<Remove_cv<T>>
struct position_type_t<array<T> const>
{
    using type = Pointer_type<T const>;
};

template <typename T>
requires Semiregular<Remove_cv<T>>
struct size_type_t<array<T>>
{
    using type = pointer_diff;
};

template <typename T>
requires Regular<Remove_cv<T>>
constexpr auto
operator==(array<T> const& x, array<T> const& y) -> bool
{
    return linearizable_equal(x, y);
}

template <typename T>
requires Regular<Remove_cv<T>>
constexpr auto
operator<(array<T> const& x, array<T> const& y) -> bool
{
    return linearizable_less(x, y);
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr void
reserve(array<T>& x, Size_type<array<T>> n)
{
    if (n < size(x) or n == capacity(x)) return;
    array<Underlying_type<T>> temp(n);
    at(temp.header).limit = copy(first(x), limit(x), first(temp));
    swap(x, temp);
}

template <typename T, typename U>
requires
    Semiregular<Remove_cv<T>> and
    Semiregular<Remove_cv<U>> and
    Constructible<T, U>
constexpr auto
insert(back<array<T>> arr, U const& x) -> back<array<T>>
{
    auto& seq = at(arr.sequence);
    if (is_full(seq))
    {
        auto n = size(at(arr.sequence));
        reserve(seq, max(Size_type<array<T>>{1}, n + n));
    }
    construct(at(load(seq.header).limit), x);
    increment(at(seq.header).limit);
    return seq;
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr void
push(array<T>& arr, T const& x)
{
    insert(back<array<T>>(arr), x);
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr auto
erase(back<array<T>> arr) -> back<array<T>>
{
    auto& header = at(arr.sequence).header;
    decrement(at(header).limit);
    destroy(at(load(header).limit));
    if (is_empty(at(arr.sequence))) {
        deallocate_array(header);
        header = Remove_ref<decltype(header)>{nullptr};
    }
    return arr;
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr void
erase_all(array<T>& x)
{
    while (!is_empty(x)) erase(back<array<T>>(x));
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr void
pop(array<T>& arr)
{
    erase(back<array<T>>(arr));
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr auto
first(array<T> const& x) -> Position_type<array<T>>
{
    if (x.header == Pointer_type<array_prefix<T>>{nullptr}) return Pointer_type<T>{nullptr};
    return pointer_to(at(x.header).x);
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr auto
limit(array<T> const& x) -> Position_type<array<T>>
{
    if (x.header == Pointer_type<array_prefix<T>>{nullptr}) return Pointer_type<T>{nullptr};
    return at(x.header).limit;
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr auto
limit_of_storage(array<T> const& x) -> Position_type<array<T>>
{
    if (x.header == Pointer_type<array_prefix<T>>{nullptr}) return Pointer_type<T>{nullptr};
    return at(x.header).limit_of_storage;
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr auto
is_empty(T const& x) -> bool
{
    return first(x) == limit(x);
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr auto
is_full(array<T> const& x) -> bool
{
    return limit(x) == limit_of_storage(x);
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr auto
size(array<T> const& x) -> Size_type<array<T>>
{
    return limit(x) - first(x);
}

template <typename T>
requires Semiregular<Remove_cv<T>>
constexpr auto
capacity(array<T> const& x) -> Size_type<array<T>>
{
    return limit_of_storage(x) - first(x);
}

}
