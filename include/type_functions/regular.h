#pragma once

#include <type_traits>

namespace elements {

template <typename T>
requires Movable<std::remove_cv_t<T>>
using Remove_const = std::remove_cv_t<T>;

template <typename T>
requires Movable<std::remove_reference_t<T>>
using Remove_ref = std::remove_reference_t<T>;

template <typename T>
requires Movable<Remove_const<Remove_ref<T>>>
using Decay = typename std::decay<T>::type;

template <typename T>
requires Movable<Decay<T>>
struct value_type_t
{
    using type = T;
};

template <typename T>
requires Movable<Decay<T>>
using Value_type = typename value_type_t<T>::type;

}
