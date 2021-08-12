#include "catch.hpp"

#include "graph.h"
#include "tree.h"

namespace e = elements;

namespace elements {

struct bifurcate_forward_node
{
    e::Pointer_type<bifurcate_forward_node> left = nullptr;
    e::Pointer_type<bifurcate_forward_node> right = nullptr;
    int value = 0;
};

template <>
struct value_type_t<bifurcate_forward_node>
{
    using type = int;
};

constexpr auto
operator==(bifurcate_forward_node const& x, bifurcate_forward_node const& y) -> bool
{
    return x.left == y.left and x.right == y.right;
}

constexpr auto
operator!=(bifurcate_forward_node const& x, bifurcate_forward_node const& y) -> bool
{
    return !(x == y);
}

static_assert(std::regular<bifurcate_forward_node>);

struct forward_bicursor
{
    e::Pointer_type<bifurcate_forward_node> cur{nullptr};
};

template <>
struct value_type_t<forward_bicursor>
{
    using type = int;
};

template <>
struct weight_type_t<forward_bicursor>
{
    using type = pointer_diff;
};

constexpr auto
operator==(forward_bicursor const& x, forward_bicursor const& y) -> bool
{
    return x.cur == y.cur;
}

constexpr auto
operator!=(forward_bicursor const& x, forward_bicursor const& y) -> bool
{
    return !(x == y);
}

constexpr auto
is_empty(forward_bicursor cur) -> bool
{
    return cur.cur == nullptr;
}

constexpr auto
has_left_successor(forward_bicursor cur) -> bool
{
    return load(cur.cur).left != nullptr;
}

constexpr auto
has_right_successor(forward_bicursor cur) -> bool
{
    return load(cur.cur).right != nullptr;
}

constexpr void
increment_left(forward_bicursor& cur)
{
    cur.cur = load(cur.cur).left;
}

constexpr void
increment_right(forward_bicursor& cur)
{
    cur.cur = load(cur.cur).right;
}

constexpr auto
load(forward_bicursor cur) -> int const&
{
    return load(cur.cur).value;
}

static_assert(e::Bicursor<forward_bicursor>);

struct linked_forward_bicursor
{
    e::Pointer_type<bifurcate_forward_node> cur{nullptr};
};

template <>
struct value_type_t<linked_forward_bicursor>
{
    using type = int;
};

template <>
struct weight_type_t<linked_forward_bicursor>
{
    using type = pointer_diff;
};

constexpr auto
operator==(linked_forward_bicursor const& x, linked_forward_bicursor const& y) -> bool
{
    return x.cur == y.cur;
}

constexpr auto
operator!=(linked_forward_bicursor const& x, linked_forward_bicursor const& y) -> bool
{
    return !(x == y);
}

constexpr auto
is_empty(linked_forward_bicursor cur) -> bool
{
    return cur.cur == nullptr;
}

constexpr auto
has_left_successor(linked_forward_bicursor cur) -> bool
{
    return load(cur.cur).left != nullptr;
}

constexpr auto
has_right_successor(linked_forward_bicursor cur) -> bool
{
    return load(cur.cur).right != nullptr;
}

constexpr void
increment_left(linked_forward_bicursor& cur)
{
    cur.cur = load(cur.cur).left;
}

constexpr void
increment_right(linked_forward_bicursor& cur)
{
    cur.cur = load(cur.cur).right;
}

constexpr void
set_left_successor(linked_forward_bicursor cur, linked_forward_bicursor succ)
{
    at(cur.cur).left = succ.cur;
}

constexpr void
set_right_successor(linked_forward_bicursor cur, linked_forward_bicursor succ)
{
    at(cur.cur).right = succ.cur;
}

constexpr auto
load(linked_forward_bicursor cur) -> int const&
{
    return load(cur.cur).value;
}

static_assert(e::Linked_bicursor<linked_forward_bicursor>);

struct bifurcate_bidirectional_node
{
    e::Pointer_type<bifurcate_bidirectional_node> left = nullptr;
    e::Pointer_type<bifurcate_bidirectional_node> right = nullptr;
    e::Pointer_type<bifurcate_bidirectional_node> back = nullptr;
    int value = 0;
};

template <>
struct value_type_t<bifurcate_bidirectional_node>
{
    using type = int;
};

constexpr auto
operator==(bifurcate_bidirectional_node const& x, bifurcate_bidirectional_node const& y) -> bool
{
    return x.left == y.left and x.right == y.right and x.back == y.back;
}

constexpr auto
operator!=(bifurcate_bidirectional_node const& x, bifurcate_bidirectional_node const& y) -> bool
{
    return !(x == y);
}

static_assert(std::regular<bifurcate_bidirectional_node>);

struct bidirectional_bicursor
{
    e::Pointer_type<bifurcate_bidirectional_node> cur{nullptr};
};

template <>
struct value_type_t<bidirectional_bicursor>
{
    using type = int;
};

template <>
struct weight_type_t<bidirectional_bicursor>
{
    using type = pointer_diff;
};

constexpr auto
operator==(bidirectional_bicursor const& x, bidirectional_bicursor const& y) -> bool
{
    return x.cur == y.cur;
}

constexpr auto
operator!=(bidirectional_bicursor const& x, bidirectional_bicursor const& y) -> bool
{
    return !(x == y);
}

constexpr auto
is_empty(bidirectional_bicursor cur) -> bool
{
    return cur.cur == nullptr;
}

constexpr auto
has_left_successor(bidirectional_bicursor cur) -> bool
{
    return load(cur.cur).left != nullptr;
}

constexpr auto
has_right_successor(bidirectional_bicursor cur) -> bool
{
    return load(cur.cur).right != nullptr;
}

constexpr auto
has_predecessor(bidirectional_bicursor cur) -> bool
{
    return load(cur.cur).back != nullptr;
}

constexpr void
increment_left(bidirectional_bicursor& cur)
{
    cur.cur = load(cur.cur).left;
}

constexpr void
increment_right(bidirectional_bicursor& cur)
{
    cur.cur = load(cur.cur).right;
}

constexpr void
decrement(bidirectional_bicursor& cur)
{
    cur.cur = load(cur.cur).back;
}

constexpr auto
load(bidirectional_bicursor cur) -> int const&
{
    return load(cur.cur).value;
}

}

template <e::Bicursor C>
struct df_visitor
{
    int pre_visit = 0;
    int in_visit = 0;
    int post_visit = 0;
    int left_branches = 0;
    int leaves = 0;
    int right_branches = 0;

    void operator()(e::df_visit visit, C cur)
    {
        switch (visit)
        {
            case e::df_visit::preorder:
                e::increment(pre_visit);
                if (e::has_left_successor(cur)) e::increment(left_branches);
                return;
            case e::df_visit::inorder:
                e::increment(in_visit);
                if (!(e::has_left_successor(cur) or e::has_right_successor(cur))) e::increment(leaves);
                return;
            case e::df_visit::postorder:
                e::increment(post_visit);
                if (e::has_right_successor(cur)) e::increment(right_branches);
                return;
        }
    }
};

template <e::Linked_bicursor C>
struct rotating_df_visitor
{
    int visits = 0;
    int left_branches = 0;
    int leaves = 0;
    int right_branches = 0;

    void operator()(C cur)
    {
        e::increment(visits);
        if (e::has_left_successor(cur)) e::increment(left_branches);
        if (!(e::has_left_successor(cur) or e::has_right_successor(cur))) e::increment(leaves);
        if (e::has_right_successor(cur)) e::increment(right_branches);
    }
};

SCENARIO ("Graph traversal", "[bicursor]")
{
    SECTION ("DAG testing")
    {
        e::bifurcate_bidirectional_node root;
        e::bidirectional_bicursor cur{pointer_to(root)};
        REQUIRE (e::is_dag(cur));

        root.left = e::pointer_to(root);
        root.back = e::pointer_to(root);
        REQUIRE (!e::is_dag(cur));

        root.left = nullptr;
        root.right = e::pointer_to(root);
        REQUIRE (!e::is_dag(cur));

        root.right = nullptr;
        root.back = nullptr;
        e::bifurcate_bidirectional_node left_branch;
        root.left = e::pointer_to(left_branch);
        left_branch.back = e::pointer_to(root);
        REQUIRE (e::is_dag(cur));

        left_branch.left = e::pointer_to(root);
        root.back = e::pointer_to(left_branch);
        REQUIRE (!e::is_dag(cur));

        left_branch.left = nullptr;
        left_branch.right = e::pointer_to(root);
        REQUIRE (!e::is_dag(cur));

        root.left = nullptr;
        root.back = nullptr;
        e::bifurcate_bidirectional_node right_branch;
        root.right = e::pointer_to(right_branch);
        right_branch.back = e::pointer_to(root);
        REQUIRE (e::is_dag(cur));

        right_branch.left = e::pointer_to(root);
        root.back = e::pointer_to(right_branch);
        REQUIRE (!e::is_dag(cur));

        right_branch.left = nullptr;
        right_branch.right = e::pointer_to(root);
        REQUIRE (!e::is_dag(cur));

        root.back = nullptr;
        root.left = e::pointer_to(left_branch);
        left_branch.back = e::pointer_to(root);
        left_branch.left = nullptr;
        left_branch.right = nullptr;
        root.right = e::pointer_to(right_branch);
        right_branch.back = e::pointer_to(root);
        e::bifurcate_bidirectional_node right_leaf;
        right_branch.left = e::pointer_to(right_leaf);
        right_branch.right = nullptr;
        right_leaf.back = e::pointer_to(right_branch);
        REQUIRE (e::is_dag(cur));

        right_leaf.left = e::pointer_to(right_leaf);
        REQUIRE (!e::is_dag(cur));

        right_leaf.left = e::pointer_to(right_branch);
        REQUIRE (!e::is_dag(cur));

        right_leaf.left = e::pointer_to(root);
        REQUIRE (!e::is_dag(cur));

        right_leaf.right = e::pointer_to(root);
        REQUIRE (!e::is_dag(cur));

        right_leaf.left = nullptr;
        right_leaf.right = nullptr;
        right_branch.right = e::pointer_to(right_leaf);
        REQUIRE (!e::is_dag(cur));
    }
}

SCENARIO ("Tree traversal", "[bicursor]")
{
    SECTION ("Bicursors")
    {
        e::bifurcate_forward_node root;
        e::forward_bicursor cur{pointer_to(root)};

        REQUIRE (!e::is_empty(cur));
        REQUIRE (!e::has_left_successor(cur));
        REQUIRE (!e::has_right_successor(cur));

        REQUIRE (e::tree_weight(cur) == 1);
        REQUIRE (e::tree_height(cur) == 1);

        e::bifurcate_forward_node left_branch;
        e::bifurcate_forward_node right_branch;

        root.left = e::pointer_to(left_branch);
        root.right = e::pointer_to(right_branch);

        REQUIRE (e::has_left_successor(cur));
        REQUIRE (e::has_right_successor(cur));

        REQUIRE (e::tree_weight(cur) == 3);
        REQUIRE (e::tree_height(cur) == 2);

        e::bifurcate_forward_node left_leaf;
        left_branch.left = e::pointer_to(left_leaf);
        e::forward_bicursor left_cur{pointer_to(left_branch)};

        REQUIRE (e::tree_weight(cur) == 4);
        REQUIRE (e::tree_height(cur) == 3);

        REQUIRE (e::has_left_successor(left_cur));
        REQUIRE (!e::has_right_successor(left_cur));

        REQUIRE (e::tree_weight(left_cur) == 2);
        REQUIRE (e::tree_height(left_cur) == 2);

        e::bifurcate_forward_node right_leaf;
        right_branch.right = e::pointer_to(right_leaf);
        e::forward_bicursor right_cur{pointer_to(right_branch)};

        REQUIRE (!e::has_left_successor(right_cur));
        REQUIRE (e::has_right_successor(right_cur));

        REQUIRE (e::tree_weight(right_cur) == 2);
        REQUIRE (e::tree_height(right_cur) == 2);

        REQUIRE (e::tree_weight(cur) == 5);
        REQUIRE (e::tree_height(cur) == 3);

        df_visitor<e::forward_bicursor> visit;
        visit = e::tree_traverse_nonempty(cur, visit);
        REQUIRE (visit.pre_visit == 5);
        REQUIRE (visit.in_visit == 5);
        REQUIRE (visit.post_visit == 5);

        REQUIRE (visit.left_branches == 2);
        REQUIRE (visit.right_branches == 2);
        REQUIRE (visit.leaves == 2);
    }

    SECTION ("Bidirectional Bicursors")
    {
        e::bifurcate_bidirectional_node root;
        e::bidirectional_bicursor cur{pointer_to(root)};

        REQUIRE (!e::is_empty(cur));
        REQUIRE (!e::has_left_successor(cur));
        REQUIRE (!e::has_right_successor(cur));

        REQUIRE (e::tree_weight(cur) == 1);
        REQUIRE (e::tree_height(cur) == 1);

        e::bifurcate_bidirectional_node left_branch;
        e::bifurcate_bidirectional_node right_branch;

        REQUIRE (!e::is_reachable(cur, e::bidirectional_bicursor{pointer_to(left_branch)}));
        REQUIRE (!e::is_reachable(cur, e::bidirectional_bicursor{pointer_to(right_branch)}));

        root.left = e::pointer_to(left_branch);
        left_branch.back = e::pointer_to(root);
        root.right = e::pointer_to(right_branch);
        right_branch.back = e::pointer_to(root);

        REQUIRE (e::is_reachable(cur, e::bidirectional_bicursor{pointer_to(left_branch)}));
        REQUIRE (e::is_reachable(cur, e::bidirectional_bicursor{pointer_to(right_branch)}));

        REQUIRE (e::has_left_successor(cur));
        REQUIRE (e::has_right_successor(cur));

        REQUIRE (e::tree_weight(cur) == 3);
        REQUIRE (e::tree_height(cur) == 2);

        e::bifurcate_bidirectional_node left_leaf;
        left_branch.left = e::pointer_to(left_leaf);
        left_leaf.back = e::pointer_to(left_branch);
        e::bidirectional_bicursor left_cur{pointer_to(left_branch)};

        REQUIRE (e::is_reachable(cur, e::bidirectional_bicursor{pointer_to(left_leaf)}));

        REQUIRE (e::tree_weight(cur) == 4);
        REQUIRE (e::tree_height(cur) == 3);

        REQUIRE (e::has_left_successor(left_cur));
        REQUIRE (!e::has_right_successor(left_cur));

        REQUIRE (e::tree_weight(left_cur) == 2);
        REQUIRE (e::tree_height(left_cur) == 2);

        e::bifurcate_bidirectional_node right_leaf;
        right_branch.right = e::pointer_to(right_leaf);
        right_leaf.back = e::pointer_to(right_branch);
        e::bidirectional_bicursor right_cur{pointer_to(right_branch)};

        REQUIRE (e::is_reachable(cur, e::bidirectional_bicursor{pointer_to(right_leaf)}));

        REQUIRE (!e::has_left_successor(right_cur));
        REQUIRE (e::has_right_successor(right_cur));

        REQUIRE (e::tree_weight(right_cur) == 2);
        REQUIRE (e::tree_height(right_cur) == 2);

        REQUIRE (e::tree_weight(cur) == 5);
        REQUIRE (e::tree_height(cur) == 3);

        df_visitor<e::bidirectional_bicursor> visit;
        visit = e::tree_traverse_nonempty(cur, visit);
        REQUIRE (visit.pre_visit == 5);
        REQUIRE (visit.in_visit == 5);
        REQUIRE (visit.post_visit == 5);

        REQUIRE (visit.left_branches == 2);
        REQUIRE (visit.right_branches == 2);
        REQUIRE (visit.leaves == 2);
    }

    SECTION ("Linked Bicursors")
    {
        e::bifurcate_forward_node root;
        e::linked_forward_bicursor cur{pointer_to(root)};

        REQUIRE (!e::is_empty(cur));
        REQUIRE (!e::has_left_successor(cur));
        REQUIRE (!e::has_right_successor(cur));

        REQUIRE (e::tree_weight(cur) == 1);
        REQUIRE (e::tree_height(cur) == 1);

        e::bifurcate_forward_node left_branch;
        e::bifurcate_forward_node right_branch;

        root.left = e::pointer_to(left_branch);
        root.right = e::pointer_to(right_branch);

        REQUIRE (e::has_left_successor(cur));
        REQUIRE (e::has_right_successor(cur));

        REQUIRE (e::tree_weight(cur) == 3);
        REQUIRE (e::tree_height(cur) == 2);

        e::bifurcate_forward_node left_leaf;
        left_branch.left = e::pointer_to(left_leaf);
        e::linked_forward_bicursor left_cur{pointer_to(left_branch)};

        REQUIRE (e::tree_weight(cur) == 4);
        REQUIRE (e::tree_height(cur) == 3);

        REQUIRE (e::has_left_successor(left_cur));
        REQUIRE (!e::has_right_successor(left_cur));

        REQUIRE (e::tree_weight(left_cur) == 2);
        REQUIRE (e::tree_height(left_cur) == 2);

        e::bifurcate_forward_node right_leaf;
        right_branch.right = e::pointer_to(right_leaf);
        e::linked_forward_bicursor right_cur{pointer_to(right_branch)};

        REQUIRE (!e::has_left_successor(right_cur));
        REQUIRE (e::has_right_successor(right_cur));

        REQUIRE (e::tree_weight(right_cur) == 2);
        REQUIRE (e::tree_height(right_cur) == 2);

        REQUIRE (e::tree_weight(cur) == 5);
        REQUIRE (e::tree_height(cur) == 3);

        df_visitor<e::linked_forward_bicursor> visit;
        visit = e::tree_traverse_nonempty(cur, visit);
        REQUIRE (visit.pre_visit == 5);
        REQUIRE (visit.in_visit == 5);
        REQUIRE (visit.post_visit == 5);

        REQUIRE (visit.left_branches == 2);
        REQUIRE (visit.right_branches == 2);
        REQUIRE (visit.leaves == 2);

        rotating_df_visitor<e::linked_forward_bicursor> rotating_visit;
        rotating_visit = e::tree_traverse_rotating(cur, rotating_visit);
        REQUIRE (rotating_visit.visits == 15);

        REQUIRE (rotating_visit.left_branches == 8);
        REQUIRE (rotating_visit.right_branches == 8);
        REQUIRE (rotating_visit.leaves == 2);

        rotating_df_visitor<e::linked_forward_bicursor> phased_rotating_visit0;
        rotating_df_visitor<e::linked_forward_bicursor> phased_rotating_visit1;
        rotating_df_visitor<e::linked_forward_bicursor> phased_rotating_visit2;
        phased_rotating_visit0 = e::tree_traverse_phased_rotating(cur, 0, phased_rotating_visit0);
        phased_rotating_visit1 = e::tree_traverse_phased_rotating(cur, 1, phased_rotating_visit1);
        phased_rotating_visit2 = e::tree_traverse_phased_rotating(cur, 2, phased_rotating_visit2);
        REQUIRE (phased_rotating_visit0.visits == 5);
        REQUIRE (phased_rotating_visit1.visits == 5);
        REQUIRE (phased_rotating_visit2.visits == 5);

        REQUIRE (phased_rotating_visit0.left_branches == 4);
        REQUIRE (phased_rotating_visit1.left_branches == 4);
        REQUIRE (phased_rotating_visit2.left_branches == 0);
        REQUIRE (phased_rotating_visit0.right_branches == 4);
        REQUIRE (phased_rotating_visit1.right_branches == 0);
        REQUIRE (phased_rotating_visit2.right_branches == 4);
        REQUIRE (phased_rotating_visit0.leaves == 0);
        REQUIRE (phased_rotating_visit1.leaves == 1);
        REQUIRE (phased_rotating_visit2.leaves == 1);
    }
}

SCENARIO ("Tree isomorphism", "[isomorphism]")
{
    SECTION ("Bicursor tree")
    {
        e::bifurcate_forward_node root0;
        e::bifurcate_forward_node root1;

        e::forward_bicursor cur0{e::pointer_to(root0)};
        e::forward_bicursor cur1{e::pointer_to(root1)};

        REQUIRE (e::tree_isomorphic(cur0, cur1));

        e::bifurcate_forward_node left_branch0;
        e::bifurcate_forward_node right_branch0;
        root0.left = e::pointer_to(left_branch0);
        root0.right = e::pointer_to(right_branch0);

        REQUIRE (!e::tree_isomorphic(cur0, cur1));

        e::bifurcate_forward_node left_branch1;
        root1.left = e::pointer_to(left_branch1);

        REQUIRE (!e::tree_isomorphic(cur0, cur1));

        e::bifurcate_forward_node right_branch1;
        root1.right = e::pointer_to(right_branch1);

        REQUIRE (e::tree_isomorphic(cur0, cur1));
    }

    SECTION ("Bidirectional bicursor tree")
    {
        e::bifurcate_bidirectional_node root0;
        e::bifurcate_bidirectional_node root1;

        e::bidirectional_bicursor cur0{e::pointer_to(root0)};
        e::bidirectional_bicursor cur1{e::pointer_to(root1)};

        REQUIRE (e::tree_isomorphic(cur0, cur1));

        e::bifurcate_bidirectional_node left_branch0;
        e::bifurcate_bidirectional_node right_branch0;
        root0.left = e::pointer_to(left_branch0);
        left_branch0.back = e::pointer_to(root0);
        root0.right = e::pointer_to(right_branch0);
        right_branch0.back = e::pointer_to(root0);

        REQUIRE (!e::tree_isomorphic(cur0, cur1));

        e::bifurcate_bidirectional_node left_branch1;
        root1.left = e::pointer_to(left_branch1);
        left_branch1.back = e::pointer_to(root1);

        REQUIRE (!e::tree_isomorphic(cur0, cur1));

        e::bifurcate_bidirectional_node right_branch1;
        root1.right = e::pointer_to(right_branch1);
        right_branch1.back = e::pointer_to(root1);

        REQUIRE (e::tree_isomorphic(cur0, cur1));
    }
}

SCENARIO ("Tree equivalence", "[equivalence]")
{
    SECTION ("Bicursor tree")
    {
        e::bifurcate_forward_node root0;
        e::bifurcate_forward_node root1;

        e::forward_bicursor cur0{e::pointer_to(root0)};
        e::forward_bicursor cur1{e::pointer_to(root1)};
        e::forward_bicursor empty_cur;

        REQUIRE (e::tree_equivalent(empty_cur, empty_cur, e::eq{}));
        REQUIRE (!e::tree_equivalent(empty_cur, cur0, e::eq{}));
        REQUIRE (!e::tree_equivalent(cur0, empty_cur, e::eq{}));
        REQUIRE (e::tree_equivalent(cur0, cur1, e::eq{}));

        root0.value = 1;

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        root1.value = 1;

        e::bifurcate_forward_node left_branch0;
        e::bifurcate_forward_node right_branch0;
        root0.left = e::pointer_to(left_branch0);
        root0.right = e::pointer_to(right_branch0);

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        e::bifurcate_forward_node left_branch1;
        root1.left = e::pointer_to(left_branch1);

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        e::bifurcate_forward_node right_branch1;
        root1.right = e::pointer_to(right_branch1);

        REQUIRE (e::tree_equivalent(cur0, cur1, e::eq{}));

        left_branch0.value = 1;

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        left_branch1.value = 1;

        REQUIRE (e::tree_equivalent(cur0, cur1, e::eq{}));

        right_branch0.value = 1;

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        right_branch1.value = 1;

        REQUIRE (e::tree_equivalent(cur0, cur1, e::eq{}));
    }

    SECTION ("Bidirectional bicursor tree")
    {
        e::bifurcate_bidirectional_node root0;
        e::bifurcate_bidirectional_node root1;

        e::bidirectional_bicursor cur0{e::pointer_to(root0)};
        e::bidirectional_bicursor cur1{e::pointer_to(root1)};
        e::bidirectional_bicursor empty_cur;

        REQUIRE (e::tree_equivalent(empty_cur, empty_cur, e::eq{}));
        REQUIRE (!e::tree_equivalent(empty_cur, cur0, e::eq{}));
        REQUIRE (!e::tree_equivalent(cur0, empty_cur, e::eq{}));
        REQUIRE (e::tree_equivalent(cur0, cur1, e::eq{}));

        root0.value = 1;

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        root1.value = 1;

        e::bifurcate_bidirectional_node left_branch0;
        e::bifurcate_bidirectional_node right_branch0;
        root0.left = e::pointer_to(left_branch0);
        left_branch0.back = e::pointer_to(root0);
        root0.right = e::pointer_to(right_branch0);
        right_branch0.back = e::pointer_to(root0);

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        e::bifurcate_bidirectional_node left_branch1;
        root1.left = e::pointer_to(left_branch1);
        left_branch1.back = e::pointer_to(root1);

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        e::bifurcate_bidirectional_node right_branch1;
        root1.right = e::pointer_to(right_branch1);
        right_branch1.back = e::pointer_to(root1);

        REQUIRE (e::tree_equivalent(cur0, cur1, e::eq{}));

        left_branch0.value = 1;

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        left_branch1.value = 1;

        REQUIRE (e::tree_equivalent(cur0, cur1, e::eq{}));

        right_branch0.value = 1;

        REQUIRE (!e::tree_equivalent(cur0, cur1, e::eq{}));

        right_branch1.value = 1;

        REQUIRE (e::tree_equivalent(cur0, cur1, e::eq{}));
    }
}

SCENARIO ("Tree ordering", "[ordering]")
{
    SECTION ("Bicursor tree")
    {
        e::bifurcate_forward_node root0;
        e::bifurcate_forward_node root1;

        e::forward_bicursor cur0{e::pointer_to(root0)};
        e::forward_bicursor cur1{e::pointer_to(root1)};
        e::forward_bicursor empty_cur;

        REQUIRE (!e::tree_compare(empty_cur, empty_cur, e::lt{}));
        REQUIRE (e::tree_compare(empty_cur, cur0, e::lt{}));
        REQUIRE (!e::tree_compare(cur0, empty_cur, e::lt{}));
        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        root0.value = -1;

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        root1.value = -1;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        e::bifurcate_forward_node left_branch1;
        root1.left = e::pointer_to(left_branch1);

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        root1.value = -2;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        root1.value = -1;
        e::bifurcate_forward_node left_branch0;
        root0.left = e::pointer_to(left_branch0);

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        left_branch0.value = -1;

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        left_branch1.value = -2;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        left_branch1.value = -1;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        e::bifurcate_forward_node right_branch1;
        root1.right = e::pointer_to(right_branch1);

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        e::bifurcate_forward_node right_branch0;
        root0.right = e::pointer_to(right_branch0);

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        right_branch0.value = -1;

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        right_branch1.value = -2;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));
    }

    SECTION ("Bidirectional bicursor tree")
    {
        e::bifurcate_bidirectional_node root0;
        e::bifurcate_bidirectional_node root1;

        e::bidirectional_bicursor cur0{e::pointer_to(root0)};
        e::bidirectional_bicursor cur1{e::pointer_to(root1)};
        e::bidirectional_bicursor empty_cur;

        REQUIRE (!e::tree_compare(empty_cur, empty_cur, e::lt{}));
        REQUIRE (e::tree_compare(empty_cur, cur0, e::lt{}));
        REQUIRE (!e::tree_compare(cur0, empty_cur, e::lt{}));
        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        root0.value = -1;

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        root1.value = -1;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        e::bifurcate_bidirectional_node left_branch1;
        root1.left = e::pointer_to(left_branch1);
        left_branch1.back = e::pointer_to(root1);

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        root1.value = -2;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        root1.value = -1;
        e::bifurcate_bidirectional_node left_branch0;
        root0.left = e::pointer_to(left_branch0);
        left_branch0.back = e::pointer_to(root0);

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        left_branch0.value = -1;

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        left_branch1.value = -2;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        left_branch1.value = -1;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        e::bifurcate_bidirectional_node right_branch1;
        root1.right = e::pointer_to(right_branch1);
        right_branch1.back = e::pointer_to(root0);

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        e::bifurcate_bidirectional_node right_branch0;
        root0.right = e::pointer_to(right_branch0);
        right_branch0.back = e::pointer_to(root0);

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));

        right_branch0.value = -1;

        REQUIRE (e::tree_compare(cur0, cur1, e::lt{}));

        right_branch1.value = -2;

        REQUIRE (!e::tree_compare(cur0, cur1, e::lt{}));
    }
}
