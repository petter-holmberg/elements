#include "catch.hpp"

#include "zip.h"

namespace e = elements;

SCENARIO ("Zipping and unzipping", "[zip]")
{
    SECTION ("Zipping")
    {
        int z[]{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

        SECTION ("Zipping two equally-sized arrays into one")
        {
            int x[]{0, 1, 2, 3, 4};
            int y[]{5, 6, 7, 8, 9};

            auto cur = e::zip(x, x + 5, y, y + 5, z);

            REQUIRE(cur == z + 10);

            CHECK (z[0] == 0);
            CHECK (z[1] == 5);
            CHECK (z[2] == 1);
            CHECK (z[3] == 6);
            CHECK (z[4] == 2);
            CHECK (z[5] == 7);
            CHECK (z[6] == 3);
            CHECK (z[7] == 8);
            CHECK (z[8] == 4);
            CHECK (z[9] == 9);
        }

        SECTION ("Zipping two arrays into one when the first one is longer")
        {
            int x[]{0, 1, 2, 3, 4};
            int y[]{5, 6, 7};

            auto cur = e::zip(x, x + 5, y, y + 3, z);

            REQUIRE(cur == z + 8);

            CHECK (z[0] == 0);
            CHECK (z[1] == 5);
            CHECK (z[2] == 1);
            CHECK (z[3] == 6);
            CHECK (z[4] == 2);
            CHECK (z[5] == 7);
            CHECK (z[6] == 3);
            CHECK (z[7] == 4);
            CHECK (z[8] == -1);
            CHECK (z[9] == -1);
        }

        SECTION ("Zipping two arrays into one when the second one is longer")
        {
            int x[]{0, 1, 2};
            int y[]{5, 6, 7, 8, 9};

            auto cur = e::zip(x, x + 3, y, y + 5, z);

            REQUIRE(cur == z + 8);

            CHECK (z[0] == 0);
            CHECK (z[1] == 5);
            CHECK (z[2] == 1);
            CHECK (z[3] == 6);
            CHECK (z[4] == 2);
            CHECK (z[5] == 7);
            CHECK (z[6] == 8);
            CHECK (z[7] == 9);
            CHECK (z[8] == -1);
            CHECK (z[9] == -1);
        }
    }

    SECTION ("Unzipping")
    {
        SECTION ("Unzipping an evenly-sized array into two")
        {
            int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            int y[]{-1, -1, -1, -1, -1};
            int z[]{-1, -1, -1, -1, -1};

            auto cur = e::unzip(x, x + 10, y, z);

            REQUIRE(cur.m0 == y + 5);
            REQUIRE(cur.m1 == z + 5);

            CHECK (y[0] == 0);
            CHECK (y[1] == 2);
            CHECK (y[2] == 4);
            CHECK (y[3] == 6);
            CHECK (y[4] == 8);
            CHECK (z[0] == 1);
            CHECK (z[1] == 3);
            CHECK (z[2] == 5);
            CHECK (z[3] == 7);
            CHECK (z[4] == 9);
        }

        SECTION ("Unzipping an odd-sized array into two")
        {
            int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8};
            int y[]{-1, -1, -1, -1, -1};
            int z[]{-1, -1, -1, -1, -1};

            auto cur = e::unzip(x, x + 9, y, z);

            REQUIRE(cur.m0 == y + 5);
            REQUIRE(cur.m1 == z + 4);

            CHECK (y[0] == 0);
            CHECK (y[1] == 2);
            CHECK (y[2] == 4);
            CHECK (y[3] == 6);
            CHECK (y[4] == 8);
            CHECK (z[0] == 1);
            CHECK (z[1] == 3);
            CHECK (z[2] == 5);
            CHECK (z[3] == 7);
            CHECK (z[4] == -1);
        }
    }
}
