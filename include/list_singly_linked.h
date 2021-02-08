#pragma once

#include "cursor.h"

namespace elements {

template <typename T>
struct list_node_singly_linked
{
    Pointer_type<list_node_singly_linked<T>> next{};
    T x;

    constexpr
    list_node_singly_linked() = default;

    explicit constexpr
    list_node_singly_linked(T const& x_, Pointer_type<list_node_singly_linked<T>> next_ = {})
        : next{next_}
        , x{x_}
    {}

    explicit constexpr
    list_node_singly_linked(T&& x_, Pointer_type<list_node_singly_linked<T>> next_ = {})
        : next{next_}
        , x{fw<T>(x_)}
    {}
};

template <typename T>
struct list_singly_linked_cursor
{
    Pointer_type<list_node_singly_linked<T>> cur{};

    constexpr
    list_singly_linked_cursor() = default;

    explicit constexpr
    list_singly_linked_cursor(Pointer_type<list_node_singly_linked<T>> cur_)
        : cur{cur_}
    {}
};

template <typename T>
struct value_type_t<list_singly_linked_cursor<T>>
{
    using type = T;
};

template <typename T>
struct difference_type_t<list_singly_linked_cursor<T>>
{
    using type = Difference_type<Pointer_type<list_node_singly_linked<T>>>;
};

template <typename T>
constexpr auto
operator==(list_singly_linked_cursor<T> const& cur0, list_singly_linked_cursor<T> const& cur1) -> bool
{
    return cur0.cur == cur1.cur;
}

template <typename T>
constexpr void
increment(list_singly_linked_cursor<T>& cur)
{
    cur.cur = load(cur.cur).next;
}

template <typename T>
constexpr auto
load(list_singly_linked_cursor<T> cur) -> T const&
{
    return load(cur.cur).x;
}

template <typename T>
constexpr void
store(list_singly_linked_cursor<T>& cur, T const& value)
{
    store(at(cur.cur).x, value);
}

template <typename T>
constexpr void
store(list_singly_linked_cursor<T>& cur, T&& value)
{
    store(at(cur.cur).x, fw<T>(value));
}

template <typename T>
constexpr auto
at(list_singly_linked_cursor<T> const& cur) -> T const&
{
    return load(cur.cur).x;
}

template <typename T>
constexpr auto
at(list_singly_linked_cursor<T>& cur) -> T&
{
    return at(cur.cur).x;
}

template <typename T>
constexpr auto
next_link(list_singly_linked_cursor<T>& cur) -> Pointer_type<list_node_singly_linked<T>>&
{
    return at(cur.cur).next;
}

template <typename T>
constexpr auto
precedes(list_singly_linked_cursor<T> const& cur0, list_singly_linked_cursor<T> const& cur1) -> bool
{
    return precedes(cur0.cur, cur1.cur);
}

}
