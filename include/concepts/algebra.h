#pragma once

#include "invocable.h"
#include "regular.h"

namespace elements {

// Group-like

template <typename S, typename Op>
concept Semigroup =
    Regular<S> and
    Binary_operation<Op>;
    // axiom associative(Op) {
    //     Op(Op(a, b), c) == Op(a, Op(b, c));
    // }

template <typename S>
concept Additive_semigroup =
    Regular<S> and
    requires (S const& a) {
        { a + a } -> S;
    } and
    requires (S& a) {
        { a += a } -> S&;
    };
    // axiom associative(+) {
    //     (a + b) + c == a + (b + c);
    // }
    // axiom commutative(+) {
    //     a + b == b + a;
    // }

template <typename S>
concept Multiplicative_semigroup =
    Regular<S> and
    requires (S const& a) {
        { a * a } -> S;
    } and
    requires (S& a) {
        { a *= a } -> S&;
    };
    // axiom associative(*) {
    //     (a * b) * c == a * (b * c);
    // }

template <typename S, typename Op>
requires Semigroup<S, Op>
struct identity_element_t;

template <typename S, typename Op>
requires Semigroup<S, Op>
constexpr S identity_element = identity_element_t<S, Op>::value;

template <typename M, typename Op>
concept Monoid =
    Semigroup<M, Op> and
    requires {
        M{identity_element<M, Op>};
    };
    // axiom identity(identity_element<M, Op>) {
    //     Op(identity_element<M, Op>, a) == a;
    //     Op(a, identity_element<M, Op>) == a;
    // }

template <Additive_semigroup S>
struct zero_t;

template <Additive_semigroup S>
constexpr S zero = zero_t<S>::value;

template <Additive_semigroup S>
struct sum;

template <typename M>
concept Additive_monoid =
    Additive_semigroup<M> and
    requires {
        M{zero<M>};
    };
    // axiom identity(zero<M>) {
    //     zero<M> + a == a;
    //     a + zero<M> == a;
    // }

template <Additive_semigroup S>
struct zero_t
{
    static constexpr S value = S{0};
};

template <Multiplicative_semigroup S>
struct one_t;

template <Multiplicative_semigroup S>
constexpr S one = one_t<S>::value;

template <Multiplicative_semigroup S>
struct product;

template <typename M>
concept Multiplicative_monoid =
    Multiplicative_semigroup<M> and
    requires {
        M{one<M>};
    };
    // axiom identity(one<M>) {
    //     one<M> * a == a;
    //     a * one<M> == a;
    // }

template <Multiplicative_semigroup S>
struct one_t
{
    static constexpr S value = S{1};
};

template <typename M, typename Op>
requires Monoid<M, Op>
struct inverse_operation;

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
    } and
    requires (G& a) {
        { a -= a } -> G&;
    };
    // axiom inverse(-, +, zero<G>) {
    //     a + -a == zero<G>;
    //     -a + a == zero<G>;
    // }

template <Multiplicative_monoid M>
struct reciprocal;

template <typename G>
concept Multiplicative_group =
    Multiplicative_monoid<G> and
    requires (G const& a) {
        { reciprocal<G>{}(a) } -> G;
        { a / a } -> G;
    } and
    requires (G& a) {
        { a /= a } -> G&;
    };
    // axiom inverse(reciprocal, product<G>, one<G>) {
    //     a * reciprocal(a) == one<G>;
    //     reciprocal(a) * a == one<G>;
    // }

// Ring-like

template <typename S, typename Add_op = sum<S>, typename Mul_op = product<S>>
concept Semiring =
    Monoid<S, Add_op> and
    Monoid<S, Mul_op>;
    // axiom distributive(Add_op, Mul_op) {
    //     Mul_op(a, Add_op(b, c)) == Add_op(Mul_op(a, b), Mul_op(a, c));
    //     Mul_op(Add_op(a, b), c) == Add_op(Mul_op(a, c), Mul_op(b, c));
    // }
    // axiom annihilation(Mul_op, zero<S>) {
    //     Mul_op(zero<S>, a) == zero<S>;
    //     Mul_op(a, zero<S>) == zero<S>;
    // }

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

// Module-like

template <typename>
struct scalar_type_t;

template <typename V>
using scalar_type = typename scalar_type_t<V>::type;

template <
    typename V, typename V_add_op = sum<V>,
    typename S = scalar_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Left_semimodule =
    Monoid<V, V_add_op> and
    Commutative_semiring<S, S_add_op, S_mul_op> and
    requires (V const& v, S const& s) {
        { s * v } -> V;
    };
    // axiom commutative(V_add_op) {
    //     MulV_add_op(a, b) == V_add_op(b, a);
    // }

template <
    typename V, typename V_add_op = sum<V>,
    typename S = scalar_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Right_semimodule =
    Monoid<V, V_add_op> and
    Commutative_semiring<S, S_add_op, S_mul_op> and
    requires (V const& v, S const& s) {
        { v * s } -> V;
    } and
    requires (V& v, S const& s) {
        { v *= s } -> V&;
    };
    // axiom commutative(V_add_op) {
    //     MulV_add_op(a, b) == V_add_op(b, a);
    // }

template <
    typename V, typename V_add_op = sum<V>,
    typename S = scalar_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Semimodule =
    Left_semimodule<V, V_add_op, S, S_add_op, S_mul_op> and
    Right_semimodule<V, V_add_op, S, S_add_op, S_mul_op>;

template <
    typename V, typename V_add_op = sum<V>,
    typename S = scalar_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Left_module =
    Left_semimodule<V, V_add_op, S, S_add_op, S_mul_op> and
    Group<V, V_add_op> and
    Ring<S, S_add_op, S_mul_op>;

template <
    typename V, typename V_add_op = sum<V>,
    typename S = scalar_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Right_module =
    Right_semimodule<V, V_add_op, S, S_add_op, S_mul_op> and
    Group<V, V_add_op> and
    Ring<S, S_add_op, S_mul_op>;

template <
    typename V, typename V_add_op = sum<V>,
    typename S = scalar_type<V>, typename S_add_op = sum<S>, typename S_mul_op = product<S>>
concept Module =
    Left_module<V, V_add_op, S, S_add_op, S_mul_op> and
    Right_module<V, V_add_op, S, S_add_op, S_mul_op> and
    Commutative_ring<S, S_add_op, S_mul_op>;

// Linear algebra

template <typename V, typename S = scalar_type<V>>
concept Vector_space =
    Module<V, sum<V>, S, sum<S>, product<S>> and
    Field<S, sum<S>, product<S>> and
    requires (V const& v, S const& s) {
        { v / s } -> V;
    } and
    requires (V& v, S const& s) {
        { v /= s } -> V&;
    };

template <typename>
struct vector_type_t;

template <typename P>
using vector_type = typename vector_type_t<P>::type;

template <typename P, typename V = vector_type<P>, typename S = scalar_type<V>>
concept Affine_space =
    Regular<P> and
    Vector_space<V, S> and
    requires (P const& p, V const& v) {
        { p - p } -> V;
        { p + v } -> P;
        { p - v } -> P;
    } and
    requires (P& p, V const& v) {
        { p += v } -> P&;
        { p -= v } -> P&;
    };

}
