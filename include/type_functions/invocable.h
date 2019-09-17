#pragma once

#include <tuple>

#include "regular.h"

namespace elements {

template <typename>
struct invocable_t;

// Free function with no arguments
template <typename Ret>
struct invocable_t<Ret()>
{
    using codomain_type = Ret;

    static constexpr std::size_t arity = 0;

    using domain_type = void;
};

// Free function
template <typename Ret, typename... Args>
struct invocable_t<Ret(Args...)>
{
    using codomain_type = Ret;

    static constexpr std::size_t arity = sizeof...(Args);

    template <std::size_t n>
    struct parameter_t
    {
        static_assert(n < arity, "arity > parameter index");
        using type = std::tuple_element_t<n, std::tuple<Args...>>;
    };

    using domain_type = Decay<typename parameter_t<0>::type>;
};

// Function pointer
template <typename Ret, typename... Args>
struct invocable_t<Ret(*)(Args...)> : invocable_t<Ret(Args...)>
{};

// Member function pointer
template <typename T, typename Ret, typename... Args>
struct invocable_t<Ret(T::*)(Args...)> : invocable_t<Ret(T&, Args...)>
{
    using domain_type = Decay<typename invocable_t<Ret(T&, Args...)>::template parameter_t<1>::type>;
};

// Const member function pointer
template <typename T, typename Ret, typename... Args>
struct invocable_t<Ret(T::*)(Args...) const> : invocable_t<Ret(T::*)(Args...)>
{};

// Member object pointer
template <typename T, typename Ret>
struct invocable_t<Ret(T::*)> : invocable_t<Ret(T&)>
{};

// Function object
template <typename T>
struct invocable_t
{
    private:
        using call_type = invocable_t<decltype(&T::operator())>;
    public:
        using codomain_type = typename call_type::codomain_type;

        static constexpr std::size_t arity = call_type::arity - 1;

        template <std::size_t n>
        struct parameter_t
        {
            static_assert(n < arity, "arity > parameter index");
            using type = typename call_type::template parameter_t<n + 1>::type;
        };

        using domain_type = Decay<typename parameter_t<0>::type>;
};

// Lvalue reference
template <typename T>
struct invocable_t<T&> : invocable_t<T>
{};

template <typename T>
using Codomain = typename invocable_t<T>::codomain_type;

template <typename T, std::size_t n>
using Input_type = typename invocable_t<T>::template parameter_t<n>::type;

template <typename T>
constexpr auto Arity = invocable_t<T>::arity;

template <typename T>
using Domain = typename invocable_t<T>::domain_type;

template <typename Fun, typename... Args>
using Result_type = std::invoke_result_t<Fun, Args...>;

}
