#pragma once

#include <type_traits>

namespace elements {

template <typename T>
requires Semiregular<std::remove_cv_t<T>>
using remove_cv = std::remove_cv_t<T>;

template <typename T>
requires Semiregular<std::remove_reference_t<T>>
using remove_ref = std::remove_reference_t<T>;

template <typename T>
requires Semiregular<remove_cv<remove_ref<T>>>
using decay = typename std::decay<T>::type;

template <typename T>
requires Semiregular<decay<T>>
struct value_type_t
{
    using type = T;
};

template <typename T>
requires Semiregular<decay<T>>
using value_type = typename value_type_t<T>::type;

template <typename T>
requires Semiregular<decay<T>>
struct underlying_type_t
{
    using type = T;
};

template <typename T>
requires Semiregular<decay<T>>
using underlying_type = typename underlying_type_t<T>::type;

template <typename T>
struct distance_type_t;

template <typename T>
requires Semiregular<decay<T>>
using distance_type = typename distance_type_t<T>::type;

template <typename T>
struct difference_type_t;

template <typename T>
requires Semiregular<decay<T>>
using difference_type = typename difference_type_t<T>::type;

template <typename T>
struct size_type_t;

template <typename T>
requires Semiregular<decay<T>>
using size_type = typename size_type_t<T>::type;

}
