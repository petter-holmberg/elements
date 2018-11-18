#pragma once

#include "concepts/algebra.h"
#include "concepts/regular.h"

namespace elements {

template <typename>
struct difference_type_t;

template <typename T>
requires Semiregular<decay<T>>
using difference_type = typename difference_type_t<T>::type;

// Pointers

template <typename T>
using pointer_type = T*;

template <typename T>
requires Semiregular<decay<T>>
struct value_type_t<pointer_type<T>>
{
    using type = T;
};

template <typename T>
constexpr bool is_forward_position<pointer_type<T>> = true;

template <typename T>
requires Semiregular<decay<T>>
struct distance_type_t<pointer_type<T>>
{
    using type = std::ptrdiff_t;
};

template <typename T>
requires Semiregular<decay<T>>
struct position_type_t
{
    using type = pointer_type<T>;
};

}
