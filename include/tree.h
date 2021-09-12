#pragma once

#include "graph.h"
#include "ordering.h"

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

template <Bidirectional_bicursor C>
constexpr auto
is_reachable(C cur0, C cur1) -> bool
//[[expects axiom: is_tree(cur0)]]
{
    if (is_empty(cur0)) return false;
    return is_reachable_nonempty(cur0, cur1);
}

template <Bidirectional_bicursor C>
constexpr auto
is_reachable_nonempty(C cur0, C cur1) -> bool
//[[expects axiom: is_tree(cur0)]]
//[[expects: !is_empty(cur0)]]
{
    auto root{cur0};
    auto visit{df_visit::preorder};
    do {
        if (cur0 == cur1) return true;
        traverse_step(visit, cur0);
    } while (cur0 != root or visit != df_visit::postorder);
    return false;
}

template <Bicursor C>
constexpr auto
tree_weight(C cur) -> Weight_type<C>
//[[expects axiom: is_tree(cur)]]
{
    if constexpr(!Bidirectional_bicursor<C> and Linked_bicursor<C>) {
        return tree_weight_rotating(cur);
    } else {
        if (is_empty(cur)) return 0;
        return tree_weight_nonempty(cur);
    }
}

template <Linked_bicursor C>
constexpr auto
tree_weight_rotating(C cur) -> Weight_type<C>
//[[expects axiom: is_tree(cur)]]
{
    Weight_type<C> n{0};
    tree_traverse_rotating(cur, [&n](C const&){ increment(n); });
    return n / Weight_type<C>{3};
}

template <Bicursor C>
constexpr auto
tree_weight_nonempty(C cur) -> Weight_type<C>
//[[expects axiom: is_tree(cur)]]
//[[expects: !is_empty(cur)]]
{
    Weight_type<C> left_weight{0};
    Weight_type<C> right_weight{0};
    if (has_left_successor(cur)) {
        left_weight = tree_weight_nonempty(left_successor(cur));
    }
    if (has_right_successor(cur)) {
        right_weight = tree_weight_nonempty(right_successor(cur));
    }
    return successor(left_weight + right_weight);
}

template <Bidirectional_bicursor C>
constexpr auto
tree_weight_nonempty(C cur) -> Weight_type<C>
//[[expects axiom: is_tree(cur)]]
//[[expects: !is_empty(cur)]]
{
    auto root{cur};
    auto visit{df_visit::preorder};
    Weight_type<C> weight{1};
    do {
        traverse_step(visit, cur);
        if (visit == df_visit::preorder) increment(weight);
    } while (cur != root or visit != df_visit::postorder);
    return weight;
}

template <Bicursor C>
constexpr auto
tree_height(C cur) -> Weight_type<C>
//[[expects axiom: is_tree(cur)]]
{
    if (is_empty(cur)) return 0;
    return tree_height_nonempty(cur);
}

template <Bicursor C>
constexpr auto
tree_height_nonempty(C cur) -> Weight_type<C>
//[[expects axiom: is_tree(cur)]]
{
    if (is_empty(cur)) return 0;
    Weight_type<C> left_height{0};
    Weight_type<C> right_height{0};
    if (has_left_successor(cur)) {
        left_height = tree_height_nonempty(left_successor(cur));
    }
    if (has_right_successor(cur)) {
        right_height = tree_height_nonempty(right_successor(cur));
    }
    return successor(max(left_height, right_height));
}

template <Bidirectional_bicursor C>
constexpr auto
tree_height_nonempty(C cur) -> Weight_type<C>
//[[expects axiom: is_tree(cur)]]
{
    auto root{cur};
    auto visit{df_visit::preorder};
    Weight_type<C> max_height{1};
    Weight_type<C> height{1};
    do {
        height = (height - Weight_type<C>{1}) + Weight_type<C>{traverse_step(visit, cur) + 1};
        max_accumulate(max_height, height);
    } while (cur != root or visit != df_visit::postorder);
    return max_height;
}

template <Bicursor C, Invocable<df_visit, C> P>
constexpr auto
tree_traverse(C cur, P proc) -> P
//[[expects axiom: is_tree(cur)]]
{
    if (is_empty(cur)) return proc;
    return tree_traverse_nonempty(cur, proc);
}

template <Bicursor C, Invocable<df_visit, C> P>
constexpr auto
tree_traverse_nonempty(C cur, P proc) -> P
//[[expects axiom: is_tree(cur)]]
//[[expects: !is_empty(cur)]]
{
    invoke(proc, df_visit::preorder, cur);
    if (has_left_successor(cur)) {
        proc = tree_traverse_nonempty(left_successor(cur), proc);
    }
    invoke(proc, df_visit::inorder, cur);
    if (has_right_successor(cur)) {
        proc = tree_traverse_nonempty(right_successor(cur), proc);
    }
    invoke(proc, df_visit::postorder, cur);
    return proc;
}

template <Bidirectional_bicursor C, Invocable<df_visit, C> P>
constexpr auto
tree_traverse_nonempty(C cur, P proc) -> P
//[[expects axiom: is_tree(cur)]]
//[[expects: !is_empty(cur)]]
{
    auto root{cur};
    auto visit{df_visit::preorder};
    invoke(proc, df_visit::preorder, cur);
    do {
        traverse_step(visit, cur);
        invoke(proc, visit, cur);
    } while (cur != root or visit != df_visit::postorder);
    return proc;
}

template <Linked_bicursor C>
constexpr void
tree_rotate(C& curr, C& prev)
//[[expects: !is_empty(cur)]]
{
    C temp{left_successor(curr)};
    set_left_successor(curr, right_successor(curr));
    set_right_successor(curr, prev);
    if (is_empty(temp)) {
        prev = temp;
    } else {
        prev = curr;
        curr = temp;
    }
}

template <Linked_bicursor C, Invocable<C> P>
constexpr auto
tree_traverse_rotating(C cur, P proc) -> P
//[[expects axiom: is_tree(cur)]]
{
    if (is_empty(cur)) return proc;
    C curr{cur};
    C prev;
    do {
        proc(curr);
        tree_rotate(curr, prev);
    } while (curr != cur);
    do {
        proc(curr);
        tree_rotate(curr, prev);
    } while (curr != cur);
    proc(curr);
    tree_rotate(curr, prev);
    return proc;
}

template <Linked_bicursor C, Invocable<C> P>
constexpr auto
tree_traverse_phased_rotating(C cur, int phase, P proc) -> P
//[[expects axiom: is_tree(cur)]]
//[[expects: phase >= 0 and phase < 3]]
{
    auto phased_applicator = [period = 3, phase, n = 0, &proc](C x) mutable
    {
        if (n == phase) proc(x);
        increment(n);
        if (n == period) n = 0;
    };
    tree_traverse_rotating(cur, phased_applicator);
    return proc;
}

template <Bicursor C0, Bicursor C1>
constexpr auto
tree_isomorphic(C0 cur0, C1 cur1) -> bool
//[[expects axiom: is_tree(cur0)]]
//[[expects axiom: is_tree(cur1)]]
{
    if (is_empty(cur0)) return is_empty(cur1);
    if (is_empty(cur1)) return false;
    return tree_isomorphic_nonempty(cur0, cur1);
}

template <Bicursor C0, Bicursor C1>
constexpr auto
tree_isomorphic_nonempty(C0 cur0, C1 cur1) -> bool
//[[expects axiom: is_tree(cur0)]]
//[[expects axiom: is_tree(cur1)]]
//[[expects: !is_empty(cur0)]]
//[[expects: !is_empty(cur1)]]
{
    if (has_left_successor(cur0)) {
        if (has_left_successor(cur1)) {
            if (!tree_isomorphic_nonempty(left_successor(cur0), left_successor(cur1))) {
                return false;
            }
        } else {
            return false;
        }
    } else if (has_left_successor(cur1)) {
        return false;
    }

    if (has_right_successor(cur0)) {
        if (has_right_successor(cur1)) {
            if (!tree_isomorphic_nonempty(right_successor(cur0), right_successor(cur1))) {
                return false;
            }
        } else {
            return false;
        }
    } else if (has_right_successor(cur1)) {
        return false;
    }
    return true;
}

template <Bidirectional_bicursor C0, Bidirectional_bicursor C1>
constexpr auto
tree_isomorphic_nonempty(C0 cur0, C1 cur1) -> bool
//[[expects axiom: is_tree(cur0)]]
//[[expects: !is_empty(cur0)]]
//[[expects axiom: is_tree(cur1)]]
//[[expects: !is_empty(cur1)]]
{
    C0 root0{cur0};
    auto visit0{df_visit::preorder};
    auto visit1{df_visit::preorder};
    while (true) {
        traverse_step(visit0, cur0);
        traverse_step(visit1, cur1);
        if (visit0 != visit1) return false;
        if (cur0 == root0 && visit0 == df_visit::postorder) return true;
    }
}

template <Bicursor C0, Bicursor C1, Relation<Value_type<C0>, Value_type<C1>> R>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
tree_equivalent(C0 cur0, C1 cur1, R rel) -> bool
//[[expects axiom: loadable_tree(cur0)]]
//[[expects axiom: loadable_tree(cur1)]]
//[[expects axiom: equivalence(rel)]]
{
    if (is_empty(cur0)) return is_empty(cur1);
    if (is_empty(cur1)) return false;
    return tree_equivalent_nonempty(cur0, cur1, rel);
}

template <Bicursor C0, Bicursor C1, Relation<Value_type<C0>, Value_type<C1>> R>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
tree_equivalent_nonempty(C0 cur0, C1 cur1, R rel) -> bool
//[[expects axiom: loadable_tree(cur0)]]
//[[expects: !is_empty(cur0)]]
//[[expects axiom: loadable_tree(cur1)]]
//[[expects: !is_empty(cur1)]]
//[[expects axiom: equivalence(rel)]]
{
    if (!invoke(rel, load(cur0), load(cur1))) return false;
    if (has_left_successor(cur0)) {
        if (has_left_successor(cur1)) {
            if (!tree_equivalent_nonempty(left_successor(cur0), left_successor(cur1), rel)) {
                return false;
            }
        } else {
            return false;
        }
    } else if (has_left_successor(cur1)) {
        return false;
    }

    if (has_right_successor(cur0)) {
        if (has_right_successor(cur1)) {
            if (!tree_equivalent_nonempty(right_successor(cur0), right_successor(cur1), rel)) {
                return false;
            }
        } else {
            return false;
        }
    } else if (has_right_successor(cur1)) {
        return false;
    }
    return true;
}

template <Bidirectional_bicursor C0, Bidirectional_bicursor C1, Relation<Value_type<C0>, Value_type<C1>> R>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
tree_equivalent(C0 cur0, C1 cur1, R rel) -> bool
//[[expects axiom: loadable_tree(cur0)]]
//[[expects axiom: loadable_tree(cur1)]]
//[[expects axiom: equivalence(rel)]]
{
    if (is_empty(cur0)) return is_empty(cur1);
    if (is_empty(cur1)) return false;
    C0 root0{cur0};
    auto visit0{df_visit::preorder};
    auto visit1{df_visit::preorder};
    while (true) {
        if (visit0 == df_visit::preorder && !invoke(rel, load(cur0), load(cur1))) return false;
        traverse_step(visit0, cur0);
        traverse_step(visit1, cur1);
        if (visit0 != visit1) return false;
        if (cur0 == root0 && visit0 == df_visit::postorder) return true;
    }
}

template <Bicursor C0, Bicursor C1>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
tree_equal(C0 cur0, C1 cur1) -> bool
//[[expects axiom: loadable_tree(cur0)]]
//[[expects axiom: loadable_tree(cur1)]]
{
    return tree_equivalent(cur0, cur1, eq{});
}

template <Bicursor C0, Bicursor C1, Relation<Value_type<C0>, Value_type<C1>> R>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
tree_compare(C0 cur0, C1 cur1, R rel) -> bool
//[[expects axiom: loadable_tree(cur0)]]
//[[expects axiom: loadable_tree(cur1)]]
//[[expects axiom: weak_ordering(rel)]]
{
    if (is_empty(cur1)) return false;
    if (is_empty(cur0)) return true;
    return tree_compare_nonempty(cur0, cur1, rel);
}

template <Bicursor C0, Bicursor C1, Relation<Value_type<C0>, Value_type<C1>> R>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
tree_compare_nonempty(C0 cur0, C1 cur1, R rel) -> bool
//[[expects axiom: loadable_tree(cur0)]]
//[[expects: !is_empty(cur0)]]
//[[expects axiom: loadable_tree(cur1)]]
//[[expects: !is_empty(cur1)]]
//[[expects axiom: weak_ordering(rel)]]
{
    if (invoke(rel, load(cur0), load(cur1))) return true;
    if (invoke(rel, load(cur1), load(cur0))) return false;
    if (!has_left_successor(cur1)) return false;
    if (!has_left_successor(cur0)) return true;
    if (tree_compare_nonempty(left_successor(cur0), left_successor(cur1), rel)) return true;
    if (tree_compare_nonempty(left_successor(cur1), left_successor(cur0), rel)) return false;
    if (!has_right_successor(cur1)) return false;
    if (!has_right_successor(cur0)) return true;
    if (tree_compare_nonempty(right_successor(cur0), right_successor(cur1), rel)) return true;
    return false;
}

template <Bidirectional_bicursor C0, Bidirectional_bicursor C1, Relation<Value_type<C0>, Value_type<C1>> R>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
tree_compare(C0 cur0, C1 cur1, R rel) -> bool
//[[expects axiom: loadable_tree(cur0)]]
//[[expects axiom: loadable_tree(cur1)]]
//[[expects axiom: weak_ordering(rel)]]
{
    if (is_empty(cur1)) return false;
    if (is_empty(cur0)) return true;
    C0 root0{cur0};
    auto visit0{df_visit::preorder};
    auto visit1{df_visit::preorder};
    while (true) {
        if (visit0 == df_visit::preorder) {
            if (invoke(rel, load(cur0), load(cur1))) return true;
            if (invoke(rel, load(cur1), load(cur0))) return false;
        }
        traverse_step(visit0, cur0);
        traverse_step(visit1, cur1);
        if (visit0 != visit1) return visit0 > visit1;
        if (cur0 == root0 && visit0 == df_visit::postorder) return false;
    }
}

template <Bicursor C0, Bicursor C1>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
tree_less(C0 cur0, C1 cur1) -> bool
//[[expects axiom: loadable_tree(cur0)]]
//[[expects axiom: loadable_tree(cur1)]]
{
    return tree_compare(cur0, cur1, lt{});
}

template <Bicursor C0, Bicursor C1>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
tree_shape_less(C0 cur0, C1 cur1) -> bool
//[[expects axiom: loadable_tree(cur0)]]
//[[expects axiom: loadable_tree(cur1)]]
{
    return tree_compare(cur0, cur1, [](Value_type<C0> const&, Value_type<C1> const&){ return false; });
}

template <Linked_bicursor C, typename Cons>
constexpr auto
//[[expects axiom: is_tree(cur)]]
tree_copy(C cur) -> C
{
    Cons node_construct;
    if (is_empty(cur)) return cur;
    C stack{node_construct(cur, cur, C{})};
    C new_cur{stack};
    while (!is_empty(stack)) {
        cur = left_successor(stack);
        C left{left_successor(cur)};
        C right{right_successor(cur)};
        C top{stack};
        if (!is_empty(left)) {
            if (!is_empty(right)) {
                right = node_construct(right, right, right_successor(stack));
                stack = node_construct(left, left, right);
            } else {
                right = C{};
                stack = node_construct(left, left, right_successor(stack));
            }
            left = stack;
        } else if (!is_empty(right)) {
            stack = node_construct(right, right, right_successor(stack));
            right = stack;
        } else {
            stack = right_successor(stack);
        }
        set_right_successor(top, right);
        set_left_successor(top, left);
    }
    return new_cur;
}

template <Bicursor C, typename Del>
void tree_erase(C cur, Del node_delete)
//[[expects axiom: is_tree(cur)]]
{
    if (is_empty(cur)) return;
    C stack;
    while (true) {
        C left{left_successor(cur)};
        C right{right_successor(cur)};
        if (!is_empty(left)) {
            if (!is_empty(right)) {
                set_left_successor(cur, stack);
                stack = cur;
            } else {
                node_delete(cur);
            }
            cur = left;
        } else if (!is_empty(right)) {
            node_delete(cur);
            cur = right;
        } else {
            node_delete(cur);
            if (!is_empty(stack)) {
                cur = stack;
                stack = left_successor(stack);
                set_left_successor(cur, {});
            } else {
                return;
            }
        }
    }
}

template <typename T, typename Proc>
concept Traversable =
    Invocable<Proc, df_visit, Cursor_type<T>> and
    requires (T& x) {
        first(x);
    } and
    requires (T& x, Proc proc) {
        { is_empty(x) } -> Boolean_testable;
        { height(x) } -> Same_as<Weight_type<T>>;
        { weight(x) } -> Same_as<Weight_type<T>>;
        { traverse(x, proc) } -> Same_as<Proc>;
    };

}
