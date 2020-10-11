#include "catch.hpp"

#include "ordering.h"

namespace e = elements;

struct s
{
    s() = default;

    s(int value_, int stability_index_)
        : value{value_}, stability_index{stability_index_}
    {}

    int value{0};
    int stability_index{0};
};

auto operator==(s const& x, s const& y) -> bool
{
    return x.value == y.value and x.stability_index == y.stability_index;
}

auto operator!=(s const& x, s const& y) -> bool
{
    return !(x == y);
}

namespace elements {

template <>
struct lt<s>
{
    constexpr auto
    operator()(s const& a, s const& b) const -> bool
    {
        return a.value < b.value;
    }
};

}

SCENARIO ("Checking ordering", "[ordering]")
{
    GIVEN ("Three values")
    {
        s a{0, 0};
        s b{0, 1};
        s c{1, 0};
        s d{1, 1};

        THEN ("they are stably selected")
        {
            REQUIRE (e::select_0_2(a, b, e::lt{}) == a);
            REQUIRE (e::select_0_2(b, a, e::lt{}) == b);
            REQUIRE (e::select_0_2(b, c, e::lt{}) == b);
            REQUIRE (e::select_0_2(c, b, e::lt{}) == b);

            REQUIRE (e::min(a, b) == a);
            REQUIRE (e::min(b, a) == b);
            REQUIRE (e::min(b, c) == b);
            REQUIRE (e::min(c, b) == b);

            REQUIRE (e::select_1_2(a, b, e::lt{}) == b);
            REQUIRE (e::select_1_2(b, a, e::lt{}) == a);
            REQUIRE (e::select_1_2(c, d, e::lt{}) == d);
            REQUIRE (e::select_1_2(d, c, e::lt{}) == c);

            REQUIRE (e::max(a, b) == b);
            REQUIRE (e::max(b, a) == a);
            REQUIRE (e::max(c, d) == d);
            REQUIRE (e::max(d, c) == c);

            REQUIRE (e::select_0_3(a, b, c, e::lt{}) == a);
            REQUIRE (e::select_0_3(a, c, b, e::lt{}) == a);
            REQUIRE (e::select_0_3(b, a, c, e::lt{}) == b);
            REQUIRE (e::select_0_3(b, c, a, e::lt{}) == b);
            REQUIRE (e::select_0_3(c, a, b, e::lt{}) == a);
            REQUIRE (e::select_0_3(c, b, a, e::lt{}) == b);

            REQUIRE (e::select_1_3(a, b, c, e::lt{}) == b);
            REQUIRE (e::select_1_3(a, c, b, e::lt{}) == b);
            REQUIRE (e::select_1_3(b, a, c, e::lt{}) == a);
            REQUIRE (e::select_1_3(b, c, a, e::lt{}) == a);
            REQUIRE (e::select_1_3(c, a, b, e::lt{}) == b);
            REQUIRE (e::select_1_3(c, b, a, e::lt{}) == a);

            REQUIRE (e::median(a, b, c) == b);
            REQUIRE (e::median(a, c, b) == b);
            REQUIRE (e::median(b, a, c) == a);
            REQUIRE (e::median(b, c, a) == a);
            REQUIRE (e::median(c, a, b) == b);
            REQUIRE (e::median(c, b, a) == a);

            REQUIRE (e::select_2_3(b, c, d, e::lt{}) == d);
            REQUIRE (e::select_2_3(b, d, c, e::lt{}) == c);
            REQUIRE (e::select_2_3(c, b, d, e::lt{}) == d);
            REQUIRE (e::select_2_3(c, d, b, e::lt{}) == d);
            REQUIRE (e::select_2_3(d, b, c, e::lt{}) == c);
            REQUIRE (e::select_2_3(d, c, b, e::lt{}) == c);
        }
    }
}
