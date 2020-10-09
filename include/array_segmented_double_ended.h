#pragma once

#include "flat_map.h"
#include "functional.h"
#include "lexicographical.h"
#include "map.h"
#include "ordering.h"
#include "swap.h"
#include "array_double_ended.h"

#define byte char

namespace elements {

template <Movable T, pointer_diff k = 256>
requires (0 < k) and (k <= std::numeric_limits<pointer_diff>::max() / sizeof(T))
struct array_segmented_double_ended;

template <Movable T, pointer_diff k>
struct array_segmented_double_ended_cursor
{
    Pointer_type<array_double_ended<T>> index{};
    Pointer_type<T> segment{};

    constexpr
    array_segmented_double_ended_cursor() = default;

    constexpr
    array_segmented_double_ended_cursor(
        Pointer_type<array_double_ended<T>> index_,
        Pointer_type<T> segment_
    )
        : index{index_}
        , segment{segment_}
    {}
};

template <Movable T, pointer_diff k>
struct value_type_t<array_segmented_double_ended_cursor<T, k>>
{
    using type = T;
};

template <Movable T, pointer_diff k>
struct difference_type_t<array_segmented_double_ended_cursor<T, k>>
{
    using type = Difference_type<Pointer_type<T>>;
};

template <Movable T, pointer_diff k>
struct index_cursor_type_t<array_segmented_double_ended_cursor<T, k>>
{
    using type = Pointer_type<array_double_ended<T>>;
};

template <Movable T, pointer_diff k>
struct segment_cursor_type_t<array_segmented_double_ended_cursor<T, k>>
{
    using type = Pointer_type<T>;
};

template <Movable T, pointer_diff k>
constexpr auto
operator==(array_segmented_double_ended_cursor<T, k> const& cur0, array_segmented_double_ended_cursor<T, k> const& cur1) -> bool
{
    return cur0.index == cur1.index and cur0.segment == cur1.segment;
}

template <Movable T, pointer_diff k>
constexpr auto
index_cursor(array_segmented_double_ended_cursor<T, k> const& cur) -> Index_cursor_type<array_segmented_double_ended_cursor<T, k>> const&
{
    return cur.index;
}

template <Movable T, pointer_diff k>
constexpr auto
index_cursor(array_segmented_double_ended_cursor<T, k>& cur) -> Index_cursor_type<array_segmented_double_ended_cursor<T, k>>&
{
    return cur.index;
}

template <Movable T, pointer_diff k>
constexpr auto
segment_cursor(array_segmented_double_ended_cursor<T, k> const& cur) -> Segment_cursor_type<array_segmented_double_ended_cursor<T, k>> const&
{
    return cur.segment;
}

template <Movable T, pointer_diff k>
constexpr auto
segment_cursor(array_segmented_double_ended_cursor<T, k>& cur) -> Segment_cursor_type<array_segmented_double_ended_cursor<T, k>>&
{
    return cur.segment;
}

template <Movable T, pointer_diff k>
constexpr auto
load(array_segmented_double_ended_cursor<T, k> const& cur) -> T const&
{
    return load(cur.segment);
}

template <Movable T, pointer_diff k>
constexpr void
store(array_segmented_double_ended_cursor<T, k>& cur, T const& value)
{
    store(cur.segment, value);
}

template <Movable T, pointer_diff k>
constexpr void
store(array_segmented_double_ended_cursor<T, k>& cur, Value_type<T>&& value)
{
    store(cur.segment, fw<T>(value));
}

template <Movable T, pointer_diff k>
constexpr auto
at(array_segmented_double_ended_cursor<T, k> const& cur) -> T const&
{
    return at(segment_cur(cur));
}

template <Movable T, pointer_diff k>
constexpr auto
at(array_segmented_double_ended_cursor<T, k>& cur) -> T&
{
    return at(segment_cur(cur));
}

template <Movable T, pointer_diff k>
constexpr auto
precedes(array_segmented_double_ended_cursor<T, k> const& cur0, array_segmented_double_ended_cursor<T, k> const& cur1) -> bool
{
    return precedes(cur0.index, cur1.index) or precedes(cur0.segment, cur1.segment);
}

template <Movable T, pointer_diff k>
requires (0 < k) and (k <= std::numeric_limits<pointer_diff>::max() / sizeof(T))
struct array_segmented_double_ended
{
    array_double_ended<array_double_ended<T>> index;

    constexpr
    array_segmented_double_ended() = default;

    constexpr
    array_segmented_double_ended(std::initializer_list<T> x)
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    array_segmented_double_ended(Size_type<array_segmented_double_ended<T, k>> size, T const& x)
    {
        if (size == Zero<decltype(size)>) return;
        auto n_segments{size / k + min(One<decltype(size)>, size % k)};
        index = array_double_ended<array_double_ended<T>>(n_segments);
        while (precedes(limit(index), limit_of_storage(index))) {
            emplace(index, array_double_ended<T>(k, x));
        }
    }

    constexpr auto
    operator[](pointer_diff i) -> T&
    {
        auto& first_segment{at(first(index))};
        if (size(index) == 1) {
            return first_segment[i];
        } else {
            auto j = i + (k - size(first_segment));
            return at(first_of_storage(index[j / k]) + (j % k));
        }
    }

    constexpr auto
    operator[](pointer_diff i) const -> T const&
    {
        auto const& first_segment{load(first(index))};
        if (size(index) == 1) {
            return first_segment[i];
        } else {
            auto j = i + (k - size(load(first(index))));
            return load(first_of_storage(index[j / k]) + (j % k));
        }
    }

    template <Operation<T> Op>
    constexpr auto
    fmap(Op op) -> array_segmented_double_ended<T, k>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{op}(first(at(this))));
        return at(this);
    }

    template <Regular_invocable<T> F>
    constexpr auto
    fmap(F fun) const -> array_segmented_double_ended<Return_type<F, T>, k>
    {
        using elements::map;
        array_segmented_double_ended<Return_type<F, T>, k> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }

    template <Regular_invocable<T> F>
    requires Same_as<array_segmented_double_ended<Decay<T>>, Return_type<F, T>>
    constexpr auto
    flat_map(F fun) -> array_segmented_double_ended<T, k>&
    {
        using elements::flat_map;
        auto x = flat_map(first(at(this)), limit(at(this)), fun);
        swap(at(this), x);
        return at(this);
    }

    template <Regular_invocable<T> F>
    requires Regular<T>
    constexpr auto
    flat_map(F fun) const -> Return_type<F, T>
    {
        using elements::flat_map;
        return flat_map(first(at(this)), limit(at(this)), fun);
    }
};

template <Movable T, pointer_diff k>
struct value_type_t<array_segmented_double_ended<T, k>>
{
    using type = T;
};

template <Movable T, pointer_diff k>
struct cursor_type_t<array_segmented_double_ended<T, k>>
{
    using type = array_segmented_double_ended_cursor<T, k>;
};

template <Movable T, pointer_diff k>
struct cursor_type_t<array_segmented_double_ended<T, k> const>
{
    using type = array_segmented_double_ended_cursor<T const, k>;
};

template <Movable T, pointer_diff k>
struct size_type_t<array_segmented_double_ended<T, k>>
{
    using type = Difference_type<array_segmented_double_ended_cursor<T, k>>;
};

template <Movable T, pointer_diff k>
constexpr auto
operator==(array_segmented_double_ended<T, k> const& x, array_segmented_double_ended<T, k> const& y) -> bool
{
    return x.index == y.index;
}

template <Movable T, pointer_diff k>
constexpr auto
operator<(array_segmented_double_ended<T, k> const& x, array_segmented_double_ended<T, k> const& y) -> bool
{
    return x.index < y.index;
}

template <Movable T, pointer_diff k>
constexpr void
swap(array_segmented_double_ended<T, k>& x, array_segmented_double_ended<T, k>& y)
{
    swap(x.index, y.index);
}

template <Movable T, pointer_diff k, typename U>
constexpr auto
insert(back<array_segmented_double_ended<T, k>> arr, U&& x) -> back<array_segmented_double_ended<T, k>>
{
    auto& seq = base(arr);
    if (!precedes(limit(seq), limit_of_storage(seq))) {
        if (is_empty(seq)) {
            emplace(seq.index, array_double_ended<T>(k, half(k)));
        } else {
            emplace(seq.index, array_double_ended<T>(k, Zero<decltype(k)>));
        }
    }
    emplace(at(predecessor(limit(seq.index))), fw<U>(x));
    return seq;
}

template <Movable T, pointer_diff k, typename U>
constexpr auto
insert(front<array_segmented_double_ended<T, k>> arr, U&& x) -> front<array_segmented_double_ended<T, k>>
{
    auto& seq = base(arr);
    if (!precedes(first(seq), first_of_storage(seq))) {
        if (is_empty(seq)) {
            emplace_first(seq.index, array_double_ended<T>(k, half(k)));
        } else {
            emplace_first(seq.index, array_double_ended<T>(k, k));
        }
    }
    emplace_first(at(first(seq.index)), fw<U>(x));
    return seq;
}

template <Movable T, pointer_diff k, typename U>
constexpr void
emplace(array_segmented_double_ended<T, k>& arr, U&& x)
{
    insert(back{arr}, fw<U>(x));
}

template <Movable T, pointer_diff k, typename U>
constexpr void
emplace_first(array_segmented_double_ended<T, k>& arr, U&& x)
{
    insert(front{arr}, fw<U>(x));
}

template <Movable T, pointer_diff k, typename U>
constexpr void
push(array_segmented_double_ended<T, k>& arr, U x)
{
    insert(back{arr}, mv(x));
}

template <Movable T, pointer_diff k, typename U>
constexpr void
push_first(array_segmented_double_ended<T, k>& arr, U x)
{
    insert(front{arr}, mv(x));
}

template <Movable T, pointer_diff k>
constexpr auto
erase(back<array_segmented_double_ended<T, k>> arr) -> back<array_segmented_double_ended<T, k>>
{
    auto& seq = base(arr);
    auto cur{limit(seq)};
    pop(at(index_cur(cur)));
    if (is_empty(load(index_cur(cur)))) {
        pop(seq.index);
    }
    return arr;
}

template <Movable T, pointer_diff k>
constexpr auto
erase(front<array_segmented_double_ended<T, k>> arr) -> front<array_segmented_double_ended<T, k>>
{
    auto& seq = base(arr);
    auto cur{first(seq)};
    pop_first(at(index_cur(cur)));
    if (is_empty(load(index_cur(cur)))) {
        pop_first(seq.index);
    }
    return arr;
}

template <Movable T, pointer_diff k>
constexpr void
erase_all(array_segmented_double_ended<T, k>& x)
{
    erase_all(x.index);
}

template <Movable T, pointer_diff k>
constexpr void
pop(array_segmented_double_ended<T, k>& arr)
{
    erase(back{arr});
}

template <Movable T, pointer_diff k>
constexpr void
pop_first(array_segmented_double_ended<T, k>& arr)
{
    erase(front{arr});
}

template <Movable T, pointer_diff k>
constexpr auto
first(array_segmented_double_ended<T, k> const& x) -> Cursor_type<array_segmented_double_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {first(x.index), first(at(first(x.index)))};
}

template <Movable T, pointer_diff k>
constexpr auto
limit(array_segmented_double_ended<T, k> const& x) -> Cursor_type<array_segmented_double_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {predecessor(limit(x.index)), limit(at(predecessor(limit(x.index))))};
}

template <Movable T, pointer_diff k>
constexpr auto
first_of_storage(array_segmented_double_ended<T, k> const& x) -> Cursor_type<array_segmented_double_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {first(x.index), first_of_storage(at(first(x.index)))};
}

template <Movable T, pointer_diff k>
constexpr auto
limit_of_storage(array_segmented_double_ended<T, k> const& x) -> Cursor_type<array_segmented_double_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {predecessor(limit(x.index)), limit_of_storage(at(predecessor(limit(x.index))))};
}

template <Movable T, pointer_diff k>
constexpr auto
is_empty(array_segmented_double_ended<T, k> const& x) -> bool
{
    return is_empty(x.index);
}

template <Movable T, pointer_diff k>
constexpr auto
size(array_segmented_double_ended<T, k> const& x) -> Size_type<array_segmented_double_ended<T, k>>
{
    if (is_empty(x)) return 0;
    if (size(x.index) == 1) return size(load(first(x.index)));
    if (size(x.index) == 2) return size(load(first(x.index))) + size(load(predecessor(limit(x.index))));
    return size(load(first(x.index))) + (predecessor(predecessor(size(x.index))) * k) + size(load(predecessor(limit(x.index))));
}

template <Movable T, pointer_diff k>
constexpr auto
capacity(array_segmented_double_ended<T, k> const& x) -> Size_type<array_segmented_double_ended<T, k>>
{
    return size(x.index) * k;
}

}
