#pragma once

#include <cstddef>

namespace elements {

template <typename T>
struct vector_type_t<T*>
{
    using type = std::ptrdiff_t;
};

}
