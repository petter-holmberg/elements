#pragma once

#include "flat_map.h"
#include "list_singly_linked.h"
#include "lexicographical.h"
#include "ordering.h"
#include "swap.h"

namespace elements {

template <typename T>
struct list_singly_linked_front
{
    list_singly_linked_cursor<T> head;

    constexpr
    list_singly_linked_front() = default;

    constexpr
    list_singly_linked_front(list_singly_linked_front const& x)
    {
        auto src = first(x);
        auto dst = pointer_to(at(this).head.cur);
        while (precedes(src, limit(x))) {
            at(dst) = new list_node_singly_linked{load(src)};
            dst = pointer_to(at(at(dst)).next);
            increment(src);
        }
    }

    constexpr
    list_singly_linked_front(list_singly_linked_front&& x)
    {
        head = x.head;
        x.head = {};
    }

    constexpr
    list_singly_linked_front(Size_type<list_singly_linked_front<T>> size, T const& x)
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    constexpr auto
    operator=(list_singly_linked_front const& x) -> list_singly_linked_front&
    {
        using elements::swap;
        list_singly_linked_front temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(list_singly_linked_front&& x) -> list_singly_linked_front&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(head, x.head);
        }
        return at(this);
    }

    constexpr
    ~list_singly_linked_front()
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
struct value_type_t<list_singly_linked_front<T>>
{
    using type = T;
};

template <typename T>
struct cursor_type_t<list_singly_linked_front<T>>
{
    using type = list_singly_linked_cursor<T>;
};

template <typename T>
struct cursor_type_t<list_singly_linked_front<T const>>
{
    using type = list_singly_linked_cursor<T const>;
};

template <typename T>
struct size_type_t<list_singly_linked_front<T>>
{
    using type = pointer_diff;
};

template <typename T>
struct functor_t<list_singly_linked_front<T>>
{
    using constructor_type = list_singly_linked_front<T>;

    template <Operation<T> Op>
    static constexpr auto
    fmap(list_singly_linked_front<T>& x, Op op) -> list_singly_linked_front<T>&
    {
        using elements::copy;
        copy(first(x), limit(x), map_sink{op}(first(x)));
        return x;
    }

    template <Regular_invocable<T> F>
    static constexpr auto
    fmap(list_singly_linked_front<T> const& x, F fun) -> list_singly_linked_front<Return_type<F, T>>
    {
        using elements::map;
        list_singly_linked_front<Return_type<F, T>> y;
        map(first(x), limit(x), insert_sink{}(front{y}), fun);
        reverse(y);
        return y;
    }
};

template <typename T>
struct monad_t<list_singly_linked_front<T>>
{
    template <Regular_invocable<T> F>
    static constexpr auto
    bind(list_singly_linked_front<T> const& x, F fun) -> Return_type<F, T>
    {
        using elements::flat_map;
        auto y{flat_map<list_singly_linked_cursor<T>, list_singly_linked_cursor<T>, F, front<Return_type<F, T>>>(first(x), limit(x), fun)};
        reverse(y);
        return y;
    }
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

template <typename T>
constexpr void
swap(list_singly_linked_front<T>& x, list_singly_linked_front<T>& y)
{
    swap(x.head, y.head);
}

template <typename T, typename U>
constexpr auto
insert(front<list_singly_linked_front<T>> list, U&& x) -> front<list_singly_linked_front<T>>
{
    auto& seq = base(list);
    seq.head = list_singly_linked_cursor{
        new list_node_singly_linked{fw<U>(x), first(seq).cur}
    };
    return seq;
}

template <typename T, Constructible_from<T> U>
constexpr auto
insert(after<list_singly_linked_front<T>> list, U&& x) -> after<list_singly_linked_front<T>>
{
    auto& seq = base(list);
    auto node = list_singly_linked_cursor{new list_node_singly_linked<T>{fw<U>(x)}};
    if (precedes(current(list), limit(list))) {
        set_link_forward(node, successor(current(list)));
        set_link_forward(current(list), node);
    } else {
        set_link_forward(node, first(list));
        seq.head = node;
    }
    return after{seq, node};
}

template <typename T, Constructible_from<T> U>
constexpr void
emplace_first(list_singly_linked_front<T>& list, U&& x)
{
    insert(front{list}, fw<U>(x));
}

template <typename T, Constructible_from<T> U>
constexpr void
push_first(list_singly_linked_front<T>& list, U x)
{
    insert(front{list}, mv(x));
}

template <typename T>
constexpr auto
erase(front<list_singly_linked_front<T>> list) -> front<list_singly_linked_front<T>>
{
    auto& seq = base(list);
    auto cur = successor(first(seq));
    delete first(seq).cur;
    seq.head = cur;
    return list;
}

template <typename T>
constexpr auto
erase(after<list_singly_linked_front<T>> list) -> after<list_singly_linked_front<T>>
{
    auto rem_cur = successor(current(list));
    set_link_forward(current(list), successor(successor(current(list))));
    delete rem_cur.cur;
    return list;
}

template <typename T>
constexpr void
erase_all(list_singly_linked_front<T>& x)
{
    while (!is_empty(x)) erase(front{x});
}

template <typename T>
constexpr void
pop_first(list_singly_linked_front<T>& list)
{
    erase(front{list});
}

template <typename T>
constexpr auto
first(list_singly_linked_front<T> const& x) -> Cursor_type<list_singly_linked_front<T>>
{
    return x.head;
}

template <typename T>
constexpr auto
limit(list_singly_linked_front<T> const&) -> Cursor_type<list_singly_linked_front<T>>
{
    return {};
}

template <typename T>
constexpr auto
is_empty(list_singly_linked_front<T> const& x) -> bool
{
    return first(x) == limit(x);
}

template <typename T>
constexpr auto
size(list_singly_linked_front<T> const& x) -> Size_type<list_singly_linked_front<T>>
{
    return limit(x) - first(x);
}

template <typename S>
    requires Forward_cursor<Cursor_type<S>>
struct linker_to_head
{
    S set_link;

    linker_to_head(S const& set_link_)
        : set_link{set_link_}
    {}

    void operator()(Cursor_type<S>& head, Cursor_type<S>& cur)
    {
        auto temp = successor(cur);
        set_link(cur, head);
        head = cur;
        cur = temp;
    }
};

template <typename S>
    requires Forward_cursor<Cursor_type<S>>
constexpr auto
reverse_append(Cursor_type<S> cur, Cursor_type<S> lim, Cursor_type<S> head, S set_link) -> Cursor_type<S>
//[[expects: is_bounded_range(cur, lim)]]
{
    linker_to_head<S> link_to_head{set_link};
    while (cur != lim) link_to_head(head, cur);
    return head;
}

template <typename T>
constexpr void
reverse(list_singly_linked_front<T>& x)
{
    x.head = reverse_append(first(x), limit(x), limit(x), forward_linker<Cursor_type<list_singly_linked_front<T>>>{});
}

}
