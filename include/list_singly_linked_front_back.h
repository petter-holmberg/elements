#pragma once

#include "list_singly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct list_singly_linked_front_back
{
    list_singly_linked_position<T> front;
    list_singly_linked_position<T> back;

    list_singly_linked_front_back() = default;

    list_singly_linked_front_back(list_singly_linked_front_back const& x)
    {
        auto src = first(x);
        while (precedes(src, limit(x))) {
            push_last(at(this), load(src));
            increment(src);
        }
    }

    list_singly_linked_front_back(list_singly_linked_front_back&& x)
    {
        front = x.front;
        x.front = {};
    }

    list_singly_linked_front_back(std::initializer_list<T> x)
    {
        auto src = std::cbegin(x);
        while (src != std::cend(x)) {
            push_last(at(this), load(src));
            increment(src);
        }
    }

    list_singly_linked_front_back(Size_type<list_singly_linked_front_back<T>> size, T const& x)
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    auto
    operator=(list_singly_linked_front_back const& x) -> list_singly_linked_front_back&
    {
        list_singly_linked_front_back temp(x);
        swap(at(this), temp);
        return at(this);
    }

    auto
    operator=(list_singly_linked_front_back&& x) -> list_singly_linked_front_back&
    {
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(front, x.front);
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
        auto pos = front + i;
        return at(pos);
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_position<T>> i) const -> T const&
    {
        auto pos = front + i;
        return load(pos);
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
    return equal_lexicographical(first(x), limit(x), first(y), limit(y));
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_singly_linked_front_back<T> const& x, list_singly_linked_front_back<T> const& y) -> bool
{
    return less_lexicographical(first(x), limit(x), first(y), limit(y));
}

template <Movable T>
constexpr void
swap(list_singly_linked_front_back<T>& x, list_singly_linked_front_back<T>& y)
{
    swap(x.front, y.front);
    swap(x.back, y.back);
}

template <Movable T, typename U>
constexpr auto
insert(front<list_singly_linked_front_back<T>> list, U&& x) -> front<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_position{new list_node_singly_linked{fw<U>(x), first(seq).pos}};
    if (is_empty(seq)) {
        seq.back = node;
    }
    seq.front = node;
    return seq;
}

template <Movable T, typename U>
constexpr auto
insert(back<list_singly_linked_front_back<T>> list, U&& x) -> front<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_position{new list_node_singly_linked{fw<U>(x)}};
    if (is_empty(seq)) {
        seq.front = node;
    } else {
        set_link_forward(seq.back, node);
    }
    seq.back = node;
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
            seq.back = node;
        }
        set_link_forward(node, successor(current(list)));
        set_link_forward(current(list), node);
    } else {
        set_link_forward(node, first(list));
        seq.front = node;
        seq.back = node;
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
        seq.back = pos;
    }
    seq.front = pos;
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
        seq.back = current(list);
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
    return x.front;
}

template <Movable T>
constexpr auto
last(list_singly_linked_front_back<T> const& x) -> Position_type<list_singly_linked_front_back<T>>
{
    return x.back;
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
