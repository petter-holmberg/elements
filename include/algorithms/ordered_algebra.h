#pragma once

#include "concepts/ordered_algebra.h"

namespace elements {

template <Ordered_additive_group G>
constexpr auto
abs(G const& a) -> G
{
    if (a < G{0}) return -a;
    else return a;
}

}
