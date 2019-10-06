#include "catch.hpp"

#include "result.h"

namespace e = elements;

SCENARIO ("Using result", "[result]")
{
    e::result<int, bool> x;

    static_assert(e::Totally_ordered<decltype(x)>);

    SECTION ("Total ordering")
    {
        e::result<int, bool> y(1);
        REQUIRE (e::axiom_Totally_ordered(x, y));

        auto z(e::fail<int>(true));

        REQUIRE (e::axiom_Totally_ordered(z, y));
    }

    SECTION ("Monadic interface")
    {
        auto fn0 = [](int const& a){ return a + 1; };
        auto fn1 = [](int a){ return a + 0.5; };
        auto fn2 = [](double a) { return e::result<double, bool>(a * 2); };
        auto fn3 = [](double){ return e::fail<double>(true); };

        x.map(fn0);
        auto y = x.map(fn1);
        y = y.flat_map(fn2);

        REQUIRE (y);
        REQUIRE (load(y) == 3.0);

        y = y.flat_map(fn3);

        REQUIRE (!y);
        REQUIRE (e::error(y));

        auto z(e::fail<int>(true));

        z.map(fn0);
        REQUIRE (!z);
        REQUIRE (e::error(z));

        auto w = z.map(fn1);
        REQUIRE (!w);
        REQUIRE (e::error(w));

        w = e::result<double, bool>(0.5);
    }
}
