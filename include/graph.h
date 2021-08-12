#pragma once

#include "cursor.h"
#include "transformation.h"

namespace elements {

enum struct df_visit
{
    preorder,
    inorder,
    postorder
};

template <Bidirectional_bicursor C>
struct df_visitor
{
    df_visit visit{df_visit::preorder};
    C cur{nullptr};
};

template <Bidirectional_bicursor C>
constexpr auto
operator==(df_visitor<C> const& x, df_visitor<C> const& y) -> bool
{
    return x.visit == y.visit and x.cur == y.cur;
}

template <Bidirectional_bicursor C>
constexpr auto
operator!=(df_visitor<C> const& x, df_visitor<C> const& y) -> bool
{
    return !(x == y);
}

template <Bidirectional_bicursor C>
struct distance_type_t<df_visitor<C>>
{
    using type = Weight_type<C>;
};

template <Bidirectional_bicursor C>
struct difference_type_t<df_visitor<C>>
{
    using type = Weight_type<C>;
};

template <Bidirectional_bicursor C>
constexpr auto
can_traverse_step(df_visitor<C> const& vis) -> bool
{
    return has_predecessor(vis.cur) or vis.visit != df_visit::postorder;
}

template <Bidirectional_bicursor C>
constexpr auto
traverse_step(df_visit& visit, C& cur) -> int
//[[expects: can_traverse_step(vis)]]
{
    switch (visit) {
        case df_visit::preorder:
            if (has_left_successor(cur)) {
                increment_left(cur);
                return 1;
            } else {
                visit = df_visit::inorder;
                return 0;
            }
        case df_visit::inorder:
            if (has_right_successor(cur)) {
                visit = df_visit::preorder;
                increment_right(cur);
                return 1;
            } else {
                visit = df_visit::postorder;
                return 0;
            }
        case df_visit::postorder:
            if (is_left_successor(cur)) {
                visit = df_visit::inorder;
            }
            decrement(cur);
            return -1;
    }
    return 0;
}

template <Bidirectional_bicursor C>
constexpr auto
traverse_step(df_visitor<C>& vis) -> int
//[[expects: can_traverse_step(vis)]]
{
    switch (vis.visit) {
        case df_visit::preorder:
            if (has_left_successor(vis.cur)) {
                increment_left(vis.cur);
                return 1;
            } else {
                vis.visit = df_visit::inorder;
                return 0;
            }
        case df_visit::inorder:
            if (has_right_successor(vis.cur)) {
                vis.visit = df_visit::preorder;
                increment_right(vis.cur);
                return 1;
            } else {
                vis.visit = df_visit::postorder;
                return 0;
            }
        case df_visit::postorder:
            if (is_left_successor(vis.cur)) {
                vis.visit = df_visit::inorder;
            }
            decrement(vis.cur);
            return -1;
    }
    return 0;
}

template <Bidirectional_bicursor C>
constexpr void
traverse_step_action(df_visitor<C>& vis)
//[[expects: can_traverse_step(vis)]]
{
    traverse_step(vis.visit, vis.cur);
}

template <Bidirectional_bicursor C>
constexpr auto
is_dag(C cur) -> bool
{
    if (is_empty(cur)) return true;
    return is_terminating(df_visitor{df_visit::preorder, cur}, traverse_step_action<C>, can_traverse_step<C>);
}

}
