#pragma once

#include "list_singly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct list_singly_linked_front
{
    list_singly_linked_position<T> head;

    constexpr
    list_singly_linked_front() = default;

    constexpr
    list_singly_linked_front(list_singly_linked_front const& x)
    {
        auto src = first(x);
        auto dst = pointer_to(at(this).head.pos);
        while (precedes(src, limit(x))) {
            at(dst) = new list_node_singly_linked{load(src)};
            dst = pointer_to(at(at(dst)).pos_next);
            increment(src);
        }
    }

    list_singly_linked_front(list_singly_linked_front&& x)
    {
        head = x.head;
        x.head = {};
    }

    list_singly_linked_front(std::initializer_list<T> x)
    {
        auto src = first(x);
        auto dst = pointer_to(at(this).head.pos);
        while (src != limit(x)) {
            at(dst) = new list_node_singly_linked{load(src)};
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
        using elements::swap;
        list_singly_linked_front temp(x);
        swap(at(this), temp);
        return at(this);
    }

    auto
    operator=(list_singly_linked_front&& x) -> list_singly_linked_front&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(head, x.head);
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
    map(Fun fun) -> list_singly_linked_front<T>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function Fun>
    requires Same_as<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> list_singly_linked_front<Codomain<Fun>>
    {
        using elements::map;
        list_singly_linked_front<Codomain<Fun>> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(front{x}), fun);
        reverse(x);
        return x;
    }

    template <Unary_function Fun>
    requires
        Same_as<Decay<T>, Domain<Fun>> and
        Same_as<list_singly_linked_front<Decay<T>>, Codomain<Fun>>
    constexpr auto
    flat_map(Fun fun) -> list_singly_linked_front<T>&
    {
        using elements::flat_map;
        auto x = flat_map<
            list_singly_linked_position<T>,
            list_singly_linked_position<T>,
            Fun,
            front<list_singly_linked_front<T>>>(first(at(this)), limit(at(this)), fun);
        reverse(x);
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
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_singly_linked_front<T> const& x, list_singly_linked_front<T> const& y) -> bool
{
    return less_range(x, y);
}

template <Movable T>
constexpr void
swap(list_singly_linked_front<T>& x, list_singly_linked_front<T>& y)
{
    swap(x.head, y.head);
}

template <Movable T, typename U>
constexpr auto
insert(front<list_singly_linked_front<T>> list, U&& x) -> front<list_singly_linked_front<T>>
{
    auto& seq = base(list);
    seq.head = list_singly_linked_position{
        new list_node_singly_linked{fw<U>(x), first(seq).pos}
    };
    return seq;
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(after<list_singly_linked_front<T>> list, U&& x) -> after<list_singly_linked_front<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_position{new list_node_singly_linked<T>{fw<U>(x)}};
    if (precedes(current(list), limit(list))) {
        set_link_forward(node, successor(current(list)));
        set_link_forward(current(list), node);
    } else {
        set_link_forward(node, first(list));
        seq.head = node;
    }
    return after{seq, node};
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_first(list_singly_linked_front<T>& list, U&& x)
{
    insert(front{list}, fw<U>(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_first(list_singly_linked_front<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <Movable T>
constexpr auto
erase(front<list_singly_linked_front<T>> list) -> front<list_singly_linked_front<T>>
{
    auto& seq = base(list);
    auto pos = successor(first(seq));
    delete first(seq).pos;
    seq.head = pos;
    return list;
}

template <Movable T>
constexpr auto
erase(after<list_singly_linked_front<T>> list) -> after<list_singly_linked_front<T>>
{
    auto rem_pos = successor(current(list));
    set_link_forward(current(list), successor(successor(current(list))));
    delete rem_pos.pos;
    return list;
}

template <Movable T>
constexpr void
erase_all(list_singly_linked_front<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <Movable T>
constexpr void
pop_first(list_singly_linked_front<T>& list)
{
    erase(front{list});
}

template <Movable T>
constexpr auto
first(list_singly_linked_front<T> const& x) -> Position_type<list_singly_linked_front<T>>
{
    return x.head;
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

template <typename S>
    requires Forward_position<Position_type<S>>
struct linker_to_head
{
    S set_link;

    linker_to_head(S const& set_link_)
        : set_link{set_link_}
    {}

    void operator()(Position_type<S>& head, Position_type<S>& pos)
    {
        auto temp = successor(pos);
        set_link(pos, head);
        head = pos;
        pos = temp;
    }
};

template <typename S>
    requires Forward_position<Position_type<S>>
constexpr auto
reverse_append(Position_type<S> pos, Position_type<S> lim, Position_type<S> head, S set_link) -> Position_type<S>
//[[expects: bounded_range(pos, lim)]]
{
    linker_to_head<S> link_to_head{set_link};
    while (pos != lim) link_to_head(head, pos);
    return head;
}

template <Movable T>
constexpr void
reverse(list_singly_linked_front<T>& x)
{
    x.head = reverse_append(first(x), limit(x), limit(x), forward_linker<Position_type<list_singly_linked_front<T>>>{});
}

}