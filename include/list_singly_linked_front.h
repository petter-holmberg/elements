#pragma once

#include "list_singly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct list_singly_linked_front
{
    list_singly_linked_position<T> front;

    list_singly_linked_front() = default;

    list_singly_linked_front(list_singly_linked_front const& x)
    {
        auto src = first(x);
        auto dst = pointer_to(at(this).front.pos);
        while (src != limit(x)) {
            at(dst) = new list_node_singly_linked{load(src)};
            dst = pointer_to(at(at(dst)).pos_next);
            increment(src);
        }
    }

    list_singly_linked_front(list_singly_linked_front&& x)
    {
        front = x.front;
        x.front = list_singly_linked_position<T>{};
    }

    list_singly_linked_front(std::initializer_list<T> x)
    {
        auto src = std::cbegin(x);
        auto dst = pointer_to(at(this).front.pos);
        while (src != std::cend(x)) {
            at(dst) = new list_node_singly_linked<T>{load(src)};
            dst = pointer_to(at(at(dst)).pos_next);
            increment(src);
        }
    }

    list_singly_linked_front(Size_type<list_singly_linked_front<T>> size, T const& x)
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    auto
    operator=(list_singly_linked_front const& x) -> list_singly_linked_front&
    {
        list_singly_linked_front temp(x);
        swap(at(this), temp);
        return at(this);
    }

    auto
    operator=(list_singly_linked_front&& x) -> list_singly_linked_front&
    {
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(front, x.front);
        }
        return at(this);
    }

    ~list_singly_linked_front()
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
struct value_type_t<list_singly_linked_front<T>>
{
    using type = T;
};

template <Movable T>
struct position_type_t<list_singly_linked_front<T>>
{
    using type = list_singly_linked_position<T>;
};

template <Movable T>
struct position_type_t<list_singly_linked_front<T const>>
{
    using type = list_singly_linked_position<T const>;
};

template <Movable T>
struct size_type_t<list_singly_linked_front<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(list_singly_linked_front<T> const& x, list_singly_linked_front<T> const& y) -> bool
{
    return equal_lexicographical(first(x), limit(x), first(y), limit(y));
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_singly_linked_front<T> const& x, list_singly_linked_front<T> const& y) -> bool
{
    return less_lexicographical(first(x), limit(x), first(y), limit(y));
}

template <Movable T>
constexpr void
swap(list_singly_linked_front<T>& x, list_singly_linked_front<T>& y)
{
    swap(x.front, y.front);
}

template <Movable T, typename U>
constexpr auto
insert(front<list_singly_linked_front<T>> list, U&& x) -> front<list_singly_linked_front<T>>
{
    auto& seq = base(list);
    seq.front = list_singly_linked_position{
        new list_node_singly_linked<T>(fw<U>(x), first(seq).pos)
    };
    return seq;
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(after<list_singly_linked_front<T>> list, U&& x) -> after<list_singly_linked_front<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_position{new list_node_singly_linked<T>(fw<U>(x))};
    if (precedes(current(list), limit(list))) {
        set_link_forward(node, successor(current(list)));
        set_link_forward(current(list), node);
    } else {
        set_link_forward(node, first(list));
        seq.front = node;
    }
    return after<list_singly_linked_front<T>>{seq, node};
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_first(list_singly_linked_front<T>& list, U&& x)
{
    insert(front<list_singly_linked_front<T>>(list), fw<U>(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_first(list_singly_linked_front<T>& list, U x)
{
    insert(front<list_singly_linked_front<T>>(list), mv(x));
}

template <Movable T>
constexpr auto
erase(front<list_singly_linked_front<T>> list) -> front<list_singly_linked_front<T>>
{
    auto& seq = base(list);
    auto pos = successor(first(seq));
    delete first(seq).pos;
    seq.front = pos;
    return seq;
}

template <Movable T>
constexpr auto
erase(after<list_singly_linked_front<T>> list) -> list_singly_linked_position<T>
{
    auto rem_pos = successor(current(list));
    set_link_forward(current(list), successor(successor(current(list))));
    delete rem_pos.pos;
    return successor(current(list));
}

template <Movable T>
constexpr void
erase_all(list_singly_linked_front<T>& x)
{
    while (!is_empty(x)) erase(front<list_singly_linked_front<T>>(x));
}

template <Movable T>
constexpr void
pop_first(list_singly_linked_front<T>& arr)
{
    erase(front<list_singly_linked_front<T>>(arr));
}

template <Movable T>
constexpr auto
first(list_singly_linked_front<T> const& x) -> Position_type<list_singly_linked_front<T>>
{
    return x.front;
}

template <Movable T>
constexpr auto
limit(list_singly_linked_front<T> const&) -> Position_type<list_singly_linked_front<T>>
{
    return {};
}

template <Movable T>
constexpr auto
is_empty(list_singly_linked_front<T> const& x) -> bool
{
    return first(x) == limit(x);
}

template <Movable T>
constexpr auto
size(list_singly_linked_front<T> const& x) -> Size_type<list_singly_linked_front<T>>
{
    return limit(x) - first(x);
}

}
