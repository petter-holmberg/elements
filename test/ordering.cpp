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

            REQUIRE (e::select_1_3_ac(a, b, c, e::lt{}) == b);
            REQUIRE (e::select_1_3_ac(a, c, b, e::lt{}) == b);
            REQUIRE (e::select_1_3_ac(b, a, c, e::lt{}) == a);
            REQUIRE (e::select_1_3_ac(b, c, a, e::lt{}) == a);

            REQUIRE (e::clamp(a, b, c) == b);
            REQUIRE (e::clamp(a, c, b) == b);
            REQUIRE (e::clamp(b, a, c) == a);
            REQUIRE (e::clamp(b, c, a) == a);

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

            REQUIRE (e::select_1_4_ab_cd(a, c, b, d, e::lt{}) == b);
            REQUIRE (e::select_1_4_ab_cd(a, d, b, c, e::lt{}) == b);
            REQUIRE (e::select_1_4_ab_cd(b, c, a, d, e::lt{}) == a);
            REQUIRE (e::select_1_4_ab_cd(b, d, a, c, e::lt{}) == a);

            REQUIRE (e::select_2_4_ab_cd(a, c, b, d, e::lt{}) == c);
            REQUIRE (e::select_2_4_ab_cd(a, d, b, c, e::lt{}) == d);
            REQUIRE (e::select_2_4_ab_cd(b, c, a, d, e::lt{}) == c);
            REQUIRE (e::select_2_4_ab_cd(b, d, a, c, e::lt{}) == d);

            REQUIRE (e::select_1_4_ab(a, c, b, d, e::lt{}) == b);
            REQUIRE (e::select_1_4_ab(a, c, d, b, e::lt{}) == b);
            REQUIRE (e::select_1_4_ab(a, d, b, c, e::lt{}) == b);
            REQUIRE (e::select_1_4_ab(a, d, c, b, e::lt{}) == b);
            REQUIRE (e::select_1_4_ab(b, c, a, d, e::lt{}) == a);
            REQUIRE (e::select_1_4_ab(b, c, d, a, e::lt{}) == a);
            REQUIRE (e::select_1_4_ab(b, d, a, c, e::lt{}) == a);
            REQUIRE (e::select_1_4_ab(b, d, c, a, e::lt{}) == a);

            REQUIRE (e::select_2_4_cd(b, d, a, c, e::lt{}) == d);
            REQUIRE (e::select_2_4_cd(d, b, a, c, e::lt{}) == d);
            REQUIRE (e::select_2_4_cd(b, c, a, d, e::lt{}) == c);
            REQUIRE (e::select_2_4_cd(c, b, a, d, e::lt{}) == c);
            REQUIRE (e::select_2_4_cd(a, d, b, c, e::lt{}) == d);
            REQUIRE (e::select_2_4_cd(d, a, b, c, e::lt{}) == d);
            REQUIRE (e::select_2_4_cd(a, c, b, d, e::lt{}) == c);
            REQUIRE (e::select_2_4_cd(c, a, b, d, e::lt{}) == c);

            REQUIRE (e::select_0_4(a, b, c, d, e::lt{}) == a);
            REQUIRE (e::select_0_4(a, b, d, c, e::lt{}) == a);
            REQUIRE (e::select_0_4(a, c, b, d, e::lt{}) == a);
            REQUIRE (e::select_0_4(a, c, d, b, e::lt{}) == a);
            REQUIRE (e::select_0_4(a, d, b, c, e::lt{}) == a);
            REQUIRE (e::select_0_4(a, d, c, b, e::lt{}) == a);
            REQUIRE (e::select_0_4(b, a, c, d, e::lt{}) == b);
            REQUIRE (e::select_0_4(b, a, d, c, e::lt{}) == b);
            REQUIRE (e::select_0_4(b, c, a, d, e::lt{}) == b);
            REQUIRE (e::select_0_4(b, c, d, a, e::lt{}) == b);
            REQUIRE (e::select_0_4(b, d, a, c, e::lt{}) == b);
            REQUIRE (e::select_0_4(b, d, c, a, e::lt{}) == b);
            REQUIRE (e::select_0_4(c, a, b, d, e::lt{}) == a);
            REQUIRE (e::select_0_4(c, a, d, b, e::lt{}) == a);
            REQUIRE (e::select_0_4(c, b, a, d, e::lt{}) == b);
            REQUIRE (e::select_0_4(c, b, d, a, e::lt{}) == b);
            REQUIRE (e::select_0_4(c, d, a, b, e::lt{}) == a);
            REQUIRE (e::select_0_4(c, d, b, a, e::lt{}) == b);
            REQUIRE (e::select_0_4(d, a, b, c, e::lt{}) == a);
            REQUIRE (e::select_0_4(d, a, c, b, e::lt{}) == a);
            REQUIRE (e::select_0_4(d, b, a, c, e::lt{}) == b);
            REQUIRE (e::select_0_4(d, b, c, a, e::lt{}) == b);
            REQUIRE (e::select_0_4(d, c, a, b, e::lt{}) == a);
            REQUIRE (e::select_0_4(d, c, b, a, e::lt{}) == b);

            REQUIRE (e::min(a, b, c, d) == a);
            REQUIRE (e::min(a, b, d, c) == a);
            REQUIRE (e::min(a, c, b, d) == a);
            REQUIRE (e::min(a, c, d, b) == a);
            REQUIRE (e::min(a, d, b, c) == a);
            REQUIRE (e::min(a, d, c, b) == a);
            REQUIRE (e::min(b, a, c, d) == b);
            REQUIRE (e::min(b, a, d, c) == b);
            REQUIRE (e::min(b, c, a, d) == b);
            REQUIRE (e::min(b, c, d, a) == b);
            REQUIRE (e::min(b, d, a, c) == b);
            REQUIRE (e::min(b, d, c, a) == b);
            REQUIRE (e::min(c, a, b, d) == a);
            REQUIRE (e::min(c, a, d, b) == a);
            REQUIRE (e::min(c, b, a, d) == b);
            REQUIRE (e::min(c, b, d, a) == b);
            REQUIRE (e::min(c, d, a, b) == a);
            REQUIRE (e::min(c, d, b, a) == b);
            REQUIRE (e::min(d, a, b, c) == a);
            REQUIRE (e::min(d, a, c, b) == a);
            REQUIRE (e::min(d, b, a, c) == b);
            REQUIRE (e::min(d, b, c, a) == b);
            REQUIRE (e::min(d, c, a, b) == a);
            REQUIRE (e::min(d, c, b, a) == b);

            REQUIRE (e::select_1_4(a, b, c, d, e::lt{}) == b);
            REQUIRE (e::select_1_4(a, b, d, c, e::lt{}) == b);
            REQUIRE (e::select_1_4(a, c, b, d, e::lt{}) == b);
            REQUIRE (e::select_1_4(a, c, d, b, e::lt{}) == b);
            REQUIRE (e::select_1_4(a, d, b, c, e::lt{}) == b);
            REQUIRE (e::select_1_4(a, d, c, b, e::lt{}) == b);
            REQUIRE (e::select_1_4(b, a, c, d, e::lt{}) == a);
            REQUIRE (e::select_1_4(b, a, d, c, e::lt{}) == a);
            REQUIRE (e::select_1_4(b, c, a, d, e::lt{}) == a);
            REQUIRE (e::select_1_4(b, c, d, a, e::lt{}) == a);
            REQUIRE (e::select_1_4(b, d, a, c, e::lt{}) == a);
            REQUIRE (e::select_1_4(b, d, c, a, e::lt{}) == a);
            REQUIRE (e::select_1_4(c, a, b, d, e::lt{}) == b);
            REQUIRE (e::select_1_4(c, a, d, b, e::lt{}) == b);
            REQUIRE (e::select_1_4(c, b, a, d, e::lt{}) == a);
            REQUIRE (e::select_1_4(c, b, d, a, e::lt{}) == a);
            REQUIRE (e::select_1_4(c, d, a, b, e::lt{}) == b);
            REQUIRE (e::select_1_4(c, d, b, a, e::lt{}) == a);
            REQUIRE (e::select_1_4(d, a, b, c, e::lt{}) == b);
            REQUIRE (e::select_1_4(d, a, c, b, e::lt{}) == b);
            REQUIRE (e::select_1_4(d, b, a, c, e::lt{}) == a);
            REQUIRE (e::select_1_4(d, b, c, a, e::lt{}) == a);
            REQUIRE (e::select_1_4(d, c, a, b, e::lt{}) == b);
            REQUIRE (e::select_1_4(d, c, b, a, e::lt{}) == a);

            REQUIRE (e::select_2_4(a, b, c, d, e::lt{}) == c);
            REQUIRE (e::select_2_4(a, b, d, c, e::lt{}) == d);
            REQUIRE (e::select_2_4(a, c, b, d, e::lt{}) == c);
            REQUIRE (e::select_2_4(a, c, d, b, e::lt{}) == c);
            REQUIRE (e::select_2_4(a, d, b, c, e::lt{}) == d);
            REQUIRE (e::select_2_4(a, d, c, b, e::lt{}) == d);
            REQUIRE (e::select_2_4(b, a, c, d, e::lt{}) == c);
            REQUIRE (e::select_2_4(b, a, d, c, e::lt{}) == d);
            REQUIRE (e::select_2_4(b, c, a, d, e::lt{}) == c);
            REQUIRE (e::select_2_4(b, c, d, a, e::lt{}) == c);
            REQUIRE (e::select_2_4(b, d, a, c, e::lt{}) == d);
            REQUIRE (e::select_2_4(b, d, c, a, e::lt{}) == d);
            REQUIRE (e::select_2_4(c, a, b, d, e::lt{}) == c);
            REQUIRE (e::select_2_4(c, a, d, b, e::lt{}) == c);
            REQUIRE (e::select_2_4(c, b, a, d, e::lt{}) == c);
            REQUIRE (e::select_2_4(c, b, d, a, e::lt{}) == c);
            REQUIRE (e::select_2_4(c, d, a, b, e::lt{}) == c);
            REQUIRE (e::select_2_4(c, d, b, a, e::lt{}) == c);
            REQUIRE (e::select_2_4(d, a, b, c, e::lt{}) == d);
            REQUIRE (e::select_2_4(d, a, c, b, e::lt{}) == d);
            REQUIRE (e::select_2_4(d, b, a, c, e::lt{}) == d);
            REQUIRE (e::select_2_4(d, b, c, a, e::lt{}) == d);
            REQUIRE (e::select_2_4(d, c, a, b, e::lt{}) == d);
            REQUIRE (e::select_2_4(d, c, b, a, e::lt{}) == d);

            REQUIRE (e::select_3_4(a, b, c, d, e::lt{}) == d);
            REQUIRE (e::select_3_4(a, b, d, c, e::lt{}) == c);
            REQUIRE (e::select_3_4(a, c, b, d, e::lt{}) == d);
            REQUIRE (e::select_3_4(a, c, d, b, e::lt{}) == d);
            REQUIRE (e::select_3_4(a, d, b, c, e::lt{}) == c);
            REQUIRE (e::select_3_4(a, d, c, b, e::lt{}) == c);
            REQUIRE (e::select_3_4(b, a, c, d, e::lt{}) == d);
            REQUIRE (e::select_3_4(b, a, d, c, e::lt{}) == c);
            REQUIRE (e::select_3_4(b, c, a, d, e::lt{}) == d);
            REQUIRE (e::select_3_4(b, c, d, a, e::lt{}) == d);
            REQUIRE (e::select_3_4(b, d, a, c, e::lt{}) == c);
            REQUIRE (e::select_3_4(b, d, c, a, e::lt{}) == c);
            REQUIRE (e::select_3_4(c, a, b, d, e::lt{}) == d);
            REQUIRE (e::select_3_4(c, a, d, b, e::lt{}) == d);
            REQUIRE (e::select_3_4(c, b, a, d, e::lt{}) == d);
            REQUIRE (e::select_3_4(c, b, d, a, e::lt{}) == d);
            REQUIRE (e::select_3_4(c, d, a, b, e::lt{}) == d);
            REQUIRE (e::select_3_4(c, d, b, a, e::lt{}) == d);
            REQUIRE (e::select_3_4(d, a, b, c, e::lt{}) == c);
            REQUIRE (e::select_3_4(d, a, c, b, e::lt{}) == c);
            REQUIRE (e::select_3_4(d, b, a, c, e::lt{}) == c);
            REQUIRE (e::select_3_4(d, b, c, a, e::lt{}) == c);
            REQUIRE (e::select_3_4(d, c, a, b, e::lt{}) == c);
            REQUIRE (e::select_3_4(d, c, b, a, e::lt{}) == c);

            REQUIRE (e::max(a, b, c, d) == d);
            REQUIRE (e::max(a, b, d, c) == c);
            REQUIRE (e::max(a, c, b, d) == d);
            REQUIRE (e::max(a, c, d, b) == d);
            REQUIRE (e::max(a, d, b, c) == c);
            REQUIRE (e::max(a, d, c, b) == c);
            REQUIRE (e::max(b, a, c, d) == d);
            REQUIRE (e::max(b, a, d, c) == c);
            REQUIRE (e::max(b, c, a, d) == d);
            REQUIRE (e::max(b, c, d, a) == d);
            REQUIRE (e::max(b, d, a, c) == c);
            REQUIRE (e::max(b, d, c, a) == c);
            REQUIRE (e::max(c, a, b, d) == d);
            REQUIRE (e::max(c, a, d, b) == d);
            REQUIRE (e::max(c, b, a, d) == d);
            REQUIRE (e::max(c, b, d, a) == d);
            REQUIRE (e::max(c, d, a, b) == d);
            REQUIRE (e::max(c, d, b, a) == d);
            REQUIRE (e::max(d, a, b, c) == c);
            REQUIRE (e::max(d, a, c, b) == c);
            REQUIRE (e::max(d, b, a, c) == c);
            REQUIRE (e::max(d, b, c, a) == c);
            REQUIRE (e::max(d, c, a, b) == c);
            REQUIRE (e::max(d, c, b, a) == c);
        }
    }
}
