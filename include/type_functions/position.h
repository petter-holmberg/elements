#pragma once

#include <cstddef>

#include "concepts/algebra.h"
#include "concepts/regular.h"

namespace elements {

// Pointers

template <typename T>
using Pointer_type = T*;

using pointer_diff = std::ptrdiff_t;

template <typename T>
requires Movable<Decay<T>>
struct value_type_t<Pointer_type<T>>
{
    using type = T;
};

template <typename T>
struct difference_type_t<T*>
{
    using type = pointer_diff;
};

template <typename T>
requires Movable<Decay<T>>
struct position_type_t
{
    using type = Pointer_type<T>;
};

template <typename T>
requires Movable<Decay<T>>
using Position_type = typename position_type_t<T>::type;

template <typename T>
requires Movable<Decay<T>>
struct position_type_t<T const>
{
    using type = Position_type<T const>;
};

template <typename>
struct size_type_t;

template <typename T>
requires Movable<Decay<T>>
using Size_type = typename size_type_t<T>::type;

template <typename T>
constexpr auto Size = size_type_t<T>::value;

}
