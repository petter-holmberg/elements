#pragma once

#include <cstddef>

#include "concepts/algebra.h"
#include "concepts/regular.h"

namespace elements {

// Pointers

template <typename T>
using Pointer_type = T*;

template <typename T>
requires Semiregular<Decay<T>>
struct value_type_t<Pointer_type<T>>
{
    using type = T;
};

template <typename T>
struct difference_type_t<T*>
{
    using type = std::ptrdiff_t;
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

template <typename T>
requires Semiregular<Decay<T>>
using Position_type = typename position_type_t<T>::type;

template <typename T>
requires Semiregular<Decay<T>>
struct position_type_t<T const>
{
    using type = Position_type<T const>;
};

template <typename>
constexpr bool Is_contiguously_addressable = false;

template <typename T>
struct size_type_t;

template <typename T>
requires Semiregular<T>
using Size_type = typename size_type_t<T>::type;

template <typename T>
constexpr auto Size = size_type_t<T>::value;

}
