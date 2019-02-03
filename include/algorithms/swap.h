#pragma once

#include "regular.h"
#include "position.h"

namespace elements {

template <Semiregular T>
constexpr void
swap(T& x, T& y)
{
    auto z{underlying_ref(x)};
    underlying_ref(x) = underlying_ref(y);
    underlying_ref(y) = z;
}

}
