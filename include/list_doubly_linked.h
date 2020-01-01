#pragma once

#include "position.h"

namespace elements {

template <Movable T>
struct list_node_doubly_linked
{
    Pointer_type<list_node_doubly_linked<T>> pos_next{};
    Pointer_type<list_node_doubly_linked<T>> pos_prev{};
    T x;

    constexpr
    list_node_doubly_linked() = default;

    explicit constexpr
    list_node_doubly_linked(
        T const& x_,
        Pointer_type<list_node_doubly_linked<T>> pos_next_ = {},
        Pointer_type<list_node_doubly_linked<T>> pos_prev_ = {})
        : pos_next(pos_next_)
        , pos_prev(pos_prev_)
        , x(x_)
    {}
};

template <Movable T>
struct list_doubly_linked_position
{
    Pointer_type<list_node_doubly_linked<T>> pos{};

    constexpr
    list_doubly_linked_position() = default;

    explicit constexpr
    list_doubly_linked_position(Pointer_type<list_node_doubly_linked<T>> pos_)
        : pos{pos_}
    {}
};

template <Movable T>
struct value_type_t<list_doubly_linked_position<T>>
{
    using type = T;
};

template <Movable T>
struct difference_type_t<list_doubly_linked_position<T>>
{
    using type = Difference_type<Pointer_type<list_node_doubly_linked<T>>>;
};

template <Movable T>
bool operator==(list_doubly_linked_position<T> const& x, list_doubly_linked_position<T> const& y)
{
    return x.pos == y.pos;
}

template <Movable T>
constexpr void
increment(list_doubly_linked_position<T>& x)
{
    x.pos = load(x.pos).pos_next;
}

template <Movable T>
constexpr void
decrement(list_doubly_linked_position<T>& x)
{
    x.pos = load(x.pos).pos_prev;
}

template <Movable T>
constexpr auto
load(list_doubly_linked_position<T> x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr void
store(list_doubly_linked_position<T>& x, T const& value)
{
    store(at(x.pos).x, value);
}

template <Movable T>
constexpr void
store(list_doubly_linked_position<T>& x, T&& value)
{
    store(at(x.pos).x, fw<T>(value));
}

template <Movable T>
constexpr auto
at(list_doubly_linked_position<T> const& x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr auto
at(list_doubly_linked_position<T>& x) -> T&
{
    return at(x.pos).x;
}

template <Movable T>
constexpr auto
next_link(list_doubly_linked_position<T>& x) -> Pointer_type<list_node_doubly_linked<T>>&
{
    return at(x.pos).pos_next;
}

template <Movable T>
constexpr auto
prev_link(list_doubly_linked_position<T>& x) -> Pointer_type<list_node_doubly_linked<T>>&
{
    return at(x.pos).pos_prev;
}

template <Movable T>
constexpr auto
precedes(list_doubly_linked_position<T> const& x, list_doubly_linked_position<T> const& y) -> bool
{
    return precedes(x.pos, y.pos);
}

}
