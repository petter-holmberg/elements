#pragma once

#include <initializer_list>

#include "adapters/map_position.h"
#include "algorithms/algebra.h"
#include "algorithms/copy.h"
#include "algorithms/dynamic_sequence.h"
#include "algorithms/map.h"
#include "algorithms/ordering.h"
#include "algorithms/position.h"
#include "algorithms/swap.h"
#include "concepts/invocable.h"
#include "concepts/position.h"
#include "concepts/regular.h"

namespace elements {

template <Movable T>
struct array_prefix
{
    Pointer_type<T> limit;
    Pointer_type<T> limit_of_storage;
    T x;
};

template <Movable T>
constexpr auto
allocate_array(Difference_type<Pointer_type<T>> n) -> Pointer_type<array_prefix<T>>
{
    if (is_zero(n)) return nullptr;
    auto remote = allocate<array_prefix<T>>(predecessor(n) * static_cast<pointer_diff>(sizeof(T)));
    auto const& first = pointer_to(at(remote).x);
    at(remote).limit = first;
    at(remote).limit_of_storage = first + n;
    return remote;
}

template <Movable T>
constexpr void
deallocate_array(Pointer_type<array_prefix<T>> prefix)
{
    deallocate(prefix);
}

template <Movable T>
struct array
{
    Pointer_type<array_prefix<T>> header{nullptr};

    constexpr
    array() = default;

    constexpr
    array(array const& x)
        : header{allocate_array<T>(size(x))}
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    array(array&& x)
    {
        header = x.header;
        x.header = nullptr;
    }

    constexpr
    array(std::initializer_list<T> x)
        : header{allocate_array<T>(Size_type<array<T>>{static_cast<pointer_diff>(std::size(x))})}
    {
        insert_range(x, back{at(this)});
    }

    explicit constexpr
    array(Size_type<array<T>> capacity)
        : header{allocate_array<T>(capacity)}
    {}

    constexpr
    array(Size_type<array<T>> size, T const& x)
        : array(size, size, x)
    {}

    constexpr
    array(Size_type<array<T>> capacity, Size_type<array<T>> size, T const& x)
        : header{allocate_array<T>(capacity)}
    {
        while (!is_zero(size)) {
            push(at(this), x);
            decrement(size);
        }
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
        return load(first(at(this)) + i);
    }

    template <Unary_function Fun>
    requires
        Same<Decay<T>, Domain<Fun>> and
        Same<Decay<T>, Codomain<Fun>>
    constexpr auto
    map(Fun fun) -> array<T>&
    {
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> array<Codomain<Fun>>
    {
        using elements::map;
        array<Codomain<Fun>> x;
        reserve(x, size(at(this)));
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }

    template <Unary_function Fun>
    requires
        Same<Decay<T>, Domain<Fun>> and
        Same<array<Decay<T>>, Codomain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> array<T>&
    {
        using elements::flat_map;
        auto x = flat_map(first(at(this)), limit(at(this)), fun);
        swap(at(this), x);
        return at(this);
    }

    template <Unary_function Fun>
    requires
        Same<Decay<T>, Domain<Fun>> and
        Regular<T>
    constexpr auto
    flat_map(Fun fun) const -> Codomain<Fun>
    {
        using elements::flat_map;
        return flat_map(first(at(this)), limit(at(this)), fun);
    }
};

template <Movable T>
struct value_type_t<array<T>>
{
    using type = T;
};

template <Movable T>
struct position_type_t<array<T>>
{
    using type = Pointer_type<T>;
};

template <Movable T>
struct position_type_t<array<T> const>
{
    using type = Pointer_type<T const>;
};

template <Movable T>
struct size_type_t<array<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(array<T> const& x, array<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(array<T> const& x, array<T> const& y) -> bool
{
    return less_range(x, y);
}

template <Movable T>
constexpr void
swap(array<T>& x, array<T>& y)
{
    swap(x.header, y.header);
}

template <Movable T>
constexpr void
reserve(array<T>& x, Size_type<array<T>> n)
{
    if (n < size(x) or n == capacity(x)) return;
    array<T> temp(n);
    auto pos = first(x);
    auto dst = first(temp);
    while (precedes(pos, limit(x))) {
        construct(at(dst), mv(at(pos)));
        increment(pos);
        increment(dst);
    }
    at(temp.header).limit = dst;
    swap(x, temp);
}

template <Movable T, typename U>
constexpr auto
insert(back<array<T>> arr, U&& x) -> back<array<T>>
{
    auto& seq = at(arr.range);
    if (limit(seq) == limit_of_storage(seq)) {
        reserve(seq, max(One<Size_type<array<T>>>, twice(size(seq))));
    }
    auto& header = at(seq.header);
    construct(at(header.limit), fw<U>(x));
    increment(header.limit);
    return seq;
}

template <Movable T, typename U>
constexpr void
emplace(array<T>& arr, U&& x)
{
    insert(back<array<T>>(arr), fw<U>(x));
}

template <Movable T, typename U>
constexpr void
push(array<T>& arr, U x)
{
    insert(back<array<T>>(arr), mv(x));
}

template <Movable T>
constexpr auto
erase(back<array<T>> arr) -> back<array<T>>
{
    auto& header = at(arr.range).header;
    decrement(at(header).limit);
    destroy(at(load(header).limit));
    if (is_empty(load(arr.range))) {
        deallocate_array(header);
        header = nullptr;
    }
    return arr;
}

template <Movable T>
constexpr void
erase_all(array<T>& x)
{
    while (!is_empty(x)) erase(back<array<T>>(x));
}

template <Movable T>
constexpr void
pop(array<T>& arr)
{
    erase(back<array<T>>(arr));
}

template <Movable T>
constexpr auto
first(array<T> const& x) -> Position_type<array<T>>
{
    if (x.header == nullptr) return nullptr;
    return pointer_to(at(x.header).x);
}

template <Movable T>
constexpr auto
limit(array<T> const& x) -> Position_type<array<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit;
}

template <Movable T>
constexpr auto
limit_of_storage(array<T> const& x) -> Position_type<array<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit_of_storage;
}

template <Movable T>
constexpr auto
is_empty(array<T> const& x) -> bool
{
    return first(x) == limit(x);
}

template <Movable T>
constexpr auto
size(array<T> const& x) -> Size_type<array<T>>
{
    return limit(x) - first(x);
}

template <Movable T>
constexpr auto
capacity(array<T> const& x) -> Size_type<array<T>>
{
    return limit_of_storage(x) - first(x);
}

}
