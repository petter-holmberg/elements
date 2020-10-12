#include "catch.hpp"

#include "rational.h"

namespace e = elements;

SCENARIO ("Using rational numbers", "[rational]")
{
    e::rational x{1, 2};
    e::rational y{2, 4};
    e::rational z{3, 4};

    static_assert(e::Field<decltype(x)>);

    SECTION ("Algebra")
    {
        REQUIRE (e::axiom_Field(x, y, z));

        REQUIRE (x == x);
        REQUIRE (x == y);
        REQUIRE (y != z);
        REQUIRE (y < z);
        REQUIRE (y >= x);
        REQUIRE (z >= y);
        REQUIRE (z > y);
        REQUIRE (x <= y);
        REQUIRE (y <= z);

        REQUIRE (x + y == y + x);
        REQUIRE (x * y == y * x);
        REQUIRE ((x + y) + z == x + (y + z));
        REQUIRE ((x * y) * z == x * (y * z));
        REQUIRE (x * (y + z) == x * y + x * z);
        REQUIRE ((x + y) * z == x * z + y * z);

        REQUIRE (x + y == e::rational{1, 1});
        REQUIRE (x + z == e::rational{5, 4});

        REQUIRE (+x == e::rational{1, 2});
        REQUIRE (-x == e::rational{-1, 2});

        REQUIRE (x - y == e::rational{0, 1});
        REQUIRE (x - z == e::rational{-1, 4});
        REQUIRE (z - x == e::rational{1, 4});

        REQUIRE (x * y == e::rational{1, 4});
        REQUIRE (x * z == e::rational{3, 8});

        REQUIRE (x / y == e::rational{1, 1});
        REQUIRE (x / z == e::rational{2, 3});
        REQUIRE (z / x == e::rational{3, 2});

        REQUIRE (x * 2 == e::rational{1, 1});
        REQUIRE (2 * z == e::rational{3, 2});
    }
}
