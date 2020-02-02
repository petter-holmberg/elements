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

        static_assert(e::Monad<decltype(x)>);
        static_assert(e::Functor<decltype(x)>);

        x.fmap(fn0);
        auto y = x.fmap(fn1);
        y = e::chain(y, fn2);

        REQUIRE (y);
        REQUIRE (load(y) == 3.0);

        y = e::chain(y, fn3);

        REQUIRE (!y);
        REQUIRE (e::error(y));

        auto z(e::fail<int>(true));

        z.fmap(fn0);
        REQUIRE (!z);
        REQUIRE (e::error(z));

        auto w = z.fmap(fn1);
        REQUIRE (!w);
        REQUIRE (e::error(w));

        w = e::result<double, bool>(0.5);
    }
}
