#pragma once

#include <type_traits>
#include <utility>

#if __cplusplus <= 201703L
    #define concept concept bool // Use C++20 syntax
#endif

#include "algorithms/algebra.h"
