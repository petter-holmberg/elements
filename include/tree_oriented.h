#pragma once

#include "swap.h"
#include "tree.h"

namespace elements {

template <typename T>
struct tree_oriented_node
{
    [[no_unique_address]] T value;
    Pointer_type<tree_oriented_node<T>> left{};
    Pointer_type<tree_oriented_node<T>> right{};

    constexpr
    tree_oriented_node() = default;

    explicit constexpr
    tree_oriented_node(
        T value_,
        Pointer_type<tree_oriented_node<T>> left_ = {},
        Pointer_type<tree_oriented_node<T>> right_ = {})
        : value{value_}
        , left{left_}
        , right{right_}
    {}
};

template <typename T>
struct value_type_t<tree_oriented_node<T>>
{
    using type = T;
};

template <typename T>
constexpr auto
operator==(tree_oriented_node<T> const& x, tree_oriented_node<T> const& y) -> bool
{
    return x.value == y.value and x.left == y.left and x.right == y.right;
}

template <typename T>
struct tree_oriented_bicursor
{
    Pointer_type<tree_oriented_node<T>> cur{};
};

template <typename T>
struct value_type_t<tree_oriented_bicursor<T>>
{
    using type = T;
};

template <typename T>
struct weight_type_t<tree_oriented_bicursor<T>>
{
    using type = pointer_diff;
};

template <typename T>
constexpr auto
operator==(tree_oriented_bicursor<T> const& x, tree_oriented_bicursor<T> const& y) -> bool
{
    return x.cur == y.cur;
}

template <typename T>
constexpr auto
operator<(tree_oriented_bicursor<T> const& x, tree_oriented_bicursor<T> const& y) -> bool
{
    return x.cur < y.cur;
}

template <typename T>
constexpr auto
is_empty(tree_oriented_bicursor<T> cur) -> bool
{
    return cur.cur == nullptr;
}

template <typename T>
constexpr auto
has_left_successor(tree_oriented_bicursor<T> cur) -> bool
{
    return load(cur.cur).left != nullptr;
}

template <typename T>
constexpr auto
has_right_successor(tree_oriented_bicursor<T> cur) -> bool
{
    return load(cur.cur).right != nullptr;
}

template <typename T>
constexpr void
increment_left(tree_oriented_bicursor<T>& cur)
{
    cur.cur = load(cur.cur).left;
}

template <typename T>
constexpr void
increment_right(tree_oriented_bicursor<T>& cur)
{
    cur.cur = load(cur.cur).right;
}

template <typename T>
constexpr void
set_left_successor(tree_oriented_bicursor<T> cur, tree_oriented_bicursor<T> succ)
{
    at(cur.cur).left = succ.cur;
}

template <typename T>
constexpr void
set_right_successor(tree_oriented_bicursor<T> cur, tree_oriented_bicursor<T> succ)
{
    at(cur.cur).right = succ.cur;
}

template <typename T>
constexpr auto
load(tree_oriented_bicursor<T> cur) -> int const&
{
    return load(cur.cur).value;
}

template <typename T>
struct tree_oriented_node_construct
{
    tree_oriented_node_construct() = default;

    constexpr auto
    operator()(T const& x, tree_oriented_bicursor<T> l = {}, tree_oriented_bicursor<T> r = {}) -> tree_oriented_bicursor<T>
    {
        return {new tree_oriented_node{x, l.cur, r.cur}};
    }

    constexpr auto
    operator()(tree_oriented_bicursor<T> cur) -> tree_oriented_bicursor<T>
    {
        return invoke(*this, load(cur), left_successor(cur), right_successor(cur));
    }

    constexpr auto
    operator()(tree_oriented_bicursor<T> cur, tree_oriented_bicursor<T> left, tree_oriented_bicursor<T> right) -> tree_oriented_bicursor<T>
    {
        return invoke(*this, load(cur), left, right);
    }
};

template <typename T>
struct tree_oriented_node_destroy
{
    tree_oriented_node_destroy() = default;

    constexpr void
    operator()(tree_oriented_bicursor<T> cur)
    {
        delete cur.cur;
    }
};

template <typename T, typename Cons = tree_oriented_node_construct<T>, typename Des = tree_oriented_node_destroy<T>>
struct tree_oriented
{
    tree_oriented_bicursor<T> root{};

    constexpr
    tree_oriented() = default;

    constexpr
    tree_oriented(tree_oriented const& x)
        : root{tree_copy<tree_oriented_bicursor<T>, Cons>(x.root)}
    {}

    constexpr
    tree_oriented(tree_oriented&& x)
    {
        root = x.root;
        x.root = {};
    }

    explicit constexpr
    tree_oriented(T const& x)
        : root{x}
    {}

    constexpr
    tree_oriented(T const& x, tree_oriented const& left, tree_oriented const& right)
        : root{Cons{}(x)}
    {
        set_left_successor(root, tree_copy<tree_oriented_bicursor<T>, Cons>(left.root));
        set_right_successor(root, tree_copy<tree_oriented_bicursor<T>, Cons>(right.root));
    }

    constexpr auto
    operator=(tree_oriented const& x) -> tree_oriented&
    {
        using elements::swap;
        tree_oriented temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(tree_oriented&& x) -> tree_oriented&
    {
        using elements::swap;
        if (this != pointer_to(x)) {
            tree_erase(root, Des{});
            swap(root, x.root);
        }
        return at(this);
    }

    constexpr
    ~tree_oriented()
    {
        tree_erase(root, Des{});
    }
};

template <typename T>
struct value_type_t<tree_oriented<T>>
{
    using type = T;
};

template <typename T>
struct cursor_type_t<tree_oriented<T>>
{
    using type = tree_oriented_bicursor<T>;
};

template <typename T>
struct cursor_type_t<tree_oriented<T const>>
{
    using type = tree_oriented_bicursor<T const>;
};

template <typename T>
struct weight_type_t<tree_oriented<T>>
{
    using type = pointer_diff;
};

template <typename T>
constexpr auto
first(tree_oriented<T> const& x) -> Cursor_type<tree_oriented<T>>
{
    return x.root;
}

template <typename T>
constexpr auto
is_empty(tree_oriented<T> const& x) -> bool
{
    return is_empty(x.root);
}

template <typename T>
bool operator==(const tree_oriented<T>& x, const tree_oriented<T>& y)
{
    if (is_empty(x)) return is_empty(y);
    if (is_empty(y)) return false;
    return tree_equal(first(x), first(y));
}

template <typename T>
constexpr auto
operator<(tree_oriented<T> const& x, tree_oriented<T> const& y) -> bool
{
    if (is_empty(x)) return !is_empty(y);
    if (is_empty(y)) return false;
    return tree_less(first(x), first(y));
}

template <typename T>
constexpr void
swap(tree_oriented<T>& x, tree_oriented<T>& y)
{
    swap(x.root, y.root);
}

template <typename T>
constexpr auto
height(tree_oriented<T> const& x) -> Weight_type<tree_oriented<T>>
{
    return tree_height(first(x));
}

template <typename T>
constexpr auto
weight(tree_oriented<T> const& x) -> Weight_type<tree_oriented<T>>
{
    return tree_weight(first(x));
}

template <typename T, Invocable<df_visit, Cursor_type<tree_oriented<T>>> Proc>
constexpr auto
traverse(tree_oriented<T>& x, Proc proc) -> Proc
{
    return tree_traverse(first(x), proc);
}

template <typename T, Invocable<df_visit, Cursor_type<tree_oriented<T const>>> Proc>
constexpr auto
traverse(tree_oriented<T> const& x, Proc proc) -> Proc
{
    return tree_traverse(first(x), proc);
}

template <typename T, Invocable<Cursor_type<tree_oriented<T>>> Proc>
constexpr auto
traverse_rotating(tree_oriented<T>& x, Proc proc) -> T
{
    return traverse_phased_rotating(first(x), 0, proc);
}

}
