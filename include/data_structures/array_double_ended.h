#pragma once

#include <initializer_list>

#include "adapters/reverse_position.h"
#include "algorithms/algebra.h"
#include "algorithms/copy.h"
#include "algorithms/map.h"
#include "algorithms/ordering.h"
#include "algorithms/position.h"
#include "algorithms/swap.h"
#include "concepts/invocable.h"
#include "concepts/position.h"
#include "concepts/regular.h"

namespace elements {

template <typename T>
requires Semiregular<Remove_const<T>>
struct array_double_ended_prefix
{
    Pointer_type<T> first;
    Pointer_type<T> limit;
    Pointer_type<T> limit_of_storage;
    T x;
};

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
allocate_array_double_ended(
    Difference_type<Pointer_type<T>> n,
    Difference_type<Pointer_type<T>> offset) -> Pointer_type<array_double_ended_prefix<T>>
{
    if (is_zero(n)) return nullptr;
    auto remote = allocate<array_double_ended_prefix<T>>(predecessor(n) * static_cast<pointer_diff>(sizeof(T)));
    at(remote).first = pointer_to(at(remote).x) + offset;
    at(remote).limit = load(remote).first;
    at(remote).limit_of_storage = pointer_to(at(remote).x) + n;
    return remote;
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr void
deallocate_array_double_ended(Pointer_type<array_double_ended_prefix<T>> prefix)
{
    deallocate(prefix);
}

template <typename T>
requires Semiregular<Remove_const<T>>
struct array_double_ended
{
    Pointer_type<array_double_ended_prefix<T>> header{nullptr};

    constexpr
    array_double_ended() = default;

    constexpr
    array_double_ended(array_double_ended const& x)
        : header{allocate_array_double_ended<T>(size(x), Zero<Size_type<array_double_ended<T>>>)}
    {
        at(header).limit = copy(first(x), limit(x), first(at(this)));
    }

    constexpr
    array_double_ended(array_double_ended&& x)
    {
        header = x.header;
        x.header = nullptr;
    }

    constexpr
    array_double_ended(std::initializer_list<T> x)
        : header{allocate_array_double_ended<T>(
            Size_type<array_double_ended<T>>{static_cast<pointer_diff>(std::size(x))},
            Zero<Size_type<array_double_ended<T>>>)}
    {
        at(header).limit = copy(std::begin(x), std::end(x), first(at(this)));
    }

    constexpr
    array_double_ended(Size_type<array_double_ended<T>> capacity)
        : array_double_ended(capacity, Zero<Size_type<array_double_ended<T>>>)
    {}

    constexpr
    array_double_ended(
        Size_type<array_double_ended<T>> capacity,
        Size_type<array_double_ended<T>> offset)
        : header{allocate_array_double_ended<T>(capacity, offset)}
    {}

    constexpr
    array_double_ended(
        Size_type<array_double_ended<T>> capacity,
        Size_type<array_double_ended<T>> size,
        T const& x)
        : array_double_ended<T>(capacity, Zero<Size_type<array_double_ended<T>>>, size, x)
    {}

    constexpr
    array_double_ended(
        Size_type<array_double_ended<T>> capacity,
        Size_type<array_double_ended<T>> offset,
        Size_type<array_double_ended<T>> size,
        T const& x)
        : header{allocate_array_double_ended<T>(capacity, offset)}
    {
        while (!is_zero(size)) { push(at(this), x); decrement(size); }
    }

    constexpr auto
    operator=(array_double_ended const& x) -> array_double_ended&
    {
        array_double_ended temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
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
        return at(first(at(this)) + i);
    }

    template <Unary_function Fun>
    requires
        Same<Decay<T>, Domain<Fun>> and
        Same<Decay<T>, Codomain<Fun>>
    constexpr auto
    map(Fun fun) -> array_double_ended<T>&
    {
        using elements::map;
        map(first(*this), limit(*this), first(*this), fun);
        return *this;
    }

    template <Unary_function Fun>
    requires Same<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> array_double_ended<Codomain<Fun>>
    {
        using elements::map;
        array_double_ended<Codomain<Fun>> x;
        reserve(x, size(*this), Zero<Size_type<array_double_ended<T>>>);
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
            reserve(x, size(x) + size(y), Zero<Size_type<array_double_ended<T>>>);
            auto dst = first(x) + size(x);
            copy(first(y), limit(y), dst);
            at(x.header).limit = at(x.header).limit_of_storage;
            increment(src);
        }
        return x;
    }
};

template <typename T>
requires Semiregular<Remove_const<T>>
struct value_type_t<array_double_ended<T>>
{
    using type = T;
};

template <typename T>
requires Semiregular<Remove_const<T>>
struct position_type_t<array_double_ended<T>>
{
    using type = Pointer_type<T>;
};

template <typename T>
requires Semiregular<Remove_const<T>>
struct position_type_t<array_double_ended<T> const>
{
    using type = Pointer_type<T const>;
};

template <typename T>
requires Semiregular<Remove_const<T>>
struct size_type_t<array_double_ended<T>>
{
    using type = pointer_diff;
};

template <typename T>
requires Regular<Remove_const<T>>
constexpr auto
operator==(array_double_ended<T> const& x, array_double_ended<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <typename T>
requires Default_totally_ordered<Remove_const<T>>
constexpr auto
operator<(array_double_ended<T> const& x, array_double_ended<T> const& y) -> bool
{
    return less_range(x, y);
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr void
reserve(
    array_double_ended<T>& x,
    Size_type<array_double_ended<T>> n,
    Size_type<array_double_ended<T>> offset)
{
    if (n < size(x) or n == capacity(x)) return;
    array_double_ended<T> temp(n);
    at(temp.header).limit = swap(first(x), limit(x), first(temp) + offset);
    at(temp.header).first = first(temp) + offset;
    swap(x, temp);
}

template <typename T, typename U>
requires
    Semiregular<Remove_const<T>> and
    Semiregular<Remove_const<U>> and
    Constructible<T, U>
constexpr auto
insert(back<array_double_ended<T>> arr, U const& x) -> back<array_double_ended<T>>
{
    using S = Size_type<array_double_ended<T>>;
    using P = Position_type<array_double_ended<T>>;
    auto& seq = at(arr.range);
    if (is_full(seq))
    {
        if (first(seq) == first_of_storage(seq)) {
            auto n = size(seq);
            reserve(seq, max(One<S>, twice(n)), Zero<S>);
        } else {
            P pos = first_of_storage(seq) + half(first(seq) - first_of_storage(seq));
            at(seq.header).limit = copy(first(seq), limit(seq), pos);
            at(seq.header).first = pos;
        }
    }
    construct(at(load(seq.header).limit), x);
    increment(at(seq.header).limit);
    return seq;
}

template <typename T, typename U>
requires
    Semiregular<Remove_const<T>> and
    Semiregular<Remove_const<U>> and
    Constructible<T, U>
constexpr auto
insert(front<array_double_ended<T>> arr, U const& x) -> back<array_double_ended<T>>
{
    using S = Size_type<array_double_ended<T>>;
    using P = Position_type<array_double_ended<T>>;
    auto& seq = at(arr.range);
    if (first(seq) == first_of_storage(seq))
    {
        if (limit(seq) == limit_of_storage(seq)) {
            auto n = size(seq);
            reserve(seq, max(One<S>, twice(n)), n);
        } else {
            P pos = limit(seq) + successor(half(limit_of_storage(seq) - limit(seq)));
            at(seq.header).first = copy(
                reverse_position(limit(seq)),
                reverse_position(first(seq)),
                reverse_position(pos)).pos;
            at(seq.header).limit = pos;
        }
    }
    decrement(at(seq.header).first);
    construct(at(load(seq.header).first), x);
    return seq;
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr void
push(array_double_ended<T>& arr, T const& x)
{
    insert(back<array_double_ended<T>>(arr), x);
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr void
push_first(array_double_ended<T>& arr, T const& x)
{
    insert(front<array_double_ended<T>>(arr), x);
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
erase(back<array_double_ended<T>> arr) -> back<array_double_ended<T>>
{
    auto& header = at(arr.range).header;
    decrement(at(header).limit);
    destroy(at(load(header).limit));
    if (is_empty(at(arr.range))) {
        deallocate_array_double_ended(header);
        header = nullptr;
    }
    return arr;
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
erase(front<array_double_ended<T>> arr) -> front<array_double_ended<T>>
{
    auto& header = at(arr.range).header;
    destroy(at(load(header).first));
    increment(at(header).first);
    if (is_empty(at(arr.range))) {
        deallocate_array_double_ended(header);
        header = nullptr;
    }
    return arr;
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr void
erase_all(array_double_ended<T>& x)
{
    while (!is_empty(x)) erase(back<array_double_ended<T>>(x));
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr void
pop(array_double_ended<T>& arr)
{
    erase(back<array_double_ended<T>>(arr));
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr void
pop_first(array_double_ended<T>& arr)
{
    erase(front<array_double_ended<T>>(arr));
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
first(array_double_ended<T> const& x) -> Position_type<array_double_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).first;
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
limit(array_double_ended<T> const& x) -> Position_type<array_double_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit;
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
first_of_storage(array_double_ended<T> const& x) -> Position_type<array_double_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return pointer_to(at(x.header).x);
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
limit_of_storage(array_double_ended<T> const& x) -> Position_type<array_double_ended<T>>
{
    if (x.header == nullptr) return nullptr;
    return at(x.header).limit_of_storage;
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
is_empty(array_double_ended<T> const& x) -> bool
{
    return first(x) == limit(x);
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
is_full(array_double_ended<T> const& x) -> bool
{
    return limit(x) == limit_of_storage(x);
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
size(array_double_ended<T> const& x) -> Size_type<array_double_ended<T>>
{
    return limit(x) - first(x);
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
capacity(array_double_ended<T> const& x) -> Size_type<array_double_ended<T>>
{
    return limit_of_storage(x) - first_of_storage(x);
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
capacity_back(array_double_ended<T> const& x) -> Size_type<array_double_ended<T>>
{
    return limit_of_storage(x) - first(x);
}

template <typename T>
requires Semiregular<Remove_const<T>>
constexpr auto
capacity_front(array_double_ended<T> const& x) -> Size_type<array_double_ended<T>>
{
    return limit(x) - first_of_storage(x);
}

}
