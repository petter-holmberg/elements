#pragma once

#include <tuple>

namespace elements {

template <typename... Ts>
using tuple = std::tuple<Ts...>;

template <std::size_t I, typename... Ts>
constexpr auto
get(tuple<Ts...>& t) noexcept -> typename std::tuple_element<I, tuple<Ts...>>::type&
{
    return std::get<I>(t);
}

template <std::size_t I, typename... Ts>
constexpr auto
get(tuple<Ts...>&& t) noexcept -> typename std::tuple_element<I, tuple<Ts...>>::type&&
{
    return std::get<I>(std::forward<Ts...>(t));
}

template <std::size_t I, typename... Ts>
constexpr auto
get(tuple<Ts...> const& t) noexcept -> typename std::tuple_element<I, tuple<Ts...>>::type const&
{
    return std::get<I>(t);
}

template <std::size_t I, typename... Ts>
constexpr auto
get(tuple<Ts...> const&& t) noexcept -> typename std::tuple_element<I, tuple<Ts...>>::type const&&
{
    return std::get<I>(std::forward<Ts...>(t));
}

}
