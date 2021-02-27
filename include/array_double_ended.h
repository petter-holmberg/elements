#pragma once

#include "functional.h"
#include "lexicographical.h"
#include "map.h"
#include "memory.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <typename T>
struct array_double_ended_prefix
{
    Pointer_type<T> first;
    Pointer_type<T> limit;
    Pointer_type<T> limit_of_storage;
    T x;
};

template <typename T, Invocable auto alloc>
constexpr auto
allocate_array_double_ended(
    Difference_type<Pointer_type<T>> n,
    Difference_type<Pointer_type<T>> offset = Zero<Difference_type<Pointer_type<T>>>)
    -> Pointer_type<array_double_ended_prefix<T>>
{
    if (is_zero(n)) return nullptr;
    auto remote = reinterpret_cast<Pointer_type<array_double_ended_prefix<T>>>(
        allocate(alloc(), static_cast<pointer_diff>(sizeof(array_double_ended_prefix<T>)) + predecessor(n) * static_cast<pointer_diff>(sizeof(T))).first);
    auto const& first = pointer_to(at(remote).x);
    at(remote).first = first + offset;
    at(remote).limit = load(remote).first;
    at(remote).limit_of_storage = first + n;
    return remote;
}

template <typename T, Invocable auto alloc>
constexpr void
deallocate_array_double_ended(Pointer_type<array_double_ended_prefix<T>> prefix)
{
    deallocate(alloc(), memory{reinterpret_cast<Pointer_type<byte>>(prefix), prefix->limit_of_storage - prefix->first});
}

template <typename T, Invocable auto alloc = array_allocator<T>>
struct array_double_ended
{
    Pointer_type<array_double_ended_prefix<T>> header{};

    constexpr
    array_double_ended() = default;

    constexpr
    array_double_ended(array_double_ended const& x)
        : header{allocate_array_double_ended<T, alloc>(size(x))}
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    array_double_ended(array_double_ended&& x)
    {
        header = x.header;
        x.header = {};
    }

    explicit constexpr
    array_double_ended(Size_type<array_double_ended<T, alloc>> capacity)
        : array_double_ended(capacity, Zero<Size_type<array_double_ended<T, alloc>>>)
    {}

    constexpr
    array_double_ended(
        Size_type<array_double_ended<T, alloc>> capacity,
        Size_type<array_double_ended<T, alloc>> offset)
        : header{allocate_array_double_ended<T, alloc>(capacity, offset)}
    {}

    constexpr
    array_double_ended(
        Size_type<array_double_ended<T, alloc>> capacity,
        Size_type<array_double_ended<T, alloc>> size,
        T const& x)
        : array_double_ended<T, alloc>(capacity, Zero<Size_type<array_double_ended<T, alloc>>>, size, x)
    {}

    constexpr
    array_double_ended(
        Size_type<array_double_ended<T, alloc>> capacity,
        Size_type<array_double_ended<T, alloc>> offset,
        Size_type<array_double_ended<T, alloc>> size,
        T const& x)
        : header{allocate_array_double_ended<T, alloc>(capacity, offset)}
    {
        while (!is_zero(size)) {
            push(at(this), x);
            decrement(size);
        }
    }

    constexpr auto
    operator=(array_double_ended const& x) -> array_double_ended&
    {
        using elements::swap;
        array_double_ended temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(array_double_ended&& x) -> array_double_ended&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(header, x.header);
        }
        return at(this);
    }

    constexpr
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

    template <Operation<T> Op>
    constexpr auto
    fmap(Op op) -> array_double_ended<T, alloc>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{op}(first(at(this))));
        return at(this);
    }

    template <Regular_invocable<T> F>
    constexpr auto
    fmap(F fun) const -> array_double_ended<Return_type<F, T>>
    {
        using elements::map;
        array_double_ended<Return_type<F, T>> x;
        reserve(x, size(at(this)));
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        at(x.header).limit = at(x.header).limit_of_storage;
        return x;
    }
};

template <typename T, Invocable auto alloc>
struct value_type_t<array_double_ended<T, alloc>>
{
    using type = T;
};

template <typename T, Invocable auto alloc>
struct cursor_type_t<array_double_ended<T, alloc>>
{
    using type = Pointer_type<T>;
};

template <typename T, Invocable auto alloc>
struct cursor_type_t<array_double_ended<T, alloc> const>
{
    using type = Pointer_type<T const>;
};

template <typename T, Invocable auto alloc>
struct size_type_t<array_double_ended<T, alloc>>
{
    using type = pointer_diff;
};

template <Regular T, Invocable auto alloc>
constexpr auto
operator==(array_double_ended<T, alloc> const& x, array_double_ended<T, alloc> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T, Invocable auto alloc>
constexpr auto
operator<(array_double_ended<T, alloc> const& x, array_double_ended<T, alloc> const& y) -> bool
{
    return less_range(x, y);
}

template <typename T, Invocable auto alloc>
constexpr void
swap(array_double_ended<T, alloc>& x, array_double_ended<T, alloc>& y)
{
    swap(x.header, y.header);
}

template <typename T, Invocable auto alloc>
constexpr void
reserve(
    array_double_ended<T, alloc>& x,
    Size_type<array_double_ended<T, alloc>> n,
    Size_type<array_double_ended<T, alloc>> offset = Zero<Size_type<array_double_ended<T, alloc>>>)
{
    if (n < size(x) or n == capacity(x)) return;
    array_double_ended<T, alloc> temp(n);
    auto cur = first(x);
    auto dst = first(temp) + offset;
    at(temp.header).first = dst;
    while (precedes(cur, limit(x))) {
        construct(at(dst), mv(at(cur)));
        increment(cur);
        increment(dst);
    }
    at(temp.header).limit = dst;
    swap(x, temp);
}

template <typename T, typename U, Invocable auto alloc>
constexpr auto
insert(back<array_double_ended<T, alloc>> arr, U&& x) -> back<array_double_ended<T, alloc>>
{
    auto& seq = base(arr);
    if (!precedes(limit(seq), limit_of_storage(seq))) {
        if (!precedes(first(seq), first_of_storage(seq))) {
            reserve(seq, max(One<Size_type<array_double_ended<T, alloc>>>, twice(size(seq))));
        } else {
            auto cur = first_of_storage(seq) + half(first(seq) - first_of_storage(seq));
            at(seq.header).limit = swap(first(seq), limit(seq), cur);
            at(seq.header).first = cur;
        }
    }
    auto& header = at(seq.header);
    construct(at(header.limit), fw<U>(x));
    increment(header.limit);
    return seq;
}

template <typename T, typename U, Invocable auto alloc>
constexpr auto
insert(front<array_double_ended<T, alloc>> arr, U&& x) -> front<array_double_ended<T, alloc>>
{
    auto& seq = base(arr);
    if (!precedes(first(seq), first_of_storage(seq))) {
        if (!precedes(limit(seq), limit_of_storage(seq))) {
            auto n = size(seq);
            reserve(seq, max(One<Size_type<array_double_ended<T, alloc>>>, twice(n)), n);
        } else {
            auto cur = limit(seq) + successor(half(limit_of_storage(seq) - limit(seq)));
            at(seq.header).first = swap(
                reverse_cursor(limit(seq)),
                reverse_cursor(first(seq)),
                reverse_cursor(cur)).cur;
            at(seq.header).limit = cur;
        }
    }
    auto& header = at(seq.header);
    decrement(header.first);
    construct(at(header.first), fw<U>(x));
    return seq;
}

template <typename T, typename U, Invocable auto alloc>
constexpr void
emplace(array_double_ended<T, alloc>& arr, U&& x)
{
    insert(back{arr}, fw<U>(x));
}

template <typename T, typename U, Invocable auto alloc>
constexpr void
push(array_double_ended<T, alloc>& arr, U x)
{
    insert(back{arr}, T{mv(x)});
}

template <typename T, typename U, Invocable auto alloc>
constexpr void
emplace_first(array_double_ended<T, alloc>& arr, U&& x)
{
    insert(front{arr}, fw<U>(x));
}

template <typename T, typename U, Invocable auto alloc>
constexpr void
push_first(array_double_ended<T, alloc>& arr, U x)
{
    insert(front{arr}, T{mv(x)});
}

template <typename T, Invocable auto alloc>
constexpr auto
erase(back<array_double_ended<T, alloc>> arr) -> back<array_double_ended<T, alloc>>
{
    auto& seq = base(arr);
    auto& header = seq.header;
    decrement(at(header).limit);
    destroy(at(load(header).limit));
    if (is_empty(seq)) {
        deallocate_array_double_ended<T, alloc>(header);
        header = nullptr;
    }
    return arr;
}

template <typename T, Invocable auto alloc>
constexpr auto
erase(front<array_double_ended<T, alloc>> arr) -> front<array_double_ended<T, alloc>>
{
    auto& seq = base(arr);
    auto& header = seq.header;
    destroy(at(load(header).first));
    increment(at(header).first);
    if (is_empty(seq)) {
        deallocate_array_double_ended<T, alloc>(header);
        header = nullptr;
    }
    return arr;
}

template <typename T, Invocable auto alloc>
constexpr void
erase_all(array_double_ended<T, alloc>& x)
{
    while (!is_empty(x)) erase(back{x});
}

template <typename T, Invocable auto alloc>
constexpr void
pop(array_double_ended<T, alloc>& arr)
{
    erase(back{arr});
}

template <typename T, Invocable auto alloc>
constexpr void
pop_first(array_double_ended<T, alloc>& arr)
{
    erase(front{arr});
}

template <typename T, Invocable auto alloc>
constexpr auto
first(array_double_ended<T, alloc> const& x) -> Cursor_type<array_double_ended<T, alloc>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).first;
}

template <typename T, Invocable auto alloc>
constexpr auto
limit(array_double_ended<T, alloc> const& x) -> Cursor_type<array_double_ended<T, alloc>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit;
}

template <typename T, Invocable auto alloc>
constexpr auto
first_of_storage(array_double_ended<T, alloc> const& x) -> Cursor_type<array_double_ended<T, alloc>>
{
    if (x.header == nullptr) return nullptr;
    return pointer_to(at(x.header).x);
}

template <typename T, Invocable auto alloc>
constexpr auto
limit_of_storage(array_double_ended<T, alloc> const& x) -> Cursor_type<array_double_ended<T, alloc>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit_of_storage;
}

template <typename T, Invocable auto alloc>
constexpr auto
is_empty(array_double_ended<T, alloc> const& x) -> bool
{
    return first(x) == limit(x);
}

template <typename T, Invocable auto alloc>
constexpr auto
size(array_double_ended<T, alloc> const& x) -> Size_type<array_double_ended<T, alloc>>
{
    return limit(x) - first(x);
}

template <typename T, Invocable auto alloc>
constexpr auto
capacity(array_double_ended<T, alloc> const& x) -> Size_type<array_double_ended<T, alloc>>
{
    return limit_of_storage(x) - first_of_storage(x);
}

}
