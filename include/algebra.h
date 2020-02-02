#pragma once

#include "invocable.h"

namespace elements {

template <Regular T>
constexpr auto
axiom_Commutative(T const& a, T const& b) noexcept -> bool
{
    return a + b == b + a;
}

// Group-like

template <typename S, typename Op>
concept Semigroup =
    Regular<S> and
    Binary_operation<Op>;
    // axiom associative(Op) {
    //     Op(Op(a, b), c) == Op(a, Op(b, c));
    // }

template <typename S, typename Op>
requires Semigroup<S, Op>
struct identity_element_t;

template <typename S, typename Op>
requires Semigroup<S, Op>
S const Identity_element = identity_element_t<S, Op>::value;

template <typename S>
concept Additive_semigroup =
    Regular<S> and
    requires (S const& a) {
        { a + a } -> S;
    };
    // axiom associative(+) {
    //     (a + b) + c == a + (b + c);
    // }
    // axiom commutative(+) {
    //     a + b == b + a;
    // }

template <Additive_semigroup S>
constexpr auto
axiom_Additive_semigroup(S const& a, S const& b, S const& c) noexcept -> bool
{
    // Associative
    if ((a + b) + c != a + (b + c)) return false;

    // Commutative
    if (a + b != b + a) return false;

    return true;
}

template <Additive_semigroup S>
constexpr auto
operator+=(S& a, S const& b) -> S&
{
    a = a + b;
    return a;
}

template <Additive_semigroup S>
struct sum
{
    constexpr auto
    operator()(S const& a, S const& b) const -> S
    {
        return a + b;
    }
};

template <Additive_semigroup S>
struct zero_type_t
{
    static S const value;
};

template <Additive_semigroup S>
S const Zero = zero_type_t<S>::value;

template <Additive_semigroup S>
S const zero_type_t<S>::value = S(0);

template <Additive_semigroup S>
constexpr auto zero = S::zero_v;

template <Additive_semigroup S>
struct identity_element_t<S, sum<S>>
{
    static S const value;
};

template <Additive_semigroup S>
S const identity_element_t<S, sum<S>>::value = Zero<S>;

template <typename S>
concept Multiplicative_semigroup =
    Regular<S> and
    requires (S const& a) {
        { a * a } -> S;
    };
    // axiom associative(*) {
    //     (a * b) * c == a * (b * c);
    // }

template <Multiplicative_semigroup S>
constexpr auto
axiom_Multiplicative_semigroup(S const& a, S const& b, S const& c) noexcept -> bool
{
    // Associative
    if ((a * b) * c != a * (b * c)) return false;

    return true;
}

template <Multiplicative_semigroup S>
constexpr auto
operator*=(S& a, S const& b) -> S&
{
    a = a * b;
    return a;
}

template <Multiplicative_semigroup S>
struct product
{
    constexpr auto
    operator()(S const& a, S const& b) const -> S
    {
        return a * b;
    }
};

template <Multiplicative_semigroup S>
struct one_type_t;

template <Multiplicative_semigroup S>
S const One = one_type_t<S>::value;

template <Multiplicative_semigroup S>
struct one_type_t
{
    static S const value;
};

template <Multiplicative_semigroup S>
struct identity_element_t<S, product<S>>
{
    static S const value;
};

template <Multiplicative_semigroup S>
S const identity_element_t<S, product<S>>::value = One<S>;

template <Multiplicative_semigroup S>
S const one_type_t<S>::value = S(1);

template <typename M, typename Op>
concept Monoid =
    Semigroup<M, Op> and
    requires {
        M{Identity_element<M, Op>};
    };
    // axiom identity(identity_element<M, Op>) {
    //     Op(identity_element<M, Op>, a) == a;
    //     Op(a, identity_element<M, Op>) == a;
    // }

template <typename M, typename Op>
requires Monoid<M, Op>
struct inverse_operation;

template <typename M>
concept Additive_monoid =
    Additive_semigroup<M> and
    requires {
        M{Zero<M>};
    };
    // axiom identity(Zero<M>) {
    //     Zero<M> + a == a;
    //     a + Zero<M> == a;
    // }

template <Additive_monoid M>
constexpr auto
axiom_Additive_monoid(M const& a, M const& b, M const& c) noexcept -> bool
{
    if (!axiom_Additive_semigroup(a, b, c)) return false;

    // Identity
    if (a + Zero<M> != a) return false;
    if (a + Zero<M> != Zero<M> + a) return false;

    return true;
}

template <Additive_monoid M>
constexpr auto
operator+(M& a) -> M&
{
    return a;
}

template <Additive_monoid M>
struct positive
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return +a;
    }
};

template <Additive_monoid M>
struct negative
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return -a;
    }
};

template <Additive_monoid M>
struct inverse_operation<M, sum<M>>
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return negative<M>()(a);
    }
};

template <typename M>
concept Multiplicative_monoid =
    Multiplicative_semigroup<M> and
    requires {
        M{One<M>};
    };
    // axiom identity(One<M>) {
    //     One<M> * a == a;
    //     a * One<M> == a;
    // }

template <Multiplicative_monoid M>
constexpr auto
axiom_Multiplicative_monoid(M const& a, M const& b, M const& c) noexcept -> bool
{
    if (!(axiom_Multiplicative_semigroup(a, b, c))) return false;

    // Identity
    if (a * One<M> != a) return false;
    if (a * One<M> != One<M> * a) return false;

    return true;
}

template <Multiplicative_monoid M>
struct reciprocal
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return One<M> / a;
    }
};

template <Multiplicative_monoid M>
struct inverse_operation<M, product<M>>
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return reciprocal<M>()(a);
    }
};

template <typename G, typename Op>
concept Group =
    Monoid<G, Op> and
    Unary_operation<inverse_operation<G, Op>>;
    // axiom inverse(inverse_operation<G, Op>, Op, identity_element<G, Op>) {
    //     Op(a, inverse_operation<G, Op>{}(a)) == identity_element<G, Op>;
    //     Op(inverse_operation<G, Op>{}(a), a) == identity_element<G, Op>;
    // }

template <typename G>
concept Additive_group =
    Additive_monoid<G> and
    requires (G const& a) {
        { -a } -> G;
        { a - a } -> G;
    };
    // axiom inverse(-, +, Zero<G>) {
    //     a + -a == Zero<G>;
    //     -a + a == Zero<G>;
    // }

template <Additive_group G>
constexpr auto
axiom_Additive_group(G const& a, G const& b, G const& c) noexcept -> bool
    //[[expects: a != Zero<G> && b != Zero<G> && c != Zero<G>]]
{
    if (!axiom_Additive_monoid(a, b, c)) return false;

    // Cancellation
    if (a + (-a) != Zero<G>) return false;
    if (a + (-a) != -a + a) return false;
    if (-(-a) != a) return false;

    // Subtraction
    if (a - Zero<G> != a) return false;
    if (Zero<G> - a != -a) return false;
    if (a - b != a + (-b)) return false;

    return true;
}

template <Additive_group G>
struct difference
{
    constexpr auto
    operator()(G const& a, G const& b) const -> G
    {
        return a - b;
    }
};

template <Additive_group G>
constexpr auto
operator-=(G& x, G const& y) -> G&
{
    x = x - y;
    return x;
}

template <typename G>
concept Multiplicative_group =
    Multiplicative_monoid<G> and
    requires (G const& a) {
        { reciprocal<G>{}(a) } -> G;
        { a / a } -> G;
    };
    // axiom inverse(reciprocal, product<G>, One<G>) {
    //     a * reciprocal(a) == One<G>;
    //     reciprocal(a) * a == One<G>;
    // }

template <Multiplicative_group G>
constexpr auto
axiom_Multiplicative_group(G const& a, G const& b, G const& c) noexcept -> bool
    //[[expects: a != Zero<G> && b != Zero<G> && c != Zero<G>]]
    //[[expects: a != One<G> && b != One<G> && c != One<G>]]
{
    if (!axiom_Multiplicative_monoid(a, b, c)) return false;

    // Division
    if (a / One<G> != a) return false;
    if (a / a != One<G>) return false;

    return true;
}

template <Multiplicative_group G>
struct quotient
{
    constexpr auto
    operator()(G const& a, G const& b) const -> G
    {
        return a / b;
    }
};

template <Multiplicative_group G>
constexpr auto
operator/=(G& x, G const& y) -> G&
{
    x = x / y;
    return x;
}

template <typename S, typename Add_op = sum<S>, typename Mul_op = product<S>>
concept Semiring =
    Monoid<S, Add_op> and
    Monoid<S, Mul_op>;
    // axiom distributive(Add_op, Mul_op) {
    //     Mul_op(a, Add_op(b, c)) == Add_op(Mul_op(a, b), Mul_op(a, c));
    //     Mul_op(Add_op(a, b), c) == Add_op(Mul_op(a, c), Mul_op(b, c));
    // }
    // axiom annihilation(Mul_op, Zero<S>) {
    //     Mul_op(Zero<S>, a) == Zero<S>;
    //     Mul_op(a, Zero<S>) == Zero<S>;
    // }

template <typename S>
requires Semiring<S>
constexpr auto
axiom_Semiring(S const& a, S const& b, S const& c) noexcept -> bool
    //[[expects: a != Zero<S> && b != Zero<S> && c != Zero<S>]]
    //[[expects: a != One<S> && b != One<S> && c != One<S>]]
{
    if (!axiom_Additive_monoid(a, b, c)) return false;
    if (!axiom_Multiplicative_monoid(a, b, c)) return false;

    // Distributive
    if (a * (b + c) != a * b + a * c) return false;
    if ((a + b) * c != a * c + b * c) return false;

    return true;
}

template <typename S, typename Add_op = sum<S>, typename Mul_op = product<S>>
concept Commutative_semiring =
    Semiring<S, Add_op, Mul_op>;
    // axiom commutative(Mul_op) {
    //     Mul_op(a, b) == Mul_op(b, a);
    // }

template <typename R, typename Add_op = sum<R>, typename Mul_op = product<R>>
concept Ring =
    Semiring<R, Add_op, Mul_op> and
    Group<R, Add_op>;

template <typename R>
requires Ring<R>
constexpr auto
axiom_Ring(R const& a, R const& b, R const& c) noexcept -> bool
    //[[expects: a != Zero<R> && b != Zero<R> && c != Zero<R>]]
    //[[expects: a != One<R> && b != One<R> && c != One<R>]]
{
    if (!axiom_Semiring(a, b, c)) return false;
    if (!axiom_Additive_group(a, b, c)) return false;

    return true;
}

template <typename R, typename Add_op = sum<R>, typename Mul_op = product<R>>
concept Commutative_ring =
    Ring<R, Add_op, Mul_op>;
    // axiom commutative(Mul_op) {
    //     Mul_op(a, b) == Mul_op(b, a);
    // }

template <typename I, typename Add_op = sum<I>, typename Mul_op = product<I>>
concept Integral_domain =
    Commutative_ring<I, Add_op, Mul_op>;
    // axiom no_nonzero_zero_divisors(Mul_op) {
    //     a, b != 0 => Mul_op(a, b) != 0;
    // }

template <typename F, typename Add_op = sum<F>, typename Mul_op = product<F>>
concept Field =
    Integral_domain<F, Add_op, Mul_op> and
    Group<F, Mul_op>;

template <typename T>
struct value_type_t
{
    using type = T;
};

template <typename T>
using Value_type = typename value_type_t<T>::type;

template <
    typename V, typename V_add_op = sum<V>,
    typename S = Value_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Left_semimodule =
    Monoid<V, V_add_op> and
    Commutative_semiring<S, S_add_op, S_mul_op> and
    requires (V const& v, S const& s) {
        { s * v } -> V;
    };
    // axiom commutative(V_add_op) {
    //     V_add_op(a, b) == V_add_op(b, a);
    // }
    // axiom distributive(V_add_op, *) {
    //     a * V_add_op(v, w) == V_add_op(a * v, a * w);
    // }
    // axiom distributive(S_add_op, *) {
    //     S_add_op(a, b) * v == a * v + b * v;
    // }
    // axiom compatibility(S_mul_op, *) {
    //     S_mul_op(a, b) * v == a * (b * v);
    // }
    // axiom identity(One<V>) {
    //     One<V> * a == a;
    // }

template <
    typename V, typename V_add_op = sum<V>,
    typename S = Value_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Right_semimodule =
    Monoid<V, V_add_op> and
    Commutative_semiring<S, S_add_op, S_mul_op> and
    requires (V const& v, S const& s) {
        { v * s } -> V;
    };
    // axiom commutative(V_add_op) {
    //     V_add_op(a, b) == V_add_op(b, a);
    // }
    // axiom distributive(V_add_op, *) {
    //     V_add_op(v, w) * a == V_add_op(v * a, w * a);
    // }
    // axiom distributive(S_add_op, *) {
    //     v * S_add_op(a, b) == v * a + v * b;
    // }
    // axiom compatibility(S_mul_op, *) {
    //     v * S_mul_op(a, b) == (v * b) * a;
    // }
    // axiom identity(One<V>) {
    //     a * One<V> == a;
    // }

template <typename V, typename S = Value_type<V>>
requires Right_semimodule<V, S>
constexpr auto
operator*=(V& v, S const& a) -> V&
{
    v = v * a;
    return v;
}

template <
    typename V, typename V_add_op = sum<V>,
    typename S = Value_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Semimodule =
    Left_semimodule<V, V_add_op, S, S_add_op, S_mul_op> and
    Right_semimodule<V, V_add_op, S, S_add_op, S_mul_op>;

template <
    typename V, typename V_add_op = sum<V>,
    typename S = Value_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Left_module =
    Left_semimodule<V, V_add_op, S, S_add_op, S_mul_op> and
    Group<V, V_add_op> and
    Ring<S, S_add_op, S_mul_op>;

template <
    typename V, typename V_add_op = sum<V>,
    typename S = Value_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Right_module =
    Right_semimodule<V, V_add_op, S, S_add_op, S_mul_op> and
    Group<V, V_add_op> and
    Ring<S, S_add_op, S_mul_op>;

template <
    typename V, typename V_add_op = sum<V>,
    typename S = Value_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Module =
    Left_module<V, V_add_op, S, S_add_op, S_mul_op> and
    Right_module<V, V_add_op, S, S_add_op, S_mul_op> and
    Commutative_ring<S, S_add_op, S_mul_op>;

template <typename V, typename S = Value_type<V>>
concept Vector_space =
    Module<V, sum<V>, S, sum<S>, product<S>> and
    Field<S, sum<S>, product<S>>;

template <typename V, typename S = Value_type<V>>
requires Vector_space<V, S>
constexpr auto
operator-(V const& v, V const& w) -> V
{
    return v + (-w);
}

template <typename V, typename S = Value_type<V>>
requires Vector_space<V>
constexpr auto
operator/(V const& v, Value_type<V> const& a) -> V
{
    return (One<Value_type<V>> / a) * v;
}

template <typename V, typename S = Value_type<V>>
requires Vector_space<V>
constexpr auto
operator/=(V const& v, Value_type<V> const& a) -> V
{
    v = v / a;
    return v;
}

template <typename>
struct difference_type_t;

template <typename T>
using Difference_type = typename difference_type_t<T>::type;

}
