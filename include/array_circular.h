#pragma once

#include "functional.h"
#include "lexicographical.h"
#include "map.h"
#include "memory.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <typename T, Invocable auto alloc = array_allocator<T>>
struct array_circular;

template <typename T, Invocable auto alloc>
struct array_circular_cursor
{
    Pointer_type<array_circular<T, alloc>> arr{};
    Difference_type<Pointer_type<T>> i{Zero<Difference_type<Pointer_type<T>>>};

    constexpr
    array_circular_cursor() = default;

    explicit constexpr
    array_circular_cursor(
        Pointer_type<array_circular<T, alloc>> arr_,
        Difference_type<Pointer_type<T>> i_ = Zero<Difference_type<Pointer_type<T>>>)
        : arr{arr_}
        , i{i_}
    {}
};

template <typename T, Invocable auto alloc>
struct value_type_t<array_circular_cursor<T, alloc>>
{
    using type = T;
};

template <typename T, Invocable auto alloc>
struct difference_type_t<array_circular_cursor<T, alloc>>
{
    using type = Difference_type<Pointer_type<T>>;
};

template <typename T, Invocable auto alloc>
constexpr auto
operator==(array_circular_cursor<T, alloc> const& cur0, array_circular_cursor<T, alloc> const& cur1) -> bool
{
    return cur0.arr == cur1.arr and cur0.i == cur1.i;
}

template <typename T, Invocable auto alloc>
constexpr void
increment(array_circular_cursor<T, alloc>& cur)
{
    increment(cur.i);
}

template <typename T, Invocable auto alloc>
constexpr void
decrement(array_circular_cursor<T, alloc>& cur)
{
    decrement(cur.i);
}

template <typename T, Invocable auto alloc>
constexpr auto
load(array_circular_cursor<T, alloc> const& cur) -> T const&
{
    return load(cur.arr)[cur.i];
}

template <typename T, Invocable auto alloc>
constexpr void
store(array_circular_cursor<T, alloc>& cur, T const& value)
{
    at(cur.arr)[cur.i] = value;
}

template <typename T, Invocable auto alloc>
constexpr void
store(array_circular_cursor<T, alloc>& cur, Value_type<T>&& value)
{
    at(cur.arr)[cur.i] = fw<T>(value);
}

template <typename T, Invocable auto alloc>
constexpr auto
at(array_circular_cursor<T, alloc> const& cur) -> T const&
{
    return load(cur.arr)[cur.i];
}

template <typename T, Invocable auto alloc>
constexpr auto
at(array_circular_cursor<T, alloc>& cur) -> T&
{
    return at(cur.arr)[cur.i];
}

template <typename T, Invocable auto alloc>
constexpr auto
precedes(array_circular_cursor<T, alloc> const& cur0, array_circular_cursor<T, alloc> const& cur1) -> bool
{
    return cur0.i != cur1.i;
}

template <typename T, Invocable auto alloc>
struct array_circular_prefix
{
    Pointer_type<T> first;
    Pointer_type<T> limit;
    Pointer_type<T> limit_of_storage;
    pointer_diff size;
    T x;
};

template <typename T, Invocable auto alloc>
constexpr auto
allocate_array_circular(Difference_type<Pointer_type<T>> n) -> Pointer_type<array_circular_prefix<T, alloc>>
{
    if (is_zero(n)) return nullptr;
    auto remote = reinterpret_cast<Pointer_type<array_circular_prefix<T, alloc>>>(
        allocate(alloc(), static_cast<pointer_diff>(sizeof(array_circular_prefix<T, alloc>)) + predecessor(n) * static_cast<pointer_diff>(sizeof(T))).first);
    auto const& first = pointer_to(at(remote).x);
    at(remote).first = first;
    at(remote).limit = first;
    at(remote).limit_of_storage = first + n;
    at(remote).size = Zero<pointer_diff>;
    return remote;
}

template <typename T, Invocable auto alloc>
constexpr void
deallocate_array_circular(Pointer_type<array_circular_prefix<T, alloc>> prefix)
{
    deallocate(alloc(), memory{reinterpret_cast<Pointer_type<byte>>(prefix), prefix->limit_of_storage - prefix->first});
}

template <typename T, Invocable auto alloc>
struct array_circular
{
    Pointer_type<array_circular_prefix<T, alloc>> header{};

    constexpr
    array_circular() = default;

    constexpr
    array_circular(array_circular const& x)
        : header{allocate_array_circular<T, alloc>(size(x))}
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    array_circular(array_circular&& x)
    {
        header = x.header;
        x.header = {};
    }

    explicit constexpr
    array_circular(Size_type<array_circular<T, alloc>> capacity)
        : header{allocate_array_circular<T, alloc>(capacity)}
    {}

    constexpr
    array_circular(
        Size_type<array_circular<T, alloc>> capacity,
        Size_type<array_circular<T, alloc>> size,
        T const& x)
        : array_circular<T, alloc>(capacity)
    {
        while (!is_zero(size)) {
            push(at(this), x);
            decrement(size);
        }
    }

    constexpr auto
    operator=(array_circular const& x) -> array_circular&
    {
        using elements::swap;
        array_circular temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(array_circular&& x) -> array_circular&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(header, x.header);
        }
        return at(this);
    }

    constexpr
    ~array_circular()
    {
        erase_all(at(this));
    }

    constexpr auto
    operator[](pointer_diff i) -> T&
    {
        auto const& wrap = at(header).limit_of_storage - at(header).first;
        if (i < wrap) {
            return at(at(header).first + i);
        } else {
            return at(pointer_to(at(header).x) + (i - wrap));
        }
    }

    constexpr auto
    operator[](pointer_diff i) const -> T const&
    {
        auto const& wrap = load(header).limit_of_storage - load(header).first;
        if (i < wrap) {
            return load(at(header).first + i);
        } else {
            return load(pointer_to(at(header).x) + (i - wrap));
        }
    }

    template <Operation<T> Op>
    constexpr auto
    fmap(Op op) -> array_circular<T, alloc>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{op}(first(at(this))));
        return at(this);
    }

    template <Regular_invocable<T> F>
    constexpr auto
    fmap(F fun) const -> array_circular<Return_type<F, T>>
    {
        using elements::map;
        array_circular<Return_type<F, T>> x;
        reserve(x, size(at(this)));
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        at(x.header).limit = at(x.header).limit_of_storage;
        return x;
    }
};

template <typename T, Invocable auto alloc>
struct value_type_t<array_circular<T, alloc>>
{
    using type = T;
};

template <typename T, Invocable auto alloc>
struct cursor_type_t<array_circular<T, alloc>>
{
    using type = array_circular_cursor<T, alloc>;
};

template <typename T, Invocable auto alloc>
struct cursor_type_t<array_circular<T, alloc> const>
{
    using type = array_circular_cursor<T const, alloc>;
};

template <typename T, Invocable auto alloc>
struct size_type_t<array_circular<T, alloc>>
{
    using type = pointer_diff;
};

template <Regular T, Invocable auto alloc>
constexpr auto
operator==(array_circular<T, alloc> const& x, array_circular<T, alloc> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T, Invocable auto alloc>
constexpr auto
operator<(array_circular<T, alloc> const& x, array_circular<T, alloc> const& y) -> bool
{
    return less_range(x, y);
}

template <typename T, Invocable auto alloc>
constexpr void
swap(array_circular<T, alloc>& x, array_circular<T, alloc>& y)
{
    swap(x.header, y.header);
}

template <typename T, Invocable auto alloc>
constexpr void
reserve(
    array_circular<T, alloc>& x,
    Size_type<array_circular<T, alloc>> n,
    Size_type<array_circular<T, alloc>> offset = Zero<Size_type<array_circular<T, alloc>>>)
{
    if (n < size(x) or n == capacity(x)) return;
    array_circular<T, alloc> temp(n);
    auto cur = first(x);
    auto dst = first(temp) + offset;
    while (precedes(cur, limit(x))) {
        construct(at(dst), mv(at(cur)));
        increment(cur);
        increment(dst);
    }
    at(temp.header).first = at(temp.header).first + offset;
    at(temp.header).limit = at(temp.header).first + dst.i;
    at(temp.header).size = size(x);
    swap(x, temp);
}

template <typename T, Invocable auto alloc, typename U>
constexpr auto
insert(back<array_circular<T, alloc>> arr, U&& x) -> back<array_circular<T, alloc>>
{
    auto& seq = base(arr);
    if (size(seq) == capacity(seq)) {
        reserve(seq, max(One<Size_type<array_circular<T, alloc>>>, twice(size(seq))));
    } else if (load(seq.header).limit == load(seq.header).limit_of_storage) {
        at(seq.header).limit = pointer_to(at(seq.header).x);
    }
    auto& header = at(seq.header);
    construct(at(header.limit), fw<U>(x));
    increment(header.limit);
    increment(header.size);
    return seq;
}

template <typename T, Invocable auto alloc, typename U>
constexpr auto
insert(front<array_circular<T, alloc>> arr, U&& x) -> front<array_circular<T, alloc>>
{
    auto& seq = base(arr);
    if (size(seq) == capacity(seq)) {
        reserve(seq, max(One<Size_type<array_circular<T, alloc>>>, twice(size(seq))), size(seq));
    } else if (load(seq.header).first == pointer_to(load(seq.header).x)) {
        at(seq.header).first = load(seq.header).limit_of_storage;
    }
    auto& header = at(seq.header);
    decrement(header.first);
    construct(at(header.first), fw<U>(x));
    increment(header.size);
    return seq;
}

template <typename T, Invocable auto alloc, typename U>
constexpr void
emplace(array_circular<T, alloc>& arr, U&& x)
{
    insert(back{arr}, fw<U>(x));
}

template <typename T, Invocable auto alloc, typename U>
constexpr void
push(array_circular<T, alloc>& arr, U x)
{
    insert(back{arr}, mv(x));
}

template <typename T, Invocable auto alloc, typename U>
constexpr void
emplace_first(array_circular<T, alloc>& arr, U&& x)
{
    insert(front{arr}, fw<U>(x));
}

template <typename T, Invocable auto alloc, typename U>
constexpr void
push_first(array_circular<T, alloc>& arr, T x)
{
    insert(front{arr}, mv(x));
}

template <typename T, Invocable auto alloc>
constexpr auto
erase(back<array_circular<T, alloc>> arr) -> back<array_circular<T, alloc>>
{
    auto& seq = base(arr);
    auto& header = seq.header;
    if (load(header).limit == pointer_to(at(header).x)) {
        at(header).limit = load(header).limit_of_storage;
    }
    decrement(at(header).limit);
    destroy(at(load(header).limit));
    decrement(at(header).size);
    if (is_empty(seq)) {
        deallocate_array_circular<T, alloc>(header);
        header = nullptr;
    }
    return arr;
}

template <typename T, Invocable auto alloc>
constexpr auto
erase(front<array_circular<T, alloc>> arr) -> front<array_circular<T, alloc>>
{
    auto& seq = base(arr);
    auto& header = seq.header;
    destroy(at(load(header).first));
    increment(at(header).first);
    if (load(header).first == load(header).limit_of_storage) {
        at(header).first = pointer_to(at(header).x);
    }
    decrement(at(header).size);
    if (is_empty(seq)) {
        deallocate_array_circular<T, alloc>(header);
        header = nullptr;
    }
    return arr;
}

template <typename T, Invocable auto alloc>
constexpr void
erase_all(array_circular<T, alloc>& x)
{
    while (!is_empty(x)) erase(back{x});
}

template <typename T, Invocable auto alloc>
constexpr void
pop(array_circular<T, alloc>& arr)
{
    erase(back{arr});
}

template <typename T, Invocable auto alloc>
constexpr void
pop_first(array_circular<T, alloc>& arr)
{
    erase(front{arr});
}

template <typename T, Invocable auto alloc>
constexpr auto
first(array_circular<T, alloc> const& x) -> Cursor_type<array_circular<T, alloc>>
{
    if (x.header == nullptr) return {};
    return array_circular_cursor{const_cast<Pointer_type<array_circular<T, alloc>>>(pointer_to(x))};
}

template <typename T, Invocable auto alloc>
constexpr auto
limit(array_circular<T, alloc> const& x) -> Cursor_type<array_circular<T, alloc>>
{
    if (x.header == nullptr) return {};
    return array_circular_cursor{const_cast<Pointer_type<array_circular<T, alloc>>>(pointer_to(x)), size(x)};
}

template <typename T, Invocable auto alloc>
constexpr auto
is_empty(array_circular<T, alloc> const& x) -> bool
{
    if (x.header == nullptr) return true;
    return load(x.header).size == Zero<Size_type<array_circular<T, alloc>>>;
}

template <typename T, Invocable auto alloc>
constexpr auto
size(array_circular<T, alloc> const& x) -> Size_type<array_circular<T, alloc>>
{
    if (x.header == nullptr) return Zero<Size_type<array_circular<T, alloc>>>;
    return load(x.header).size;
}

template <typename T, Invocable auto alloc>
constexpr auto
capacity(array_circular<T, alloc> const& x) -> Size_type<array_circular<T, alloc>>
{
    if (x.header == nullptr) return Zero<Size_type<array_circular<T, alloc>>>;
    return load(x.header).limit_of_storage - pointer_to(load(x.header).x);
}

}
