#pragma once

#include "algebra.h"
#include "regular.h"

namespace elements {

struct bit
{
    bool v = false;

    constexpr
    bit() = default;

    explicit constexpr
    bit(bool value) : v{value} {}
};

constexpr auto
operator==(bit x, bit y) -> bool
{
    return x.v == y.v;
}

constexpr auto
operator<(bit x, bit y) -> bool
{
    return x.v < y.v;
}

constexpr auto
operator+(bit x, bit y) -> bit
{
    return bit{bool(x.v ^ y.v)};
}

constexpr auto
operator*(bit x, bit y) -> bit
{
    return bit{bool(x.v & y.v)};
}

auto contradiction = [](bit, bit) -> bit { return bit{0}; };

auto conjunction = [](bit p, bit q) -> bit { return p * q; };

auto nonimplication = [](bit p, bit q) -> bit { return p + p * q; };

auto projection_left = [](bit p, bit) -> bit { return p; };

auto converse_nonimplication = [](bit p, bit q) -> bit { return q + p * q; };

auto projection_right = [](bit, bit q) -> bit { return q; };

auto nonequivalence = [](bit p, bit q) -> bit { return p + q; };

auto disjunction = [](bit p, bit q) -> bit { return p + q + p * q; };

auto nondisjunction = [](bit p, bit q) -> bit { return bit{1} + p + q + p * q; };

auto equivalence = [](bit p, bit q) -> bit { return bit{1} + p + q; };

auto nonprojection_right = [](bit, bit q) -> bit { return bit{1} + q; };

auto converse_implication = [](bit p, bit q) -> bit { return bit{1} + q + p * q; };

auto nonprojection_left = [](bit p, bit) -> bit { return bit{1} + p; };

auto implication = [](bit p, bit q) -> bit { return bit{1} + p + p * q; };

auto nonconjunction = [](bit p, bit q) -> bit { return bit{1} + p * q; };

auto tautology = [](bit, bit) -> bit { return bit{1}; };

}
