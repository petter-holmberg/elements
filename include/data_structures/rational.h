#pragma once

#include "algorithms/regular.h"
#include "algorithms/ordered_algebra.h"
#include "algorithms/position.h"

namespace elements {

template <Ordered_integral_domain I>
struct rational
{
    constexpr
    rational() = default;

    constexpr
    rational(I const& p_)
        : p{p_}, q{One<I>}
    {}

    constexpr
    rational(I const& p_, I const& q_)
    //[[expects: q != Zero<I>]]
        : p{p_}, q{q_}
    {}

    I p = Zero<I>;
    I q = One<I>;
};

template <Ordered_integral_domain I>
struct underlying_type_t<rational<I>>
{
    using type = rational<Underlying_type<I>>;
};

template <Ordered_integral_domain I>
struct value_type_t<rational<I>>
{
    using type = I;
};

template <Ordered_integral_domain I>
struct zero_type<rational<I>>
{
    static constexpr rational<I> value = rational<I>{Zero<I>, One<I>};
};

template <Ordered_integral_domain I>
struct one_type<rational<I>>
{
    static constexpr rational<I> value = rational<I>{One<I>, One<I>};
};

template <Ordered_integral_domain I>
constexpr auto
operator==(rational<I> const& x, rational<I> const& y) -> bool
{
    return x.p * y.q == x.q * y.p;
}

template <Ordered_integral_domain I>
constexpr auto
operator<(rational<I> const& x, rational<I> const& y) -> bool
{
    return x.p * y.q < x.q * y.p;
}

template <Ordered_integral_domain I>
constexpr auto
operator+(rational<I> const& x, rational<I> const& y) -> rational<I>
{
    return {x.p * y.q + x.q * y.p, x.q * y.q};
}

template <Ordered_integral_domain I>
constexpr auto
operator*(rational<I> const& x, rational<I> const& y) -> rational<I>
{
    return {x.p * y.p, x.q * y.q};
}

template <Ordered_integral_domain I>
constexpr auto
operator-(rational<I> const& x) -> rational<I>
{
    return {-x.p, x.q};
}

template <Ordered_integral_domain I>
constexpr auto
operator-(rational<I> const& x, rational<I> const& y) -> rational<I>
{
    return x + (-y);
}

template <Ordered_integral_domain I>
struct reciprocal<rational<I>>
{
    constexpr auto
    operator()(rational<I> const& x) const -> rational<I>
    // [[expects: x.p != 0]]
    {
        return {x.q, x.p};
    }
};

template <Ordered_integral_domain I>
constexpr auto
operator/(rational<I> const& x, rational<I> const& y) -> rational<I>
{
    return {x.p * y.q, x.q * y.p};
}

template <Ordered_integral_domain I>
// and Left_semimodule<rational<I>>
constexpr auto
operator*(I const& n, rational<I> const& x) -> rational<I>
{
    return {n * x.p, x.q};
}

template <Ordered_integral_domain I>
//  and Right_semimodule<rational<I>>
constexpr auto
operator*(rational<I> const& x, I const& n) -> rational<I>
{
    return {x.p * n, x.q};
}

}