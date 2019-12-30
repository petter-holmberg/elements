#pragma once

#include "list_doubly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct list_doubly_linked_circular_position
{
    Pointer_type<list_node_doubly_linked<T>> pos{};
    Pointer_type<list_node_doubly_linked<T>> front{};
    Pointer_type<list_node_doubly_linked<T>> back{};

    constexpr
    list_doubly_linked_circular_position() = default;

    explicit constexpr
    list_doubly_linked_circular_position(
        Pointer_type<list_node_doubly_linked<T>> pos_,
        Pointer_type<list_node_doubly_linked<T>> front_,
        Pointer_type<list_node_doubly_linked<T>> back_)
        : pos{pos_}
        , front{front_}
        , back{back_}
    {}
};

template <Movable T>
struct value_type_t<list_doubly_linked_circular_position<T>>
{
    using type = T;
};

template <Movable T>
struct difference_type_t<list_doubly_linked_circular_position<T>>
{
    using type = Difference_type<Pointer_type<list_node_doubly_linked<T>>>;
};

template <Movable T>
bool operator==(list_doubly_linked_circular_position<T> const& x, list_doubly_linked_circular_position<T> const& y)
{
    return x.pos == y.pos and x.front == y.front and x.back == y.back;
}

template <Movable T>
constexpr void
increment(list_doubly_linked_circular_position<T>& x)
{
    if (precedes(x.pos, x.back)) {
        x.pos = next_link(x);
    } else {
        x = {};
    }
}

template <Movable T>
constexpr void
decrement(list_doubly_linked_circular_position<T>& x)
{
    if (precedes(x.front, x.pos)) {
        x.pos = prev_link(x);
    } else {
        x = {};
    }
}

template <Movable T>
constexpr auto
load(list_doubly_linked_circular_position<T> x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr void
store(list_doubly_linked_circular_position<T>& x, T const& value)
{
    store(x.pos, value);
}

template <Movable T>
constexpr void
store(list_doubly_linked_circular_position<T>& x, T&& value)
{
    store(x.pos, fw<T>(value));
}

template <Movable T>
constexpr auto
at(list_doubly_linked_circular_position<T> const& x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr auto
at(list_doubly_linked_circular_position<T>& x) -> T&
{
    return at(x.pos).x;
}

template <Movable T>
constexpr auto
next_link(list_doubly_linked_circular_position<T> const& x) -> Pointer_type<list_node_doubly_linked<T>>&
{
    return at(x.pos).pos_next;
}

template <Movable T>
constexpr auto
prev_link(list_doubly_linked_circular_position<T> const& x) -> Pointer_type<list_node_doubly_linked<T>>&
{
    return at(x.pos).pos_prev;
}

template <Movable T>
constexpr auto
precedes(list_doubly_linked_circular_position<T> const& x, list_doubly_linked_circular_position<T> const& y) -> bool
{
    return precedes(x.pos, y.pos);
}

template <Movable T>
struct list_doubly_linked_circular
{
    Pointer_type<list_node_doubly_linked<T>> front{};

    constexpr
    list_doubly_linked_circular() = default;

    constexpr
    list_doubly_linked_circular(list_doubly_linked_circular const& x)
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    list_doubly_linked_circular(list_doubly_linked_circular&& x)
    {
        front = x.front;
        x.front = {};
    }

    constexpr
    list_doubly_linked_circular(std::initializer_list<T> x)
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    list_doubly_linked_circular(Size_type<list_doubly_linked_circular<T>> size, T const& x)
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    constexpr auto
    operator=(list_doubly_linked_circular const& x) -> list_doubly_linked_circular&
    {
        using elements::swap;
        list_doubly_linked_circular temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(list_doubly_linked_circular&& x) -> list_doubly_linked_circular&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(front, x.front);
        }
        return at(this);
    }

    ~list_doubly_linked_circular()
    {
        erase_all(at(this));
    }

    constexpr auto
    operator[](Difference_type<list_doubly_linked_circular_position<T>> i) -> T&
    {
        auto pos = first(at(this)) + i;
        return at(pos);
    }

    constexpr auto
    operator[](Difference_type<list_doubly_linked_circular_position<T>> i) const -> T const&
    {
        auto pos = first(at(this)) + i;
        return load(pos);
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<Decay<T>, Codomain<Fun>>
    constexpr auto
    map(Fun fun) -> list_doubly_linked_circular<T>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> list_doubly_linked_circular<Codomain<Fun>>
    {
        using elements::map;
        list_doubly_linked_circular<Codomain<Fun>> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<list_doubly_linked_circular<Decay<T>>, Codomain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> list_doubly_linked_circular<T>&
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
struct value_type_t<list_doubly_linked_circular<T>>
{
    using type = T;
};

template <Movable T>
struct position_type_t<list_doubly_linked_circular<T>>
{
    using type = list_doubly_linked_circular_position<T>;
};

template <Movable T>
struct position_type_t<list_doubly_linked_circular<T const>>
{
    using type = list_doubly_linked_circular_position<T const>;
};

template <Movable T>
struct size_type_t<list_doubly_linked_circular<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(list_doubly_linked_circular<T> const& x, list_doubly_linked_circular<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_doubly_linked_circular<T> const& x, list_doubly_linked_circular<T> const& y) -> bool
{
    return less_range(x, y);
}

template <Movable T>
constexpr void
swap(list_doubly_linked_circular<T>& x, list_doubly_linked_circular<T>& y)
{
    swap(x.front, y.front);
}

template <Movable T, typename U>
constexpr auto
insert(front<list_doubly_linked_circular<T>> list, U&& x) -> front<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).pos, last(seq).pos};
    if (is_empty(seq)) {
        at(node).pos_next = node;
        at(node).pos_prev = node;
    } else {
        at(at(seq.front).pos_prev).pos_next = node;
        at(seq.front).pos_prev = node;
    }
    seq.front = node;
    return seq;
}

template <Movable T, typename U>
constexpr auto
insert(back<list_doubly_linked_circular<T>> list, U&& x) -> back<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).pos, last(seq).pos};
    if (is_empty(seq)) {
        at(node).pos_next = node;
        at(node).pos_prev = node;
        seq.front = node;
    } else {
        at(at(seq.front).pos_prev).pos_next = node;
        at(seq.front).pos_prev = node;
    }
    return seq;
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(before<list_doubly_linked_circular<T>> list, U&& x) -> before<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).pos, last(seq).pos};
    if (precedes(current(list), limit(list))) {
        if (!precedes(seq.front, current(list).pos)) {
            seq.front = node;
        }
        at(prev_link(current(list))).pos_next = node;
        at(node).pos_prev = prev_link(current(list));
        prev_link(current(list)) = node;
        at(node).pos_next = current(list).pos;
    } else {
        if (is_empty(seq)) {
            at(node).pos_next = node;
            at(node).pos_prev = node;
            seq.front = node;
        } else {
            at(seq.front).pos_prev = node;
            at(at(node).pos_prev).pos_next = node;
        }
    }
    return before{seq, list_doubly_linked_circular_position{node, first(seq).pos, last(seq).pos}};
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(after<list_doubly_linked_circular<T>> list, U&& x) -> after<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).pos, last(seq).pos};
    if (precedes(current(list), limit(list))) {
        at(next_link(current(list))).pos_prev = node;
        at(node).pos_next = next_link(current(list));
        at(current(list).pos).pos_next = node;
        at(node).pos_prev = current(list).pos;
    } else {
        if (is_empty(seq)) {
            at(node).pos_next = node;
            at(node).pos_prev = node;
            seq.front = node;
        } else {
            at(seq.front).pos_prev = node;
            at(at(node).pos_prev).pos_next = node;
        }
    }
    return after{seq, list_doubly_linked_circular_position{node, first(seq).pos, last(seq).pos}};
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_first(list_doubly_linked_circular<T>& list, U&& x)
{
    insert(front{(list), fw<U>(x)});
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_first(list_doubly_linked_circular<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_last(list_doubly_linked_circular<T>& list, U&& x)
{
    insert(back{list}, fw<U>(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_last(list_doubly_linked_circular<T>& list, U x)
{
    insert(back{list}, mv(x));
}

template <Movable T>
constexpr auto
erase(front<list_doubly_linked_circular<T>> list) -> front<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto pos = seq.front;
    if (precedes(seq.front, at(seq.front).pos_next)) {
        at(at(pos).pos_prev).pos_next = at(pos).pos_next;
        at(at(pos).pos_next).pos_prev = at(pos).pos_prev;
        seq.front = at(pos).pos_next;
    } else {
        seq.front = {};
    }
    delete pos;
    return list;
}

template <Movable T>
constexpr auto
erase(back<list_doubly_linked_circular<T>> list) -> back<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto pos = at(seq.front).pos_prev;
    if (precedes(seq.front, at(seq.front).pos_next)) {
        at(at(pos).pos_prev).pos_next = seq.front;
        at(seq.front).pos_prev = at(pos).pos_prev;
    } else {
        seq.front = {};
    }
    delete pos;
    return list;
}

template <Movable T>
constexpr auto
erase(before<list_doubly_linked_circular<T>> list) -> before<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto& node = at(current(list).pos);
    auto pos = node.pos_prev;
    if (precedes(seq.front, at(seq.front).pos_next)) {
        if (!precedes(seq.front, pos)) {
            seq.front = current(list).pos;
        }
        node.pos_prev = at(pos).pos_prev;
        at(at(pos).pos_prev).pos_next = current(list).pos;
    } else {
        seq.front = {};
    }
    delete pos;
    return list;
}

template <Movable T>
constexpr auto
erase(after<list_doubly_linked_circular<T>> list) -> after<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto& node = at(current(list).pos);
    auto pos = node.pos_next;
    if (precedes(seq.front, at(seq.front).pos_next)) {
        if (!precedes(seq.front, pos)) {
            seq.front = at(pos).pos_next;
        }
        node.pos_next = at(pos).pos_next;
        at(at(pos).pos_next).pos_prev = at(pos).pos_prev;
    } else {
        seq.front = {};
    }
    delete pos;
    return list;
}

template <Movable T>
constexpr void
erase_all(list_doubly_linked_circular<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <Movable T>
constexpr void
pop_first(list_doubly_linked_circular<T>& list)
{
    erase(front{list});
}

template <Movable T>
constexpr void
pop_last(list_doubly_linked_circular<T>& list)
{
    erase(back{list});
}

template <Movable T>
constexpr auto
first(list_doubly_linked_circular<T> const& x) -> Position_type<list_doubly_linked_circular<T>>
{
    if (is_empty(x)) {
        return {};
    } else {
        return list_doubly_linked_circular_position{x.front, x.front, at(x.front).pos_prev};
    }
}

template <Movable T>
constexpr auto
last(list_doubly_linked_circular<T> const& x) -> Position_type<list_doubly_linked_circular<T>>
{
    if (is_empty(x)) {
        return {};
    } else {
        return list_doubly_linked_circular_position{at(x.front).pos_prev, x.front, at(x.front).pos_prev};
    }
}

template <Movable T>
constexpr auto
limit(list_doubly_linked_circular<T> const&) -> Position_type<list_doubly_linked_circular<T>>
{
    return {};
}

template <Movable T>
constexpr auto
is_empty(list_doubly_linked_circular<T> const& x) -> bool
{
    return x.front == nullptr;
}

template <Movable T>
constexpr auto
size(list_doubly_linked_circular<T> const& x) -> Size_type<list_doubly_linked_circular<T>>
{
    return limit(x) - first(x);
}

}
