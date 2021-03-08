#include "catch.hpp"

#include "affine_space.h"
#include "array_segmented_double_ended.h"

namespace e = elements;

struct s
{
    e::array_segmented_double_ended<s> x;
};

SCENARIO ("Using segmented double-ended array", "[array_segmented_double_ended]")
{
    e::array_segmented_double_ended<int> x;
    e::emplace(x, 0);
    e::emplace(x, 1);
    e::emplace(x, 2);
    e::emplace(x, 3);
    e::emplace(x, 4);

    static_assert(e::Dynamic_sequence<decltype(x), e::back<decltype(x)>>);
    static_assert(e::Affine_space<e::Index_cursor_type<e::Cursor_type<decltype(x)>>>);
    static_assert(e::Affine_space<e::Segment_cursor_type<e::Cursor_type<decltype(x)>>>);

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
            e::array_segmented_double_ended<int> y;
            e::emplace(y, 0);
            e::emplace(y, 1);
            e::emplace(y, 2);
            e::emplace(y, 3);

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (!(x < y));
            REQUIRE (x >= y);
            REQUIRE (x > y);
            REQUIRE (!(x <= y));
        }

        {
            e::array_segmented_double_ended<int> y;
            e::emplace(y, 0);
            e::emplace(y, 1);
            e::emplace(y, 2);
            e::emplace(y, 3);
            e::emplace(y, 4);
            e::emplace(y, 5);

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (x < y);
            REQUIRE (!(x >= y));
            REQUIRE (!(x > y));
            REQUIRE (x <= y);
        }

        {
            e::array_segmented_double_ended<int> y;
            e::emplace(y, 0);
            e::emplace(y, -1);
            e::emplace(y, -2);
            e::emplace(y, -3);
            e::emplace(y, -4);

            REQUIRE (!(x == y));
            REQUIRE (x != y);
            REQUIRE (!(x < y));
            REQUIRE (x >= y);
            REQUIRE (x > y);
            REQUIRE (!(x <= y));
        }

        {
            e::array_segmented_double_ended<int> y;
            e::emplace(y, 5);
            e::emplace(y, 6);
            e::emplace(y, 7);
            e::emplace(y, 8);
            e::emplace(y, 9);

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
            e::array_segmented_double_ended<int> y;
            e::emplace(y, 5);
            e::emplace(y, 6);
            e::emplace(y, 7);
            e::emplace(y, 8);
            e::emplace(y, 9);

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

    SECTION("Growing array")
    {
        e::array_segmented_double_ended<int, 3> y;
        REQUIRE (e::size(y) == 0);

        e::push(y, 0);
        REQUIRE (e::size(y) == 1);
        REQUIRE (e::capacity(y) == 3);
        REQUIRE (y[0] == 0);

        e::push(y, 1);
        REQUIRE (e::size(y) == 2);
        REQUIRE (e::capacity(y) == 3);
        REQUIRE (y[0] == 0);
        REQUIRE (y[1] == 1);

        e::push(y, 2);
        REQUIRE (e::size(y) == 3);
        REQUIRE (e::capacity(y) == 6);
        REQUIRE (y[0] == 0);
        REQUIRE (y[1] == 1);
        REQUIRE (y[2] == 2);

        e::push(y, 3);
        REQUIRE (e::size(y) == 4);
        REQUIRE (e::capacity(y) == 6);
        REQUIRE (y[0] == 0);
        REQUIRE (y[1] == 1);
        REQUIRE (y[2] == 2);
        REQUIRE (y[3] == 3);

        e::push_first(y, -1);
        REQUIRE (e::size(y) == 5);
        REQUIRE (e::capacity(y) == 6);
        REQUIRE (y[0] == -1);
        REQUIRE (y[1] == 0);
        REQUIRE (y[2] == 1);
        REQUIRE (y[3] == 2);
        REQUIRE (y[4] == 3);

        e::push_first(y, -2);
        REQUIRE (e::size(y) == 6);
        REQUIRE (e::capacity(y) == 9);
        REQUIRE (y[0] == -2);
        REQUIRE (y[1] == -1);
        REQUIRE (y[2] == 0);
        REQUIRE (y[3] == 1);
        REQUIRE (y[4] == 2);
        REQUIRE (y[5] == 3);

        e::push_first(y, -3);
        REQUIRE (e::size(y) == 7);
        REQUIRE (e::capacity(y) == 9);
        REQUIRE (y[0] == -3);
        REQUIRE (y[1] == -2);
        REQUIRE (y[2] == -1);
        REQUIRE (y[3] == 0);
        REQUIRE (y[4] == 1);
        REQUIRE (y[5] == 2);
        REQUIRE (y[6] == 3);
    }

    SECTION ("Monadic interface")
    {
        auto fn0 = [](int const& i){
            e::array_segmented_double_ended<int> ret;
            e::emplace(ret, i);
            e::emplace(ret, -i);
            return ret;
        };
        auto fn1 = [](int const& i){ return i + 0.5; };

        static_assert(e::Functor<decltype(x)>);
        static_assert(e::Monad<decltype(x)>);

        auto y = e::fmap(e::chain(x, fn0), fn1);

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

        auto fn2 = [](int const& i){
            e::array_segmented_double_ended<int, 3> ret;
            e::emplace(ret, i);
            e::emplace(ret, -i);
            return ret;
        };
        e::array_segmented_double_ended<int, 3> z;
        e::emplace(z, 0);
        e::emplace(z, 1);
        e::emplace(z, 2);
        e::emplace(z, 3);
        e::emplace(z, 4);
        e::emplace(z, 5);
        e::emplace(z, 6);
        e::emplace(z, 7);

        auto w = e::fmap(e::chain(z, fn2), fn1);

        REQUIRE (e::size(w) == 16);
        REQUIRE (w[0] == 0.5);
        REQUIRE (w[1] == 0.5);
        REQUIRE (w[2] == 1.5);
        REQUIRE (w[3] == -0.5);
        REQUIRE (w[4] == 2.5);
        REQUIRE (w[5] == -1.5);
        REQUIRE (w[6] == 3.5);
        REQUIRE (w[7] == -2.5);
        REQUIRE (w[8] == 4.5);
        REQUIRE (w[9] == -3.5);
        REQUIRE (w[10] == 5.5);
        REQUIRE (w[11] == -4.5);
        REQUIRE (w[12] == 6.5);
        REQUIRE (w[13] == -5.5);
        REQUIRE (w[14] == 7.5);
        REQUIRE (w[15] == -6.5);
    }
}
