#pragma once

namespace elements {

struct empty_t {};

constexpr auto
operator==(empty_t, empty_t) -> bool
{
    return true;
}

constexpr auto
operator<(empty_t, empty_t) -> bool
{
    return false;
}

static_assert(Totally_ordered<empty_t>);

template <typename T>
struct tree_node_singly_linked
{
    [[no_unique_address]] T value;
    Pointer_type<tree_node_singly_linked<T>> left{};
    Pointer_type<tree_node_singly_linked<T>> right{};

    constexpr
    tree_node_singly_linked() = default;

    explicit constexpr
    tree_node_singly_linked(
        T value_,
        Pointer_type<tree_node_singly_linked<T>> left_ = {},
        Pointer_type<tree_node_singly_linked<T>> right_ = {})
        : value{value_}
        , left{left_}
        , right{right_}
    {}
};

template <typename T>
struct value_type_t<tree_node_singly_linked<T>>
{
    using type = T;
};

template <typename T>
constexpr auto
operator==(tree_node_singly_linked<T> const& x, tree_node_singly_linked<T> const& y) -> bool
{
    return x.value == y.value and x.left == y.left and x.right == y.right;
}

template <typename T>
struct tree_singly_linked_bicursor
{
    Pointer_type<tree_node_singly_linked<T>> cur{};
};

template <typename T>
struct value_type_t<tree_singly_linked_bicursor<T>>
{
    using type = T;
};

template <typename T>
struct weight_type_t<tree_singly_linked_bicursor<T>>
{
    using type = pointer_diff;
};

template <typename T>
constexpr auto
operator==(tree_singly_linked_bicursor<T> const& x, tree_singly_linked_bicursor<T> const& y) -> bool
{
    return x.cur == y.cur;
}

template <typename T>
constexpr auto
operator<(tree_singly_linked_bicursor<T> const& x, tree_singly_linked_bicursor<T> const& y) -> bool
{
    return x.cur < y.cur;
}

template <typename T>
constexpr auto
is_empty(tree_singly_linked_bicursor<T> cur) -> bool
{
    return cur.cur == nullptr;
}

template <typename T>
constexpr auto
has_left_successor(tree_singly_linked_bicursor<T> cur) -> bool
{
    return load(cur.cur).left != nullptr;
}

template <typename T>
constexpr auto
has_right_successor(tree_singly_linked_bicursor<T> cur) -> bool
{
    return load(cur.cur).right != nullptr;
}

template <typename T>
constexpr void
increment_left(tree_singly_linked_bicursor<T>& cur)
{
    cur.cur = load(cur.cur).left;
}

template <typename T>
constexpr void
increment_right(tree_singly_linked_bicursor<T>& cur)
{
    cur.cur = load(cur.cur).right;
}

template <typename T>
constexpr void
set_left_successor(tree_singly_linked_bicursor<T> cur, tree_singly_linked_bicursor<T> succ)
{
    at(cur.cur).left = succ.cur;
}

template <typename T>
constexpr void
set_right_successor(tree_singly_linked_bicursor<T> cur, tree_singly_linked_bicursor<T> succ)
{
    at(cur.cur).right = succ.cur;
}

template <typename T>
constexpr auto
load(tree_singly_linked_bicursor<T> cur) -> int const&
{
    return load(cur.cur).value;
}

template<typename T>
    requires(Regular(T))
struct tree_singly_linked
{
    typedef stree_coordinate<T> C;
    typedef stree_node_construct<T> Cons;
    C root;
    stree() : root(0) { }
    stree(T x) : root(Cons()(x)) { }
    stree(T x, const stree& left, const stree& right) : root(Cons()(x))
    {
        set_left_successor(root, bifurcate_copy<C, Cons>(left.root));
        set_right_successor(root, bifurcate_copy<C, Cons>(right.root));
    }
    stree(const stree& x) : root(bifurcate_copy<C, Cons>(x.root)) { }
    ~stree() { bifurcate_erase(root, stree_node_destroy<T>()); }
    void operator=(stree x) { swap(root, x.root); }
};

}
