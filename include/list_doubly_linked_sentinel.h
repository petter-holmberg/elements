#pragma once

#include "list_doubly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct list_node_doubly_linked_sentinel_links
{
    Pointer_type<list_node_doubly_linked<T>> pos_next{};
    Pointer_type<list_node_doubly_linked<T>> pos_prev{};

    constexpr
    list_node_doubly_linked_sentinel_links()
        : pos_next{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
        , pos_prev{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
    {}
};

template <Movable T>
constexpr auto
allocate_list_doubly_linked_sentinel_node() -> list_doubly_linked_position<T>
{
    return list_doubly_linked_position<T>{
        reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(
            new list_node_doubly_linked_sentinel_links<T>)};
}

template <Movable T>
constexpr void
deallocate_list_doubly_linked_sentinel_node(list_doubly_linked_position<T> pos)
{
    delete reinterpret_cast<Pointer_type<list_node_doubly_linked_sentinel_links<T>>>(pos.pos);
}

template <Movable T>
struct list_doubly_linked_sentinel
{
    list_doubly_linked_position<T> sentinel{};

    constexpr
    list_doubly_linked_sentinel()
        : sentinel{allocate_list_doubly_linked_sentinel_node<T>()}
    {}

    constexpr
    list_doubly_linked_sentinel(list_doubly_linked_sentinel const& x)
        : sentinel{allocate_list_doubly_linked_sentinel_node<T>()}
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    list_doubly_linked_sentinel(list_doubly_linked_sentinel&& x)
    {
        sentinel = x.sentinel;
        x.sentinel = allocate_list_doubly_linked_sentinel_node<T>();
    }

    constexpr
    list_doubly_linked_sentinel(std::initializer_list<T> x)
        : sentinel{allocate_list_doubly_linked_sentinel_node<T>()}
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    list_doubly_linked_sentinel(Size_type<list_doubly_linked_sentinel<T>> size, T const& x)
        : sentinel{allocate_list_doubly_linked_sentinel_node<T>()}
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    constexpr auto
    operator=(list_doubly_linked_sentinel const& x) -> list_doubly_linked_sentinel&
    {
        using elements::swap;
        list_doubly_linked_sentinel temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(list_doubly_linked_sentinel&& x) -> list_doubly_linked_sentinel&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(sentinel, x.sentinel);
        }
        return at(this);
    }

    ~list_doubly_linked_sentinel()
    {
        erase_all(at(this));
        deallocate_list_doubly_linked_sentinel_node<T>(sentinel);
    }

    constexpr auto
    operator[](Difference_type<list_doubly_linked_position<T>> i) -> T&
    {
        auto pos = first(at(this)) + i;
        return at(pos);
    }

    constexpr auto
    operator[](Difference_type<list_doubly_linked_position<T>> i) const -> T const&
    {
        auto pos = first(at(this)) + i;
        return load(pos);
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<Decay<T>, Codomain<Fun>>
    constexpr auto
    map(Fun fun) -> list_doubly_linked_sentinel<T>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> list_doubly_linked_sentinel<Codomain<Fun>>
    {
        using elements::map;
        list_doubly_linked_sentinel<Codomain<Fun>> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<list_doubly_linked_sentinel<Decay<T>>, Codomain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> list_doubly_linked_sentinel<T>&
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
struct value_type_t<list_doubly_linked_sentinel<T>>
{
    using type = T;
};

template <Movable T>
struct position_type_t<list_doubly_linked_sentinel<T>>
{
    using type = list_doubly_linked_position<T>;
};

template <Movable T>
struct position_type_t<list_doubly_linked_sentinel<T const>>
{
    using type = list_doubly_linked_position<T const>;
};

template <Movable T>
struct size_type_t<list_doubly_linked_sentinel<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(list_doubly_linked_sentinel<T> const& x, list_doubly_linked_sentinel<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_doubly_linked_sentinel<T> const& x, list_doubly_linked_sentinel<T> const& y) -> bool
{
    return less_range(x, y);
}

template <Movable T>
constexpr void
swap(list_doubly_linked_sentinel<T>& x, list_doubly_linked_sentinel<T>& y)
{
    swap(x.sentinel, y.sentinel);
}

template <Movable T, typename U>
constexpr auto
insert(front<list_doubly_linked_sentinel<T>> list, U&& x) -> front<list_doubly_linked_sentinel<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).pos, limit(seq).pos};
    next_link(seq.sentinel) = node;
    at(at(node).pos_next).pos_prev = node;
    return list;
}

template <Movable T, typename U>
constexpr auto
insert(back<list_doubly_linked_sentinel<T>> list, U&& x) -> back<list_doubly_linked_sentinel<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), limit(seq).pos, last(seq).pos};
    at(predecessor(seq.sentinel).pos).pos_next = node;
    prev_link(seq.sentinel) = node;
    return list;
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(before<list_doubly_linked_sentinel<T>> list, U&& x) -> before<list_doubly_linked_sentinel<T>>
{
    auto& seq = base(list);
    auto pos = current(list);
    auto node = new list_node_doubly_linked{fw<U>(x), pos.pos, at(pos.pos).pos_prev};
    prev_link(pos) = node;
    at(at(node).pos_prev).pos_next = node;
    return before{seq, list_doubly_linked_position{node}};
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(after<list_doubly_linked_sentinel<T>> list, U&& x) -> after<list_doubly_linked_sentinel<T>>
{
    auto& seq = base(list);
    auto pos = current(list);
    auto node = new list_node_doubly_linked{fw<U>(x), at(pos.pos).pos_next, pos.pos};
    at(at(pos.pos).pos_next).pos_prev = node;
    next_link(pos) = node;
    return after{seq, list_doubly_linked_position{node}};
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_first(list_doubly_linked_sentinel<T>& list, U&& x)
{
    insert(front{(list), fw<U>(x)});
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_first(list_doubly_linked_sentinel<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_last(list_doubly_linked_sentinel<T>& list, U&& x)
{
    insert(back{list}, fw<U>(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_last(list_doubly_linked_sentinel<T>& list, U x)
{
    insert(back{list}, mv(x));
}

template <Movable T>
constexpr auto
erase(front<list_doubly_linked_sentinel<T>> list) -> front<list_doubly_linked_sentinel<T>>
{
    auto& seq = base(list);
    auto pos = first(seq);
    set_link_bidirectional(predecessor(pos), successor(pos));
    delete pos.pos;
    return list;
}

template <Movable T>
constexpr auto
erase(back<list_doubly_linked_sentinel<T>> list) -> back<list_doubly_linked_sentinel<T>>
{
    auto& seq = base(list);
    auto pos = last(seq);
    set_link_bidirectional(predecessor(pos), successor(pos));
    delete pos.pos;
    return list;
}

template <Movable T>
constexpr void
erase(list_doubly_linked_position<T> pos)
{
    set_link_bidirectional(predecessor(pos), successor(pos));
    delete pos.pos;
}

template <Movable T>
constexpr void
erase_all(list_doubly_linked_sentinel<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <Movable T>
constexpr void
pop_first(list_doubly_linked_sentinel<T>& list)
{
    erase(front{list});
}

template <Movable T>
constexpr void
pop_last(list_doubly_linked_sentinel<T>& list)
{
    erase(back{list});
}

template <Movable T>
constexpr auto
first(list_doubly_linked_sentinel<T> const& x) -> Position_type<list_doubly_linked_sentinel<T>>
{
    return successor(x.sentinel);
}

template <Movable T>
constexpr auto
last(list_doubly_linked_sentinel<T> const& x) -> Position_type<list_doubly_linked_sentinel<T>>
{
    return predecessor(x.sentinel);
}

template <Movable T>
constexpr auto
limit(list_doubly_linked_sentinel<T> const& x) -> Position_type<list_doubly_linked_sentinel<T>>
{
    return x.sentinel;
}

template <Movable T>
constexpr auto
is_empty(list_doubly_linked_sentinel<T> const& x) -> bool
{
    return !precedes(first(x), limit(x));
}

template <Movable T>
constexpr auto
size(list_doubly_linked_sentinel<T> const& x) -> Size_type<list_doubly_linked_sentinel<T>>
{
    return limit(x) - first(x);
}

}