#pragma once

#include "cursor.h"

namespace elements {

template <Movable T>
struct list_node_doubly_linked
{
    Pointer_type<list_node_doubly_linked<T>> next{};
    Pointer_type<list_node_doubly_linked<T>> prev{};
    T x;

    constexpr
    list_node_doubly_linked() = default;

    explicit constexpr
    list_node_doubly_linked(
        T const& x_,
        Pointer_type<list_node_doubly_linked<T>> next_ = {},
        Pointer_type<list_node_doubly_linked<T>> prev_ = {})
        : next{next_}
        , prev{prev_}
        , x{x_}
    {}
};

template <Movable T>
struct list_doubly_linked_cursor
{
    Pointer_type<list_node_doubly_linked<T>> cur{};

    constexpr
    list_doubly_linked_cursor() = default;

    explicit constexpr
    list_doubly_linked_cursor(Pointer_type<list_node_doubly_linked<T>> cur_)
        : cur{cur_}
    {}
};

template <Movable T>
struct value_type_t<list_doubly_linked_cursor<T>>
{
    using type = T;
};

template <Movable T>
struct difference_type_t<list_doubly_linked_cursor<T>>
{
    using type = Difference_type<Pointer_type<list_node_doubly_linked<T>>>;
};

template <Movable T>
bool operator==(list_doubly_linked_cursor<T> const& cur0, list_doubly_linked_cursor<T> const& cur1)
{
    return cur0.cur == cur1.cur;
}

template <Movable T>
constexpr void
increment(list_doubly_linked_cursor<T>& cur)
{
    cur.cur = load(cur.cur).next;
}

template <Movable T>
constexpr void
decrement(list_doubly_linked_cursor<T>& cur)
{
    cur.cur = load(cur.cur).prev;
}

template <Movable T>
constexpr auto
load(list_doubly_linked_cursor<T> cur) -> T const&
{
    return load(cur.cur).x;
}

template <Movable T>
constexpr void
store(list_doubly_linked_cursor<T>& cur, T const& value)
{
    store(at(cur.cur).x, value);
}

template <Movable T>
constexpr void
store(list_doubly_linked_cursor<T>& cur, T&& value)
{
    store(at(cur.cur).x, fw<T>(value));
}

template <Movable T>
constexpr auto
at(list_doubly_linked_cursor<T> const& cur) -> T const&
{
    return load(cur.cur).x;
}

template <Movable T>
constexpr auto
at(list_doubly_linked_cursor<T>& cur) -> T&
{
    return at(cur.cur).x;
}

template <Movable T>
constexpr auto
next_link(list_doubly_linked_cursor<T>& cur) -> Pointer_type<list_node_doubly_linked<T>>&
{
    return at(cur.cur).next;
}

template <Movable T>
constexpr auto
prev_link(list_doubly_linked_cursor<T>& cur) -> Pointer_type<list_node_doubly_linked<T>>&
{
    return at(cur.cur).prev;
}

template <Movable T>
constexpr auto
precedes(list_doubly_linked_cursor<T> const& cur0, list_doubly_linked_cursor<T> const& cur1) -> bool
{
    return precedes(cur0.cur, cur1.cur);
}

}
