#include "catch.hpp"

#include "functional.h"
#include "pair.h"

namespace e = elements;

SCENARIO ("Using pairs", "[pair]")
{
    static_assert(e::Totally_ordered<e::pair<int>>);

    SECTION ("Creating pairs of move-only objects")
    {
        e::pair x(e::loadable_position(0), 1);

        e::loadable_position pos(1);
        e::pair y(0, e::mv(pos));
    }

    SECTION ("Getting pair elements")
    {
        e::pair x(0, 1);

        REQUIRE (e::get<0>(x) == 0);
        REQUIRE (e::get<1>(x) == 1);

        REQUIRE (e::get<0>(e::pair{0, 1}) == 0);
        REQUIRE (e::get<1>(e::pair{0, 1}) == 1);
    }

    SECTION ("Getting pair elements of different type")
    {
        e::pair x(0, true);

        REQUIRE (e::get<int>(x) == 0);
        REQUIRE (e::get<bool>(x) == true);

        REQUIRE (e::get<int>(e::pair{0, true}) == 0);
        REQUIRE (e::get<bool>(e::pair{0, true}) == true);
   }

    SECTION ("Functor interface")
    {
        e::pair x(0, 1);

        auto fn = [](int const& i){ return i + 0.5; };

        static_assert(e::Functor<decltype(x), decltype(fn)>);

        auto y = x.map(fn);

        REQUIRE (e::get<0>(y) == 0.5);
        REQUIRE (e::get<1>(y) == 1.5);
    }
}
