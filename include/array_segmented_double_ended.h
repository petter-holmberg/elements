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

template <typename T, pointer_diff k = 256, Invocable auto alloc = array_allocator<T>>
//requires (0 < k) and (k <= std::numeric_limits<pointer_diff>::max() / sizeof(T))
struct array_segmented_double_ended;

template <typename T, pointer_diff k, Invocable auto alloc>
struct array_segmented_double_ended_cursor
{
    Pointer_type<array_double_ended<T, alloc>> index{};
    Pointer_type<T> segment{};

    constexpr
    array_segmented_double_ended_cursor() = default;

    constexpr
    array_segmented_double_ended_cursor(
        Pointer_type<array_double_ended<T, alloc>> index_,
        Pointer_type<T> segment_
    )
        : index{index_}
        , segment{segment_}
    {}
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct value_type_t<array_segmented_double_ended_cursor<T, k, alloc>>
{
    using type = T;
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct difference_type_t<array_segmented_double_ended_cursor<T, k, alloc>>
{
    using type = Difference_type<Pointer_type<T>>;
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct index_cursor_type_t<array_segmented_double_ended_cursor<T, k, alloc>>
{
    using type = Pointer_type<array_double_ended<T, alloc>>;
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct segment_cursor_type_t<array_segmented_double_ended_cursor<T, k, alloc>>
{
    using type = Pointer_type<T>;
};

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
operator==(array_segmented_double_ended_cursor<T, k, alloc> const& cur0, array_segmented_double_ended_cursor<T, k, alloc> const& cur1) -> bool
{
    return cur0.index == cur1.index and cur0.segment == cur1.segment;
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
index_cursor(array_segmented_double_ended_cursor<T, k, alloc> const& cur) -> Index_cursor_type<array_segmented_double_ended_cursor<T, k, alloc>> const&
{
    return cur.index;
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
index_cursor(array_segmented_double_ended_cursor<T, k, alloc>& cur) -> Index_cursor_type<array_segmented_double_ended_cursor<T, k, alloc>>&
{
    return cur.index;
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
segment_cursor(array_segmented_double_ended_cursor<T, k, alloc> const& cur) -> Segment_cursor_type<array_segmented_double_ended_cursor<T, k, alloc>> const&
{
    return cur.segment;
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
segment_cursor(array_segmented_double_ended_cursor<T, k, alloc>& cur) -> Segment_cursor_type<array_segmented_double_ended_cursor<T, k, alloc>>&
{
    return cur.segment;
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
load(array_segmented_double_ended_cursor<T, k, alloc> const& cur) -> T const&
{
    return load(cur.segment);
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr void
store(array_segmented_double_ended_cursor<T, k, alloc>& cur, T const& value)
{
    store(cur.segment, value);
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr void
store(array_segmented_double_ended_cursor<T, k, alloc>& cur, Value_type<T>&& value)
{
    store(cur.segment, fw<T>(value));
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
at(array_segmented_double_ended_cursor<T, k, alloc> const& cur) -> T const&
{
    return at(segment_cur(cur));
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
at(array_segmented_double_ended_cursor<T, k, alloc>& cur) -> T&
{
    return at(segment_cur(cur));
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
precedes(array_segmented_double_ended_cursor<T, k, alloc> const& cur0, array_segmented_double_ended_cursor<T, k, alloc> const& cur1) -> bool
{
    return precedes(cur0.index, cur1.index) or precedes(cur0.segment, cur1.segment);
}

template <typename T, pointer_diff k, Invocable auto alloc>
//requires (0 < k) and (k <= std::numeric_limits<pointer_diff>::max() / sizeof(T))
struct array_segmented_double_ended
{
    array_double_ended<array_double_ended<T, alloc>, alloc> index;

    constexpr
    array_segmented_double_ended() = default;

    constexpr
    array_segmented_double_ended(Size_type<array_segmented_double_ended<T, k, alloc>> size, T const& x)
    {
        if (size == Zero<decltype(size)>) return;
        auto n_segments{size / k + min(One<decltype(size)>, size % k)};
        index = array_double_ended<array_double_ended<T, alloc>>(n_segments);
        while (precedes(limit(index), limit_of_storage(index))) {
            emplace(index, array_double_ended<T, alloc>(k, x));
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
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct value_type_t<array_segmented_double_ended<T, k, alloc>>
{
    using type = T;
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct cursor_type_t<array_segmented_double_ended<T, k, alloc>>
{
    using type = array_segmented_double_ended_cursor<T, k, alloc>;
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct cursor_type_t<array_segmented_double_ended<T, k, alloc> const>
{
    using type = array_segmented_double_ended_cursor<T const, k, alloc>;
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct size_type_t<array_segmented_double_ended<T, k, alloc>>
{
    using type = Difference_type<array_segmented_double_ended_cursor<T, k, alloc>>;
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct functor_t<array_segmented_double_ended<T, k, alloc>>
{
    using constructor_type = array_segmented_double_ended<T, k, alloc>;

    template <Operation<T> Op>
    static constexpr auto
    fmap(array_segmented_double_ended<T, k, alloc>& x, Op op) -> array_segmented_double_ended<T, k, alloc>&
    {
        using elements::copy;
        copy(first(x), limit(x), map_sink{op}(first(x)));
        return x;
    }

    template <Regular_invocable<T> F>
    static constexpr auto
    fmap(array_segmented_double_ended<T, k, alloc>&& x, F fun) -> array_segmented_double_ended<Return_type<F, T>, k, alloc>
    {
        using elements::map;
        array_segmented_double_ended<Return_type<F, T>, k, alloc> y;
        map(first(x), limit(x), insert_sink{}(back{y}), fun);
        return y;
    }
};

template <typename T, pointer_diff k, Invocable auto alloc>
struct monad_t<array_segmented_double_ended<T, k, alloc>>
{
    template <Regular_invocable<T> F>
    static constexpr auto
    bind(array_segmented_double_ended<T, k, alloc>& x, F fun) -> Return_type<F, T>&
    {
        using elements::flat_map;
        auto y{flat_map(first(x), limit(x), fun)};
        swap(x, y);
        return x;
    }

    template <Regular_invocable<T> F>
    static constexpr auto
    bind(array_segmented_double_ended<T, k, alloc>&& x, F fun) -> Return_type<F, T>
    {
        using elements::flat_map;
        return flat_map(first(x), limit(x), fun);
    }
};

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
operator==(array_segmented_double_ended<T, k, alloc> const& x, array_segmented_double_ended<T, k, alloc> const& y) -> bool
{
    return x.index == y.index;
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
operator<(array_segmented_double_ended<T, k, alloc> const& x, array_segmented_double_ended<T, k, alloc> const& y) -> bool
{
    return x.index < y.index;
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr void
swap(array_segmented_double_ended<T, k, alloc>& x, array_segmented_double_ended<T, k, alloc>& y)
{
    swap(x.index, y.index);
}

template <typename T, pointer_diff k, Invocable auto alloc, typename U>
constexpr auto
insert(back<array_segmented_double_ended<T, k, alloc>> arr, U&& x) -> back<array_segmented_double_ended<T, k, alloc>>
{
    auto& seq = base(arr);
    if (!precedes(limit(seq), limit_of_storage(seq))) {
        if (is_empty(seq)) {
            emplace(seq.index, array_double_ended<T, alloc>(k, half(k)));
        } else {
            emplace(seq.index, array_double_ended<T, alloc>(k, Zero<decltype(k)>));
        }
    }
    emplace(at(predecessor(limit(seq.index))), fw<U>(x));
    return seq;
}

template <typename T, pointer_diff k, Invocable auto alloc, typename U>
constexpr auto
insert(front<array_segmented_double_ended<T, k, alloc>> arr, U&& x) -> front<array_segmented_double_ended<T, k, alloc>>
{
    auto& seq = base(arr);
    if (!precedes(first(seq), first_of_storage(seq))) {
        if (is_empty(seq)) {
            emplace_first(seq.index, array_double_ended<T, alloc>(k, half(k)));
        } else {
            emplace_first(seq.index, array_double_ended<T, alloc>(k, k));
        }
    }
    emplace_first(at(first(seq.index)), fw<U>(x));
    return seq;
}

template <typename T, pointer_diff k, Invocable auto alloc, typename U>
constexpr void
emplace(array_segmented_double_ended<T, k, alloc>& arr, U&& x)
{
    insert(back{arr}, fw<U>(x));
}

template <typename T, pointer_diff k, Invocable auto alloc, typename U>
constexpr void
emplace_first(array_segmented_double_ended<T, k, alloc>& arr, U&& x)
{
    insert(front{arr}, fw<U>(x));
}

template <typename T, pointer_diff k, Invocable auto alloc, typename U>
constexpr void
push(array_segmented_double_ended<T, k, alloc>& arr, U x)
{
    insert(back{arr}, mv(x));
}

template <typename T, pointer_diff k, Invocable auto alloc, typename U>
constexpr void
push_first(array_segmented_double_ended<T, k, alloc>& arr, U x)
{
    insert(front{arr}, mv(x));
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
erase(back<array_segmented_double_ended<T, k, alloc>> arr) -> back<array_segmented_double_ended<T, k, alloc>>
{
    auto& seq = base(arr);
    auto cur{limit(seq)};
    pop(at(index_cur(cur)));
    if (is_empty(load(index_cur(cur)))) {
        pop(seq.index);
    }
    return arr;
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
erase(front<array_segmented_double_ended<T, k, alloc>> arr) -> front<array_segmented_double_ended<T, k, alloc>>
{
    auto& seq = base(arr);
    auto cur{first(seq)};
    pop_first(at(index_cur(cur)));
    if (is_empty(load(index_cur(cur)))) {
        pop_first(seq.index);
    }
    return arr;
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr void
erase_all(array_segmented_double_ended<T, k, alloc>& x)
{
    erase_all(x.index);
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr void
pop(array_segmented_double_ended<T, k, alloc>& arr)
{
    erase(back{arr});
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr void
pop_first(array_segmented_double_ended<T, k, alloc>& arr)
{
    erase(front{arr});
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
first(array_segmented_double_ended<T, k, alloc> const& x) -> Cursor_type<array_segmented_double_ended<T, k, alloc>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {first(x.index), first(at(first(x.index)))};
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
limit(array_segmented_double_ended<T, k, alloc> const& x) -> Cursor_type<array_segmented_double_ended<T, k, alloc>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {predecessor(limit(x.index)), limit(at(predecessor(limit(x.index))))};
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
first_of_storage(array_segmented_double_ended<T, k, alloc> const& x) -> Cursor_type<array_segmented_double_ended<T, k, alloc>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {first(x.index), first_of_storage(at(first(x.index)))};
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
limit_of_storage(array_segmented_double_ended<T, k, alloc> const& x) -> Cursor_type<array_segmented_double_ended<T, k, alloc>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {predecessor(limit(x.index)), limit_of_storage(at(predecessor(limit(x.index))))};
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
is_empty(array_segmented_double_ended<T, k, alloc> const& x) -> bool
{
    return is_empty(x.index);
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
size(array_segmented_double_ended<T, k, alloc> const& x) -> Size_type<array_segmented_double_ended<T, k, alloc>>
{
    if (is_empty(x)) return 0;
    if (size(x.index) == 1) return size(load(first(x.index)));
    if (size(x.index) == 2) return size(load(first(x.index))) + size(load(predecessor(limit(x.index))));
    return size(load(first(x.index))) + (predecessor(predecessor(size(x.index))) * k) + size(load(predecessor(limit(x.index))));
}

template <typename T, pointer_diff k, Invocable auto alloc>
constexpr auto
capacity(array_segmented_double_ended<T, k, alloc> const& x) -> Size_type<array_segmented_double_ended<T, k, alloc>>
{
    return size(x.index) * k;
}

}
