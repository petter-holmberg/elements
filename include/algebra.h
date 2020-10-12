#pragma once

#include "invocable.h"

namespace elements {

// Group-like

template <Regular S, Operation<S, S> Op>
constexpr auto
axiom_Associative(S const& a, S const& b, S const& c, Op op) noexcept -> bool
{
    if (op(op(a, b), c) != op(a, op(b, c))) return false;

    return true;
}

template <Regular S, Operation<S, S> Op>
constexpr auto
axiom_Commutative(S const& a, S const& b, Op op) noexcept -> bool
{
    if (op(a, b) != op(b, a)) return false;

    return true;
}

template <typename S, typename Op>
concept Semigroup =
    Regular<S> and
    Operation<Op, S, S>;

template <typename S, typename Op>
requires Semigroup<S, Op>
constexpr auto
axiom_Semigroup(S const& a, S const& b, S const& c, Op op) noexcept -> bool
{
    if (!axiom_Associative(a, b, c, op)) return false;

    return true;
}

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
        { a + a } -> Same_as<S>;
    };

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

template <typename T = void>
struct sum
{
    template <Additive_semigroup S>
    constexpr auto
    operator()(S const& a, S const& b) const -> S
    {
        return a + b;
    }
};

template <>
struct sum<void>
{
    template <Additive_semigroup S>
    constexpr auto
    operator()(S const& a, S const& b) const -> S
    {
        return sum<S>{}(a, b);
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
struct identity_element_t<S, sum<void>>
{
    static S const value;
};

template <Additive_semigroup S>
S const identity_element_t<S, sum<S>>::value = Zero<S>;

template <Additive_semigroup S>
S const identity_element_t<S, sum<void>>::value = Zero<S>;

template <typename S>
concept Multiplicative_semigroup =
    Regular<S> and
    requires (S const& a) {
        { a * a } -> Same_as<S>;
    };

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

template <typename T = void>
struct product
{
    template <Multiplicative_semigroup S>
    constexpr auto
    operator()(S const& a, S const& b) const -> S
    {
        return a * b;
    }
};

template <>
struct product<void>
{
    template <Multiplicative_semigroup S>
    constexpr auto
    operator()(S const& a, S const& b) const -> S
    {
        return product<S>{}(a, b);
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
struct identity_element_t<S, product<void>>
{
    static S const value;
};

template <Multiplicative_semigroup S>
S const identity_element_t<S, product<S>>::value = One<S>;

template <Multiplicative_semigroup S>
S const identity_element_t<S, product<void>>::value = One<S>;

template <Multiplicative_semigroup S>
S const one_type_t<S>::value = S(1);

template <typename M, typename Op>
concept Monoid =
    Semigroup<M, Op> and
    requires {
        M{Identity_element<M, Op>};
    };

template <typename M, typename Op>
requires Monoid<M, Op>
constexpr auto
axiom_Monoid(M const& a, M const& b, M const& c, Op op) noexcept -> bool
{
    if (!axiom_Semigroup(a, b, c, op)) return false;

    // Identity
    if (op(a, Identity_element<M, Op>) != a) return false;
    if (op(a, Identity_element<M, Op>) != op(Identity_element<M, Op>, a)) return false;

    return true;
}

template <typename M, typename Op>
requires Monoid<M, Op>
struct inverse_operation;

template <typename M>
concept Additive_monoid =
    Additive_semigroup<M> and
    requires {
        M{Zero<M>};
    };

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

template <typename T = void>
struct positive
{
    template <Additive_monoid M>
    constexpr auto
    operator()(M const& a) const -> M
    {
        return +a;
    }
};

template <>
struct positive<void>
{
    template <Additive_monoid M>
    constexpr auto
    operator()(M const& a) const -> M
    {
        return positive<M>{}(a);
    }
};

template <typename T = void>
struct negative
{
    template <Additive_monoid M>
    constexpr auto
    operator()(M const& a) const -> M
    {
        return -a;
    }
};

template <>
struct negative<void>
{
    template <Additive_monoid M>
    constexpr auto
    operator()(M const& a) const -> M
    {
        return negative<M>{}(a);
    }
};

template <Additive_monoid M>
struct inverse_operation<M, sum<M>>
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return negative<M>{}(a);
    }
};

template <Additive_monoid M>
struct inverse_operation<M, sum<void>>
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return negative<M>{}(a);
    }
};

template <typename M>
concept Multiplicative_monoid =
    Multiplicative_semigroup<M> and
    requires {
        M{One<M>};
    };

template <Multiplicative_monoid M>
constexpr auto
axiom_Multiplicative_monoid(M const& a, M const& b, M const& c) noexcept -> bool
{
    if (!(axiom_Multiplicative_semigroup(a, b, c))) return false;

    // Identity
    if (One<M> * a != a) return false;
    if (a * One<M> != a) return false;

    return true;
}

template <typename T = void>
struct reciprocal
{
    template <Multiplicative_monoid M>
    constexpr auto
    operator()(M const& a) const -> M
    {
        return One<M> / a;
    }
};

template <>
struct reciprocal<void>
{
    template <Multiplicative_monoid M>
    constexpr auto
    operator()(M const& a) const -> M
    {
        return reciprocal<M>{}(a);
    }
};

template <Multiplicative_monoid M>
struct inverse_operation<M, product<M>>
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return reciprocal{}(a);
    }
};

template <Multiplicative_monoid M>
struct inverse_operation<M, product<void>>
{
    constexpr auto
    operator()(M const& a) const -> M
    {
        return reciprocal{}(a);
    }
};

template <typename G, typename Op>
concept Group =
    Monoid<G, Op> and
    Operation<inverse_operation<G, Op>, G>;

template <typename G, typename Op>
requires Group<G, Op>
constexpr auto
axiom_Group(G const& a, G const& b, G const& c, Op op) noexcept -> bool
{
    if (!axiom_Monoid(a, b, c, op)) return false;

    // Inverse
    if (op(a, inverse_operation<G, Op>{}(a)) != Identity_element<G, Op>) return false;
    if (op(inverse_operation<G, Op>{}(a), a) != Identity_element<G, Op>) return false;

    return true;
}

template <typename G>
concept Additive_group =
    Additive_monoid<G> and
    requires (G const& a) {
        { -a } -> Same_as<G>;
        { a - a } -> Same_as<G>;
    };

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

template <typename T = void>
struct difference
{
    template <Additive_group G>
    constexpr auto
    operator()(G const& a, G const& b) const -> G
    {
        return a - b;
    }
};

template <>
struct difference<void>
{
    template <Additive_group G>
    constexpr auto
    operator()(G const& a, G const& b) const -> G
    {
        return difference<G>{}(a, b);
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
        { reciprocal{}(a) } -> Same_as<G>;
        { a / a } -> Same_as<G>;
    };

template <Multiplicative_group G>
constexpr auto
axiom_Multiplicative_group(G const& a, G const& b, G const& c) noexcept -> bool
    //[[expects: a != Zero<G> && b != Zero<G> && c != Zero<G>]]
    //[[expects: a != One<G> && b != One<G> && c != One<G>]]
{
    if (!axiom_Multiplicative_monoid(a, b, c)) return false;

    // Inverse
    if (a * reciprocal{}(a) != One<G>) return false;
    if (reciprocal{}(a) * a != One<G>) return false;

    // Division
    if (a / One<G> != a) return false;
    if (a / a != One<G>) return false;

    return true;
}

template <typename T = void>
struct quotient
{
    template <Multiplicative_group G>
    constexpr auto
    operator()(G const& a, G const& b) const -> G
    {
        return a / b;
    }
};

template <>
struct quotient<void>
{
    template <Multiplicative_group G>
    constexpr auto
    operator()(G const& a, G const& b) const -> G
    {
        return quotient<G>{}(a, b);
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
requires
    Monoid<S, Add_op> and
    Monoid<S, Mul_op>
constexpr auto
axiom_Distributive(S const& a, S const& b, S const& c, Add_op add_op = {}, Mul_op mul_op = {}) noexcept -> bool
{
    if (mul_op(a, add_op(b, c)) != add_op(mul_op(a, b), mul_op(a, c))) return false;
    if (mul_op(add_op(a, b), c) != add_op(mul_op(a, c), mul_op(b, c))) return false;

    return true;
}

template <typename S, typename Add_op = sum<S>, typename Mul_op = product<S>>
requires
    Monoid<S, Add_op> and
    Monoid<S, Mul_op>
constexpr auto
axiom_Annihilation(S const& a, Mul_op mul_op = {}) noexcept -> bool
{
    if (mul_op(Identity_element<S, Add_op>, a) != Identity_element<S, Add_op>) return false;
    if (mul_op(a, Identity_element<S, Add_op>) != Identity_element<S, Add_op>) return false;

    return true;
}

template <typename S, typename Add_op = sum<S>, typename Mul_op = product<S>>
concept Semiring =
    Monoid<S, Add_op> and
    Monoid<S, Mul_op>;

template <typename S, typename Add_op = sum<S>, typename Mul_op = product<S>>
requires Semiring<S, Add_op, Mul_op>
constexpr auto
axiom_Semiring(S const& a, S const& b, S const& c, Add_op add_op = {}, Mul_op mul_op = {}) noexcept -> bool
{
    if (!axiom_Monoid(a, b, c, add_op)) return false;
    if (!axiom_Monoid(a, b, c, mul_op)) return false;

    if (!axiom_Distributive(a, b, c, add_op, mul_op)) return false;
    if (!axiom_Distributive(a, c, b, add_op, mul_op)) return false;
    if (!axiom_Distributive(b, a, c, add_op, mul_op)) return false;
    if (!axiom_Distributive(b, c, a, add_op, mul_op)) return false;
    if (!axiom_Distributive(c, a, b, add_op, mul_op)) return false;
    if (!axiom_Distributive(c, b, a, add_op, mul_op)) return false;

    if (!axiom_Annihilation<S, Add_op, Mul_op>(a, mul_op)) return false;
    if (!axiom_Annihilation<S, Add_op, Mul_op>(b, mul_op)) return false;
    if (!axiom_Annihilation<S, Add_op, Mul_op>(c, mul_op)) return false;

    return true;
}

template <typename S, typename Add_op = sum<S>, typename Mul_op = product<S>>
concept Commutative_semiring =
    Semiring<S, Add_op, Mul_op>;

template <typename S, typename Add_op = sum<S>, typename Mul_op = product<S>>
requires Commutative_semiring<S, Add_op, Mul_op>
constexpr auto
axiom_Commutative_semiring(S const& a, S const& b, S const& c, Add_op add_op = {}, Mul_op mul_op = {}) noexcept -> bool
{
    if (!axiom_Semiring(a, b, c, add_op, mul_op)) return false;

    if (!axiom_Commutative(a, b, mul_op)) return false;
    if (!axiom_Commutative(a, c, mul_op)) return false;
    if (!axiom_Commutative(b, c, mul_op)) return false;

    return true;
}

template <typename R, typename Add_op = sum<R>, typename Mul_op = product<R>>
concept Ring =
    Semiring<R, Add_op, Mul_op> and
    Group<R, Add_op>;

template <typename R, typename Add_op = sum<R>, typename Mul_op = product<R>>
requires Ring<R, Add_op, Mul_op>
constexpr auto
axiom_Ring(R const& a, R const& b, R const& c, Add_op add_op = {}, Mul_op mul_op = {}) noexcept -> bool
    //[[expects: a != Zero<R> && b != Zero<R> && c != Zero<R>]]
    //[[expects: a != One<R> && b != One<R> && c != One<R>]]
{
    if (!axiom_Semiring(a, b, c, add_op, mul_op)) return false;
    if (!axiom_Group(a, b, c, add_op)) return false;

    return true;
}

template <typename R, typename Add_op = sum<R>, typename Mul_op = product<R>>
concept Commutative_ring =
    Ring<R, Add_op, Mul_op>;

template <typename R, typename Add_op = sum<R>, typename Mul_op = product<R>>
requires Commutative_ring<R, Add_op, Mul_op>
constexpr auto
axiom_Commutative_ring(R const& a, R const& b, R const& c, Add_op add_op = {}, Mul_op mul_op = {}) noexcept -> bool
    //[[expects: a != Zero<R> && b != Zero<R> && c != Zero<R>]]
    //[[expects: a != One<R> && b != One<R> && c != One<R>]]
{
    if (!axiom_Ring(a, b, c, add_op, mul_op)) return false;

    if (!axiom_Commutative(a, b, mul_op)) return false;
    if (!axiom_Commutative(a, c, mul_op)) return false;
    if (!axiom_Commutative(b, c, mul_op)) return false;

    return true;
}

template <typename I, typename Add_op = sum<I>, typename Mul_op = product<I>>
concept Integral_domain =
    Commutative_ring<I, Add_op, Mul_op>;

template <typename I, typename Add_op = sum<I>, typename Mul_op = product<I>>
requires Integral_domain<I, Add_op, Mul_op>
constexpr auto
axiom_Integral_domain(I const& a, I const& b, I const& c, Add_op add_op = {}, Mul_op mul_op = {}) noexcept -> bool
    //[[expects: a != Zero<R> && b != Zero<R> && c != Zero<R>]]
    //[[expects: a != One<R> && b != One<R> && c != One<R>]]
{
    if (!axiom_Commutative_ring(a, b, c, add_op, mul_op)) return false;

    // No non-zero divisors
    if (a != Identity_element<I, Add_op> and b != Identity_element<I, Add_op>) {
        if (mul_op(a, b) == Identity_element<I, Add_op>) return false;
    }

    return true;
}

template <typename F, typename Add_op = sum<F>, typename Mul_op = product<F>>
concept Field =
    Integral_domain<F, Add_op, Mul_op> and
    Group<F, Mul_op>;

template <typename F, typename Add_op = sum<F>, typename Mul_op = product<F>>
requires Field<F, Add_op, Mul_op>
constexpr auto
axiom_Field(F const& a, F const& b, F const& c, Add_op add_op = {}, Mul_op mul_op = {}) noexcept -> bool
    //[[expects: a != Zero<R> && b != Zero<R> && c != Zero<R>]]
    //[[expects: a != One<R> && b != One<R> && c != One<R>]]
{
    if (!axiom_Integral_domain(a, b, c, add_op, mul_op)) return false;
    if (!axiom_Group(a, b, c, mul_op)) return false;

    return true;
}

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
        { s * v } -> Same_as<V>;
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
        { v * s } -> Same_as<V>;
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
