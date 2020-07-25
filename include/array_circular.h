#pragma once

#include "functional.h"
#include "lexicographical.h"
#include "map.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct array_circular;

template <Constructible_from T>
struct array_circular_cursor
{
    Pointer_type<array_circular<T>> arr{};
    Difference_type<Pointer_type<T>> i{Zero<Difference_type<Pointer_type<T>>>};

    constexpr
    array_circular_cursor() = default;

    explicit constexpr
    array_circular_cursor(
        Pointer_type<array_circular<T>> arr_,
        Difference_type<Pointer_type<T>> i_ = Zero<Difference_type<Pointer_type<T>>>)
        : arr{arr_}
        , i{i_}
    {}
};

template <Constructible_from T>
struct value_type_t<array_circular_cursor<T>>
{
    using type = T;
};

template <Constructible_from T>
struct difference_type_t<array_circular_cursor<T>>
{
    using type = Difference_type<Pointer_type<T>>;
};

template <Movable T>
constexpr auto
operator==(array_circular_cursor<T> const& cur0, array_circular_cursor<T> const& cur1) -> bool
{
    return cur0.arr == cur1.arr and cur0.i == cur1.i;
}

template <Movable T>
constexpr void
increment(array_circular_cursor<T>& cur)
{
    increment(cur.i);
}

template <Movable T>
constexpr void
decrement(array_circular_cursor<T>& cur)
{
    decrement(cur.i);
}

template <Movable T>
constexpr auto
load(array_circular_cursor<T> const& cur) -> T const&
{
    return load(cur.arr)[cur.i];
}

template <Movable T>
constexpr void
store(array_circular_cursor<T>& cur, T const& value)
{
    at(cur.arr)[cur.i] = value;
}

template <Movable T>
constexpr void
store(array_circular_cursor<T>& cur, Value_type<T>&& value)
{
    at(cur.arr)[cur.i] = fw<T>(value);
}

template <Movable T>
constexpr auto
at(array_circular_cursor<T> const& cur) -> T const&
{
    return load(cur.arr)[cur.i];
}

template <Movable T>
constexpr auto
at(array_circular_cursor<T>& cur) -> T&
{
    return at(cur.arr)[cur.i];
}

template <Movable T>
constexpr auto
precedes(array_circular_cursor<T> const& cur0, array_circular_cursor<T> const& cur1) -> bool
{
    return cur0.i != cur1.i;
}

template <Movable T>
struct array_circular_prefix
{
    Pointer_type<T> first;
    Pointer_type<T> limit;
    Pointer_type<T> limit_of_storage;
    pointer_diff size;
    T x;
};

template <Movable T>
constexpr auto
allocate_array_circular(Difference_type<Pointer_type<T>> n) -> Pointer_type<array_circular_prefix<T>>
{
    if (is_zero(n)) return nullptr;
    auto remote = allocate<array_circular_prefix<T>>(predecessor(n) * static_cast<pointer_diff>(sizeof(T)));
    auto const& first = pointer_to(at(remote).x);
    at(remote).first = first;
    at(remote).limit = first;
    at(remote).limit_of_storage = first + n;
    at(remote).size = Zero<pointer_diff>;
    return remote;
}

template <Movable T>
constexpr void
deallocate_array_circular(Pointer_type<array_circular_prefix<T>> prefix)
{
    deallocate(prefix);
}

template <Movable T>
struct array_circular
{
    Pointer_type<array_circular_prefix<T>> header{};

    constexpr
    array_circular() = default;

    constexpr
    array_circular(array_circular const& x)
        : header{allocate_array_circular<T>(size(x))}
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    array_circular(array_circular&& x)
    {
        header = x.header;
        x.header = {};
    }

    constexpr
    array_circular(std::initializer_list<T> x)
        : header{allocate_array_circular<T>(
            Size_type<array_circular<T>>{static_cast<pointer_diff>(std::size(x))})}
    {
        insert_range(x, back{at(this)});
    }

    explicit constexpr
    array_circular(Size_type<array_circular<T>> capacity)
        : header{allocate_array_circular<T>(capacity)}
    {}

    constexpr
    array_circular(
        Size_type<array_circular<T>> capacity,
        Size_type<array_circular<T>> size,
        T const& x)
        : array_circular<T>(capacity)
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

    template <Unary_function F>
    requires
        Same_as<Decay<T>, Domain<F>> and
        Same_as<Decay<T>, Codomain<F>>
    constexpr auto
    fmap(F fun) -> array_circular<T>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function F>
    requires Same_as<Decay<T>, Domain<F>>
    constexpr auto
    fmap(F fun) const -> array_circular<Codomain<F>>
    {
        using elements::map;
        array_circular<Codomain<F>> x;
        reserve(x, size(at(this)));
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        at(x.header).limit = at(x.header).limit_of_storage;
        return x;
    }
};

template <Movable T>
struct value_type_t<array_circular<T>>
{
    using type = T;
};

template <Movable T>
struct cursor_type_t<array_circular<T>>
{
    using type = array_circular_cursor<T>;
};

template <Movable T>
struct cursor_type_t<array_circular<T> const>
{
    using type = array_circular_cursor<T const>;
};

template <Movable T>
struct size_type_t<array_circular<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(array_circular<T> const& x, array_circular<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(array_circular<T> const& x, array_circular<T> const& y) -> bool
{
    return less_range(x, y);
}

template <Movable T>
constexpr void
swap(array_circular<T>& x, array_circular<T>& y)
{
    swap(x.header, y.header);
}

template <Movable T>
constexpr void
reserve(
    array_circular<T>& x,
    Size_type<array_circular<T>> n,
    Size_type<array_circular<T>> offset = Zero<Size_type<array_circular<T>>>)
{
    if (n < size(x) or n == capacity(x)) return;
    array_circular<T> temp(n);
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

template <Movable T, typename U>
constexpr auto
insert(back<array_circular<T>> arr, U&& x) -> back<array_circular<T>>
{
    auto& seq = base(arr);
    if (size(seq) == capacity(seq)) {
        reserve(seq, max(One<Size_type<array_circular<T>>>, twice(size(seq))));
    } else if (load(seq.header).limit == load(seq.header).limit_of_storage) {
        at(seq.header).limit = pointer_to(at(seq.header).x);
    }
    auto& header = at(seq.header);
    construct(at(header.limit), fw<U>(x));
    increment(header.limit);
    increment(header.size);
    return seq;
}

template <Movable T, typename U>
constexpr auto
insert(front<array_circular<T>> arr, U&& x) -> front<array_circular<T>>
{
    auto& seq = base(arr);
    if (size(seq) == capacity(seq)) {
        reserve(seq, max(One<Size_type<array_circular<T>>>, twice(size(seq))), size(seq));
    } else if (load(seq.header).first == pointer_to(load(seq.header).x)) {
        at(seq.header).first = load(seq.header).limit_of_storage;
    }
    auto& header = at(seq.header);
    decrement(header.first);
    construct(at(header.first), fw<U>(x));
    increment(header.size);
    return seq;
}

template <Movable T, typename U>
constexpr void
emplace(array_circular<T>& arr, U&& x)
{
    insert(back{arr}, fw<U>(x));
}

template <Movable T, typename U>
constexpr void
push(array_circular<T>& arr, U x)
{
    insert(back{arr}, mv(x));
}

template <Movable T, typename U>
constexpr void
emplace_first(array_circular<T>& arr, U&& x)
{
    insert(front{arr}, fw<U>(x));
}

template <Movable T, typename U>
constexpr void
push_first(array_circular<T>& arr, T x)
{
    insert(front{arr}, mv(x));
}

template <Movable T>
constexpr auto
erase(back<array_circular<T>> arr) -> back<array_circular<T>>
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
        deallocate_array_circular(header);
        header = nullptr;
    }
    return arr;
}

template <Movable T>
constexpr auto
erase(front<array_circular<T>> arr) -> front<array_circular<T>>
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
        deallocate_array_circular(header);
        header = nullptr;
    }
    return arr;
}

template <Movable T>
constexpr void
erase_all(array_circular<T>& x)
{
    while (!is_empty(x)) erase(back{x});
}

template <Movable T>
constexpr void
pop(array_circular<T>& arr)
{
    erase(back{arr});
}

template <Movable T>
constexpr void
pop_first(array_circular<T>& arr)
{
    erase(front{arr});
}

template <Movable T>
constexpr auto
first(array_circular<T> const& x) -> Cursor_type<array_circular<T>>
{
    if (x.header == nullptr) return {};
    return array_circular_cursor{const_cast<Pointer_type<array_circular<T>>>(pointer_to(x))};
}

template <Movable T>
constexpr auto
limit(array_circular<T> const& x) -> Cursor_type<array_circular<T>>
{
    if (x.header == nullptr) return {};
    return array_circular_cursor{const_cast<Pointer_type<array_circular<T>>>(pointer_to(x)), size(x)};
}

template <Movable T>
constexpr auto
is_empty(array_circular<T> const& x) -> bool
{
    if (x.header == nullptr) return true;
    return load(x.header).size == Zero<Size_type<array_circular<T>>>;
}

template <Movable T>
constexpr auto
size(array_circular<T> const& x) -> Size_type<array_circular<T>>
{
    if (x.header == nullptr) return Zero<Size_type<array_circular<T>>>;
    return load(x.header).size;
}

template <Movable T>
constexpr auto
capacity(array_circular<T> const& x) -> Size_type<array_circular<T>>
{
    if (x.header == nullptr) return Zero<Size_type<array_circular<T>>>;
    return load(x.header).limit_of_storage - pointer_to(load(x.header).x);
}

}
