#pragma once

#include "functional.h"
#include "lexicographical.h"
#include "map.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct array_single_ended_prefix
{
    Pointer_type<T> limit;
    Pointer_type<T> limit_of_storage;
    T x;
};

template <Movable T>
constexpr auto
allocate_array_single_ended(Difference_type<Pointer_type<T>> n) -> Pointer_type<array_single_ended_prefix<T>>
{
    if (is_zero(n)) return nullptr;
    auto remote = allocate<array_single_ended_prefix<T>>(predecessor(n) * static_cast<pointer_diff>(sizeof(T)));
    auto const& first = pointer_to(at(remote).x);
    at(remote).limit = first;
    at(remote).limit_of_storage = first + n;
    return remote;
}

template <Movable T>
constexpr void
deallocate_array_single_ended(Pointer_type<array_single_ended_prefix<T>> prefix)
{
    deallocate(prefix);
}

template <Movable T>
struct array_single_ended
{
    Pointer_type<array_single_ended_prefix<T>> header{};

    constexpr
    array_single_ended() = default;

    constexpr
    array_single_ended(array_single_ended const& x)
        : header{allocate_array_single_ended<T>(size(x))}
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    array_single_ended(array_single_ended&& x)
    {
        header = x.header;
        x.header = {};
    }

    constexpr
    array_single_ended(std::initializer_list<T> x)
        : header{allocate_array_single_ended<T>(Size_type<array_single_ended<T>>{static_cast<pointer_diff>(std::size(x))})}
    {
        insert_range(x, back{at(this)});
    }

    explicit constexpr
    array_single_ended(Size_type<array_single_ended<T>> capacity)
        : header{allocate_array_single_ended<T>(capacity)}
    {}

    constexpr
    array_single_ended(Size_type<array_single_ended<T>> size, T const& x)
        : array_single_ended(size, size, x)
    {}

    constexpr
    array_single_ended(Size_type<array_single_ended<T>> capacity, Size_type<array_single_ended<T>> size, T const& x)
        : header{allocate_array_single_ended<T>(capacity)}
    {
        while (!is_zero(size)) {
            push(at(this), x);
            decrement(size);
        }
    }

    constexpr auto
    operator=(array_single_ended const& x) -> array_single_ended&
    {
        using elements::swap;
        array_single_ended temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(array_single_ended&& x) -> array_single_ended&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(header, x.header);
        }
        return at(this);
    }

    ~array_single_ended()
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
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<Decay<T>, Codomain<Fun>>
    constexpr auto
    fmap(Fun fun) -> array_single_ended<T>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    fmap(Fun fun) const -> array_single_ended<Codomain<Fun>>
    {
        using elements::map;
        array_single_ended<Codomain<Fun>> x;
        reserve(x, size(at(this)));
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }
};

template <Movable T>
struct value_type_t<array_single_ended<T>>
{
    using type = T;
};

template <Movable T>
struct position_type_t<array_single_ended<T>>
{
    using type = Pointer_type<T>;
};

template <Movable T>
struct position_type_t<array_single_ended<T> const>
{
    using type = Pointer_type<T const>;
};

template <Movable T>
struct size_type_t<array_single_ended<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(array_single_ended<T> const& x, array_single_ended<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(array_single_ended<T> const& x, array_single_ended<T> const& y) -> bool
{
    return less_range(x, y);
}

template <Movable T>
constexpr void
swap(array_single_ended<T>& x, array_single_ended<T>& y)
{
    swap(x.header, y.header);
}

template <Movable T>
constexpr void
reserve(array_single_ended<T>& x, Size_type<array_single_ended<T>> n)
{
    if (n < size(x) or n == capacity(x)) return;
    array_single_ended<T> temp(n);
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
insert(back<array_single_ended<T>> arr, U&& x) -> back<array_single_ended<T>>
{
    auto& seq = base(arr);
    if (!precedes(limit(seq), limit_of_storage(seq))) {
        reserve(seq, max(One<Size_type<array_single_ended<T>>>, twice(size(seq))));
    }
    auto& header = at(seq.header);
    construct(at(header.limit), fw<U>(x));
    increment(header.limit);
    return seq;
}

template <Movable T, typename U>
constexpr void
emplace(array_single_ended<T>& arr, U&& x)
{
    insert(back{arr}, fw<U>(x));
}

template <Movable T, typename U>
constexpr void
push(array_single_ended<T>& arr, U x)
{
    insert(back{arr}, mv(x));
}

template <Movable T>
constexpr auto
erase(back<array_single_ended<T>> arr) -> back<array_single_ended<T>>
{
    auto& seq = base(arr);
    auto& header = seq.header;
    decrement(at(header).limit);
    destroy(at(load(header).limit));
    if (is_empty(seq)) {
        deallocate_array_single_ended(header);
        header = nullptr;
    }
    return arr;
}

template <Movable T>
constexpr void
erase_all(array_single_ended<T>& x)
{
    while (!is_empty(x)) erase(back{x});
}

template <Movable T>
constexpr void
pop(array_single_ended<T>& arr)
{
    erase(back{arr});
}

template <Movable T>
constexpr auto
first(array_single_ended<T> const& x) -> Position_type<array_single_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return pointer_to(at(x.header).x);
}

template <Movable T>
constexpr auto
limit(array_single_ended<T> const& x) -> Position_type<array_single_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit;
}

template <Movable T>
constexpr auto
limit_of_storage(array_single_ended<T> const& x) -> Position_type<array_single_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit_of_storage;
}

template <Movable T>
constexpr auto
is_empty(array_single_ended<T> const& x) -> bool
{
    return first(x) == limit(x);
}

template <Movable T>
constexpr auto
size(array_single_ended<T> const& x) -> Size_type<array_single_ended<T>>
{
    return limit(x) - first(x);
}

template <Movable T>
constexpr auto
capacity(array_single_ended<T> const& x) -> Size_type<array_single_ended<T>>
{
    return limit_of_storage(x) - first(x);
}

}
