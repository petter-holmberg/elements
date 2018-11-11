#pragma once

#include <cstddef>

namespace elements {

template <>
struct scalar_type_t<std::ptrdiff_t>
{
    using type = std::ptrdiff_t;
};

template <typename T>
struct vector_type_t<T*>
{
    using type = std::ptrdiff_t;
};

}
