#include "catch.hpp"

#include "elements.h"

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
    return x != y;
}

namespace elements {

template <>
struct less<s>
{
    auto operator()(s const& a, s const& b) -> bool
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
            REQUIRE (e::select_0_2(a, b, e::less<s>{}) == a);
            REQUIRE (e::select_0_2(b, a, e::less<s>{}) == b);
            REQUIRE (e::select_0_2(b, c, e::less<s>{}) == b);
            REQUIRE (e::select_0_2(c, b, e::less<s>{}) == b);

            REQUIRE (e::min(a, b) == a);
            REQUIRE (e::min(b, a) == b);
            REQUIRE (e::min(b, c) == b);
            REQUIRE (e::min(c, b) == b);

            REQUIRE (e::select_1_2(a, b, e::less<s>{}) == b);
            REQUIRE (e::select_1_2(b, a, e::less<s>{}) == a);
            REQUIRE (e::select_1_2(c, d, e::less<s>{}) == d);
            REQUIRE (e::select_1_2(d, c, e::less<s>{}) == c);

            REQUIRE (e::max(a, b) == b);
            REQUIRE (e::max(b, a) == a);
            REQUIRE (e::max(c, d) == d);
            REQUIRE (e::max(d, c) == c);

            REQUIRE (e::select_0_3(a, b, c, e::less<s>{}) == a);
            REQUIRE (e::select_0_3(a, c, b, e::less<s>{}) == a);
            REQUIRE (e::select_0_3(b, a, c, e::less<s>{}) == b);
            REQUIRE (e::select_0_3(b, c, a, e::less<s>{}) == b);
            REQUIRE (e::select_0_3(c, a, b, e::less<s>{}) == a);
            REQUIRE (e::select_0_3(c, b, a, e::less<s>{}) == b);

            REQUIRE (e::select_1_3(a, b, c, e::less<s>{}) == b);
            REQUIRE (e::select_1_3(a, c, b, e::less<s>{}) == b);
            REQUIRE (e::select_1_3(b, a, c, e::less<s>{}) == a);
            REQUIRE (e::select_1_3(b, c, a, e::less<s>{}) == a);
            REQUIRE (e::select_1_3(c, a, b, e::less<s>{}) == b);
            REQUIRE (e::select_1_3(c, b, a, e::less<s>{}) == a);

            REQUIRE (e::median(a, b, c) == b);
            REQUIRE (e::median(a, c, b) == b);
            REQUIRE (e::median(b, a, c) == a);
            REQUIRE (e::median(b, c, a) == a);
            REQUIRE (e::median(c, a, b) == b);
            REQUIRE (e::median(c, b, a) == a);

            REQUIRE (e::select_2_3(b, c, d, e::less<s>{}) == d);
            REQUIRE (e::select_2_3(b, d, c, e::less<s>{}) == c);
            REQUIRE (e::select_2_3(c, b, d, e::less<s>{}) == d);
            REQUIRE (e::select_2_3(c, d, b, e::less<s>{}) == d);
            REQUIRE (e::select_2_3(d, b, c, e::less<s>{}) == c);
            REQUIRE (e::select_2_3(d, c, b, e::less<s>{}) == c);
        }
    }
}
