#pragma once

#include "functional.h"
#include "list_singly_linked.h"
#include "lexicographical.h"
#include "map.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct list_singly_linked_circular_cursor
{
    Pointer_type<list_node_singly_linked<T>> cur{};
    Pointer_type<list_node_singly_linked<T>> back{};

    constexpr
    list_singly_linked_circular_cursor() = default;

    explicit constexpr
    list_singly_linked_circular_cursor(
        Pointer_type<list_node_singly_linked<T>> cur_,
        Pointer_type<list_node_singly_linked<T>> back_)
        : cur{cur_}
        , back{back_}
    {}
};

template <Movable T>
struct value_type_t<list_singly_linked_circular_cursor<T>>
{
    using type = T;
};

template <Movable T>
struct difference_type_t<list_singly_linked_circular_cursor<T>>
{
    using type = Difference_type<Pointer_type<list_node_singly_linked<T>>>;
};

template <Movable T>
bool operator==(list_singly_linked_circular_cursor<T> const& cur0, list_singly_linked_circular_cursor<T> const& cur1)
{
    return cur0.cur == cur1.cur and cur0.back == cur1.back;
}

template <Movable T>
constexpr void
increment(list_singly_linked_circular_cursor<T>& cur)
{
    if (precedes(cur.cur, cur.back)) {
        cur.cur = next_link(cur);
    } else {
        cur = {};
    }
}

template <Movable T>
constexpr auto
load(list_singly_linked_circular_cursor<T> cur) -> T const&
{
    return load(cur.cur).x;
}

template <Movable T>
constexpr void
store(list_singly_linked_circular_cursor<T>& cur, T const& value)
{
    store(cur.cur, value);
}

template <Movable T>
constexpr void
store(list_singly_linked_circular_cursor<T>& cur, T&& value)
{
    store(cur.cur, fw<T>(value));
}

template <Movable T>
constexpr auto
at(list_singly_linked_circular_cursor<T> const& cur) -> T const&
{
    return load(cur.cur).x;
}

template <Movable T>
constexpr auto
at(list_singly_linked_circular_cursor<T>& cur) -> T&
{
    return at(cur.cur).x;
}

template <Movable T>
constexpr auto
next_link(list_singly_linked_circular_cursor<T> const& cur) -> Pointer_type<list_node_singly_linked<T>>&
{
    return at(cur.cur).next;
}

template <Movable T>
constexpr auto
precedes(list_singly_linked_circular_cursor<T> const& cur0, list_singly_linked_circular_cursor<T> const& cur1) -> bool
{
    return precedes(cur0.cur, cur1.cur);
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
    operator[](Difference_type<list_singly_linked_circular_cursor<T>> i) -> T&
    {
        auto cur = first(at(this)) + i;
        return at(cur);
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_circular_cursor<T>> i) const -> T const&
    {
        auto cur = first(at(this)) + i;
        return load(cur);
    }

    template <Unary_function F>
    requires
        Same_as<Decay<T>, Domain<F>> and
        Same_as<Decay<T>, Codomain<F>>
    constexpr auto
    fmap(F fun) -> list_singly_linked_circular<T>&
    {
        using elements::copy;
        copy(first(at(this)), limit(at(this)), map_sink{fun}(first(at(this))));
        return at(this);
    }

    template <Unary_function F>
    requires Same_as<Decay<T>, Domain<F>>
    constexpr auto
    fmap(F fun) const -> list_singly_linked_circular<Codomain<F>>
    {
        using elements::map;
        list_singly_linked_circular<Codomain<F>> x;
        map(first(at(this)), limit(at(this)), insert_sink{}(back{x}), fun);
        return x;
    }
};

template <Movable T>
struct value_type_t<list_singly_linked_circular<T>>
{
    using type = T;
};

template <Movable T>
struct cursor_type_t<list_singly_linked_circular<T>>
{
    using type = list_singly_linked_circular_cursor<T>;
};

template <Movable T>
struct cursor_type_t<list_singly_linked_circular<T const>>
{
    using type = list_singly_linked_circular_cursor<T const>;
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
    auto node = new list_node_singly_linked{fw<U>(x), first(seq).cur};
    if (is_empty(seq)) {
        at(node).next = node;
        seq.tail = node;
    } else {
        at(seq.tail).next = node;
    }
    return seq;
}

template <Movable T, typename U>
constexpr auto
insert(back<list_singly_linked_circular<T>> list, U&& x) -> back<list_singly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_singly_linked{fw<U>(x), first(seq).cur};
    if (is_empty(seq)) {
        at(node).next = node;
    } else {
        at(seq.tail).next = node;
    }
    seq.tail = node;
    return seq;
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(after<list_singly_linked_circular<T>> list, U&& x) -> after<list_singly_linked_circular<T>>
{
    auto& seq = base(list);
    auto node = new list_node_singly_linked{fw<U>(x), first(seq).cur};
    if (precedes(current(list), limit(list))) {
        at(node).next = at(current(list).cur).next;
        at(current(list).cur).next = node;
        if (!precedes(current(list), last(list))) {
            seq.tail = node;
        }
    } else {
        if (is_empty(seq)) {
            at(node).next = node;
        } else {
            at(seq.tail).next = node;
        }
        seq.tail = node;
    }
    return after{seq, list_singly_linked_circular_cursor{node, last(seq).cur}};
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
    auto cur = at(seq.tail).next;
    if (precedes(seq.tail, at(seq.tail).next)) {
        at(seq.tail).next = at(cur).next;
    } else {
        seq.tail = {};
    }
    delete cur;
    return list;
}

template <Movable T>
constexpr auto
erase(after<list_singly_linked_circular<T>> list) -> after<list_singly_linked_circular<T>>
{
    auto& seq = base(list);
    auto& node = at(current(list).cur);
    auto cur = node.next;
    if (precedes(seq.tail, at(seq.tail).next)) {
        node.next = at(cur).next;
        if (!precedes(cur, seq.tail)) {
            at(seq.tail).next = seq.tail;
            seq.tail = current(list).cur;
        }
    } else {
        seq.tail = {};
    }
    delete cur;
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
first(list_singly_linked_circular<T> const& x) -> Cursor_type<list_singly_linked_circular<T>>
{
    if (is_empty(x)) {
        return {};
    } else {
        return list_singly_linked_circular_cursor{at(x.tail).next, x.tail};
    }
}

template <Movable T>
constexpr auto
last(list_singly_linked_circular<T> const& x) -> Cursor_type<list_singly_linked_circular<T>>
{
    if (is_empty(x)) {
        return {};
    } else {
        return list_singly_linked_circular_cursor{x.tail, x.tail};
    }
}

template <Movable T>
constexpr auto
limit(list_singly_linked_circular<T> const&) -> Cursor_type<list_singly_linked_circular<T>>
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
