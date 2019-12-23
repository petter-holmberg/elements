#pragma once

#include "lexicographical.h"
#include "ordering.h"
#include "position.h"
#include "swap.h"

namespace elements {

template <Movable T>
struct list_node_singly_linked
{
    T x;
    Pointer_type<list_node_singly_linked<T>> pos_next = nullptr;

    list_node_singly_linked() = default;

    explicit
    list_node_singly_linked(T const& x_, Pointer_type<list_node_singly_linked<T>> pos_next_ = nullptr)
        : x(x_)
        , pos_next(pos_next_)
    {}

    explicit
    list_node_singly_linked(T&& x_, Pointer_type<list_node_singly_linked<T>> pos_next_ = nullptr)
        : x(fw<T>(x_))
        , pos_next(pos_next_)
    {}
};

template <Movable T>
struct list_singly_linked_position
{
    Pointer_type<list_node_singly_linked<T>> pos = nullptr;

    list_singly_linked_position() = default;

    explicit
    list_singly_linked_position(Pointer_type<list_node_singly_linked<T>> pos_)
        : pos{pos_}
    {}
};

template <Movable T>
struct value_type_t<list_singly_linked_position<T>>
{
    using type = T;
};

template <Movable T>
struct difference_type_t<list_singly_linked_position<T>>
{
    using type = Difference_type<Pointer_type<list_node_singly_linked<T>>>;
};

template <Movable T>
bool operator==(list_singly_linked_position<T> const& x, list_singly_linked_position<T> const& y)
{
    return x.pos == y.pos;
}

template <Movable T>
constexpr void
increment(list_singly_linked_position<T>& x)
{
    x.pos = load(x.pos).pos_next;
}

template <Movable T>
constexpr auto
load(list_singly_linked_position<T> x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr void
store(list_singly_linked_position<T>& x, T const& value)
{
    store(x.pos, value);
}

template <Movable T>
constexpr void
store(list_singly_linked_position<T>& x, T&& value)
{
    store(x.pos, fw<T>(value));
}

template <Movable T>
constexpr auto
at(list_singly_linked_position<T> const& x) -> T const&
{
    return load(x.pos).x;
}

template <Movable T>
constexpr auto
at(list_singly_linked_position<T>& x) -> T&
{
    return at(x.pos).x;
}

template <Movable T>
constexpr auto
next_link(list_singly_linked_position<T>& x) -> Pointer_type<list_node_singly_linked<T>>&
{
    return at(x.pos).pos_next;
}

template <Movable T>
constexpr auto
precedes(list_singly_linked_position<T> const& x, list_singly_linked_position<T> const& y) -> bool
{
    return x.pos != y.pos;
}

template <Movable T>
struct list_singly_linked_basic
{
    list_singly_linked_position<T> header;

    list_singly_linked_basic() = default;

    list_singly_linked_basic(list_singly_linked_basic const& x)
    {
        auto src = first(x);
        auto dst = pointer_to(at(this).header.pos);
        while (src != limit(x)) {
            at(dst) = new list_node_singly_linked<T>(load(src));
            dst = pointer_to(at(at(dst)).pos_next);
            increment(src);
        }
    }

    list_singly_linked_basic(list_singly_linked_basic&& x)
    {
        header = x.header;
        x.header = list_singly_linked_position<T>{};
    }

    list_singly_linked_basic(std::initializer_list<T> x)
    {
        auto src = std::cbegin(x);
        auto dst = pointer_to(at(this).header.pos);
        while (src != std::cend(x)) {
            at(dst) = new list_node_singly_linked<T>(*src);
            dst = pointer_to(at(at(dst)).pos_next);
            increment(src);
        }
    }

    list_singly_linked_basic(Size_type<list_singly_linked_basic<T>> size, T const& x)
    {
        while (!is_zero(size)) {
            push_first(at(this), x);
            decrement(size);
        }
    }

    auto
    operator=(list_singly_linked_basic const& x) -> list_singly_linked_basic&
    {
        list_singly_linked_basic temp(x);
        swap(at(this), temp);
        return at(this);
    }

    auto
    operator=(list_singly_linked_basic&& x) -> list_singly_linked_basic&
    {
        if (this != pointer_to(x)) {
            erase_all(at(this));
            swap(header, x.header);
        }
        return at(this);
    }

    ~list_singly_linked_basic()
    {
        erase_all(at(this));
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_position<T>> i) -> T&
    {
        auto pos = header + i;
        return at(pos);
    }

    constexpr auto
    operator[](Difference_type<list_singly_linked_position<T>> i) const -> T const&
    {
        auto pos = header + i;
        return load(pos);
    }
};

template <Movable T>
struct value_type_t<list_singly_linked_basic<T>>
{
    using type = T;
};

template <Movable T>
struct position_type_t<list_singly_linked_basic<T>>
{
    using type = list_singly_linked_position<T>;
};

template <Movable T>
struct position_type_t<list_singly_linked_basic<T const>>
{
    using type = list_singly_linked_position<T const>;
};

template <Movable T>
struct size_type_t<list_singly_linked_basic<T>>
{
    using type = pointer_diff;
};

template <Regular T>
constexpr auto
operator==(list_singly_linked_basic<T> const& x, list_singly_linked_basic<T> const& y) -> bool
{
    return equal_lexicographical(first(x), limit(x), first(y), limit(y));
}

template <Default_totally_ordered T>
constexpr auto
operator<(list_singly_linked_basic<T> const& x, list_singly_linked_basic<T> const& y) -> bool
{
    return less_lexicographical(first(x), limit(x), first(y), limit(y));
}

template <Movable T>
constexpr void
swap(list_singly_linked_basic<T>& x, list_singly_linked_basic<T>& y)
{
    swap(x.header, y.header);
}

template <Movable T, typename U>
constexpr auto
insert(front<list_singly_linked_basic<T>> list, U&& x) -> front<list_singly_linked_basic<T>>
{
    auto& seq = at(list.seq);
    seq.header = list_singly_linked_position<T>{
        new list_node_singly_linked<T>(fw<U>(x), first(seq).pos)
    };
    return seq;
}

template <Movable T, Constructible_from<T> U>
constexpr auto
insert(after<list_singly_linked_basic<T>> pos, U&& x) -> after<list_singly_linked_basic<T>>
{
    list_singly_linked_position<T> node(allocate_raw<list_node_singly_linked<T>>());
    construct(at(node), fw<U>(x));
    if (precedes(current(pos), limit(pos))) {
        set_link_forward(node, successor(current(pos)));
        set_link_forward(current(pos), node);
    } else {
        set_link_forward(node, first(pos));
        base(pos).header = node;
    }
    return after<list_singly_linked_basic<T>>{base(pos), node};
}

template <Movable T, Constructible_from<T> U>
constexpr void
emplace_first(list_singly_linked_basic<T>& list, U&& x)
{
    insert(front<list_singly_linked_basic<T>>(list), fw<U>(x));
}

template <Movable T, Constructible_from<T> U>
constexpr void
push_first(list_singly_linked_basic<T>& list, U x)
{
    insert(front<list_singly_linked_basic<T>>(list), mv(x));
}

template <Movable T>
constexpr auto
erase(front<list_singly_linked_basic<T>> list) -> front<list_singly_linked_basic<T>>
{
    auto& seq = at(list.seq);
    auto pos = at(seq.header.pos).pos_next;
    delete seq.header.pos;
    seq.header = list_singly_linked_position<T>{pos};
    return seq;
}

template <Movable T>
constexpr auto
erase(after<list_singly_linked_basic<T>> list) -> list_singly_linked_position<T>
{
    auto& cur_node = at(list.pos.pos);
    auto& rem_node = at(cur_node.pos_next);
    cur_node.pos_next = rem_node.pos_next;
    destroy(rem_node);
    free_raw(rem_node);
    return list_singly_linked_position<T>{cur_node.pos_next};
}

template <Movable T>
constexpr void
erase_all(list_singly_linked_basic<T>& x)
{
    while (!is_empty(x)) erase(front<list_singly_linked_basic<T>>(x));
}

template <Movable T>
constexpr void
pop_first(list_singly_linked_basic<T>& arr)
{
    erase(front<list_singly_linked_basic<T>>(arr));
}

template <Movable T>
constexpr auto
first(list_singly_linked_basic<T> const& x) -> Position_type<list_singly_linked_basic<T>>
{
    return x.header;
}

template <Movable T>
constexpr auto
limit(list_singly_linked_basic<T> const&) -> Position_type<list_singly_linked_basic<T>>
{
    return {};
}

template <Movable T>
constexpr auto
is_empty(list_singly_linked_basic<T> const& x) -> bool
{
    return first(x) == limit(x);
}

template <Movable T>
constexpr auto
size(list_singly_linked_basic<T> const& x) -> Size_type<list_singly_linked_basic<T>>
{
    return limit(x) - first(x);
}

}
