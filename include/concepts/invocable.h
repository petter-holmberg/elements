#pragma once

#include "regular.h"
#include "type_functions/invocable.h"
#include "type_functions/regular.h"

namespace elements {

template <typename P>
concept Procedure =
    requires (domain<P> const& x) {
        domain<P>(x);
        Same<arity<P>, std::size_t>;
    };

template <typename F>
concept Functional_procedure =
    Procedure<F> and
    Regular<domain<F>>;

template <typename F>
concept Unary_function =
    Functional_procedure<F> and
    arity<F> == 1 and
    Regular<domain<F>>;

template <typename F>
concept Homogeneous_function =
    Functional_procedure<F> and
    arity<F> > 0 and
    Regular<domain<F>>;

template <typename Op>
concept Operation =
    Homogeneous_function<Op> and
    Same<codomain<Op>, domain<Op>>;

template <typename Op>
concept Unary_operation =
    Operation<Op> and
    arity<Op> == 1;

template <typename Op>
concept Binary_operation =
    Operation<Op> and
    arity<Op> == 2;

template <typename P>
concept Predicate =
    Functional_procedure<P> and
    Same<codomain<P>, bool>;

template <typename P>
concept Unary_predicate =
    Predicate<P> and
    Unary_function<P>;

template <typename P>
concept Homogeneous_predicate =
    Predicate<P> and
    Homogeneous_function<P>;

template <typename R>
concept Relation =
    Homogeneous_predicate<R> and
    arity<R> == 2;

}
