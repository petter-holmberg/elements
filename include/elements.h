#pragma once

#include <type_traits>
#include <utility>

#if __cplusplus <= 201703L
    #define concept concept bool // Use C++20 syntax
#endif

#include "algorithms/algebra.h"
#include "algorithms/regular.h"
#include "algorithms/ordered_algebra.h"
#include "algorithms/ordering.h"
#include "algorithms/position.h"
#include "algorithms/search.h"

#include "data_structures/tuple.h"
