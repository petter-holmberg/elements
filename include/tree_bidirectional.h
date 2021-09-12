#pragma once

#include "swap.h"
#include "tree.h"

namespace elements {

template <typename T>
struct tree_bidirectional_node
{
    [[no_unique_address]] T value;
    Pointer_type<tree_bidirectional_node<T>> left{};
    Pointer_type<tree_bidirectional_node<T>> right{};
    Pointer_type<tree_bidirectional_node<T>> prev{};

    constexpr
    tree_bidirectional_node() = default;

    explicit constexpr
    tree_bidirectional_node(
        T value_,
        Pointer_type<tree_bidirectional_node<T>> left_ = {},
        Pointer_type<tree_bidirectional_node<T>> right_ = {},
        Pointer_type<tree_bidirectional_node<T>> prev_ = {})
        : value{value_}
        , left{left_}
        , right{right_}
        , prev{prev_}
    {}
};

template <typename T>
struct value_type_t<tree_bidirectional_node<T>>
{
    using type = T;
};

template <typename T>
constexpr auto
operator==(tree_bidirectional_node<T> const& x, tree_bidirectional_node<T> const& y) -> bool
{
    return x.value == y.value and x.left == y.left and x.right == y.right and x.prev == y.prev;
}

template <typename T>
struct tree_bidirectional_bicursor
{
    Pointer_type<tree_bidirectional_node<T>> cur{};
};

template <typename T>
struct value_type_t<tree_bidirectional_bicursor<T>>
{
    using type = T;
};

template <typename T>
struct weight_type_t<tree_bidirectional_bicursor<T>>
{
    using type = pointer_diff;
};

template <typename T>
constexpr auto
operator==(tree_bidirectional_bicursor<T> const& x, tree_bidirectional_bicursor<T> const& y) -> bool
{
    return x.cur == y.cur;
}

template <typename T>
constexpr auto
operator<(tree_bidirectional_bicursor<T> const& x, tree_bidirectional_bicursor<T> const& y) -> bool
{
    return x.cur < y.cur;
}

template <typename T>
constexpr auto
is_empty(tree_bidirectional_bicursor<T> cur) -> bool
{
    return cur.cur == nullptr;
}

template <typename T>
constexpr auto
has_left_successor(tree_bidirectional_bicursor<T> cur) -> bool
{
    return load(cur.cur).left != nullptr;
}

template <typename T>
constexpr auto
has_right_successor(tree_bidirectional_bicursor<T> cur) -> bool
{
    return load(cur.cur).right != nullptr;
}

template <typename T>
constexpr auto
has_predecessor(tree_bidirectional_bicursor<T> cur) -> bool
{
    return load(cur.cur).prev != nullptr;
}

template <typename T>
constexpr void
increment_left(tree_bidirectional_bicursor<T>& cur)
{
    cur.cur = load(cur.cur).left;
}

template <typename T>
constexpr void
increment_right(tree_bidirectional_bicursor<T>& cur)
{
    cur.cur = load(cur.cur).right;
}

template <typename T>
constexpr void
decrement(tree_bidirectional_bicursor<T>& cur)
{
    cur.cur = load(cur.cur).prev;
}

template <typename T>
constexpr void
set_left_successor(tree_bidirectional_bicursor<T> cur, tree_bidirectional_bicursor<T> succ)
{
    at(cur.cur).left = succ.cur;
    if (!is_empty(succ)) set_predecessor(succ, cur);
}

template <typename T>
constexpr void
set_right_successor(tree_bidirectional_bicursor<T> cur, tree_bidirectional_bicursor<T> succ)
{
    at(cur.cur).right = succ.cur;
    if (!is_empty(succ)) set_predecessor(succ, cur);
}

template <typename T>
constexpr void
set_predecessor(tree_bidirectional_bicursor<T> cur, tree_bidirectional_bicursor<T> pred)
{
    at(cur.cur).prev = pred.cur;
}

template <typename T>
constexpr auto
load(tree_bidirectional_bicursor<T> cur) -> int const&
{
    return load(cur.cur).value;
}

template <typename T>
struct tree_bidirectional_node_construct
{
    tree_bidirectional_node_construct() = default;

    constexpr auto
    operator()(T const& x, tree_bidirectional_bicursor<T> l = {}, tree_bidirectional_bicursor<T> r = {}) -> tree_bidirectional_bicursor<T>
    {
        return {new tree_bidirectional_node{x, l.cur, r.cur}};
    }

    constexpr auto
    operator()(tree_bidirectional_bicursor<T> cur) -> tree_bidirectional_bicursor<T>
    {
        return invoke(*this, load(cur), left_successor(cur), right_successor(cur));
    }

    constexpr auto
    operator()(tree_bidirectional_bicursor<T> cur, tree_bidirectional_bicursor<T> left, tree_bidirectional_bicursor<T> right) -> tree_bidirectional_bicursor<T>
    {
        return invoke(*this, load(cur), left, right);
    }
};

template <typename T>
struct tree_bidirectional_node_destroy
{
    tree_bidirectional_node_destroy() = default;

    constexpr void
    operator()(tree_bidirectional_bicursor<T> cur)
    {
        delete cur.cur;
    }
};

template <typename T, typename Cons = tree_bidirectional_node_construct<T>, typename Des = tree_bidirectional_node_destroy<T>>
struct tree_bidirectional
{
    tree_bidirectional_bicursor<T> root{};

    constexpr
    tree_bidirectional() = default;

    constexpr
    tree_bidirectional(tree_bidirectional const& x)
        : root{tree_copy<tree_bidirectional_bicursor<T>, Cons>(x.root)}
    {}

    constexpr
    tree_bidirectional(tree_bidirectional&& x)
    {
        root = x.root;
        x.root = {};
    }

    explicit constexpr
    tree_bidirectional(T const& x)
        : root{x}
    {}

    constexpr
    tree_bidirectional(T const& x, tree_bidirectional const& left, tree_bidirectional const& right)
        : root{Cons{}(x)}
    {
        set_left_successor(root, tree_copy<tree_bidirectional_bicursor<T>, Cons>(left.root));
        set_right_successor(root, tree_copy<tree_bidirectional_bicursor<T>, Cons>(right.root));
    }

    constexpr auto
    operator=(tree_bidirectional const& x) -> tree_bidirectional&
    {
        using elements::swap;
        tree_bidirectional temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(tree_bidirectional&& x) -> tree_bidirectional&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            tree_erase(root, Des{});
            swap(root, x.root);
        }
        return at(this);
    }

    constexpr
    ~tree_bidirectional()
    {
        tree_erase(root, Des{});
    }
};

template <typename T>
struct value_type_t<tree_bidirectional<T>>
{
    using type = T;
};

template <typename T>
struct cursor_type_t<tree_bidirectional<T>>
{
    using type = tree_bidirectional_bicursor<T>;
};

template <typename T>
struct cursor_type_t<tree_bidirectional<T const>>
{
    using type = tree_bidirectional_bicursor<T const>;
};

template <typename T>
struct weight_type_t<tree_bidirectional<T>>
{
    using type = pointer_diff;
};

template <typename T>
constexpr auto
first(tree_bidirectional<T> const& x) -> Cursor_type<tree_bidirectional<T>>
{
    return x.root;
}

template <typename T>
constexpr auto
is_empty(tree_bidirectional<T> const& x) -> bool
{
    return is_empty(x.root);
}

template <typename T>
bool operator==(const tree_bidirectional<T>& x, const tree_bidirectional<T>& y)
{
    if (is_empty(x)) return is_empty(y);
    if (is_empty(y)) return false;
    return tree_equal(first(x), first(y));
}

template <typename T>
constexpr auto
operator<(tree_bidirectional<T> const& x, tree_bidirectional<T> const& y) -> bool
{
    if (is_empty(x)) return !is_empty(y);
    if (is_empty(y)) return false;
    return tree_less(first(x), first(y));
}

template <typename T>
constexpr void
swap(tree_bidirectional<T>& x, tree_bidirectional<T>& y)
{
    swap(x.root, y.root);
}

template <typename T>
constexpr auto
height(tree_bidirectional<T> const& x) -> Weight_type<tree_bidirectional<T>>
{
    return tree_height(first(x));
}

template <typename T>
constexpr auto
weight(tree_bidirectional<T> const& x) -> Weight_type<tree_bidirectional<T>>
{
    return tree_weight(first(x));
}

template <typename T, Invocable<df_visit, Cursor_type<tree_bidirectional<T>>> Proc>
constexpr auto
traverse(tree_bidirectional<T>& x, Proc proc) -> Proc
{
    return tree_traverse(first(x), proc);
}

template <typename T, Invocable<df_visit, Cursor_type<tree_bidirectional<T const>>> Proc>
constexpr auto
traverse(tree_bidirectional<T> const& x, Proc proc) -> Proc
{
    return tree_traverse(first(x), proc);
}

template <typename T, Invocable<Cursor_type<tree_bidirectional<T>>> Proc>
constexpr auto
traverse_rotating(tree_bidirectional<T>& x, Proc proc) -> T
{
    return traverse_phased_rotating(first(x), 0, proc);
}

}
