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
    };
    // axiom associative(*) {
    //     (a * b) * c == a * (b * c);
    // }

template <typename S, typename Op>
requires Semigroup<S, Op>
struct identity_element_t;

template <typename S, typename Op>
requires Semigroup<S, Op>
constexpr S Identity_element = identity_element_t<S, Op>::value;

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

template <Additive_semigroup S>
struct zero_type;

template <Additive_semigroup S>
constexpr S Zero = zero_type<S>::value;

template <Additive_semigroup S>
struct sum;

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

template <Additive_semigroup S>
struct zero_type
{
    static constexpr S value = S{0};
};

template <Multiplicative_semigroup S>
struct one_type;

template <Multiplicative_semigroup S>
constexpr S One = one_type<S>::value;

template <Multiplicative_semigroup S>
struct product;

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

template <Multiplicative_semigroup S>
struct one_type
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
    };
    // axiom inverse(-, +, Zero<G>) {
    //     a + -a == Zero<G>;
    //     -a + a == Zero<G>;
    // }

template <Multiplicative_monoid M>
struct reciprocal;

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

// Ring-like

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

// Linear algebra

template <typename V, typename S = Value_type<V>>
concept Vector_space =
    Module<V, sum<V>, S, sum<S>, product<S>> and
    Field<S, sum<S>, product<S>>;

template <typename>
struct vector_type_t;

template <typename P>
using Vector_type = typename vector_type_t<P>::type;

template <typename P, typename V = Vector_type<P>, typename S = Value_type<V>>
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
