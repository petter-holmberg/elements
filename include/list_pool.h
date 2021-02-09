#pragma once

#include "array_single_ended.h"
#include "integer.h"

namespace elements {

template <typename T, Integral N>
struct list_pool_node
{
    T value;
    N next;
};

template <typename T, Integral N>
struct value_type_t<list_pool_node<T, N>>
{
    using type = T;
};

template <typename T, Integral N>
constexpr auto
operator==(list_pool_node<T, N> const& x, list_pool_node<T, N> const& y) -> bool
{
    return x.value == y.value && x.next == y.next;
}

template <typename T, Integral N>
constexpr auto
operator<(list_pool_node<T, N> const& x, list_pool_node<T, N> const& y) -> bool
{
    return x.value < y.value or (!(y.value < x.value) and x.next < y.next);
}

template <typename T, Integral N = Size_type<array_single_ended<T>>>
struct list_pool
{
    array_single_ended<list_pool_node<T, N>> pool;
    N free_list;

    constexpr auto
    limit() const -> N
    {
        return N(0);
    }

    constexpr
    list_pool()
        : free_list(limit())
    {}

    explicit
    list_pool(Size_type<array_single_ended<list_pool_node<T, N>>> capacity)
        : pool(capacity)
        , free_list(limit())
    {}

    constexpr auto
    operator[](N x) -> list_pool_node<T, N>&
    {
        return pool[x - 1];
    }

    constexpr auto
    operator[](N x) const -> list_pool_node<T, N> const&
    {
        return pool[x - 1];
    }

    constexpr auto
    new_node() -> N
    {
        emplace(pool, list_pool_node<T, N>{});
        return N(size(pool));
    }

    constexpr
    operator bool() const
    {
        return pool;
    }
};

template <typename T, Integral N>
constexpr auto
operator==(list_pool<T, N> const& x, list_pool<T, N> const& y) -> bool
{
    return x.pool == y.pool;
}

template <typename T, Integral N>
constexpr auto
operator<(list_pool<T, N> const& x, list_pool<T, N> const& y) -> bool
{
    return x.pool < y.pool;
}

template <typename T, Integral N>
struct value_type_t<list_pool<T, N>>
{
    using type = T;
};

template <typename T, Integral N>
struct list_pool_cursor;

template <typename T, Integral N>
struct cursor_type_t<list_pool<T, N>>
{
    using type = list_pool_cursor<T, N>;
};

template <typename T, Integral N>
struct cursor_type_t<list_pool<T const, N const>>
{
    using type = list_pool_cursor<T const, N const>;
};

template <typename T, Integral N>
struct size_type_t<list_pool<T, N>>
{
    using type = Size_type<array_single_ended<T>>;
};

template <typename T>
struct node_type_t;

template <typename T, Integral N>
struct node_type_t<list_pool<T, N>>
{
    using type = list_pool_node<T, N>;
};

template <typename T>
using Node_type = typename node_type_t<T>::type;

template <typename T, Integral N = Size_type<array_single_ended<T>>>
struct list_pool_cursor
{
    Pointer_type<list_pool<T, N>> pool;
    N node;

    constexpr
    list_pool_cursor() = default;

    explicit constexpr
    list_pool_cursor(list_pool<T, N>& p)
        : pool{pointer_to(p)}
        , node{p.limit()}
    {}

    constexpr
    list_pool_cursor(list_pool<T, N>& p, N n)
        : pool{pointer_to(p)}
        , node{n}
    {}
};

template <typename T, Integral N>
struct value_type_t<list_pool_cursor<T, N>>
{
    using type = T;
};

template <typename T, Integral N>
struct difference_type_t<list_pool_cursor<T, N>>
{
    using type = Difference_type<Pointer_type<list_pool_cursor<T, N>>>;
};

template <typename T, Integral N>
constexpr auto
operator==(list_pool_cursor<T, N> const& cur0, list_pool_cursor<T, N> const& cur1) -> bool
{
    return cur0.node == cur1.node;
}

template <typename T, Integral N>
constexpr void
increment(list_pool_cursor<T, N>& cur)
{
    cur.node = next(load(cur.pool), cur.node);
}

template <typename T, Integral N>
constexpr auto
load(list_pool_cursor<T, N> cur) -> T const&
{
    return value(load(cur.pool), cur.node);
}

template <typename T, Integral N>
constexpr auto
next_link(list_pool_cursor<T, N>& cur) -> N&
{
    return next(at(cur.pool), cur.node);
}

template <typename T, Integral N>
constexpr auto
precedes(list_pool_cursor<T, N> const& cur0, list_pool_cursor<T, N> const& cur1) -> bool
{
    return cur0 != cur1;
}

template <typename T, Integral N>
constexpr auto
is_limit(list_pool<T, N> const& x, N pos) -> bool
{
    return pos == x.limit();
}

template <typename T, Integral N>
constexpr auto
is_empty(list_pool<T, N> const& x) -> bool
{
    return is_empty(x.pool);
}

template <typename T, Integral N>
constexpr auto
size(list_pool<T, N> const& x) -> Size_type<list_pool<T, N>>
{
    return size(x.pool);
}

template <typename T, Integral N>
constexpr auto
capacity(list_pool<T, N> const& x) -> Size_type<list_pool<T, N>>
{
    return capacity(x.pool);
}

template <typename T, Integral N>
constexpr void
reserve(list_pool<T, N>& x, Size_type<list_pool<T, N>> capacity)
{
    x.pool.reserve(capacity);
}

template <typename T, Integral N>
constexpr auto
value(list_pool<T, N>& x, N n) -> T&
{
    return x[n].value;
}

template <typename T, Integral N>
constexpr auto
value(list_pool<T, N> const& x, N n) -> T const&
{
    return x[n].value;
}

template <typename T, Integral N>
constexpr auto
next(list_pool<T, N>& x, N n) -> N&
{
    return x[n].next;
}

template <typename T, Integral N>
constexpr auto
next(list_pool<T, N> const& x, N n) -> N const&
{
    return x[n].next;
}

template <typename T, Integral N>
constexpr auto
allocate(list_pool<T, N>& pool, T const& x, N tail) -> N
{
    N list = pool.free_list;
    if (is_limit(pool, pool.free_list)) {
        list = pool.new_node();
    } else {
        pool.free_list = next(pool, pool.free_list);
    }
    value(pool, list) = x;
    next(pool, list) = tail;
    return list;
}

template <typename T, Integral N>
constexpr auto
free(list_pool<T, N>& pool, N n) -> N
{
    N list = next(pool, n);
    next(pool, n) = pool.free_list;
    pool.free_list = n;
    return list;
}

template <typename T, std::integral N>
void free_pool(list_pool<T, N>& pool, N n)
{
    while (!is_limit(pool, n)) n = free(pool, n);
}

}
