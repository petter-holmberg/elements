#pragma once

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

template <typename F, typename... Args>
concept Invocable =
    requires(F&& f, Args&&... args) {
        std::invoke(fw<F>(f), fw<Args>(args)...);
    };

template <typename P>
concept Procedure =
    requires (Domain<P> const& x) {
        Same_as<Arity<P>, std::size_t>;
    };

template <typename F>
concept Functional_procedure =
    Procedure<F> and
    Regular<Domain<F>>;

template <typename F>
concept Unary_function =
    Functional_procedure<F> and
    Arity<F> == 1 and
    Regular<Domain<F>>;

template <typename F>
concept Binary_function =
    Functional_procedure<F> and
    Arity<F> == 2 and
    Regular<Decay<Input_type<F, 0>>> and
    Regular<Decay<Input_type<F, 1>>>;

template <typename F>
concept Homogeneous_function =
    Functional_procedure<F> and
    Arity<F> > 0 and
    Regular<Domain<F>>;

template <typename Op>
concept Operation =
    Homogeneous_function<Op> and
    Same_as<Codomain<Op>, Domain<Op>>;

template <typename Op>
concept Unary_operation =
    Operation<Op> and
    Arity<Op> == 1;

template <typename Op>
concept Binary_operation =
    Operation<Op> and
    Arity<Op> == 2;

template <Binary_operation Op>
struct transpose_op
{
    Op op;

    explicit constexpr
    transpose_op(Op op_) : op{op_} {}

    constexpr auto
    operator()(Domain<Op> const& x, Domain<Op> const& y) -> Domain<Op>
    {
        return op(y, x);
    }
};

template <typename P>
concept Predicate =
    Functional_procedure<P> and
    Same_as<Codomain<P>, bool>;

template <typename P>
concept Unary_predicate =
    Predicate<P> and
    Unary_function<P>;

template <Unary_predicate P>
struct negation
{
    P pred;

    constexpr
    negation(P pred_) : pred{pred_} {}

    constexpr auto
    operator()(Domain<P> const& x) -> bool
    {
        return !pred(x);
    }
};

template <typename P>
concept Homogeneous_predicate =
    Predicate<P> and
    Homogeneous_function<P>;

template <typename R>
concept Relation =
    Homogeneous_predicate<R> and
    Arity<R> == 2;

template <Relation R>
struct identity_relation
{
    R rel;

    explicit constexpr
    identity_relation(R r) : rel{r} {}

    constexpr auto
    operator()(Domain<R> const& x, Domain<R> const& y) -> bool
    {
        return rel(x, y);
    }
};

template <Relation R>
struct complement
{
    R rel;

    explicit constexpr
    complement(R r) : rel{r} {}

    constexpr auto
    operator()(Domain<R> const& x, Domain<R> const& y) -> bool
    {
        return !rel(x, y);
    }
};

template <Relation R>
struct converse
{
    R rel;

    explicit constexpr
    converse(R r) : rel{r} {}

    constexpr auto
    operator()(Domain<R> const& x, Domain<R> const& y) -> bool
    {
        return rel(y, x);
    }
};

template <Relation R>
struct complement_of_converse
{
    R rel;

    explicit constexpr
    complement_of_converse(R r) : rel{r} {}

    constexpr auto
    operator()(Domain<R> const& x, Domain<R> const& y) -> bool
    {
        return !rel(y, x);
    }
};

}
