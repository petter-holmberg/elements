#pragma once

#include "list_doubly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <typename T>
struct list_node_doubly_linked_sentinel_links
{
    Pointer_type<list_node_doubly_linked<T>> next{};
    Pointer_type<list_node_doubly_linked<T>> prev{};

    constexpr
    list_node_doubly_linked_sentinel_links()
        : next{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
        , prev{reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(this)}
    {}
};

template <typename T>
constexpr auto
allocate_list_doubly_linked_sentinel_node() -> list_doubly_linked_cursor<T>
{
    return list_doubly_linked_cursor<T>{
        reinterpret_cast<Pointer_type<list_node_doubly_linked<T>>>(
            new list_node_doubly_linked_sentinel_links<T>)};
}

template <typename T>
constexpr void
deallocate_list_doubly_linked_sentinel_node(list_doubly_linked_cursor<T> cur)
{
    delete reinterpret_cast<Pointer_type<list_node_doubly_linked_sentinel_links<T>>>(cur.cur);
}

template <typename T>
struct list_doubly_linked_sentinel
{
    list_doubly_linked_cursor<T> sentinel{};

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

    constexpr
    ~list_doubly_linked_sentinel()
    {
        erase_all(at(this));
        deallocate_list_doubly_linked_sentinel_node<T>(sentinel);
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
};

template <typename T>
struct value_type_t<list_doubly_linked_sentinel<T>>
{
    using type = T;
};

template <typename T>
struct cursor_type_t<list_doubly_linked_sentinel<T>>
{
    using type = list_doubly_linked_cursor<T>;
};

template <typename T>
struct cursor_type_t<list_doubly_linked_sentinel<T const>>
{
    using type = list_doubly_linked_cursor<T const>;
};

template <typename T>
struct size_type_t<list_doubly_linked_sentinel<T>>
{
    using type = pointer_diff;
};

template <typename T>
struct functor_t<list_doubly_linked_sentinel<T>>
{
    using constructor_type = list_doubly_linked_sentinel<T>;

    template <Operation<T> Op>
    static constexpr auto
    fmap(list_doubly_linked_sentinel<T>& x, Op op) -> list_doubly_linked_sentinel<T>&
    {
        using elements::copy;
        copy(first(x), limit(x), map_sink{op}(first(x)));
        return x;
    }

    template <Regular_invocable<T> F>
    static constexpr auto
    fmap(list_doubly_linked_sentinel<T>&& x, F fun) -> list_doubly_linked_sentinel<Return_type<F, T>>
    {
        using elements::map;
        list_doubly_linked_sentinel<Return_type<F, T>> y;
        map(first(x), limit(x), insert_sink{}(back{y}), fun);
        return y;
    }
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

template <typename T>
constexpr void
swap(list_doubly_linked_sentinel<T>& x, list_doubly_linked_sentinel<T>& y)
{
    swap(x.sentinel, y.sentinel);
}

template <typename T, typename U>
constexpr auto
insert(front<list_doubly_linked_sentinel<T>> list, U&& x) -> front<list_doubly_linked_sentinel<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), first(seq).cur, limit(seq).cur};
    next_link(seq.sentinel) = node;
    at(at(node).next).prev = node;
    return list;
}

template <typename T, typename U>
constexpr auto
insert(back<list_doubly_linked_sentinel<T>> list, U&& x) -> back<list_doubly_linked_sentinel<T>>
{
    auto& seq = base(list);
    auto node = new list_node_doubly_linked{fw<U>(x), limit(seq).cur, last(seq).cur};
    at(predecessor(seq.sentinel).cur).next = node;
    prev_link(seq.sentinel) = node;
    return list;
}

template <typename T, Constructible_from<T> U>
constexpr auto
insert(before<list_doubly_linked_sentinel<T>> list, U&& x) -> before<list_doubly_linked_sentinel<T>>
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
insert(after<list_doubly_linked_sentinel<T>> list, U&& x) -> after<list_doubly_linked_sentinel<T>>
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
emplace_first(list_doubly_linked_sentinel<T>& list, U&& x)
{
    insert(front{(list), fw<U>(x)});
}

template <typename T, Constructible_from<T> U>
constexpr void
push_first(list_doubly_linked_sentinel<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
emplace_last(list_doubly_linked_sentinel<T>& list, U&& x)
{
    insert(back{list}, fw<U>(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
push_last(list_doubly_linked_sentinel<T>& list, U x)
{
    insert(back{list}, mv(x));
}

template <typename T>
constexpr auto
erase(front<list_doubly_linked_sentinel<T>> list) -> front<list_doubly_linked_sentinel<T>>
{
    auto& seq = base(list);
    auto cur = first(seq);
    set_link_bidirectional(predecessor(cur), successor(cur));
    delete cur.cur;
    return list;
}

template <typename T>
constexpr auto
erase(back<list_doubly_linked_sentinel<T>> list) -> back<list_doubly_linked_sentinel<T>>
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
erase_all(list_doubly_linked_sentinel<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <typename T>
constexpr void
pop_first(list_doubly_linked_sentinel<T>& list)
{
    erase(front{list});
}

template <typename T>
constexpr void
pop_last(list_doubly_linked_sentinel<T>& list)
{
    erase(back{list});
}

template <typename T>
constexpr auto
first(list_doubly_linked_sentinel<T> const& x) -> Cursor_type<list_doubly_linked_sentinel<T>>
{
    return successor(x.sentinel);
}

template <typename T>
constexpr auto
last(list_doubly_linked_sentinel<T> const& x) -> Cursor_type<list_doubly_linked_sentinel<T>>
{
    return predecessor(x.sentinel);
}

template <typename T>
constexpr auto
limit(list_doubly_linked_sentinel<T> const& x) -> Cursor_type<list_doubly_linked_sentinel<T>>
{
    return x.sentinel;
}

template <typename T>
constexpr auto
is_empty(list_doubly_linked_sentinel<T> const& x) -> bool
{
    return !precedes(first(x), limit(x));
}

template <typename T>
constexpr auto
size(list_doubly_linked_sentinel<T> const& x) -> Size_type<list_doubly_linked_sentinel<T>>
{
    return limit(x) - first(x);
}

}
