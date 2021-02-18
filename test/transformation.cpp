#include "catch.hpp"

#include "transformation.h"

namespace e = elements;

SCENARIO ("Distance", "[distance]")
{
    SECTION ("Calculating the distance between values under a transformation")
    {
        auto inc_2 = [](int x){ return x + 2; };

        static_assert(e::Transformation<decltype(inc_2), int>);
        static_assert(e::Same_as<e::Distance_type<decltype(inc_2), int>, unsigned int>);

        REQUIRE (e::distance(0, 0, inc_2) == 0);
        REQUIRE (e::distance(0, 10, inc_2) == 5);
    }

    SECTION ("Calculating collision point when a transformation is terminating")
    {
        auto inc_2 = [](int x){ return x + 2; };
        auto is_limit = [](int x){ return x < 10; };

        REQUIRE (e::collision_point(0, inc_2, is_limit) == 10);
        REQUIRE (e::is_terminating(0, inc_2, is_limit));
        REQUIRE (!e::is_circular(0, inc_2, is_limit));
        REQUIRE (e::connection_point(0, inc_2, is_limit) == 10);
        REQUIRE (e::orbit_structure(0, inc_2, is_limit).handle_distance == 5);
        REQUIRE (e::orbit_structure(0, inc_2, is_limit).cycle_distance == 0);
        REQUIRE (e::orbit_structure(0, inc_2, is_limit).connection == 10);
    }

    SECTION ("Calculating collision point when a transformation is circular")
    {
        auto inc_2_mod_10 = [](int x){ return (x + 2) % 10; };
        auto is_limit = [](int x){ return x < 10; };

        REQUIRE (e::collision_point(0, inc_2_mod_10, is_limit) == 8);
        REQUIRE (!e::is_terminating(0, inc_2_mod_10, is_limit));
        REQUIRE (e::is_circular(0, inc_2_mod_10, is_limit));
        REQUIRE (e::connection_point(0, inc_2_mod_10, is_limit) == 0);
        REQUIRE (e::orbit_structure(0, inc_2_mod_10, is_limit).handle_distance == 0);
        REQUIRE (e::orbit_structure(0, inc_2_mod_10, is_limit).cycle_distance == 4);
        REQUIRE (e::orbit_structure(0, inc_2_mod_10, is_limit).connection == 0);
        REQUIRE (e::orbit_structure(2, inc_2_mod_10, is_limit).connection == 2);
    }

    SECTION ("Calculating collision point when a transformation is rho-shaped")
    {
        auto inc_2_rho = [](int x){ return x == 10 ? 2 : x + 2; };
        auto is_limit = [](int x){ return x <= 10; };

        REQUIRE (e::collision_point(0, inc_2_rho, is_limit) == 8);
        REQUIRE (!e::is_terminating(0, inc_2_rho, is_limit));
        REQUIRE (!e::is_circular(0, inc_2_rho, is_limit));
        REQUIRE (e::connection_point(0, inc_2_rho, is_limit) == 2);
        REQUIRE (e::orbit_structure(0, inc_2_rho, is_limit).handle_distance == 1);
        REQUIRE (e::orbit_structure(0, inc_2_rho, is_limit).cycle_distance == 4);
        REQUIRE (e::orbit_structure(0, inc_2_rho, is_limit).connection == 2);
    }
}
