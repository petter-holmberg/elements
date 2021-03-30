#pragma once

#include "copy.h"
#include "functional.h"
#include "lexicographical.h"
#include "map.h"
#include "memory.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <typename T>
struct array_single_ended_prefix
{
    Pointer_type<T> limit;
    Pointer_type<T> limit_of_storage;
    T x;
};

template <typename T, Invocable auto alloc>
constexpr auto
allocate_array_single_ended(Difference_type<Pointer_type<T>> n) -> Pointer_type<array_single_ended_prefix<T>>
{
    if (is_zero(n)) return nullptr;
    auto remote = reinterpret_cast<Pointer_type<array_single_ended_prefix<T>>>(
        allocate(alloc(), static_cast<pointer_diff>(sizeof(array_single_ended_prefix<T>)) + predecessor(n) * static_cast<pointer_diff>(sizeof(T))).first);
    auto const& first = pointer_to(at(remote).x);
    at(remote).limit = first;
    at(remote).limit_of_storage = first + n;
    return remote;
}

template <typename T, Invocable auto alloc>
constexpr void
deallocate_array_single_ended(Pointer_type<array_single_ended_prefix<T>> prefix)
{
    deallocate(alloc(), memory{reinterpret_cast<Pointer_type<byte>>(prefix), prefix->limit_of_storage - pointer_to(prefix->x)});
}

template <typename T, Invocable auto alloc = array_allocator<T>>
struct array_single_ended
{
    Pointer_type<array_single_ended_prefix<T>> header{};

    constexpr
    array_single_ended() = default;

    constexpr
    array_single_ended(array_single_ended const& x)
        : header{allocate_array_single_ended<T, alloc>(size(x))}
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    array_single_ended(array_single_ended&& x)
    {
        header = x.header;
        x.header = {};
    }

    explicit constexpr
    array_single_ended(Size_type<array_single_ended<T, alloc>> capacity)
        : header{allocate_array_single_ended<T, alloc>(capacity)}
    {}

    constexpr
    array_single_ended(Size_type<array_single_ended<T, alloc>> size, T const& x)
        : array_single_ended(size, size, x)
    {}

    constexpr
    array_single_ended(Size_type<array_single_ended<T, alloc>> capacity, Size_type<array_single_ended<T, alloc>> size, T const& x)
        : header{allocate_array_single_ended<T, alloc>(capacity)}
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

    constexpr
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
};

template <typename T, Invocable auto alloc>
struct value_type_t<array_single_ended<T, alloc>>
{
    using type = T;
};

template <typename T, Invocable auto alloc>
struct cursor_type_t<array_single_ended<T, alloc>>
{
    using type = Pointer_type<T>;
};

template <typename T, Invocable auto alloc>
struct cursor_type_t<array_single_ended<T, alloc> const>
{
    using type = Pointer_type<T const>;
};

template <typename T, Invocable auto alloc>
struct size_type_t<array_single_ended<T, alloc>>
{
    using type = pointer_diff;
};

template <typename T, Invocable auto alloc>
struct functor_t<array_single_ended<T, alloc>>
{
    using constructor_type = array_single_ended<T, alloc>;

    template <Operation<T> Op>
    static constexpr auto
    fmap(array_single_ended<T, alloc>& x, Op op) -> array_single_ended<T, alloc>&
    {
        using elements::copy;
        copy(first(x), limit(x), map_sink{op}(first(x)));
        return x;
    }

    template <Regular_invocable<T> F>
    static constexpr auto
    fmap(array_single_ended<T, alloc>&& x, F fun) -> array_single_ended<Return_type<F, T>, alloc>
    {
        using elements::map;
        array_single_ended<Return_type<F, T>, alloc> y;
        reserve(y, size(x));
        map(first(x), limit(x), insert_sink{}(back{y}), fun);
        return y;
    }
};

template <Regular T, Invocable auto alloc>
constexpr auto
operator==(array_single_ended<T, alloc> const& x, array_single_ended<T, alloc> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T, Invocable auto alloc>
constexpr auto
operator<(array_single_ended<T, alloc> const& x, array_single_ended<T, alloc> const& y) -> bool
{
    return less_range(x, y);
}

template <typename T, Invocable auto alloc>
constexpr void
swap(array_single_ended<T, alloc>& x, array_single_ended<T, alloc>& y)
{
    swap(x.header, y.header);
}

template <typename T, Invocable auto alloc>
constexpr void
reserve(array_single_ended<T, alloc>& x, Size_type<array_single_ended<T, alloc>> n)
{
    if (n < size(x) or n == capacity(x)) return;
    array_single_ended<T, alloc> temp(n);
    auto cur = first(x);
    auto dst = first(temp);
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
insert(back<array_single_ended<T, alloc>> arr, U&& x) -> back<array_single_ended<T, alloc>>
{
    auto& seq = base(arr);
    if (!precedes(limit(seq), limit_of_storage(seq))) {
        reserve(seq, max(One<Size_type<array_single_ended<T, alloc>>>, twice(size(seq))));
    }
    auto& header = at(seq.header);
    construct(at(header.limit), fw<U>(x));
    increment(header.limit);
    return seq;
}

template <typename T, typename U, Invocable auto alloc>
constexpr void
emplace(array_single_ended<T, alloc>& arr, U&& x)
{
    insert(back{arr}, fw<U>(x));
}

template <typename T, typename U, Invocable auto alloc>
constexpr void
push(array_single_ended<T, alloc>& arr, U x)
{
    insert(back{arr}, mv(x));
}

template <typename T, Invocable auto alloc>
constexpr auto
erase(back<array_single_ended<T, alloc>> arr) -> back<array_single_ended<T, alloc>>
{
    auto& seq = base(arr);
    auto& header = seq.header;
    decrement(at(header).limit);
    destroy(at(load(header).limit));
    if (is_empty(seq)) {
        deallocate_array_single_ended<T, alloc>(header);
        header = nullptr;
    }
    return arr;
}

template <typename T, Invocable auto alloc>
constexpr void
erase_all(array_single_ended<T, alloc>& x)
{
    while (!is_empty(x)) erase<T, alloc>(back{x});
}

template <typename T, Invocable auto alloc>
constexpr void
pop(array_single_ended<T, alloc>& arr)
{
    erase(back{arr});
}

template <typename T, Invocable auto alloc>
constexpr auto
first(array_single_ended<T, alloc> const& x) -> Cursor_type<array_single_ended<T, alloc>>
{
    if (x.header == nullptr) return nullptr;
    return pointer_to(at(x.header).x);
}

template <typename T, Invocable auto alloc>
constexpr auto
limit(array_single_ended<T, alloc> const& x) -> Cursor_type<array_single_ended<T, alloc>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit;
}

template <typename T, Invocable auto alloc>
constexpr auto
limit_of_storage(array_single_ended<T, alloc> const& x) -> Cursor_type<array_single_ended<T, alloc>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit_of_storage;
}

template <typename T, Invocable auto alloc>
constexpr auto
is_empty(array_single_ended<T, alloc> const& x) -> bool
{
    return first(x) == limit(x);
}

template <typename T, Invocable auto alloc>
constexpr auto
size(array_single_ended<T, alloc> const& x) -> Size_type<array_single_ended<T, alloc>>
{
    return limit(x) - first(x);
}

template <typename T, Invocable auto alloc>
constexpr auto
capacity(array_single_ended<T, alloc> const& x) -> Size_type<array_single_ended<T, alloc>>
{
    return limit_of_storage(x) - first(x);
}

}
