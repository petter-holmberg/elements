#pragma once

#include "functional.h"
#include "lexicographical.h"
#include "map.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct array_double_ended_prefix
{
    Pointer_type<T> first;
    Pointer_type<T> limit;
    Pointer_type<T> limit_of_storage;
    T x;
};

template <Movable T>
constexpr auto
allocate_array_double_ended(
    Difference_type<Pointer_type<T>> n,
    Difference_type<Pointer_type<T>> offset = Zero<Difference_type<Pointer_type<T>>>)
    -> Pointer_type<array_double_ended_prefix<T>>
{
    if (is_zero(n)) return nullptr;
    auto remote = allocate<array_double_ended_prefix<T>>(predecessor(n) * static_cast<pointer_diff>(sizeof(T)));
    auto const& first = pointer_to(at(remote).x);
    at(remote).first = first + offset;
    at(remote).limit = load(remote).first;
    at(remote).limit_of_storage = first + n;
    return remote;
}

template <Movable T>
constexpr void
deallocate_array_double_ended(Pointer_type<array_double_ended_prefix<T>> prefix)
{
    deallocate(prefix);
}

template <Movable T>
struct array_double_ended
{
    Pointer_type<array_double_ended_prefix<T>> header{nullptr};

    array_double_ended() = default;

    array_double_ended(array_double_ended const& x)
        : header{allocate_array_double_ended<T>(size(x))}
    {
        insert_range(x, back{at(this)});
    }

    array_double_ended(array_double_ended&& x)
    {
        header = x.header;
        x.header = nullptr;
    }

    array_double_ended(std::initializer_list<T> x)
        : header{allocate_array_double_ended<T>(static_cast<pointer_diff>(std::size(x)))}
    {
        insert_range(x, back{at(this)});
    }

    explicit
    array_double_ended(Size_type<array_double_ended<T>> capacity)
        : array_double_ended(capacity, Zero<Size_type<array_double_ended<T>>>)
    {}

    array_double_ended(
        Size_type<array_double_ended<T>> capacity,
        Size_type<array_double_ended<T>> offset)
        : header{allocate_array_double_ended<T>(capacity, offset)}
    {}

    array_double_ended(
        Size_type<array_double_ended<T>> capacity,
        Size_type<array_double_ended<T>> size,
        T const& x)
        : array_double_ended<T>(capacity, Zero<Size_type<array_double_ended<T>>>, size, x)
    {}

    array_double_ended(
        Size_type<array_double_ended<T>> capacity,
        Size_type<array_double_ended<T>> offset,
        Size_type<array_double_ended<T>> size,
        T const& x)
        : header{allocate_array_double_ended<T>(capacity, offset)}
    {
        while (!is_zero(size)) {
            push(at(this), x);
            decrement(size);
        }
    }

    auto
    operator=(array_double_ended const& x) -> array_double_ended&
    {
        array_double_ended temp(x);
        swap(at(this), temp);
        return at(this);
    }

    auto
    operator=(array_double_ended&& x) -> array_double_ended&
    {
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(header, x.header);
        }
        return at(this);
    }

    ~array_double_ended()
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
    map(Fun fun) -> array_double_ended<T>&
    {
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> array_double_ended<Codomain<Fun>>
    {
        using elements::map;
        array_double_ended<Codomain<Fun>> x;
        reserve(x, size(at(this)));
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        at(x.header).limit = at(x.header).limit_of_storage;
        return x;
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<array_double_ended<Decay<T>>, Codomain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> array_double_ended<T>&
    {
        using elements::flat_map;
        auto x = flat_map(first(at(this)), limit(at(this)), fun);
        swap(at(this), x);
        return at(this);
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Regular<T>
    constexpr auto
    flat_map(Fun fun) -> Codomain<Fun>
    {
        using elements::flat_map;
        return flat_map(first(at(this)), limit(at(this)), fun);
    }
};

template <Movable T>
struct value_type_t<array_double_ended<T>>
{
    using type = T;
};

template <Movable T>
struct position_type_t<array_double_ended<T>>
{
    using type = Pointer_type<T>;
};

template <Movable T>
struct position_type_t<array_double_ended<T> const>
{
    using type = Pointer_type<T const>;
};

template <Movable T>
struct size_type_t<array_double_ended<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(array_double_ended<T> const& x, array_double_ended<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(array_double_ended<T> const& x, array_double_ended<T> const& y) -> bool
{
    return less_range(x, y);
}

template <Movable T>
constexpr void
swap(array_double_ended<T>& x, array_double_ended<T>& y)
{
    swap(x.header, y.header);
}

template <Movable T>
constexpr void
reserve(
    array_double_ended<T>& x,
    Size_type<array_double_ended<T>> n,
    Size_type<array_double_ended<T>> offset = Zero<Size_type<array_double_ended<T>>>)
{
    if (n < size(x) or n == capacity(x)) return;
    array_double_ended<T> temp(n);
    auto pos = first(x);
    auto dst = first(temp) + offset;
    at(temp.header).first = dst;
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
insert(back<array_double_ended<T>> arr, U&& x) -> back<array_double_ended<T>>
{
    auto& seq = at(arr.seq);
    if (limit(seq) == limit_of_storage(seq)) {
        if (first(seq) == first_of_storage(seq)) {
            reserve(seq, max(One<Size_type<array_double_ended<T>>>, twice(size(seq))));
        } else {
            auto pos = first_of_storage(seq) + half(first(seq) - first_of_storage(seq));
            at(seq.header).limit = swap(first(seq), limit(seq), pos);
            at(seq.header).first = pos;
        }
    }
    auto& header = at(seq.header);
    construct(at(header.limit), fw<U>(x));
    increment(header.limit);
    return seq;
}

template <Movable T, typename U>
constexpr auto
insert(front<array_double_ended<T>> arr, U&& x) -> back<array_double_ended<T>>
{
    auto& seq = at(arr.seq);
    if (first(seq) == first_of_storage(seq)) {
        if (limit(seq) == limit_of_storage(seq)) {
            auto n = size(seq);
            reserve(seq, max(One<Size_type<array_double_ended<T>>>, twice(n)), n);
        } else {
            auto pos = limit(seq) + successor(half(limit_of_storage(seq) - limit(seq)));
            at(seq.header).first = swap(
                reverse_position(limit(seq)),
                reverse_position(first(seq)),
                reverse_position(pos)).pos;
            at(seq.header).limit = pos;
        }
    }
    auto& header = at(seq.header);
    decrement(header.first);
    construct(at(header.first), fw<U>(x));
    return seq;
}

template <Movable T, typename U>
constexpr void
emplace(array_double_ended<T>& arr, U&& x)
{
    insert(back<array_double_ended<T>>(arr), fw<U>(x));
}

template <Movable T, typename U>
constexpr void
push(array_double_ended<T>& arr, U x)
{
    insert(back<array_double_ended<T>>(arr), T{mv(x)});
}

template <Movable T, typename U>
constexpr void
emplace_first(array_double_ended<T>& arr, U&& x)
{
    insert(front<array_double_ended<T>>(arr), fw<U>(x));
}

template <Movable T, typename U>
constexpr void
push_first(array_double_ended<T>& arr, U x)
{
    insert(front<array_double_ended<T>>(arr), T{mv(x)});
}

template <Movable T>
constexpr auto
erase(back<array_double_ended<T>> arr) -> back<array_double_ended<T>>
{
    auto& header = at(arr.seq).header;
    decrement(at(header).limit);
    destroy(at(load(header).limit));
    if (is_empty(load(arr.seq))) {
        deallocate_array_double_ended(header);
        header = nullptr;
    }
    return arr;
}

template <Movable T>
constexpr auto
erase(front<array_double_ended<T>> arr) -> front<array_double_ended<T>>
{
    auto& header = at(arr.seq).header;
    destroy(at(load(header).first));
    increment(at(header).first);
    if (is_empty(load(arr.seq))) {
        deallocate_array_double_ended(header);
        header = nullptr;
    }
    return arr;
}

template <Movable T>
constexpr void
erase_all(array_double_ended<T>& x)
{
    while (!is_empty(x)) erase(back<array_double_ended<T>>(x));
}

template <Movable T>
constexpr void
pop(array_double_ended<T>& arr)
{
    erase(back<array_double_ended<T>>(arr));
}

template <Movable T>
constexpr void
pop_first(array_double_ended<T>& arr)
{
    erase(front<array_double_ended<T>>(arr));
}

template <Movable T>
constexpr auto
first(array_double_ended<T> const& x) -> Position_type<array_double_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).first;
}

template <Movable T>
constexpr auto
limit(array_double_ended<T> const& x) -> Position_type<array_double_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit;
}

template <Movable T>
constexpr auto
first_of_storage(array_double_ended<T> const& x) -> Position_type<array_double_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return pointer_to(at(x.header).x);
}

template <Movable T>
constexpr auto
limit_of_storage(array_double_ended<T> const& x) -> Position_type<array_double_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit_of_storage;
}

template <Movable T>
constexpr auto
is_empty(array_double_ended<T> const& x) -> bool
{
    return first(x) == limit(x);
}

template <Movable T>
constexpr auto
size(array_double_ended<T> const& x) -> Size_type<array_double_ended<T>>
{
    return limit(x) - first(x);
}

template <Movable T>
constexpr auto
capacity(array_double_ended<T> const& x) -> Size_type<array_double_ended<T>>
{
    return limit_of_storage(x) - first_of_storage(x);
}

}
