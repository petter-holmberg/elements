#pragma once

#include "flat_map.h"
#include "list_singly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "range.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct list_singly_linked_front_back
{
    list_singly_linked_position<T> head;
    list_singly_linked_position<T> tail;

    constexpr
    list_singly_linked_front_back() = default;

    constexpr
    list_singly_linked_front_back(list_singly_linked_front_back const& x)
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    list_singly_linked_front_back(list_singly_linked_front_back&& x)
    {
        head = x.head;
        x.head = {};
    }

    constexpr
    list_singly_linked_front_back(std::initializer_list<T> x)
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    list_singly_linked_front_back(Size_type<list_singly_linked_front_back<T>> size, T const& x)
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    constexpr auto
    operator=(list_singly_linked_front_back const& x) -> list_singly_linked_front_back&
    {
        using elements::swap;
        list_singly_linked_front_back temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(list_singly_linked_front_back&& x) -> list_singly_linked_front_back&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(head, x.head);
        }
        return at(this);
    }

    ~list_singly_linked_front_back()
    {
        erase_all(at(this));
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_position<T>> i) -> T&
    {
        auto pos = head + i;
        return at(pos);
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_position<T>> i) const -> T const&
    {
        auto pos = head + i;
        return load(pos);
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<Decay<T>, Codomain<Fun>>
    constexpr auto
    map(Fun fun) -> list_singly_linked_front_back<T>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> list_singly_linked_front_back<Codomain<Fun>>
    {
        using elements::map;
        list_singly_linked_front_back<Codomain<Fun>> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<list_singly_linked_front_back<Decay<T>>, Codomain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> list_singly_linked_front_back<T>&
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
struct value_type_t<list_singly_linked_front_back<T>>
{
    using type = T;
};

template <Movable T>
struct position_type_t<list_singly_linked_front_back<T>>
{
    using type = list_singly_linked_position<T>;
};

template <Movable T>
struct position_type_t<list_singly_linked_front_back<T const>>
{
    using type = list_singly_linked_position<T const>;
};

template <Movable T>
struct size_type_t<list_singly_linked_front_back<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(list_singly_linked_front_back<T> const& x, list_singly_linked_front_back<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_singly_linked_front_back<T> const& x, list_singly_linked_front_back<T> const& y) -> bool
{
    return less_range(x, y);
}

template <Movable T>
constexpr void
swap(list_singly_linked_front_back<T>& x, list_singly_linked_front_back<T>& y)
{
    swap(x.head, y.head);
    swap(x.tail, y.tail);
}

template <Movable T, typename U>
constexpr auto
insert(front<list_singly_linked_front_back<T>> list, U&& x) -> front<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_position{new list_node_singly_linked{fw<U>(x), first(seq).pos}};
    if (is_empty(seq)) {
        seq.tail = node;
    }
    seq.head = node;
    return seq;
}

template <Movable T, typename U>
constexpr auto
insert(back<list_singly_linked_front_back<T>> list, U&& x) -> back<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_position{new list_node_singly_linked{fw<U>(x)}};
    if (is_empty(seq)) {
        seq.head = node;
    } else {
        set_link_forward(seq.tail, node);
    }
    seq.tail = node;
    return seq;
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(after<list_singly_linked_front_back<T>> list, U&& x) -> after<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_position{new list_node_singly_linked{fw<U>(x)}};
    if (precedes(current(list), limit(list))) {
        if (!precedes(current(list), last(list))) {
            seq.tail = node;
        }
        set_link_forward(node, successor(current(list)));
        set_link_forward(current(list), node);
    } else {
        set_link_forward(node, first(list));
        seq.head = node;
        seq.tail = node;
    }
    return after{seq, node};
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_first(list_singly_linked_front_back<T>& list, U&& x)
{
    insert(front{list}, fw<U>(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_first(list_singly_linked_front_back<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_last(list_singly_linked_front_back<T>& list, U&& x)
{
    insert(back{list}, fw<U>(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_last(list_singly_linked_front_back<T>& list, U x)
{
    insert(back{list}, mv(x));
}

template <Movable T>
constexpr auto
erase(front<list_singly_linked_front_back<T>> list) -> front<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto pos = successor(first(seq));
    delete first(seq).pos;
    if (!precedes(first(seq), last(seq))) {
        seq.tail = pos;
    }
    seq.head = pos;
    return list;
}

template <Movable T>
constexpr auto
erase(after<list_singly_linked_front_back<T>> list) -> after<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto rem_pos = successor(current(list));
    set_link_forward(current(list), successor(successor(current(list))));
    if (!precedes(current(list), limit(list))) {
        seq.tail = current(list);
    }
    delete rem_pos.pos;
    return list;
}

template <Movable T>
constexpr void
erase_all(list_singly_linked_front_back<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <Movable T>
constexpr void
pop_first(list_singly_linked_front_back<T>& list)
{
    erase(front{list});
}

template <Movable T>
constexpr auto
first(list_singly_linked_front_back<T> const& x) -> Position_type<list_singly_linked_front_back<T>>
{
    return x.head;
}

template <Movable T>
constexpr auto
last(list_singly_linked_front_back<T> const& x) -> Position_type<list_singly_linked_front_back<T>>
{
    return x.tail;
}

template <Movable T>
constexpr auto
limit(list_singly_linked_front_back<T> const&) -> Position_type<list_singly_linked_front_back<T>>
{
    return {};
}

template <Movable T>
constexpr auto
is_empty(list_singly_linked_front_back<T> const& x) -> bool
{
    return first(x) == limit(x);
}

template <Movable T>
constexpr auto
size(list_singly_linked_front_back<T> const& x) -> Size_type<list_singly_linked_front_back<T>>
{
    return limit(x) - first(x);
}

}
