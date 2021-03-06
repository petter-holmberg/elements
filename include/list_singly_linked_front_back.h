#pragma once

#include "list_singly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "range.h"
#include "swap.h"

namespace elements {

template <typename T>
struct list_singly_linked_front_back
{
    list_singly_linked_cursor<T> head;
    list_singly_linked_cursor<T> tail;

    constexpr
    list_singly_linked_front_back() = default;

    constexpr
    list_singly_linked_front_back(list_singly_linked_front_back const& x)
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    list_singly_linked_front_back(list_singly_linked_front_back&& x)
    {
        head = x.head;
        x.head = {};
    }

    constexpr
    list_singly_linked_front_back(Size_type<list_singly_linked_front_back<T>> size, T const& x)
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    constexpr auto
    operator=(list_singly_linked_front_back const& x) -> list_singly_linked_front_back&
    {
        using elements::swap;
        list_singly_linked_front_back temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(list_singly_linked_front_back&& x) -> list_singly_linked_front_back&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(head, x.head);
        }
        return at(this);
    }

    constexpr
    ~list_singly_linked_front_back()
    {
        erase_all(at(this));
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_cursor<T>> i) -> T&
    {
        auto cur = head + i;
        return at(cur);
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_cursor<T>> i) const -> T const&
    {
        auto cur = head + i;
        return load(cur);
    }
};

template <typename T>
struct value_type_t<list_singly_linked_front_back<T>>
{
    using type = T;
};

template <typename T>
struct cursor_type_t<list_singly_linked_front_back<T>>
{
    using type = list_singly_linked_cursor<T>;
};

template <typename T>
struct cursor_type_t<list_singly_linked_front_back<T const>>
{
    using type = list_singly_linked_cursor<T const>;
};

template <typename T>
struct size_type_t<list_singly_linked_front_back<T>>
{
    using type = pointer_diff;
};

template <typename T>
struct functor_t<list_singly_linked_front_back<T>>
{
    using constructor_type = list_singly_linked_front_back<T>;

    template <Operation<T> Op>
    static constexpr auto
    fmap(list_singly_linked_front_back<T>& x, Op op) -> list_singly_linked_front_back<T>&
    {
        using elements::copy;
        copy(first(x), limit(x), map_sink{op}(first(x)));
        return x;
    }

    template <Regular_invocable<T> F>
    static constexpr auto
    fmap(list_singly_linked_front_back<T>&& x, F fun) -> list_singly_linked_front_back<Return_type<F, T>>
    {
        using elements::map;
        list_singly_linked_front_back<Return_type<F, T>> y;
        map(first(x), limit(x), insert_sink{}(back{y}), fun);
        return y;
    }
};

template <Regular T>
constexpr auto
operator==(list_singly_linked_front_back<T> const& x, list_singly_linked_front_back<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_singly_linked_front_back<T> const& x, list_singly_linked_front_back<T> const& y) -> bool
{
    return less_range(x, y);
}

template <typename T>
constexpr void
swap(list_singly_linked_front_back<T>& x, list_singly_linked_front_back<T>& y)
{
    swap(x.head, y.head);
    swap(x.tail, y.tail);
}

template <typename T, typename U>
constexpr auto
insert(front<list_singly_linked_front_back<T>> list, U&& x) -> front<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_cursor{new list_node_singly_linked{fw<U>(x), first(seq).cur}};
    if (is_empty(seq)) {
        seq.tail = node;
    }
    seq.head = node;
    return seq;
}

template <typename T, typename U>
constexpr auto
insert(back<list_singly_linked_front_back<T>> list, U&& x) -> back<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_cursor{new list_node_singly_linked{fw<U>(x)}};
    if (is_empty(seq)) {
        seq.head = node;
    } else {
        set_link_forward(seq.tail, node);
    }
    seq.tail = node;
    return seq;
}

template <typename T, Constructible_from<T> U>
constexpr auto
insert(after<list_singly_linked_front_back<T>> list, U&& x) -> after<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_cursor{new list_node_singly_linked{fw<U>(x)}};
    if (precedes(current(list), limit(list))) {
        if (!precedes(current(list), last(list))) {
            seq.tail = node;
        }
        set_link_forward(node, successor(current(list)));
        set_link_forward(current(list), node);
    } else {
        set_link_forward(node, first(list));
        seq.head = node;
        seq.tail = node;
    }
    return after{seq, node};
}

template <typename T, Constructible_from<T> U>
constexpr void
emplace_first(list_singly_linked_front_back<T>& list, U&& x)
{
    insert(front{list}, fw<U>(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
push_first(list_singly_linked_front_back<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
emplace_last(list_singly_linked_front_back<T>& list, U&& x)
{
    insert(back{list}, fw<U>(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
push_last(list_singly_linked_front_back<T>& list, U x)
{
    insert(back{list}, mv(x));
}

template <typename T>
constexpr auto
erase(front<list_singly_linked_front_back<T>> list) -> front<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto cur = successor(first(seq));
    delete first(seq).cur;
    if (!precedes(first(seq), last(seq))) {
        seq.tail = cur;
    }
    seq.head = cur;
    return list;
}

template <typename T>
constexpr auto
erase(after<list_singly_linked_front_back<T>> list) -> after<list_singly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto rem_cur = successor(current(list));
    set_link_forward(current(list), successor(successor(current(list))));
    if (!precedes(current(list), limit(list))) {
        seq.tail = current(list);
    }
    delete rem_cur.cur;
    return list;
}

template <typename T>
constexpr void
erase_all(list_singly_linked_front_back<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <typename T>
constexpr void
pop_first(list_singly_linked_front_back<T>& list)
{
    erase(front{list});
}

template <typename T>
constexpr auto
first(list_singly_linked_front_back<T> const& x) -> Cursor_type<list_singly_linked_front_back<T>>
{
    return x.head;
}

template <typename T>
constexpr auto
last(list_singly_linked_front_back<T> const& x) -> Cursor_type<list_singly_linked_front_back<T>>
{
    return x.tail;
}

template <typename T>
constexpr auto
limit(list_singly_linked_front_back<T> const&) -> Cursor_type<list_singly_linked_front_back<T>>
{
    return {};
}

template <typename T>
constexpr auto
is_empty(list_singly_linked_front_back<T> const& x) -> bool
{
    return first(x) == limit(x);
}

template <typename T>
constexpr auto
size(list_singly_linked_front_back<T> const& x) -> Size_type<list_singly_linked_front_back<T>>
{
    return limit(x) - first(x);
}

}
