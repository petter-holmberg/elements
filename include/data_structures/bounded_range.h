#pragma once

#include "adapters/counted_position.h"
#include "position.h"
#include "regular.h"

namespace elements {

template <Position P, Limit<P> L = P>
requires Loadable<P>
struct bounded_range
{
    P first;
    L limit;

    constexpr
    bounded_range() = default;

    constexpr bounded_range(P const& first_, L const& limit_)
        : first{first_}, limit{limit_}
    {}

    constexpr auto
    operator[](Difference_type<P> x) -> Value_type<P>&
    requires Mutable<P>
    {
        return at(first + x);
    }

    constexpr auto
    operator[](Difference_type<P> x) const -> const Value_type<P>&
    {
        return load(first + x);
    }
};

template <Position P, Limit<P> L>
requires Loadable<P>
constexpr auto
first(bounded_range<P, L> const& x) -> P
{
    return x.first;
}

template <Position P, Limit<P> L>
requires Loadable<P>
constexpr auto
limit(bounded_range<P, L> const& x) -> L
{
    return x.limit;
}

template <Position P, Limit<P> L>
requires Loadable<P>
constexpr auto
operator==(bounded_range<P, L> const& x, bounded_range<P, L> const& y) -> bool
{
    return first(x) == first(y) and limit(x) == limit(y);
}

template <Position P, Limit<P> L>
requires Loadable<P>
struct less<bounded_range<P, L>>
{
    constexpr auto
    operator()(bounded_range<P, L> const& x, bounded_range<P, L> const& y) -> bool
    {
        less<P> less_pos;
        return
            less_pos(first(x), first(y)) or
            (!less_pos(first(y), first(x)) and less_pos(limit(x), limit(y)));
        }
};

template <Position P, Limit<P> L>
requires Loadable<P>
struct position_type_t<bounded_range<P, L>>
{
    using type = P;
};

template <Position P, Limit<P> L>
requires Loadable<P>
struct value_type_t<bounded_range<P, L>>
{
    using type = Value_type<P>;
};

template <Position P, Limit<P> L>
requires Loadable<P>
struct size_type_t<bounded_range<P, L>>
{
    using type = Difference_type<P>;
};

template <Position P, Limit<P> L>
requires Loadable<P>
constexpr auto
size(bounded_range<P, L> const& x) -> Difference_type<P>
{
    return limit(x) - first(x);
}

template <Position P, Limit<P> L>
requires Loadable<P>
constexpr auto
is_empty(bounded_range<P, L> const& x) -> bool
{
    return first(x) == limit(x);
}

}
