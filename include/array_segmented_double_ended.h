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
struct array_segmented_double_ended_position
{
    Pointer_type<array_double_ended<T>> pos_index{};
    Pointer_type<T> pos_segment{};

    constexpr
    array_segmented_double_ended_position() = default;

    constexpr
    array_segmented_double_ended_position(
        Pointer_type<array_double_ended<T>> index,
        Pointer_type<T> segment
    )
        : pos_index{index}
        , pos_segment{segment}
    {}
};

template <Movable T, pointer_diff k>
struct value_type_t<array_segmented_double_ended_position<T, k>>
{
    using type = T;
};

template <Movable T, pointer_diff k>
struct difference_type_t<array_segmented_double_ended_position<T, k>>
{
    using type = Difference_type<Pointer_type<T>>;
};

template <Movable T, pointer_diff k>
struct index_position_type_t<array_segmented_double_ended_position<T, k>>
{
    using type = Pointer_type<array_double_ended<T>>;
};

template <Movable T, pointer_diff k>
struct segment_position_type_t<array_segmented_double_ended_position<T, k>>
{
    using type = Pointer_type<T>;
};

template <Movable T, pointer_diff k>
constexpr auto
operator==(array_segmented_double_ended_position<T, k> const& x, array_segmented_double_ended_position<T, k> const& y) -> bool
{
    return x.pos_index == y.pos_index and x.pos_segment == y.pos_segment;
}

template <Movable T, pointer_diff k>
constexpr auto
index_pos(array_segmented_double_ended_position<T, k> const& x) -> Index_position_type<array_segmented_double_ended_position<T, k>> const&
{
    return x.pos_index;
}

template <Movable T, pointer_diff k>
constexpr auto
index_pos(array_segmented_double_ended_position<T, k>& x) -> Index_position_type<array_segmented_double_ended_position<T, k>>&
{
    return x.pos_index;
}

template <Movable T, pointer_diff k>
constexpr auto
segment_pos(array_segmented_double_ended_position<T, k> const& x) -> Segment_position_type<array_segmented_double_ended_position<T, k>> const&
{
    return x.pos_segment;
}

template <Movable T, pointer_diff k>
constexpr auto
segment_pos(array_segmented_double_ended_position<T, k>& x) -> Segment_position_type<array_segmented_double_ended_position<T, k>>&
{
    return x.pos_segment;
}

template <Movable T, pointer_diff k>
constexpr auto
load(array_segmented_double_ended_position<T, k> const& x) -> T const&
{
    return load(x.pos_segment);
}

template <Movable T, pointer_diff k>
constexpr void
store(array_segmented_double_ended_position<T, k>& x, T const& value)
{
    store(x.pos_segment, value);
}

template <Movable T, pointer_diff k>
constexpr void
store(array_segmented_double_ended_position<T, k>& x, Value_type<T>&& value)
{
    store(x.pos_segment, fw<T>(value));
}

template <Movable T, pointer_diff k>
constexpr auto
at(array_segmented_double_ended_position<T, k> const& x) -> T const&
{
    return at(segment_pos(x));
}

template <Movable T, pointer_diff k>
constexpr auto
at(array_segmented_double_ended_position<T, k>& x) -> T&
{
    return at(segment_pos(x));
}

template <Movable T, pointer_diff k>
constexpr auto
precedes(array_segmented_double_ended_position<T, k> const& x, array_segmented_double_ended_position<T, k> const& y) -> bool
{
    return precedes(x.pos_index, y.pos_index) or precedes(x.pos_segment, y.pos_segment);
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

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<Decay<T>, Codomain<Fun>>
    constexpr auto
    fmap(Fun fun) -> array_segmented_double_ended<T, k>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    fmap(Fun fun) const -> array_segmented_double_ended<Codomain<Fun>, k>
    {
        using elements::map;
        array_segmented_double_ended<Codomain<Fun>, k> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<array_segmented_double_ended<Decay<T>>, Codomain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> array_segmented_double_ended<T, k>&
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
    flat_map(Fun fun) const -> Codomain<Fun>
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
struct position_type_t<array_segmented_double_ended<T, k>>
{
    using type = array_segmented_double_ended_position<T, k>;
};

template <Movable T, pointer_diff k>
struct position_type_t<array_segmented_double_ended<T, k> const>
{
    using type = array_segmented_double_ended_position<T const, k>;
};

template <Movable T, pointer_diff k>
struct size_type_t<array_segmented_double_ended<T, k>>
{
    using type = Difference_type<array_segmented_double_ended_position<T, k>>;
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
    auto pos{limit(seq)};
    pop(at(index_pos(pos)));
    if (is_empty(load(index_pos(pos)))) {
        pop(seq.index);
    }
    return arr;
}

template <Movable T, pointer_diff k>
constexpr auto
erase(front<array_segmented_double_ended<T, k>> arr) -> front<array_segmented_double_ended<T, k>>
{
    auto& seq = base(arr);
    auto pos{first(seq)};
    pop_first(at(index_pos(pos)));
    if (is_empty(load(index_pos(pos)))) {
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
first(array_segmented_double_ended<T, k> const& x) -> Position_type<array_segmented_double_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {first(x.index), first(at(first(x.index)))};
}

template <Movable T, pointer_diff k>
constexpr auto
limit(array_segmented_double_ended<T, k> const& x) -> Position_type<array_segmented_double_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {predecessor(limit(x.index)), limit(at(predecessor(limit(x.index))))};
}

template <Movable T, pointer_diff k>
constexpr auto
first_of_storage(array_segmented_double_ended<T, k> const& x) -> Position_type<array_segmented_double_ended<T, k>>
{
    if (is_empty(x)) return {nullptr, nullptr};
    return {first(x.index), first_of_storage(at(first(x.index)))};
}

template <Movable T, pointer_diff k>
constexpr auto
limit_of_storage(array_segmented_double_ended<T, k> const& x) -> Position_type<array_segmented_double_ended<T, k>>
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
