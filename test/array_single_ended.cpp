#include "catch.hpp"

#include "affine_space.h"
#include "array_single_ended.h"

namespace e = elements;

SCENARIO ("Using single-ended array", "[array_single_ended]")
{
    e::array_single_ended<int> x{0, 1, 2, 3, 4};
    static_assert(e::Dynamic_sequence<decltype(x), e::back<decltype(x)>>);
    static_assert(e::Affine_space<e::Position_type<decltype(x)>>);

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

    SECTION ("Comparing arrays")
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
            e::array_single_ended<int> y{0, 1, 2, 3};

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (!(x < y));
            REQUIRE (x >= y);
            REQUIRE (x > y);
            REQUIRE (!(x <= y));
        }

        {
            e::array_single_ended<int> y{0, 1, 2, 3, 4, 5};

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (x < y);
            REQUIRE (!(x >= y));
            REQUIRE (!(x > y));
            REQUIRE (x <= y);
        }

        {
            e::array_single_ended<int> y{0, -1, -2, -3, -4};

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (!(x < y));
            REQUIRE (x >= y);
            REQUIRE (x > y);
            REQUIRE (!(x <= y));
        }

        {
            e::array_single_ended<int> y{5, 6, 7, 8, 9};

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (x < y);
            REQUIRE (!(x >= y));
            REQUIRE (!(x > y));
            REQUIRE (x <= y);
        }
    }

    SECTION ("Copying arrays")
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
            e::array_single_ended<int> y{5, 6, 7, 8, 9};
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

    SECTION ("Checking capacity")
    {
        e::array_single_ended<int> x0(5);

        REQUIRE (e::is_empty(x0));
        REQUIRE (e::size(x0) == 0);
        REQUIRE (e::capacity(x0) == 5);

        e::push(x0, 0);
        e::push(x0, 1);
        e::push(x0, 2);

        REQUIRE (!e::is_empty(x0));
        REQUIRE (e::size(x0) == 3);
        CHECK (x0[0] == 0);
        CHECK (x0[1] == 1);
        CHECK (x0[2] == 2);
        REQUIRE (e::capacity(x0) == 5);

        e::push(x0, 3);
        e::push(x0, 4);
        e::push(x0, 5);

        REQUIRE (!e::is_empty(x0));
        REQUIRE (e::size(x0) == 6);
        CHECK (x0[0] == 0);
        CHECK (x0[1] == 1);
        CHECK (x0[2] == 2);
        CHECK (x0[3] == 3);
        CHECK (x0[4] == 4);
        CHECK (x0[5] == 5);
        REQUIRE (e::capacity(x0) == 10);

        e::reserve(x0, 15);
        REQUIRE (e::size(x0) == 6);
        REQUIRE (e::capacity(x0) == 15);
        CHECK (x0[0] == 0);
        CHECK (x0[1] == 1);
        CHECK (x0[2] == 2);
        CHECK (x0[3] == 3);
        CHECK (x0[4] == 4);
        CHECK (x0[5] == 5);

        e::pop(x0);
        REQUIRE (e::size(x0) == 5);
        REQUIRE (e::capacity(x0) == 15);
        REQUIRE (e::limit(x0) - e::first(x0) == 5);
        REQUIRE (e::limit_of_storage(x0) - e::first(x0) == 15);
        CHECK (x0[0] == 0);
        CHECK (x0[1] == 1);
        CHECK (x0[2] == 2);
        CHECK (x0[3] == 3);
        CHECK (x0[4] == 4);

        e::erase_all(x0);
        REQUIRE (e::size(x0) == 0);
        REQUIRE (e::capacity(x0) == 0);
        REQUIRE (e::limit(x0) - e::first(x0) == 0);
        REQUIRE (e::limit_of_storage(x0) - e::first(x0) == 0);
    }

    SECTION ("Monadic interface")
    {
        auto fn0 = [](int const& i){ return e::array_single_ended<int>{i, -i}; };
        auto fn1 = [](int const& i){ return i + 0.5; };

        static_assert(e::Monad<decltype(x), decltype(fn0)>);
        static_assert(e::Functor<decltype(x), decltype(fn1)>);

        auto y = x.flat_map(fn0).map(fn1);

        REQUIRE (e::size(y) == 10);
        REQUIRE (y[0] == 0.5);
        REQUIRE (y[1] == 0.5);
        REQUIRE (y[2] == 1.5);
        REQUIRE (y[3] == -0.5);
        REQUIRE (y[4] == 2.5);
        REQUIRE (y[5] == -1.5);
        REQUIRE (y[6] == 3.5);
        REQUIRE (y[7] == -2.5);
        REQUIRE (y[8] == 4.5);
        REQUIRE (y[9] == -3.5);
    }
}
