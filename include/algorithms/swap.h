#pragma once

#include "regular.h"

namespace elements {

template <Movable T>
constexpr void
swap(T& x, T& y)
{
    auto z{underlying_ref(x)};
    underlying_ref(x) = underlying_ref(y);
    underlying_ref(y) = z;
}

}
