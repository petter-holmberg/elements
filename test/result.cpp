#include "catch.hpp"

#include "result.h"
#include "functional.h"

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

        static_assert(e::Functor<decltype(x)>);
        static_assert(e::Monad<decltype(x)>);

        x = e::fmap(e::mv(x), fn0);
        auto y = e::fmap(e::mv(x), fn1);
        y = e::chain(y, fn2);

        REQUIRE (y);
        REQUIRE (load(y) == 3.0);

        y = e::chain(y, fn3);

        REQUIRE (!y);
        REQUIRE (e::error(y));

        auto z(e::fail<int>(true));

        z = e::fmap(e::mv(z), fn0);
        REQUIRE (!z);
        REQUIRE (e::error(z));

        auto w = e::fmap(e::mv(z), fn1);
        REQUIRE (!w);
        REQUIRE (e::error(w));

        w = e::result<double, bool>(0.5);
    }
}
