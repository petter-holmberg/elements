#pragma once

#include "intrinsics.h"
#include "cursor.h"

namespace elements {

template <Cursor C, Limit<C> L = C>
requires Loadable<C>
struct bounded_range
{
    C first;
    L limit;

    constexpr
    bounded_range() = default;

    constexpr bounded_range(C const& first_, L const& limit_)
        : first{first_}, limit{limit_}
    {}

    constexpr auto
    operator[](Difference_type<C> x) -> Value_type<C>&
    requires Mutable<C>
    {
        return at(first + x);
    }

    constexpr auto
    operator[](Difference_type<C> x) const -> const Value_type<C>&
    {
        return load(first + x);
    }
};

template <Cursor C, Limit<C> L>
requires Loadable<C>
constexpr auto
first(bounded_range<C, L> const& x) -> C
{
    return x.first;
}

template <Cursor C, Limit<C> L>
requires Loadable<C>
constexpr auto
limit(bounded_range<C, L> const& x) -> C
{
    return x.limit;
}

template <Cursor C, Limit<C> L>
requires Loadable<C>
constexpr auto
operator==(bounded_range<C, L> const& x, bounded_range<C, L> const& y) -> bool
{
    return first(x) == first(y) and limit(x) == limit(y);
}

template <Cursor C, Limit<C> L>
requires Loadable<C>
struct lt<bounded_range<C, L>>
{
    constexpr auto
    operator()(bounded_range<C, L> const& x, bounded_range<C, L> const& y) -> bool
    {
        lt less_cur;
        return
            less_cur(first(x), first(y)) or
            (!less_cur(first(y), first(x)) and less_cur(limit(x), limit(y)));
    }
};

template <Cursor C, Limit<C> L>
requires Loadable<C>
struct cursor_type_t<bounded_range<C, L>>
{
    using type = C;
};

template <Cursor C, Limit<C> L>
requires Loadable<C>
struct value_type_t<bounded_range<C, L>>
{
    using type = Value_type<C>;
};

template <Cursor C, Limit<C> L>
requires Loadable<C>
struct size_type_t<bounded_range<C, L>>
{
    using type = Difference_type<C>;
};

template <Cursor C, Limit<C> L>
requires Loadable<C>
constexpr auto
size(bounded_range<C, L> const& x) -> Difference_type<C>
{
    return limit(x) - first(x);
}

template <Cursor C, Limit<C> L>
requires Loadable<C>
constexpr auto
is_empty(bounded_range<C, L> const& x) -> bool
{
    return first(x) == limit(x);
}

template <typename R>
concept Range =
    requires (R& x) {
        first(x);
        limit(x);
    } and
    requires (R& x, Size_type<R> i) {
        { is_empty(x) } -> Boolean_testable;
        { size(x) } -> Same_as<Size_type<R>>;
        { x[i] } -> Same_as<Value_type<R>&>;
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
    Mutable<Cursor_type<R>>;

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
first(back<S>& seq) -> Cursor_type<S>
{
    return first(base(seq));
}

template <Sequence S>
constexpr auto
limit(back<S>& seq) -> Cursor_type<S>
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
first(front<S>& seq) -> Cursor_type<S>
{
    return first(base(seq));
}

template <Sequence S>
constexpr auto
limit(front<S>& seq) -> Cursor_type<S>
{
    return limit(base(seq));
}

template <typename S, typename I = back<S>>
concept Dynamic_sequence =
    Sequence<S> and
    requires (I s, Value_type<S> x) {
        { insert(s, x) } -> Same_as<I>;
        { erase(s) } -> Same_as<I>;
    };

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct before
{
    Pointer_type<S> seq;
    Cursor_type<S> cur;

    before(S& seq_, Cursor_type<S> cur_)
        : seq(&seq_)
        , cur(cur_)
    {}
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct value_type_t<before<S>>
{
    using type = Value_type<S>;
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct cursor_type_t<before<S>>
{
    using type = Cursor_type<S>;
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct size_type_t<before<S>>
{
    using type = Difference_type<Cursor_type<S>>;
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
base(before<S>& cur) -> S&
{
    return at(cur.seq);
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
current(before<S>& cur) -> Cursor_type<S>
{
    return {cur.cur};
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
first(before<S>& cur) -> Cursor_type<S>
{
    return first(base(cur));
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
last(before<S>& cur) -> Cursor_type<S>
{
    return last(base(cur));
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
limit(before<S>& cur) -> Cursor_type<S>
{
    return limit(base(cur));
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct after
{
    Pointer_type<S> seq;
    Cursor_type<S> cur;

    after(S& seq_, Cursor_type<S> cur_)
        : seq(&seq_)
        , cur(cur_)
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
struct cursor_type_t<after<S>>
{
    using type = Cursor_type<S>;
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
struct size_type_t<after<S>>
{
    using type = Difference_type<Cursor_type<S>>;
};

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
base(after<S>& cur) -> S&
{
    return at(cur.seq);
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
current(after<S>& cur) -> Cursor_type<S>
{
    return {cur.cur};
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
first(after<S>& cur) -> Cursor_type<S>
{
    return first(base(cur));
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
last(after<S>& cur) -> Cursor_type<S>
{
    return last(base(cur));
}

template <typename S>
    requires Dynamic_sequence<S, front<S>>
constexpr auto
limit(after<S>& cur) -> Cursor_type<S>
{
    return limit(base(cur));
}

template <typename C>
struct insert_cursor
{
    C cur{};

    explicit constexpr
    insert_cursor(C& cur_)
        : cur{cur_}
    {}
};

template <typename C>
struct value_type_t<insert_cursor<C>>
{
    using type = Value_type<C>;
};

template <typename C>
struct cursor_type_t<insert_cursor<C>>
{
    using type = Cursor_type<C>;
};

template <typename C>
constexpr auto
precedes(insert_cursor<C> const& cur0, C const& cur1) -> bool
{
    return cur0.cur != cur1;
}

template <typename C>
constexpr auto
precedes(C const& cur0, insert_cursor<C> const& cur1) -> bool
{
    return !(precedes(cur1, cur0));
}

template <typename C>
constexpr void
increment(insert_cursor<C>&)
{}

template <typename C>
constexpr void
store(insert_cursor<C>& cur, Value_type<C> const& x)
{
    cur.cur = insert(cur.cur, x);
}

template <typename C>
constexpr void
store(insert_cursor<C>& cur, Value_type<C>&& x)
{
    cur.cur = insert(cur.cur, fw<Value_type<C>>(x));
}

struct insert_sink
{
    template <typename C>
    auto
    operator()(C cur)
    {
        return insert_cursor{cur};
    }
};

template <Sequence S, typename C>
constexpr auto
insert_range(S const& seq, C cur) -> C
{
    return copy(first(seq), limit(seq), insert_sink{}(cur)).cur;
}

}
