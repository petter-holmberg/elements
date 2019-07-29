#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Using expected", "[expected]")
{
    e::expected<int, bool> x;

    static_assert(e::Totally_ordered<decltype(x)>);

    SECTION ("Total ordering")
    {
        e::expected<int, bool> y(1);
        REQUIRE (e::axiom_Totally_ordered(x, y));

        auto z(e::make_unexpected<int>(true));

        REQUIRE (e::axiom_Totally_ordered(z, y));
    }

    SECTION ("Monadic interface")
    {
        auto fn0 = [](int const& a){ return a + 1; };
        auto fn1 = [](int a){ return a + 0.5; };
        auto fn2 = [](double a) { return e::expected<double, bool>(a * 2); };
        auto fn3 = [](double){ return e::make_unexpected<double>(true); };

        x.map(fn0);
        auto y = x.map(fn1);
        y = y.bind(fn2);

        REQUIRE (y);
        REQUIRE (load(y) == 3.0);

        y = y.bind(fn3);

        REQUIRE (!y);
        REQUIRE (e::error(y));

        auto z(e::make_unexpected<int>(true));

        z.map(fn0);
        REQUIRE (!z);
        REQUIRE (e::error(z));

        auto w = z.map(fn1);
        REQUIRE (!w);
        REQUIRE (e::error(w));

        w = e::expected<double, bool>(0.5);
    }
}
