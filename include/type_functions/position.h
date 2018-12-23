#pragma once

#include <cstddef>

#include "concepts/algebra.h"
#include "concepts/regular.h"

namespace elements {

template <typename>
struct difference_type_t;

template <typename T>
requires Semiregular<Decay<T>>
using Difference_type = typename difference_type_t<T>::type;

// Pointers

template <typename T>
using Pointer_type = T*;

template <typename T>
requires Semiregular<Decay<T>>
struct value_type_t<Pointer_type<T>>
{
    using type = T;
};

template <typename>
constexpr bool Is_forward_position = false;

template <typename T>
constexpr bool Is_forward_position<Pointer_type<T>> = true;

template <typename>
struct distance_type_t;

template <typename T>
requires Semiregular<Decay<T>>
struct distance_type_t<Pointer_type<T>>
{
    using type = std::ptrdiff_t;
};

template <typename T>
requires Semiregular<Decay<T>>
using Distance_type = typename distance_type_t<T>::type;

template <typename T>
requires Semiregular<Decay<T>>
struct position_type_t
{
    using type = Pointer_type<T>;
};

template <typename>
constexpr bool Is_contiguously_addressable = false;

template <typename T>
requires Semiregular<Decay<T>>
struct position_type_t;

template <typename T>
requires Semiregular<Decay<T>>
using Position_type = typename position_type_t<T>::type;

template <typename T>
struct size_type_t;

template <typename T>
requires Semiregular<T>
using Size_type = typename size_type_t<T>::type;

template <typename T>
constexpr auto Size_value = size_type_t<T>::value;

}
