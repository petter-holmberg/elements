#pragma once

#include "flat_map.h"
#include "functional.h"
#include "lexicographical.h"
#include "map.h"
#include "ordering.h"
#include "swap.h"
#include "array_single_ended.h"

#define byte char

namespace elements {

template <Movable T, pointer_diff k = 256>
requires (0 < k) and (k <= std::numeric_limits<pointer_diff>::max() / sizeof(T))
struct array_segmented_single_ended;

template <Movable T, pointer_diff k>
struct array_segmented_single_ended_cursor
{
    Pointer_type<array_single_ended<T>> index{};
    Pointer_type<T> segment{};

    constexpr
    array_segmented_single_ended_cursor() = default;

    constexpr
    array_segmented_single_ended_cursor(
        Pointer_type<array_single_ended<T>> index_,
        Pointer_type<T> segment_
    )
        : index{index_}
        , segment{segment_}
    {}
};

template <Movable T, pointer_diff k>
struct value_type_t<array_segmented_single_ended_cursor<T, k>>
{
    using type = T;
};

template <Movable T, pointer_diff k>
struct difference_type_t<array_segmented_single_ended_cursor<T, k>>
{
    using type = Difference_type<Pointer_type<T>>;
};

template <Movable T, pointer_diff k>
struct index_cursor_type_t<array_segmented_single_ended_cursor<T, k>>
{
    using type = Pointer_type<array_single_ended<T>>;
};

template <Movable T, pointer_diff k>
struct segment_cursor_type_t<array_segmented_single_ended_cursor<T, k>>
{
    using type = Pointer_type<T>;
};

template <Movable T, pointer_diff k>
constexpr auto
operator==(array_segmented_single_ended_cursor<T, k> const& cur0, array_segmented_single_ended_cursor<T, k> const& cur1) -> bool
{
    return cur0.index == cur1.index and cur0.segment == cur1.segment;
}

template <Movable T, pointer_diff k>
constexpr auto
index_cursor(array_segmented_single_ended_cursor<T, k> const& cur) -> Index_cursor_type<array_segmented_single_ended_cursor<T, k>> const&
{
    return cur.index;
}

template <Movable T, pointer_diff k>
constexpr auto
index_cursor(array_segmented_single_ended_cursor<T, k>& cur) -> Index_cursor_type<array_segmented_single_ended_cursor<T, k>>&
{
    return cur.index;
}

template <Movable T, pointer_diff k>
constexpr auto
segment_cursor(array_segmented_single_ended_cursor<T, k> const& cur) -> Segment_cursor_type<array_segmented_single_ended_cursor<T, k>> const&
{
    return cur.segment;
}

template <Movable T, pointer_diff k>
constexpr auto
segment_cursor(array_segmented_single_ended_cursor<T, k>& cur) -> Segment_cursor_type<array_segmented_single_ended_cursor<T, k>>&
{
    return cur.segment;
}

template <Movable T, pointer_diff k>
constexpr auto
load(array_segmented_single_ended_cursor<T, k> const& cur) -> T const&
{
    return load(cur.segment);
}

template <Movable T, pointer_diff k>
constexpr void
store(array_segmented_single_ended_cursor<T, k>& cur, T const& value)
{
    store(cur.segment, value);
}

template <Movable T, pointer_diff k>
constexpr void
store(array_segmented_single_ended_cursor<T, k>& cur, Value_type<T>&& value)
{
    store(cur.segment, fw<T>(value));
}

template <Movable T, pointer_diff k>
constexpr auto
at(array_segmented_single_ended_cursor<T, k> const& cur) -> T const&
{
    return at(segment_cursor(cur));
}

template <Movable T, pointer_diff k>
constexpr auto
at(array_segmented_single_ended_cursor<T, k>& cur) -> T&
{
    return at(segment_cursor(cur));
}

template <Movable T, pointer_diff k>
constexpr auto
precedes(array_segmented_single_ended_cursor<T, k> const& cur0, array_segmented_single_ended_cursor<T, k> const& cur1) -> bool
{
    return precedes(cur0.index, cur1.index) or precedes(cur0.segment, cur1.segment);
}

template <Movable T, pointer_diff k>
requires (0 < k) and (k <= std::numeric_limits<pointer_diff>::max() / sizeof(T))
struct array_segmented_single_ended
{
    array_single_ended<array_single_ended<T>> index;

    constexpr
    array_segmented_single_ended() = default;

    constexpr
    array_segmented_single_ended(std::initializer_list<T> x)
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    array_segmented_single_ended(Size_type<array_segmented_single_ended<T, k>> size, T const& x)
    {
        if (size == Zero<decltype(size)>) return;
        auto n_segments{size / k + min(One<decltype(size)>, size % k)};
        index = array_single_ended<array_single_ended<T>>(n_segments);
        while (precedes(limit(index), limit_of_storage(index))) {
            emplace(index, array_single_ended<T>(k, x));
        }
    }

    constexpr auto
    operator[](pointer_diff i) -> T&
    {
        return index[i / k][i % k];
    }

    constexpr auto
    operator[](pointer_diff i) const -> T const&
    {
        return index[i / k][i % k];
    }

    template <Unary_function F>
    requires
        Same_as<Decay<T>, Domain<F>> and
        Same_as<Decay<T>, Codomain<F>>
    constexpr auto
    fmap(F fun) -> array_segmented_single_ended<T, k>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function F>
    requires Same_as<Decay<T>, Domain<F>>
    constexpr auto
    fmap(F fun) const -> array_segmented_single_ended<Codomain<F>, k>
    {
        using elements::map;
        array_segmented_single_ended<Codomain<F>, k> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }

    template <Unary_function F>
    requires
        Same_as<Decay<T>, Domain<F>> and
        Same_as<array_segmented_single_ended<Decay<T>>, Codomain<F>>
    constexpr auto
    flat_map(F fun) -> array_segmented_single_ended<T, k>&
    {
        using elements::flat_map;
        auto x = flat_map(first(at(this)), limit(at(this)), fun);
        swap(at(this), x);
        return at(this);
    }

    template <Unary_function F>
    requires
        Same_as<Decay<T>, Domain<F>> and
        Regular<T>
    constexpr auto
    flat_map(F fun) const -> Codomain<F>
    {
        using elements::flat_map;
        return flat_map(first(at(this)), limit(at(this)), fun);
    }
};

template <Movable T, Unary_function F>
constexpr auto
chain(array_segmented_single_ended<T>& x, F fun) -> array_segmented_single_ended<Value_type<Codomain<F>>>
{
    return x.flat_map(fun);
}

template <Movable T, pointer_diff k>
struct value_type_t<array_segmented_single_ended<T, k>>
{
    using type = T;
};

template <Movable T, pointer_diff k>
struct cursor_type_t<array_segmented_single_ended<T, k>>
{
    using type = array_segmented_single_ended_cursor<T, k>;
};

template <Movable T, pointer_diff k>
struct cursor_type_t<array_segmented_single_ended<T, k> const>
{
    using type = array_segmented_single_ended_cursor<T const, k>;
};

template <Movable T, pointer_diff k>
struct size_type_t<array_segmented_single_ended<T, k>>
{
    using type = Difference_type<array_segmented_single_ended_cursor<T, k>>;
};

template <Movable T, pointer_diff k>
constexpr auto
operator==(array_segmented_single_ended<T, k> const& x, array_segmented_single_ended<T, k> const& y) -> bool
{
    return x.index == y.index;
}

template <Movable T, pointer_diff k>
constexpr auto
operator<(array_segmented_single_ended<T, k> const& x, array_segmented_single_ended<T, k> const& y) -> bool
{
    return x.index < y.index;
}

template <Movable T, pointer_diff k>
constexpr void
swap(array_segmented_single_ended<T, k>& x, array_segmented_single_ended<T, k>& y)
{
    swap(x.index, y.index);
}

template <Movable T, pointer_diff k, typename U>
constexpr auto
insert(back<array_segmented_single_ended<T, k>> arr, U&& x) -> back<array_segmented_single_ended<T, k>>
{
    auto& seq = base(arr);
    if (!precedes(limit(seq), limit_of_storage(seq))) {
        emplace(seq.index, array_single_ended<T>(k));
    }
    emplace(at(predecessor(limit(seq.index))), fw<U>(x));
    return seq;
}

template <Movable T, pointer_diff k, typename U>
constexpr void
emplace(array_segmented_single_ended<T, k>& arr, U&& x)
{
    insert(back{arr}, fw<U>(x));
}

template <Movable T, pointer_diff k, typename U>
constexpr void
push(array_segmented_single_ended<T, k>& arr, U x)
{
    insert(back{arr}, mv(x));
}

template <Movable T, pointer_diff k>
constexpr auto
erase(back<array_segmented_single_ended<T, k>> arr) -> back<array_segmented_single_ended<T, k>>
{
    auto& seq = base(arr);
    auto cur{limit(seq)};
    pop(at(index_cursor(cur)));
    if (is_empty(load(index_cursor(cur)))) {
        pop(seq.index);
    }
    return arr;
}

template <Movable T, pointer_diff k>
constexpr void
erase_all(array_segmented_single_ended<T, k>& x)
{
    erase_all(x.index);
}

template <Movable T, pointer_diff k>
constexpr void
pop(array_segmented_single_ended<T, k>& arr)
{
    erase(back{arr});
}

template <Movable T, pointer_diff k>
constexpr auto
first(array_segmented_single_ended<T, k> const& x) -> Cursor_type<array_segmented_single_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {first(x.index), first(at(first(x.index)))};
}

template <Movable T, pointer_diff k>
constexpr auto
limit(array_segmented_single_ended<T, k> const& x) -> Cursor_type<array_segmented_single_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {predecessor(limit(x.index)), limit(at(predecessor(limit(x.index))))};
}

template <Movable T, pointer_diff k>
constexpr auto
limit_of_storage(array_segmented_single_ended<T, k> const& x) -> Cursor_type<array_segmented_single_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {predecessor(limit(x.index)), limit_of_storage(at(predecessor(limit(x.index))))};
}

template <Movable T, pointer_diff k>
constexpr auto
is_empty(array_segmented_single_ended<T, k> const& x) -> bool
{
    return is_empty(x.index);
}

template <Movable T, pointer_diff k>
constexpr auto
size(array_segmented_single_ended<T, k> const& x) -> Size_type<array_segmented_single_ended<T, k>>
{
    if (is_empty(x)) return 0;
    return predecessor(size(x.index)) * k + size(load(predecessor(limit(x.index))));
}

template <Movable T, pointer_diff k>
constexpr auto
capacity(array_segmented_single_ended<T, k> const& x) -> Size_type<array_segmented_single_ended<T, k>>
{
    return size(x.index) * k;
}

}
