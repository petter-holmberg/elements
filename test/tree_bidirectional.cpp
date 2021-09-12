#include "catch.hpp"

#include "tree_bidirectional.h"

namespace e = elements;

template <e::Bidirectional_bicursor C>
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

SCENARIO ("Using bidirectional tree", "[tree_bidirectional]")
{
    e::tree_bidirectional<int> x;

    static_assert(e::Traversable<decltype(x), df_visitor<e::Cursor_type<decltype(x)>>>);
    static_assert(e::Bidirectional_bicursor<e::Cursor_type<decltype(x)>>);
    static_assert(e::Linked_bicursor<e::Cursor_type<decltype(x)>>);
    static_assert(e::Semiregular<decltype(x)>);

    REQUIRE (e::axiom_Regular(x));

    SECTION ("Checking elements")
    {
        auto cur{e::first(x)};

        e::tree_bidirectional<int> y{0, x, x};

        SECTION ("Empty tree")
        {
            REQUIRE (e::is_empty(x));
            REQUIRE (e::height(x) == 0);
            REQUIRE (e::weight(x) == 0);

            REQUIRE (e::is_empty(cur));
        }

        SECTION ("Tree with root")
        {
            REQUIRE (!e::is_empty(y));
            REQUIRE (e::height(y) == 1);
            REQUIRE (e::weight(y) == 1);

            cur = e::first(y);

            REQUIRE (!e::is_empty(cur));
            REQUIRE (!e::has_left_successor(cur));
            REQUIRE (!e::has_right_successor(cur));
            REQUIRE(e::load(cur) == 0);
        }

        SECTION ("Tree with left branch")
        {
            e::tree_bidirectional<int> y_left{2, y, {}};

            REQUIRE (!e::is_empty(y_left));
            REQUIRE (e::height(y_left) == 2);
            REQUIRE (e::weight(y_left) == 2);

            cur = e::first(y_left);

            REQUIRE (!e::is_empty(cur));
            REQUIRE (e::has_left_successor(cur));
            REQUIRE (!e::has_right_successor(cur));
            REQUIRE(e::load(cur) == 2);

            e::increment_left(cur);

            REQUIRE (!e::is_empty(cur));
            REQUIRE (!e::has_left_successor(cur));
            REQUIRE (!e::has_right_successor(cur));
            REQUIRE(e::load(cur) == 0);
        }

        SECTION ("Tree with right branch")
        {
            e::tree_bidirectional<int> y_right{1, {}, y};

            REQUIRE (!e::is_empty(y_right));
            REQUIRE (e::height(y_right) == 2);
            REQUIRE (e::weight(y_right) == 2);

           cur = e::first(y_right);

            REQUIRE (!e::is_empty(cur));
            REQUIRE (!e::has_left_successor(cur));
            REQUIRE (e::has_right_successor(cur));
            REQUIRE(e::load(cur) == 1);

            e::increment_right(cur);

            REQUIRE (!e::is_empty(cur));
            REQUIRE (!e::has_left_successor(cur));
            REQUIRE (!e::has_right_successor(cur));
            REQUIRE(e::load(cur) == 0);
        }

        SECTION ("Tree with left and right branch")
        {
            e::tree_bidirectional<int> y_both{3, y, y};

            REQUIRE (!e::is_empty(y_both));
            REQUIRE (e::height(y_both) == 2);
            REQUIRE (e::weight(y_both) == 3);

            cur = e::first(y_both);

            REQUIRE (!e::is_empty(cur));
            REQUIRE (e::has_left_successor(cur));
            REQUIRE (e::has_right_successor(cur));
            REQUIRE(e::load(cur) == 3);

            auto left_cur{cur};
            e::increment_right(left_cur);

            REQUIRE (!e::is_empty(left_cur));
            REQUIRE (!e::has_left_successor(left_cur));
            REQUIRE (!e::has_right_successor(left_cur));
            REQUIRE(e::load(left_cur) == 0);

            auto right_cur{cur};
            e::increment_right(right_cur);

            REQUIRE (!e::is_empty(right_cur));
            REQUIRE (!e::has_left_successor(right_cur));
            REQUIRE (!e::has_right_successor(right_cur));
            REQUIRE(e::load(right_cur) == 0);
        }
    }
}
