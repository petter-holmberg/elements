#pragma once

#include "integer.h"

namespace elements {

template <typename T>
using Pointer_type = T*;

template <typename T>
constexpr auto
pointer_to(T& x) -> Pointer_type<T>
{
    return &x;
}

using pointer_diff = std::ptrdiff_t;

template <typename T>
struct value_type_t<Pointer_type<T>>
{
    using type = T;
};

template <typename T>
struct difference_type_t<Pointer_type<T>>
{
    using type = pointer_diff;
};

template <typename T>
struct cursor_type_t
{
    using type = Pointer_type<T>;
};

template <typename T>
struct cursor_type_t<T const>
{
    using type = Pointer_type<T const>;
};

template <typename T>
using Cursor_type = typename cursor_type_t<T>::type;

template <typename>
struct size_type_t;

template <typename T>
using Size_type = typename size_type_t<T>::type;

template <typename T>
constexpr auto Size = size_type_t<T>::value;

template <typename T, typename E>
constexpr auto
allocate(E extra_bytes = Zero<E>) -> Pointer_type<T>
{
    return reinterpret_cast<Pointer_type<T>>(
        std::malloc(sizeof(T) + static_cast<std::size_t>(extra_bytes)));
}

template <typename T>
constexpr void
deallocate(Pointer_type<T> raw)
{
    std::free(reinterpret_cast<void*>(raw));
}

template <Constructible_from T>
constexpr auto
load(T const& x) -> Value_type<T> const&
{
    return x;
}

template <Constructible_from T>
constexpr auto
load(Pointer_type<T> x) -> T const&
{
    return *x;
}

template <typename L>
concept Loadable =
    Move_constructible<L> and
    requires (L const& x) {
        { load(x) } -> Same_as<Value_type<L> const&>;
    };

template <typename T>
constexpr void
store(T& x, Value_type<T> const& v)
{
    x = v;
}

template <typename T>
constexpr void
store(T& x, Value_type<T>&& v)
{
    x = std::forward<Value_type<T>>(v);
}

template <typename T>
constexpr void
store(Pointer_type<T> x, Value_type<T> const& v)
{
    *x = v;
}

template <typename T>
constexpr void
store(Pointer_type<T> x, Value_type<T>&& v)
{
    *x = std::forward<Value_type<T>>(v);
}

template <typename S>
concept Storable =
    Move_constructible<S> and
    requires (S& x, Value_type<S>&& v) {
        store(x, v);
    };

template <Constructible_from T>
constexpr auto
at(T& x) -> T&
{
    return x;
}

template <Constructible_from T>
constexpr auto
at(T const& x) -> T const&
{
    return x;
}

template <Constructible_from T>
constexpr auto
at(Pointer_type<T> x) -> T&
{
    return *x;
}

template <Constructible_from T>
constexpr auto
at(Pointer_type<T const> x) -> T const&
{
    return *x;
}

template <typename M>
concept Mutable =
    Loadable<M> and
    Storable<M> and
    requires (M& x) {
        { at(x) } -> Same_as<Value_type<M>&>;
    };

static_assert(Mutable<int>);
static_assert(Mutable<int*>);

template <Constructible_from T>
constexpr void
increment(Pointer_type<T>& x)
{
    x = x + One<Difference_type<Pointer_type<T>>>;
}

template <typename C>
concept Cursor =
    Movable<C> and
    requires (C cur) {
        increment(cur);
    };

template <Cursor C, Movable L>
constexpr auto
precedes(C const&, L const&) -> bool
{
    return false;
}

template <typename T>
requires Movable<Decay<T>>
constexpr auto
precedes(Pointer_type<T> const& cur0, Pointer_type<T> const& cur1) -> bool
{
    return cur0 != cur1;
}

template <typename L, typename C>
concept Limit =
    Regular<L> and
    Cursor<C> and
    requires (C const& cur, L const& lim) {
        { precedes(cur, lim) } -> Boolean_testable;
    };

template <Cursor C>
constexpr auto
successor(C cur) -> C
{
    increment(cur);
    return cur;
}

template <Constructible_from T>
constexpr auto
successor(Pointer_type<T> x) -> Pointer_type<T>
{
    increment(x);
    return x;
}

template <Constructible_from T>
constexpr void
decrement(Pointer_type<T>& x)
{
    x = x - One<Difference_type<Pointer_type<T>>>;
}

template <Cursor C>
constexpr auto
operator+(C cur, Difference_type<C> dist) -> C
//[[expects axiom: dist >= Zero<Difference_type<C>>]]
//[[expects axiom: is_weak_range(cur, dist)]]
{
    while (!is_zero(dist)) {
        increment(cur);
        decrement(dist);
    }
    return cur;
}

template <Cursor C>
constexpr auto
operator+(Difference_type<C> dist, C cur) -> C
//[[expects axiom: dist >= Zero<Difference_type<C>>]]
//[[expects axiom: is_weak_range(cur, dist)]]
{
    return cur + dist;
}

template <Cursor C, Limit<C> L>
constexpr auto
operator-(L lim, C cur) -> Difference_type<C>
//[[expects axiom: is_bounded_range(cur, lim)]]
{
    auto n(Zero<Difference_type<C>>);
    while (precedes(cur, lim)) {
        increment(n);
        increment(cur);
    }
    return n;
}

template <typename C>
concept Forward_cursor =
    Regular<C> and
    Cursor<C>;

template <
    Cursor S0,
    Limit<S0> L0,
    Cursor S1,
    Forward_cursor D,
    Operation<Value_type<S0>, Value_type<S1>> S_add_op,
    Operation<Value_type<S0>, Value_type<S1>> S_mul_op>
requires
    Loadable<S0> and
    Loadable<S1> and
    Storable<D> and
    Semiring<Decay<Value_type<S0>>, S_add_op, S_mul_op> and
    Semiring<Decay<Value_type<S1>>, S_add_op, S_mul_op> and
    Semiring<Decay<Value_type<D>>, S_add_op, S_mul_op>
constexpr auto
inner_product(S0 src0, L0 lim0, S1 src1, D dst, S_add_op add_op, S_mul_op mul_op) -> Value_type<D>
{
    return reduce(
        dst,
        map(mv(src0), mv(lim0), mv(src1), dst, mul_op),
        add_op,
        Zero<Value_type<D>>);
}

template <typename C>
concept Bidirectional_cursor =
    Forward_cursor<C> and
    requires (C cur) {
        decrement(cur);
    };

template <Bidirectional_cursor C>
constexpr auto
predecessor(C cur) -> C
{
    decrement(cur);
    return cur;
}

template <Constructible_from T>
constexpr auto
predecessor(Pointer_type<T> x) -> Pointer_type<T>
{
    decrement(x);
    return x;
}

template <Bidirectional_cursor C>
struct reverse_cursor
{
    C cur{};

    constexpr
    reverse_cursor() = default;

    explicit constexpr
    reverse_cursor(C const& cur_) : cur{cur_} {}
};

template <Bidirectional_cursor C>
struct value_type_t<reverse_cursor<C>>
{
    using type = Value_type<C>;
};

template <Bidirectional_cursor C>
struct difference_type_t<reverse_cursor<C>>
{
    using type = Difference_type<C>;
};

template <Bidirectional_cursor C>
constexpr auto
operator==(reverse_cursor<C> const& cur0, reverse_cursor<C> const& cur1) -> bool
{
    return cur0.cur == cur1.cur;
}

template <Bidirectional_cursor C>
constexpr void
increment(reverse_cursor<C>& cur)
{
    decrement(cur.cur);
}

template <Bidirectional_cursor C>
constexpr void
decrement(reverse_cursor<C>& cur)
{
    increment(cur.cur);
}

template <Bidirectional_cursor C>
constexpr auto
successor(reverse_cursor<C> const& cur) -> reverse_cursor<C>
{
    return reverse_cursor<C>{predecessor(cur.cur)};
}

template <Bidirectional_cursor C>
constexpr auto
predecessor(reverse_cursor<C> const& cur) -> reverse_cursor<C>
{
    return reverse_cursor<C>{successor(cur.cur)};
}

template <Bidirectional_cursor C>
requires Loadable<C>
constexpr auto
load(reverse_cursor<C> const& cur) -> Value_type<C> const&
{
    return load(predecessor(cur.cur));
}

template <Bidirectional_cursor C>
requires Storable<C>
constexpr void
store(reverse_cursor<C>& cur, Value_type<C> const& value)
{
    store(predecessor(cur.cur), value);
}

template <Bidirectional_cursor C>
requires Storable<C>
constexpr void
store(reverse_cursor<C>& cur, Value_type<C>&& value)
{
    store(predecessor(cur.cur), fw<Value_type<C>>(value));
}

template <Bidirectional_cursor C>
requires Mutable<C>
constexpr auto
at(reverse_cursor<C> const& cur) -> Value_type<C> const&
{
    return at(predecessor(cur.cur));
}

template <Bidirectional_cursor C>
requires Mutable<C>
constexpr auto
at(reverse_cursor<C>& cur) -> Value_type<C>&
{
    return at(predecessor(cur.cur));
}

template <Bidirectional_cursor C>
constexpr auto
precedes(reverse_cursor<C> const& cur0, reverse_cursor<C> const& cur1) -> bool
{
    return cur0.cur != cur1.cur;
}

template <typename C>
concept Indexed_cursor =
    Forward_cursor<C> and
    requires (C cur, Difference_type<C> n) {
        { cur + n } -> Same_as<C>;
        { cur - cur } -> Same_as<Difference_type<C>>;
    };

template <Cursor C>
requires Loadable<C>
struct loadable_cursor
{
    C cur{};

    explicit loadable_cursor(C cur_) : cur{cur_} {}

    loadable_cursor(loadable_cursor const&) = delete;

    loadable_cursor(loadable_cursor&&) = default;

    loadable_cursor& operator=(loadable_cursor const&) = delete;

    loadable_cursor& operator=(loadable_cursor&&) = default;

    ~loadable_cursor() = default;
};

template <Cursor C>
requires Loadable<C>
struct value_type_t<loadable_cursor<C>>
{
    using type = Value_type<C>;
};

template <Cursor C>
requires Loadable<C>
struct difference_type_t<loadable_cursor<C>>
{
    using type = Difference_type<C>;
};

template <Cursor C>
constexpr void
increment(loadable_cursor<C>& cur)
{
    increment(cur.cur);
}

template <Cursor C>
constexpr void
decrement(loadable_cursor<C>& cur)
{
    decrement(cur.cur);
}

template <Cursor C>
constexpr auto
successor(loadable_cursor<C> const& cur) -> loadable_cursor<C>
{
    return loadable_cursor<C>(successor(cur.cur));
}

template <Cursor C>
constexpr auto
predecessor(loadable_cursor<C> const& cur) -> loadable_cursor<C>
{
    return loadable_cursor<C>(predecessor(cur.cur));
}

template <Cursor C>
requires Loadable<C>
constexpr auto
load(loadable_cursor<C> const& cur) -> Value_type<C> const&
{
    return load(cur.cur);
}

template <Cursor C>
constexpr auto
precedes(loadable_cursor<C> const& cur0, loadable_cursor<C> const& cur1) -> bool
{
    return cur0.cur != cur1.cur;
}

template <Cursor C>
struct counted_cursor
{
    C cur{};
    Difference_type<C> count{Zero<Difference_type<C>>};

    constexpr
    counted_cursor() = default;

    explicit constexpr
    counted_cursor(C const& cur_, Difference_type<C> const& count_ = Zero<Difference_type<C>>)
        : cur{cur_}
        , count{count_}
    {}
};

template <Cursor C>
struct value_type_t<counted_cursor<C>>
{
    using type = Value_type<C>;
};

template <Cursor C>
struct difference_type_t<counted_cursor<C>>
{
    using type = Difference_type<C>;
};

template <Cursor C>
constexpr auto
operator==(counted_cursor<C> const& cur0, counted_cursor<C> const& cur1) -> bool
{
    return cur0.cur == cur1.cur and cur0.count == cur1.count;
}

template <Cursor C>
constexpr void
increment(counted_cursor<C>& cur)
{
    increment(cur.cur);
    increment(cur.count);
}

template <Bidirectional_cursor C>
constexpr void
decrement(counted_cursor<C>& cur)
{
    decrement(cur.cur);
    decrement(cur.count);
}

template <Cursor C>
constexpr auto
successor(counted_cursor<C> const& cur) -> counted_cursor<C>
{
    return counted_cursor<C>{successor(cur.cur), successor(cur.count)};
}

template <Bidirectional_cursor C>
constexpr auto
predecessor(counted_cursor<C> const& cur) -> counted_cursor<C>
{
    return counted_cursor<C>{predecessor(cur.cur), predecessor(cur.count)};
}

template <Cursor C>
requires Loadable<C>
constexpr auto
load(counted_cursor<C> const& cur) -> Value_type<C> const&
{
    return load(cur.cur);
}

template <Cursor C>
requires Storable<C>
constexpr void
store(counted_cursor<C>& cur, Value_type<C> const& value)
{
    store(cur.cur, value);
}

template <Cursor C>
requires Storable<C>
constexpr void
store(counted_cursor<C>& cur, Value_type<C>&& value)
{
    store(cur.cur, fw<Value_type<C>>(value));
}

template <Cursor C>
requires Mutable<C>
constexpr auto
at(counted_cursor<C>& cur) -> Value_type<C>&
{
    return at(cur.cur);
}

template <Cursor C, Regular T>
requires Equality_comparable_with<Difference_type<C>, T>
constexpr auto
precedes(counted_cursor<C> const& cur, T const& count) -> bool
{
    return cur.count != count;
}

template <Cursor C>
requires Loadable<C>
struct cursor
{
    C cur;

    cursor() = default;

    explicit cursor(C cur_) : cur{cur_} {}

    cursor(cursor const&) = delete;

    cursor(cursor&&) = default;

    cursor& operator=(cursor const&) = delete;

    cursor& operator=(cursor&&) = default;

    ~cursor() = default;
};

template <Cursor C>
requires Loadable<C>
struct value_type_t<cursor<C>>
{
    using type = Value_type<C>;
};

template <Cursor C>
requires Loadable<C>
struct difference_type_t<cursor<C>>
{
    using type = Difference_type<C>;
};

template <Cursor C>
constexpr void
increment(cursor<C>& cur)
{
    increment(cur.cur);
}

template <Cursor C>
constexpr auto
successor(cursor<C> const& cur) -> cursor<C>
{
    return {successor(cur.cur)};
}

template <Cursor C>
requires Loadable<C>
constexpr auto
load(cursor<C> const& cur) -> Value_type<C> const&
{
    return load(cur.cur);
}

template <Cursor C>
requires Storable<C>
constexpr void
store(cursor<C>& cur, Value_type<C> const& value)
{
    store(cur.cur, value);
}

template <Cursor C>
requires Storable<C>
constexpr void
store(cursor<C>& cur, Value_type<C>&& value)
{
    store(cur.cur, std::forward<Value_type<C>>(value));
}

template <Cursor C>
requires Mutable<C>
constexpr auto
at(cursor<C> const& cur) -> Value_type<C> const&
{
    return at(cur.cur);
}

template <Cursor C>
requires Mutable<C>
constexpr auto
at(cursor<C>& cur) -> Value_type<C>&
{
    return at(cur.cur);
}

template <Cursor C>
constexpr auto
precedes(cursor<C> const& cur0, cursor<C> const& cur1) -> bool
{
    return cur0.cur != cur1.cur;
}

template <Cursor C, Limit<C> L>
constexpr auto
precedes(cursor<C> const& cur, L const& lim) -> bool
{
    return cur.cur != lim;
}

template <Forward_cursor C>
requires Loadable<C>
struct forward_cursor
{
    C cur;

    forward_cursor() = default;

    explicit forward_cursor(C cur_) : cur{cur_} {}
};

template <Forward_cursor C>
requires Loadable<C>
struct value_type_t<forward_cursor<C>>
{
    using type = Value_type<C>;
};

template <Forward_cursor C>
requires Loadable<C>
struct difference_type_t<forward_cursor<C>>
{
    using type = Difference_type<C>;
};

template <Forward_cursor C>
constexpr auto
operator==(forward_cursor<C> const& cur0, forward_cursor<C> const& cur1) -> bool
{
    return cur0.cur == cur1.cur;
}

template <Forward_cursor C>
constexpr auto
operator==(forward_cursor<C> const& cur0, C const& cur1) -> bool
{
    return cur0.cur == cur1;
}

template <Forward_cursor C>
constexpr auto
operator==(C const& cur0, forward_cursor<C> const& cur1) -> bool
{
    return cur0 == cur1.cur;
}

template <Forward_cursor C>
constexpr void
increment(forward_cursor<C>& cur)
{
    increment(cur.cur);
}

template <Forward_cursor C>
constexpr auto
successor(forward_cursor<C> const& cur) -> forward_cursor<C>
{
    return {successor(cur.cur)};
}

template <Forward_cursor C>
requires Loadable<C>
constexpr auto
load(forward_cursor<C> const& cur) -> Value_type<C> const&
{
    return load(cur.cur);
}

template <Forward_cursor C>
requires Storable<C>
constexpr void
store(forward_cursor<C>& cur, Value_type<C> const& value)
{
    store(cur.cur, value);
}

template <Forward_cursor C>
requires Storable<C>
constexpr void
store(forward_cursor<C>& cur, Value_type<C>&& value)
{
    store(cur.cur, std::forward<Value_type<C>>(value));
}

template <Forward_cursor C>
requires Mutable<C>
constexpr auto
at(forward_cursor<C> const& cur) -> Value_type<C> const&
{
    return at(cur.cur);
}

template <Forward_cursor C>
requires Mutable<C>
constexpr auto
at(forward_cursor<C>& cur) -> Value_type<C>&
{
    return at(cur.cur);
}

template <Forward_cursor C>
constexpr auto
precedes(forward_cursor<C> const& cur0, forward_cursor<C> const& cur1) -> bool
{
    return cur0.cur != cur1.cur;
}

template <Forward_cursor C, Limit<C> L>
constexpr auto
precedes(forward_cursor<C> const& cur, L const& lim) -> bool
{
    return cur.cur != lim;
}

template <Forward_cursor C, Limit<C> L>
constexpr void
advance(C& cur, L lim)
{
    while (precedes(cur, lim)) increment(cur);
}

template <Bidirectional_cursor C>
constexpr auto
operator-(C cur, Difference_type<C> dist) -> C
//[[expects axiom: dist >= Zero<Difference_type<C>>]]
//[[expects axiom first: is_weak_range(first, dist)]]
//[[expects axiom first: cur = first + dist]]
{
    while (!is_zero(dist)) {
        decrement(cur);
        decrement(dist);
    }
    return cur;
}

template <Bidirectional_cursor C>
requires Loadable<C>
struct bidirectional_cursor
{
    C cur;

    bidirectional_cursor() = default;

    explicit bidirectional_cursor(C cur_) : cur{cur_} {}
};

template <Bidirectional_cursor C>
requires Loadable<C>
struct value_type_t<bidirectional_cursor<C>>
{
    using type = Value_type<C>;
};

template <Bidirectional_cursor C>
requires Loadable<C>
struct difference_type_t<bidirectional_cursor<C>>
{
    using type = Difference_type<C>;
};

template <Bidirectional_cursor C>
constexpr auto
operator==(bidirectional_cursor<C> const& cur0, bidirectional_cursor<C> const& cur1) -> bool
{
    return cur0.cur == cur1.cur;
}

template <Bidirectional_cursor C>
constexpr void
increment(bidirectional_cursor<C>& cur)
{
    increment(cur.cur);
}

template <Bidirectional_cursor C>
constexpr void
decrement(bidirectional_cursor<C>& cur)
{
    decrement(cur.cur);
}

template <Bidirectional_cursor C>
constexpr auto
successor(bidirectional_cursor<C> const& cur) -> bidirectional_cursor<C>
{
    return successor(cur.cur);
}

template <Bidirectional_cursor C>
constexpr auto
predecessor(bidirectional_cursor<C> const& cur) -> bidirectional_cursor<C>
{
    return predecessor(cur.cur);
}

template <Bidirectional_cursor C>
requires Loadable<C>
constexpr auto
load(bidirectional_cursor<C> const& cur) -> Value_type<C> const&
{
    return load(cur.cur);
}

template <Bidirectional_cursor C>
requires Storable<C>
constexpr void
store(bidirectional_cursor<C>& cur, Value_type<C> const& value)
{
    store(cur.cur, value);
}

template <Bidirectional_cursor C>
requires Storable<C>
constexpr void
store(bidirectional_cursor<C>& cur, Value_type<C>&& value)
{
    store(cur.cur, std::forward<Value_type<C>>(value));
}

template <Bidirectional_cursor C>
requires Mutable<C>
constexpr auto
at(bidirectional_cursor<C> const& cur) -> Value_type<C> const&
{
    return at(cur.cur);
}

template <Bidirectional_cursor C>
requires Mutable<C>
constexpr auto
at(bidirectional_cursor<C>& cur) -> Value_type<C>&
{
    return at(cur.cur);
}

template <Bidirectional_cursor C>
constexpr auto
precedes(bidirectional_cursor<C> const& cur0, bidirectional_cursor<C> const& cur1) -> bool
{
    return cur0.cur != cur1.cur;
}

template <typename C>
concept Linked_forward_cursor =
    Forward_cursor<C> and
    requires (C cur) {
        next_link(cur);
    };

template <typename L>
concept Forward_linker =
    requires (L linker, Cursor_type<L> cur) {
        linker(cur, cur);
    };

template <Linked_forward_cursor C>
struct forward_linker
{
    void operator()(C cur0, C cur1)
    {
        next_link(cur0) = cur1.cur;
    }
};

template <Linked_forward_cursor C>
struct cursor_type_t<forward_linker<C>>
{
    using type = C;
};

template <Linked_forward_cursor C>
constexpr void
set_link_forward(C cur0, C cur1)
{
    forward_linker<C>{}(cur0, cur1);
}

template <typename C>
concept Linked_bidirectional_cursor =
    Linked_forward_cursor<C> and
    requires (C cur) {
        prev_link(cur);
    };

template <Linked_bidirectional_cursor C>
struct bidirectional_linker
{
    void operator()(C cur0, C cur1)
    {
        next_link(cur0) = cur1.cur;
        prev_link(cur1) = cur0.cur;
    }
};

template <Linked_bidirectional_cursor C>
struct cursor_type_t<bidirectional_linker<C>>
{
    using type = C;
};

template <Linked_bidirectional_cursor C>
constexpr void
set_link_bidirectional(C cur0, C cur1)
{
    bidirectional_linker<C>{}(cur0, cur1);
}

template <typename>
struct index_cursor_type_t;

template <typename T>
using Index_cursor_type = typename index_cursor_type_t<T>::type;

template <typename>
struct segment_cursor_type_t;

template <typename T>
using Segment_cursor_type = typename segment_cursor_type_t<T>::type;

template <typename C>
concept Segmented_cursor =
    Movable<C> and
    requires (C x) {
        Cursor<Index_cursor_type<C>>;
        Cursor<Segment_cursor_type<C>>;
        { index_cursor(x) } -> Same_as<Index_cursor_type<C>&>;
        { segment_cursor(x) } -> Same_as<Segment_cursor_type<C>&>;
    };

}
