#pragma once

#include "list_doubly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <typename T>
struct list_doubly_linked_front_back
{
    Pointer_type<list_node_doubly_linked<T>> next{};
    Pointer_type<list_node_doubly_linked<T>> prev{};

    constexpr
    list_doubly_linked_front_back()
        : next{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
        , prev{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
    {}

    constexpr
    list_doubly_linked_front_back(list_doubly_linked_front_back const& x)
        : next{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
        , prev{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
    {
        insert_range(x, back{at(this)});
    }

    constexpr
    list_doubly_linked_front_back(list_doubly_linked_front_back&& x)
        : next{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
        , prev{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
    {
        using elements::swap;
        swap(at(this), x);
    }

    constexpr
    list_doubly_linked_front_back(Size_type<list_doubly_linked_front_back<T>> size, T const& x)
        : next{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
        , prev{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    constexpr auto
    operator=(list_doubly_linked_front_back const& x) -> list_doubly_linked_front_back&
    {
        using elements::swap;
        list_doubly_linked_front_back temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(list_doubly_linked_front_back&& x) -> list_doubly_linked_front_back&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(at(this), x);
        }
        return at(this);
    }

    constexpr
    ~list_doubly_linked_front_back()
    {
        erase_all(at(this));
    }

    constexpr auto
    operator[](Difference_type<list_doubly_linked_cursor<T>> i) -> T&
    {
        auto cur = first(at(this)) + i;
        return at(cur);
    }

    constexpr auto
    operator[](Difference_type<list_doubly_linked_cursor<T>> i) const -> T const&
    {
        auto cur = first(at(this)) + i;
        return load(cur);
    }

    template <Operation<T> Op>
    constexpr auto
    fmap(Op op) -> list_doubly_linked_front_back<T>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{op}(first(at(this))));
        return at(this);
    }

    template <Regular_invocable<T> F>
    constexpr auto
    fmap(F fun) const -> list_doubly_linked_front_back<Return_type<F, T>>
    {
        using elements::map;
        list_doubly_linked_front_back<Return_type<F, T>> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }
};

template <typename T>
struct value_type_t<list_doubly_linked_front_back<T>>
{
    using type = T;
};

template <typename T>
struct cursor_type_t<list_doubly_linked_front_back<T>>
{
    using type = list_doubly_linked_cursor<T>;
};

template <typename T>
struct cursor_type_t<list_doubly_linked_front_back<T const>>
{
    using type = list_doubly_linked_cursor<T const>;
};

template <typename T>
struct size_type_t<list_doubly_linked_front_back<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(list_doubly_linked_front_back<T> const& x, list_doubly_linked_front_back<T> const& y) -> bool
{
    return equal_range(x, y);
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_doubly_linked_front_back<T> const& x, list_doubly_linked_front_back<T> const& y) -> bool
{
    return less_range(x, y);
}

template <typename T>
constexpr void
swap_nonempty_empty(list_doubly_linked_front_back<T>& x, list_doubly_linked_front_back<T>& y)
    //[[expects: !empty(x)]]
    //[[expects: empty(y)]]
{
    at(x.next).prev = y.prev;
    at(x.prev).next = y.next;
    y.next = x.next;
    y.prev = x.prev;
    x.next = reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(pointer_to(x));
    x.prev = reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(pointer_to(x));
}

template <typename T>
constexpr void
swap(list_doubly_linked_front_back<T>& x, list_doubly_linked_front_back<T>& y)
{
    if (is_empty(x) and is_empty(y)) return;
    if (is_empty(x) and !is_empty(y)) return swap_nonempty_empty(y, x);
    if (is_empty(y)) return swap_nonempty_empty(x, y);
    swap(at(x.next).prev, at(y.next).prev);
    swap(x.next, y.next);
    swap(at(x.prev).next, at(y.prev).next);
    swap(x.prev, y.prev);
}

template <typename T, typename U>
constexpr auto
insert(front<list_doubly_linked_front_back<T>> list, U&& x) -> front<list_doubly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).cur, limit(seq).cur};
    seq.next = node;
    at(at(node).next).prev = node;
    return list;
}

template <typename T, typename U>
constexpr auto
insert(back<list_doubly_linked_front_back<T>> list, U&& x) -> back<list_doubly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), limit(seq).cur, last(seq).cur};
    at(seq.prev).next = node;
    seq.prev = node;
    return list;
}

template <typename T, Constructible_from<T> U>
constexpr auto
insert(before<list_doubly_linked_front_back<T>> list, U&& x) -> before<list_doubly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto cur = current(list);
    auto node = new list_node_doubly_linked{fw<U>(x), cur.cur, at(cur.cur).prev};
    prev_link(cur) = node;
    at(at(node).prev).next = node;
    return before{seq, list_doubly_linked_cursor{node}};
}

template <typename T, Constructible_from<T> U>
constexpr auto
insert(after<list_doubly_linked_front_back<T>> list, U&& x) -> after<list_doubly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto cur = current(list);
    auto node = new list_node_doubly_linked{fw<U>(x), at(cur.cur).next, cur.cur};
    at(at(cur.cur).next).prev = node;
    next_link(cur) = node;
    return after{seq, list_doubly_linked_cursor{node}};
}

template <typename T, Constructible_from<T> U>
constexpr void
emplace_first(list_doubly_linked_front_back<T>& list, U&& x)
{
    insert(front{(list), fw<U>(x)});
}

template <typename T, Constructible_from<T> U>
constexpr void
push_first(list_doubly_linked_front_back<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
emplace_last(list_doubly_linked_front_back<T>& list, U&& x)
{
    insert(back{list}, fw<U>(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
push_last(list_doubly_linked_front_back<T>& list, U x)
{
    insert(back{list}, mv(x));
}

template <typename T>
constexpr auto
erase(front<list_doubly_linked_front_back<T>> list) -> front<list_doubly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto cur = first(seq);
    set_link_bidirectional(predecessor(cur), successor(cur));
    delete cur.cur;
    return list;
}

template <typename T>
constexpr auto
erase(back<list_doubly_linked_front_back<T>> list) -> back<list_doubly_linked_front_back<T>>
{
    auto& seq = base(list);
    auto cur = last(seq);
    set_link_bidirectional(predecessor(cur), successor(cur));
    delete cur.cur;
    return list;
}

template <typename T>
constexpr void
erase(list_doubly_linked_cursor<T> cur)
{
    set_link_bidirectional(predecessor(cur), successor(cur));
    delete cur.cur;
}

template <typename T>
constexpr void
erase_all(list_doubly_linked_front_back<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <typename T>
constexpr void
pop_first(list_doubly_linked_front_back<T>& list)
{
    erase(front{list});
}

template <typename T>
constexpr void
pop_last(list_doubly_linked_front_back<T>& list)
{
    erase(back{list});
}

template <typename T>
constexpr auto
first(list_doubly_linked_front_back<T> const& x) -> Cursor_type<list_doubly_linked_front_back<T>>
{
    return list_doubly_linked_cursor{x.next};
}

template <typename T>
constexpr auto
last(list_doubly_linked_front_back<T> const& x) -> Cursor_type<list_doubly_linked_front_back<T>>
{
    return list_doubly_linked_cursor{x.prev};
}

template <typename T>
constexpr auto
limit(list_doubly_linked_front_back<T> const& x) -> Cursor_type<list_doubly_linked_front_back<T>>
{
    return list_doubly_linked_cursor{
        reinterpret_cast<Cursor_type<list_node_doubly_linked<T>>>(
            pointer_to(const_cast<list_doubly_linked_front_back<T>&>(x)))};
}

template <typename T>
constexpr auto
is_empty(list_doubly_linked_front_back<T> const& x) -> bool
{
    return !precedes(first(x), limit(x));
}

template <typename T>
constexpr auto
size(list_doubly_linked_front_back<T> const& x) -> Size_type<list_doubly_linked_front_back<T>>
{
    return limit(x) - first(x);
}

}
