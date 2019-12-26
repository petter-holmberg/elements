#pragma once

#include "position.h"

namespace elements {

template <Movable T>
struct list_node_singly_linked
{
    T x;
    Pointer_type<list_node_singly_linked<T>> pos_next = nullptr;

    list_node_singly_linked() = default;

    explicit
    list_node_singly_linked(T const& x_, Pointer_type<list_node_singly_linked<T>> pos_next_ = nullptr)
        : x(x_)
        , pos_next(pos_next_)
    {}

    explicit
    list_node_singly_linked(T&& x_, Pointer_type<list_node_singly_linked<T>> pos_next_ = nullptr)
        : x(fw<T>(x_))
        , pos_next(pos_next_)
    {}
};

template <Movable T>
struct list_singly_linked_position
{
    Pointer_type<list_node_singly_linked<T>> pos = nullptr;

    list_singly_linked_position() = default;

    explicit
    list_singly_linked_position(Pointer_type<list_node_singly_linked<T>> pos_)
        : pos{pos_}
    {}
};

template <Movable T>
struct value_type_t<list_singly_linked_position<T>>
{
    using type = T;
};

template <Movable T>
struct difference_type_t<list_singly_linked_position<T>>
{
    using type = Difference_type<Pointer_type<list_node_singly_linked<T>>>;
};

template <Movable T>
bool operator==(list_singly_linked_position<T> const& x, list_singly_linked_position<T> const& y)
{
    return x.pos == y.pos;
}

template <Movable T>
constexpr void
increment(list_singly_linked_position<T>& x)
{
    x.pos = load(x.pos).pos_next;
}

template <Movable T>
constexpr auto
load(list_singly_linked_position<T> x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr void
store(list_singly_linked_position<T>& x, T const& value)
{
    store(x.pos, value);
}

template <Movable T>
constexpr void
store(list_singly_linked_position<T>& x, T&& value)
{
    store(x.pos, fw<T>(value));
}

template <Movable T>
constexpr auto
at(list_singly_linked_position<T> const& x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr auto
at(list_singly_linked_position<T>& x) -> T&
{
    return at(x.pos).x;
}

template <Movable T>
constexpr auto
next_link(list_singly_linked_position<T>& x) -> Pointer_type<list_node_singly_linked<T>>&
{
    return at(x.pos).pos_next;
}

template <Movable T>
constexpr auto
precedes(list_singly_linked_position<T> const& x, list_singly_linked_position<T> const& y) -> bool
{
    return x.pos != y.pos;
}

}
