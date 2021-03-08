#pragma once

#include "list_doubly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <typename T>
struct list_doubly_linked_circular_cursor
{
    Pointer_type<list_node_doubly_linked<T>> cur{};
    Pointer_type<list_node_doubly_linked<T>> front{};
    Pointer_type<list_node_doubly_linked<T>> back{};

    constexpr
    list_doubly_linked_circular_cursor() = default;

    explicit constexpr
    list_doubly_linked_circular_cursor(
        Pointer_type<list_node_doubly_linked<T>> cur_,
        Pointer_type<list_node_doubly_linked<T>> front_,
        Pointer_type<list_node_doubly_linked<T>> back_)
        : cur{cur_}
        , front{front_}
        , back{back_}
    {}
};

template <typename T>
struct value_type_t<list_doubly_linked_circular_cursor<T>>
{
    using type = T;
};

template <typename T>
struct difference_type_t<list_doubly_linked_circular_cursor<T>>
{
    using type = Difference_type<Pointer_type<list_node_doubly_linked<T>>>;
};

template <typename T>
constexpr auto
operator==(list_doubly_linked_circular_cursor<T> const& cur0, list_doubly_linked_circular_cursor<T> const& cur1) -> bool
{
    return cur0.cur == cur1.cur and cur0.front == cur1.front and cur0.back == cur1.back;
}

template <typename T>
constexpr void
increment(list_doubly_linked_circular_cursor<T>& cur)
{
    if (precedes(cur.cur, cur.back)) {
        cur.cur = next_link(cur);
    } else {
        cur = {};
    }
}

template <typename T>
constexpr void
decrement(list_doubly_linked_circular_cursor<T>& cur)
{
    if (precedes(cur.front, cur.cur)) {
        cur.cur = prev_link(cur);
    } else {
        cur = {};
    }
}

template <typename T>
constexpr auto
load(list_doubly_linked_circular_cursor<T> cur) -> T const&
{
    return load(cur.cur).x;
}

template <typename T>
constexpr void
store(list_doubly_linked_circular_cursor<T>& cur, T const& value)
{
    store(cur.cur, value);
}

template <typename T>
constexpr void
store(list_doubly_linked_circular_cursor<T>& cur, T&& value)
{
    store(cur.cur, fw<T>(value));
}

template <typename T>
constexpr auto
at(list_doubly_linked_circular_cursor<T> const& cur) -> T const&
{
    return load(cur.cur).x;
}

template <typename T>
constexpr auto
at(list_doubly_linked_circular_cursor<T>& cur) -> T&
{
    return at(cur.cur).x;
}

template <typename T>
constexpr auto
next_link(list_doubly_linked_circular_cursor<T> const& cur) -> Pointer_type<list_node_doubly_linked<T>>&
{
    return at(cur.cur).next;
}

template <typename T>
constexpr auto
prev_link(list_doubly_linked_circular_cursor<T> const& cur) -> Pointer_type<list_node_doubly_linked<T>>&
{
    return at(cur.cur).prev;
}

template <typename T>
constexpr auto
precedes(list_doubly_linked_circular_cursor<T> const& cur0, list_doubly_linked_circular_cursor<T> const& cur1) -> bool
{
    return precedes(cur0.cur, cur1.cur);
}

template <typename T>
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

    constexpr
    ~list_doubly_linked_circular()
    {
        erase_all(at(this));
    }

    constexpr auto
    operator[](Difference_type<list_doubly_linked_circular_cursor<T>> i) -> T&
    {
        auto cur = first(at(this)) + i;
        return at(cur);
    }

    constexpr auto
    operator[](Difference_type<list_doubly_linked_circular_cursor<T>> i) const -> T const&
    {
        auto cur = first(at(this)) + i;
        return load(cur);
    }
};

template <typename T>
struct value_type_t<list_doubly_linked_circular<T>>
{
    using type = T;
};

template <typename T>
struct cursor_type_t<list_doubly_linked_circular<T>>
{
    using type = list_doubly_linked_circular_cursor<T>;
};

template <typename T>
struct cursor_type_t<list_doubly_linked_circular<T const>>
{
    using type = list_doubly_linked_circular_cursor<T const>;
};

template <typename T>
struct size_type_t<list_doubly_linked_circular<T>>
{
    using type = pointer_diff;
};

template <typename T>
struct functor_t<list_doubly_linked_circular<T>>
{
    using constructor_type = list_doubly_linked_circular<T>;

    template <Operation<T> Op>
    static constexpr auto
    fmap(list_doubly_linked_circular<T>& x, Op op) -> list_doubly_linked_circular<T>&
    {
        using elements::copy;
        copy(first(x), limit(x), map_sink{op}(first(x)));
        return x;
    }

    template <Regular_invocable<T> F>
    static constexpr auto
    fmap(list_doubly_linked_circular<T>&& x, F fun) -> list_doubly_linked_circular<Return_type<F, T>>
    {
        using elements::map;
        list_doubly_linked_circular<Return_type<F, T>> y;
        map(first(x), limit(x), insert_sink{}(back{y}), fun);
        return y;
    }
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

template <typename T>
constexpr void
swap(list_doubly_linked_circular<T>& x, list_doubly_linked_circular<T>& y)
{
    swap(x.front, y.front);
}

template <typename T, typename U>
constexpr auto
insert(front<list_doubly_linked_circular<T>> list, U&& x) -> front<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).cur, last(seq).cur};
    if (is_empty(seq)) {
        at(node).next = node;
        at(node).prev = node;
    } else {
        at(at(seq.front).prev).next = node;
        at(seq.front).prev = node;
    }
    seq.front = node;
    return seq;
}

template <typename T, typename U>
constexpr auto
insert(back<list_doubly_linked_circular<T>> list, U&& x) -> back<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).cur, last(seq).cur};
    if (is_empty(seq)) {
        at(node).next = node;
        at(node).prev = node;
        seq.front = node;
    } else {
        at(at(seq.front).prev).next = node;
        at(seq.front).prev = node;
    }
    return seq;
}

template <typename T, Constructible_from<T> U>
constexpr auto
insert(before<list_doubly_linked_circular<T>> list, U&& x) -> before<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).cur, last(seq).cur};
    if (precedes(current(list), limit(list))) {
        if (!precedes(seq.front, current(list).cur)) {
            seq.front = node;
        }
        at(prev_link(current(list))).next = node;
        at(node).prev = prev_link(current(list));
        prev_link(current(list)) = node;
        at(node).next = current(list).cur;
    } else {
        if (is_empty(seq)) {
            at(node).next = node;
            at(node).prev = node;
            seq.front = node;
        } else {
            at(seq.front).prev = node;
            at(at(node).prev).next = node;
        }
    }
    return before{seq, list_doubly_linked_circular_cursor{node, first(seq).cur, last(seq).cur}};
}

template <typename T, Constructible_from<T> U>
constexpr auto
insert(after<list_doubly_linked_circular<T>> list, U&& x) -> after<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).cur, last(seq).cur};
    if (precedes(current(list), limit(list))) {
        at(next_link(current(list))).prev = node;
        at(node).next = next_link(current(list));
        at(current(list).cur).next = node;
        at(node).prev = current(list).cur;
    } else {
        if (is_empty(seq)) {
            at(node).next = node;
            at(node).prev = node;
            seq.front = node;
        } else {
            at(seq.front).prev = node;
            at(at(node).prev).next = node;
        }
    }
    return after{seq, list_doubly_linked_circular_cursor{node, first(seq).cur, last(seq).cur}};
}

template <typename T, Constructible_from<T> U>
constexpr void
emplace_first(list_doubly_linked_circular<T>& list, U&& x)
{
    insert(front{(list), fw<U>(x)});
}

template <typename T, Constructible_from<T> U>
constexpr void
push_first(list_doubly_linked_circular<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
emplace_last(list_doubly_linked_circular<T>& list, U&& x)
{
    insert(back{list}, fw<U>(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
push_last(list_doubly_linked_circular<T>& list, U x)
{
    insert(back{list}, mv(x));
}

template <typename T>
constexpr auto
erase(front<list_doubly_linked_circular<T>> list) -> front<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto cur = seq.front;
    if (precedes(seq.front, at(seq.front).next)) {
        at(at(cur).prev).next = at(cur).next;
        at(at(cur).next).prev = at(cur).prev;
        seq.front = at(cur).next;
    } else {
        seq.front = {};
    }
    delete cur;
    return list;
}

template <typename T>
constexpr auto
erase(back<list_doubly_linked_circular<T>> list) -> back<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto cur = at(seq.front).prev;
    if (precedes(seq.front, at(seq.front).next)) {
        at(at(cur).prev).next = seq.front;
        at(seq.front).prev = at(cur).prev;
    } else {
        seq.front = {};
    }
    delete cur;
    return list;
}

template <typename T>
constexpr auto
erase(before<list_doubly_linked_circular<T>> list) -> before<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto& node = at(current(list).cur);
    auto cur = node.prev;
    if (precedes(seq.front, at(seq.front).next)) {
        if (!precedes(seq.front, cur)) {
            seq.front = current(list).cur;
        }
        node.prev = at(cur).prev;
        at(at(cur).prev).next = current(list).cur;
    } else {
        seq.front = {};
    }
    delete cur;
    return list;
}

template <typename T>
constexpr auto
erase(after<list_doubly_linked_circular<T>> list) -> after<list_doubly_linked_circular<T>>
{
    auto& seq = base(list);
    auto& node = at(current(list).cur);
    auto cur = node.next;
    if (precedes(seq.front, at(seq.front).next)) {
        if (!precedes(seq.front, cur)) {
            seq.front = at(cur).next;
        }
        node.next = at(cur).next;
        at(at(cur).next).prev = at(cur).prev;
    } else {
        seq.front = {};
    }
    delete cur;
    return list;
}

template <typename T>
constexpr void
erase_all(list_doubly_linked_circular<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <typename T>
constexpr void
pop_first(list_doubly_linked_circular<T>& list)
{
    erase(front{list});
}

template <typename T>
constexpr void
pop_last(list_doubly_linked_circular<T>& list)
{
    erase(back{list});
}

template <typename T>
constexpr auto
first(list_doubly_linked_circular<T> const& x) -> Cursor_type<list_doubly_linked_circular<T>>
{
    if (is_empty(x)) {
        return {};
    } else {
        return list_doubly_linked_circular_cursor{x.front, x.front, at(x.front).prev};
    }
}

template <typename T>
constexpr auto
last(list_doubly_linked_circular<T> const& x) -> Cursor_type<list_doubly_linked_circular<T>>
{
    if (is_empty(x)) {
        return {};
    } else {
        return list_doubly_linked_circular_cursor{at(x.front).prev, x.front, at(x.front).prev};
    }
}

template <typename T>
constexpr auto
limit(list_doubly_linked_circular<T> const&) -> Cursor_type<list_doubly_linked_circular<T>>
{
    return {};
}

template <typename T>
constexpr auto
is_empty(list_doubly_linked_circular<T> const& x) -> bool
{
    return x.front == nullptr;
}

template <typename T>
constexpr auto
size(list_doubly_linked_circular<T> const& x) -> Size_type<list_doubly_linked_circular<T>>
{
    return limit(x) - first(x);
}

}
