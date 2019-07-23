#pragma once

#include "regular.h"

namespace elements {

template <typename T>
concept Binary_integer =
    Totally_ordered<Decay<T>> and
    std::is_integral<Decay<T>>::value;

}
