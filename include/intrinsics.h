#pragma once

#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

namespace elements {

namespace exposition_only {

template <typename T, typename U>
constexpr auto Is_same = std::is_same_v<T, U>;

template <typename T, typename U>
constexpr auto Is_convertible = std::is_convertible_v<T, U>;

template <typename T>
constexpr auto Is_nothrow_destructible = std::is_nothrow_destructible_v<T>;

template <typename T, typename ...Args>
constexpr auto Is_constructible = std::is_constructible_v<T, Args...>;

template <typename T>
constexpr auto Is_lvalue_ref = std::is_lvalue_reference_v<T>;

template <typename T>
constexpr auto Is_integral = std::is_integral_v<T>;

template <typename F, typename... Args>
constexpr auto Is_nothrow_invocable = std::is_nothrow_invocable_v<F, Args...>;

template <typename T, typename U>
concept Same_as = std::is_same_v<T, U>;

}

template <typename T>
using Add_rv_ref = std::add_rvalue_reference_t<T>;

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

template <typename F, typename... Args>
using Return_type = std::invoke_result_t<F, Args...>;

template <typename F, typename... Args>
constexpr auto
invoke(F&& f, Args&&... args) noexcept(exposition_only::Is_nothrow_invocable<F, Args...>) -> Return_type<F, Args...>
{
    return std::invoke(fw<F>(f), fw<Args>(args)...);
}

template <typename T>
constexpr auto
first(std::initializer_list<T> const& x) -> T const*
{
    return std::cbegin(x);
}

template <typename T>
constexpr auto
limit(std::initializer_list<T> const& x) -> T const*
{
    return std::cend(x);
}

}
