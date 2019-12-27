#include "catch.hpp"

#include "affine_space.h"
#include "list_singly_linked_front.h"

namespace e = elements;

SCENARIO ("Using singly linked list with front access", "[list_singly_linked_front]")
{
    e::list_singly_linked_front<int> x{0, 1, 2, 3, 4};
    static_assert(e::Dynamic_sequence<decltype(x), e::front<decltype(x)>>);
    static_assert(e::Linked_forward_position<e::Position_type<decltype(x)>>);

    REQUIRE (e::axiom_Regular(x));

    SECTION ("Checking elements")
    {
        REQUIRE (!e::is_empty(x));
        REQUIRE (e::size(x) == 5);
        REQUIRE (x[0] == 0);
        REQUIRE (x[1] == 1);
        REQUIRE (x[2] == 2);
        REQUIRE (x[3] == 3);
        REQUIRE (x[4] == 4);
        x[0] = 5;
        x[1] = 4;
        x[2] = 3;
        x[3] = 2;
        x[4] = 1;
        REQUIRE (x[0] == 5);
        REQUIRE (x[1] == 4);
        REQUIRE (x[2] == 3);
        REQUIRE (x[3] == 2);
        REQUIRE (x[4] == 1);
    }

    SECTION ("Comparing lists")
    {
        {
            auto y = x;

            REQUIRE (x == y);
            REQUIRE (!(x != y));
            REQUIRE (!(x < y));
            REQUIRE (x >= y);
            REQUIRE (!(x > y));
            REQUIRE (x <= y);
        }

        {
            e::list_singly_linked_front<int> y{0, 1, 2, 3};

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (!(x < y));
            REQUIRE (x >= y);
            REQUIRE (x > y);
            REQUIRE (!(x <= y));
        }

        {
            e::list_singly_linked_front<int> y{0, 1, 2, 3, 4, 5};

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (x < y);
            REQUIRE (!(x >= y));
            REQUIRE (!(x > y));
            REQUIRE (x <= y);
        }

        {
            e::list_singly_linked_front<int> y{0, -1, -2, -3, -4};

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (!(x < y));
            REQUIRE (x >= y);
            REQUIRE (x > y);
            REQUIRE (!(x <= y));
        }

        {
            e::list_singly_linked_front<int> y{5, 6, 7, 8, 9};

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (x < y);
            REQUIRE (!(x >= y));
            REQUIRE (!(x > y));
            REQUIRE (x <= y);
        }
    }

    SECTION ("Copying lists")
    {
        {
            auto y0(x);
            decltype(x) z0;
            z0 = x;

            REQUIRE (x == y0);
            REQUIRE (x == z0);

            auto y1(std::move(y0));
            decltype(x) z1;
            z1 = std::move(z0);

            REQUIRE (x == y1);
            REQUIRE (x == z1);
        }

        {
            e::list_singly_linked_front<int> y{5, 6, 7, 8, 9};
            e::swap(x, y);

            CHECK (x[0] == 5);
            CHECK (y[0] == 0);
            CHECK (x[1] == 6);
            CHECK (y[1] == 1);
            CHECK (x[2] == 7);
            CHECK (y[2] == 2);
            CHECK (x[3] == 8);
            CHECK (y[3] == 3);
            CHECK (x[4] == 9);
            CHECK (y[4] == 4);
        }
    }

    SECTION ("Inserting elements")
    {
        e::list_singly_linked_front<int> x0;

        REQUIRE (e::is_empty(x0));
        REQUIRE (e::size(x0) == 0);

        SECTION ("Inserting at front")
        {
            e::push_first(x0, 0);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 1);
            CHECK (x0[0] == 0);

            e::push_first(x0, 1);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 2);
            CHECK (x0[0] == 1);
            CHECK (x0[1] == 0);

            e::push_first(x0, 2);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 3);
            CHECK (x0[0] == 2);
            CHECK (x0[1] == 1);
            CHECK (x0[2] == 0);
        }

        SECTION ("Inserting after position")
        {
            auto after = e::after{x0, e::first(x0)};

            e::insert(after, 0);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 1);
            CHECK (x0[0] == 0);

            after = e::after{x0, e::first(x0)};
            e::insert(after, 1);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 2);
            CHECK (x0[0] == 0);
            CHECK (x0[1] == 1);

            e::increment(after.pos);
            after = e::insert(after, 2);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 3);
            CHECK (x0[0] == 0);
            CHECK (x0[1] == 1);
            CHECK (x0[2] == 2);

            after = e::insert(after, 3);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 4);
            CHECK (x0[0] == 0);
            CHECK (x0[1] == 1);
            CHECK (x0[2] == 2);
            CHECK (x0[3] == 3);
        }
    }

    SECTION ("Erasing elements")
    {
        e::list_singly_linked_front<int> x0{0, 1, 2};
        e::list_singly_linked_front<int> x1;

        SECTION ("Erasing at front")
        {
            e::pop_first(x0);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 2);
            CHECK (x0[0] == 1);
            CHECK (x0[1] == 2);

            e::pop_first(x0);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 1);
            CHECK (x0[0] == 2);

            e::pop_first(x0);

            REQUIRE (e::is_empty(x0));
            REQUIRE (e::size(x0) == 0);
        }

        SECTION ("Erasing after position")
        {
            auto after = e::after{x0, e::first(x0)};

            after = e::erase(after);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 2);
            CHECK (x0[0] == 0);
            CHECK (x0[1] == 2);

            after = e::erase(after);

            REQUIRE (!e::is_empty(x0));
            REQUIRE (e::size(x0) == 1);
            CHECK (x0[0] == 0);

            e::pop_first(x0);

            REQUIRE (e::is_empty(x0));
            REQUIRE (e::size(x0) == 0);
        }


        SECTION ("Erasing all")
        {
            e::erase_all(x0);

            REQUIRE (e::is_empty(x0));
            REQUIRE (e::size(x0) == 0);

            e::erase_all(x1);

            REQUIRE (e::is_empty(x1));
            REQUIRE (e::size(x1) == 0);
        }
    }
}
