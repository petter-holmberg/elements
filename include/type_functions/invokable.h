#pragma once

#include <tuple>

#include "regular.h"

namespace elements {

template <typename>
struct invokable_t;

// Free function
template <typename Ret, typename... Args>
struct invokable_t<Ret(Args...)>
{
    using codomain_type = Ret;

    static constexpr std::size_t arity = sizeof...(Args);

    template <std::size_t n>
    struct parameter_t
    {
        static_assert(n < arity, "arity > parameter index");
        using type = std::tuple_element_t<n, std::tuple<Args...>>;
    };

    using domain_type = decay<typename parameter_t<0>::type>;
};

// Function pointer
template <typename Ret, typename... Args>
struct invokable_t<Ret(*)(Args...)> : invokable_t<Ret(Args...)>
{};

// Member function pointer
template <typename T, typename Ret, typename... Args>
struct invokable_t<Ret(T::*)(Args...)> : invokable_t<Ret(T&, Args...)>
{
    using domain_type = decay<typename invokable_t<Ret(T&, Args...)>::template parameter_t<1>::type>;
};

// Const member function pointer
template <typename T, typename Ret, typename... Args>
struct invokable_t<Ret(T::*)(Args...) const> : invokable_t<Ret(T::*)(Args...)>
{};

// Member object pointer
template <typename T, typename Ret>
struct invokable_t<Ret(T::*)> : invokable_t<Ret(T&)>
{};

// Function object
template <typename T>
struct invokable_t
{
    private:
        using call_type = invokable_t<decltype(&T::operator())>;
    public:
        using codomain_type = typename call_type::codomain_type;

        static constexpr std::size_t arity = call_type::arity - 1;

        template <std::size_t n>
        struct parameter_t
        {
            static_assert(n < arity, "arity > parameter index");
            using type = typename call_type::template parameter_t<n + 1>::type;
        };

        using domain_type = remove_cv<remove_ref<typename parameter_t<0>::type>>;
};

// Lvalue reference
template <typename T>
struct invokable_t<T&> : invokable_t<T>
{};

// Forwarding reference
template <typename T>
struct invokable_t<T&&> : invokable_t<T>
{};

template <typename T>
using codomain = typename invokable_t<T>::codomain_type;

template <typename T, std::size_t n>
using input_type = typename invokable_t<T>::template parameter_t<n>::type;

template <typename T>
constexpr auto arity = invokable_t<T>::arity;

template <typename T>
using domain = typename invokable_t<T>::domain_type;

}
