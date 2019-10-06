#pragma once

#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

#define concept concept bool // Use C++20 syntax

namespace elements {

template <typename T>
using Remove_const = std::remove_cv_t<T>;

template <typename T>
using Remove_ref = std::remove_reference_t<T>;

template <typename T>
using Decay = typename std::decay<T>::type;

template <typename T>
constexpr auto
fw(Remove_ref<T>& x) noexcept -> T&&
{
    return std::forward<T>(x);
}

template <typename T>
constexpr auto
fw(Remove_ref<T>&& x) noexcept -> T&&
{
    return std::forward<T>(x);
}

template <typename T>
constexpr auto
mv(T&& x) noexcept -> Remove_ref<T>&&
{
    return std::move(x);
}

}
