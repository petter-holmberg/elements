#pragma once

#include "flat_map.h"
#include "functional.h"
#include "list_singly_linked.h"
#include "lexicographical.h"
#include "map.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct list_singly_linked_circular_position
{
    Pointer_type<list_node_singly_linked<T>> pos{};
    Pointer_type<list_node_singly_linked<T>> back{};

    constexpr
    list_singly_linked_circular_position() = default;

    explicit constexpr
    list_singly_linked_circular_position(
        Pointer_type<list_node_singly_linked<T>> pos_,
        Pointer_type<list_node_singly_linked<T>> back_)
        : pos{pos_}
        , back{back_}
    {}
};

template <Movable T>
struct value_type_t<list_singly_linked_circular_position<T>>
{
    using type = T;
};

template <Movable T>
struct difference_type_t<list_singly_linked_circular_position<T>>
{
    using type = Difference_type<Pointer_type<list_node_singly_linked<T>>>;
};

template <Movable T>
bool operator==(list_singly_linked_circular_position<T> const& x, list_singly_linked_circular_position<T> const& y)
{
    return x.pos == y.pos and x.back == y.back;
}

template <Movable T>
constexpr void
increment(list_singly_linked_circular_position<T>& x)
{
    if (precedes(x.pos, x.back)) {
        x.pos = next_link(x);
    } else {
        x = {};
    }
}

template <Movable T>
constexpr auto
load(list_singly_linked_circular_position<T> x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr void
store(list_singly_linked_circular_position<T>& x, T const& value)
{
    store(x.pos, value);
}

template <Movable T>
constexpr void
store(list_singly_linked_circular_position<T>& x, T&& value)
{
    store(x.pos, fw<T>(value));
}

template <Movable T>
constexpr auto
at(list_singly_linked_circular_position<T> const& x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr auto
at(list_singly_linked_circular_position<T>& x) -> T&
{
    return at(x.pos).x;
}

template <Movable T>
constexpr auto
next_link(list_singly_linked_circular_position<T> const& x) -> Pointer_type<list_node_singly_linked<T>>&
{
    return at(x.pos).pos_next;
}

template <Movable T>
constexpr auto
precedes(list_singly_linked_circular_position<T> const& x, list_singly_linked_circular_position<T> const& y) -> bool
{
    return precedes(x.pos, y.pos);
}

template <Movable T>
struct list_singly_linked_circular
{
    Pointer_type<list_node_singly_linked<T>> tail{};

    list_singly_linked_circular() = default;

    list_singly_linked_circular(list_singly_linked_circular const& x)
    {
        insert_range(x, back{at(this)});
    }

    list_singly_linked_circular(list_singly_linked_circular&& x)
    {
        tail = x.tail;
        x.tail = {};
    }

    list_singly_linked_circular(std::initializer_list<T> x)
    {
        insert_range(x, back{at(this)});
    }

    list_singly_linked_circular(Size_type<list_singly_linked_circular<T>> size, T const& x)
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    auto
    operator=(list_singly_linked_circular const& x) -> list_singly_linked_circular&
    {
        using elements::swap;
        list_singly_linked_circular temp(x);
        swap(at(this), temp);
        return at(this);
    }

    auto
    operator=(list_singly_linked_circular&& x) -> list_singly_linked_circular&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(tail, x.tail);
        }
        return at(this);
    }

    ~list_singly_linked_circular()
    {
        erase_all(at(this));
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_circular_position<T>> i) -> T&
    {
        auto pos = first(at(this)) + i;
        return at(pos);
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_circular_position<T>> i) const -> T const&
    {
        auto pos = first(at(this)) + i;
        return load(pos);
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<Decay<T>, Codomain<Fun>>
    constexpr auto
    map(Fun fun) -> list_singly_linked_circular<T>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> list_singly_linked_circular<Codomain<Fun>>
    {
        using elements::map;
        list_singly_linked_circular<Codomain<Fun>> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<list_singly_linked_circular<Decay<T>>, Codomain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> list_singly_linked_circular<T>&
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
struct value_type_t<list_singly_linked_circular<T>>
{
    using type = T;
};

template <Movable T>
struct position_type_t<list_singly_linked_circular<T>>
{
    using type = list_singly_linked_circular_position<T>;
};

template <Movable T>
struct position_type_t<list_singly_linked_circular<T const>>
{
    using type = list_singly_linked_circular_position<T const>;
};

template <Movable T>
struct size_type_t<list_singly_linked_circular<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(list_singly_linked_circular<T> const& x, list_singly_linked_circular<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_singly_linked_circular<T> const& x, list_singly_linked_circular<T> const& y) -> bool
{
    return less_range(x, y);
}

template <Movable T>
constexpr void
swap(list_singly_linked_circular<T>& x, list_singly_linked_circular<T>& y)
{
    swap(x.tail, y.tail);
}

template <Movable T, typename U>
constexpr auto
insert(front<list_singly_linked_circular<T>> list, U&& x) -> front<list_singly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_singly_linked{fw<U>(x), first(seq).pos};
    if (is_empty(seq)) {
        at(node).pos_next = node;
        seq.tail = node;
    } else {
        at(seq.tail).pos_next = node;
    }
    return seq;
}

template <Movable T, typename U>
constexpr auto
insert(back<list_singly_linked_circular<T>> list, U&& x) -> back<list_singly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_singly_linked{fw<U>(x), first(seq).pos};
    if (is_empty(seq)) {
        at(node).pos_next = node;
    } else {
        at(seq.tail).pos_next = node;
    }
    seq.tail = node;
    return seq;
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(after<list_singly_linked_circular<T>> list, U&& x) -> after<list_singly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_singly_linked{fw<U>(x), first(seq).pos};
    if (precedes(current(list), limit(list))) {
        at(node).pos_next = at(current(list).pos).pos_next;
        at(current(list).pos).pos_next = node;
        if (!precedes(current(list), last(list))) {
            seq.tail = node;
        }
    } else {
        if (is_empty(seq)) {
            at(node).pos_next = node;
        } else {
            at(seq.tail).pos_next = node;
        }
        seq.tail = node;
    }
    return after{seq, list_singly_linked_circular_position{node, last(seq).pos}};
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_first(list_singly_linked_circular<T>& list, U&& x)
{
    insert(front{(list), fw<U>(x)});
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_first(list_singly_linked_circular<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_last(list_singly_linked_circular<T>& list, U&& x)
{
    insert(back{list}, fw<U>(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_last(list_singly_linked_circular<T>& list, U x)
{
    insert(back{list}, mv(x));
}

template <Movable T>
constexpr auto
erase(front<list_singly_linked_circular<T>> list) -> front<list_singly_linked_circular<T>>
{
    auto& seq = base(list);
    auto pos = at(seq.tail).pos_next;
    if (precedes(seq.tail, at(seq.tail).pos_next)) {
        at(seq.tail).pos_next = at(pos).pos_next;
    } else {
        seq.tail = {};
    }
    delete pos;
    return list;
}

template <Movable T>
constexpr auto
erase(after<list_singly_linked_circular<T>> list) -> after<list_singly_linked_circular<T>>
{
    auto& seq = base(list);
    auto& node = at(current(list).pos);
    auto pos = node.pos_next;
    if (precedes(seq.tail, at(seq.tail).pos_next)) {
        node.pos_next = at(pos).pos_next;
        if (!precedes(pos, seq.tail)) {
            at(seq.tail).pos_next = seq.tail;
            seq.tail = current(list).pos;
        }
    } else {
        seq.tail = {};
    }
    delete pos;
    return list;
}

template <Movable T>
constexpr void
erase_all(list_singly_linked_circular<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <Movable T>
constexpr void
pop_first(list_singly_linked_circular<T>& list)
{
    erase(front{list});
}

template <Movable T>
constexpr auto
first(list_singly_linked_circular<T> const& x) -> Position_type<list_singly_linked_circular<T>>
{
    if (is_empty(x)) {
        return {};
    } else {
        return list_singly_linked_circular_position{at(x.tail).pos_next, x.tail};
    }
}

template <Movable T>
constexpr auto
last(list_singly_linked_circular<T> const& x) -> Position_type<list_singly_linked_circular<T>>
{
    if (is_empty(x)) {
        return {};
    } else {
        return list_singly_linked_circular_position{x.tail, x.tail};
    }
}

template <Movable T>
constexpr auto
limit(list_singly_linked_circular<T> const&) -> Position_type<list_singly_linked_circular<T>>
{
    return {};
}

template <Movable T>
constexpr auto
is_empty(list_singly_linked_circular<T> const& x) -> bool
{
    return x.tail == nullptr;
}

template <Movable T>
constexpr auto
size(list_singly_linked_circular<T> const& x) -> Size_type<list_singly_linked_circular<T>>
{
    return limit(x) - first(x);
}

}
