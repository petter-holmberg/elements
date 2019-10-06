#include "catch.hpp"

#include "reverse.h"

namespace e = elements;

SCENARIO ("Using reverse", "[reverse]")
{
    int x[]{0, 1, 2, 3, 4};

    SECTION ("Bidirectional position")
    {
        auto first = e::bidirectional_position{x};
        auto limit = e::bidirectional_position{x + 5};

        SECTION ("Reversing")
        {
            e::reverse(first, limit);

            CHECK (x[0] == 4);
            CHECK (x[1] == 3);
            CHECK (x[2] == 2);
            CHECK (x[3] == 1);
            CHECK (x[4] == 0);

            e::reverse(first, limit);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }
    }
}
