#pragma once

#include <type_traits>

namespace elements {

template <typename T>
requires Semiregular<std::remove_cv_t<T>>
using Remove_cv = std::remove_cv_t<T>;

template <typename T>
requires Semiregular<std::remove_reference_t<T>>
using Remove_ref = std::remove_reference_t<T>;

template <typename T>
requires Semiregular<Remove_cv<Remove_ref<T>>>
using Decay = typename std::decay<T>::type;

template <typename T>
requires Semiregular<Decay<T>>
struct value_type_t
{
    using type = T;
};

template <typename T>
requires Semiregular<Decay<T>>
using Value_type = typename value_type_t<T>::type;

template <typename T>
requires Semiregular<Decay<T>>
struct underlying_type_t
{
    using type = T;
};

template <typename T>
requires Semiregular<Decay<T>>
using Underlying_type = typename underlying_type_t<T>::type;

}
