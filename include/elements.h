#pragma once

#include <type_traits>
#include <utility>

#define concept concept bool // Use C++20 syntax

#include "adapters/counted_position.h"
#include "adapters/loadable_position.h"
#include "adapters/loadable_forward_position.h"
#include "adapters/reverse_position.h"

#include "algorithms/algebra.h"
#include "algorithms/copy.h"
#include "algorithms/fill.h"
#include "algorithms/integer.h"
#include "algorithms/lexicographical.h"
#include "algorithms/integer.h"
#include "algorithms/map.h"
#include "algorithms/ordered_algebra.h"
#include "algorithms/ordering.h"
#include "algorithms/partition.h"
#include "algorithms/position.h"
#include "algorithms/regular.h"
#include "algorithms/search.h"
#include "algorithms/search_binary.h"
#include "algorithms/swap.h"

#include "concepts/functional.h"

#include "data_structures/array.h"
#include "data_structures/array_k.h"
#include "data_structures/binary_counter.h"
#include "data_structures/coordinate_point.h"
#include "data_structures/coordinate_vector.h"
#include "data_structures/pair.h"
#include "data_structures/polynomial.h"
#include "data_structures/bounded_range.h"
#include "data_structures/rational.h"
#include "data_structures/result.h"
