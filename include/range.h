#pragma once

#include "copy.h"

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

template <typename R>
concept Range =
    requires (R&& x) {
        { first(x) } -> Position_type<R>;
        limit(x);
    } and
    requires (R x, Size_type<R> i) {
        { is_empty(x) } -> bool;
        { size(x) } -> Size_type<R>;
        { x[i] } -> Value_type<R> const&;
        { x[i] } -> Value_type<R>&;
        // axiom {
        //     is_empty(x) == limit(x) - first(x);
        // }
        // axiom {
        //     size(x) == limit(x) - first(x);
        // }
    };

template <typename R>
concept Mutable_range =
    Range<R> and
    Mutable<Position_type<R>>;

template <typename S>
concept Sequence =
    Range<S> and
    Default_totally_ordered<S>;

template <Sequence S>
struct back
{
    Pointer_type<S> seq{};

    back() = default;

    back(S& seq_)
        : seq{pointer_to(seq_)}
    {}
};

template <Sequence S>
struct value_type_t<back<S>>
{
    using type = Value_type<S>;
};

template <Sequence S>
constexpr auto
base(back<S>& seq) -> S&
{
    return at(seq.seq);
}

template <Sequence S>
constexpr auto
first(back<S>& seq) -> Position_type<S>
{
    return first(base(seq));
}

template <Sequence S>
constexpr auto
limit(back<S>& seq) -> Position_type<S>
{
    return limit(base(seq));
}

template <Sequence S>
struct front
{
    Pointer_type<S> seq{};

    front() = default;

    front(S& seq_)
        : seq{pointer_to(seq_)}
    {}
};

template <Sequence S>
struct value_type_t<front<S>>
{
    using type = Value_type<S>;
};

template <Sequence S>
constexpr auto
base(front<S>& seq) -> S&
{
    return at(seq.seq);
}

template <Sequence S>
constexpr auto
first(front<S>& seq) -> Position_type<S>
{
    return first(base(seq));
}

template <Sequence S>
constexpr auto
limit(front<S>& seq) -> Position_type<S>
{
    return limit(base(seq));
}

template <typename S, typename I = back<S>>
concept Dynamic_sequence =
    Sequence<S> and
    requires (I s, Value_type<S> x) {
        { insert(s, x) } -> I;
        { erase(s) } -> I;
    };

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct after
{
    Pointer_type<S> seq;
    Position_type<S> pos;

    after(S& seq_, Position_type<S> pos_)
        : seq(&seq_)
        , pos(pos_)
    {}
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct value_type_t<after<S>>
{
    using type = Value_type<S>;
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct position_type_t<after<S>>
{
    using type = Position_type<S>;
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct size_type_t<after<S>>
{
    using type = Difference_type<Position_type<S>>;
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
base(after<S>& pos) -> S&
{
    return at(pos.seq);
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
current(after<S>& pos) -> Position_type<S>
{
    return {pos.pos};
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
first(after<S>& pos) -> Position_type<S>
{
    return first(base(pos));
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
last(after<S>& pos) -> Position_type<S>
{
    return last(base(pos));
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
limit(after<S>& pos) -> Position_type<S>
{
    return limit(base(pos));
}

template <typename P>
struct insert_position
{
    P pos{};

    explicit constexpr
    insert_position(P& pos_)
        : pos{pos_}
    {}
};

template <typename P>
struct value_type_t<insert_position<P>>
{
    using type = Value_type<P>;
};

template <typename P>
struct position_type_t<insert_position<P>>
{
    using type = Position_type<P>;
};

template <typename P>
constexpr auto
precedes(insert_position<P> const& pos0, P const& pos1) -> bool
{
    return pos0.pos != pos1;
}

template <typename P>
constexpr auto
precedes(P const& pos0, insert_position<P> const& pos1) -> bool
{
    return !(precedes(pos1, pos0));
}

template <typename P>
constexpr void
increment(insert_position<P>&)
{}

template <typename P>
constexpr void
store(insert_position<P>& pos, Value_type<P> const& x)
{
    pos.pos = insert(pos.pos, x);
}

template <typename P>
constexpr void
store(insert_position<P>& pos, Value_type<P>&& x)
{
    pos.pos = insert(pos.pos, fw<Value_type<P>>(x));
}

struct insert_sink
{
    template <typename P>
    auto
    operator()(P pos)
    {
        return insert_position{pos};
    }
};

template <Sequence S, typename P>
constexpr auto
insert_range(S const& seq, P pos) -> P
{
    return copy(first(seq), limit(seq), insert_sink{}(pos)).pos;
}

template <typename T, typename P>
constexpr auto
insert_range(std::initializer_list<T> seq, P pos) -> P
{
    return copy(std::begin(seq), std::end(seq), insert_sink{}(pos)).pos;
}

template <Position P, Limit<P> L, Unary_function Fun>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<Fun>> and
    Sequence<Codomain<Fun>>
constexpr auto
flat_map(P src, L lim, Fun fun) -> Codomain<Fun>
{
    Codomain<Fun> x;
    while (precedes(src, lim)) {
        auto y = fun(load(src));
        reserve(x, size(x) + size(y));
        insert_range(y, back{x});
        increment(src);
    }
    return x;
}

}
