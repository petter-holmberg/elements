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
struct position_type_t
{
    using type = Pointer_type<T>;
};

template <typename T>
using Position_type = typename position_type_t<T>::type;

template <typename T>
struct position_type_t<T const>
{
    using type = Position_type<T const>;
};

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
        { load(x) } -> Value_type<L> const&;
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
        { at(x) } -> Value_type<M>&;
    } and
    requires (M const& x) {
        { at(x) } -> Value_type<M> const&;
    };

template <Constructible_from T>
constexpr void
increment(Pointer_type<T>& x)
{
    x = x + One<Difference_type<Pointer_type<T>>>;
}

template <typename P>
concept Position =
    Movable<P> and
    requires (P x) {
        increment(x);
    };

template <Position P, Movable L>
constexpr auto
precedes(P const&, L const&) -> bool
{
    return false;
}

template <typename T>
requires Movable<Decay<T>>
constexpr auto
precedes(Pointer_type<T> const& pos0, Pointer_type<T> const& pos1) -> bool
{
    return pos0 != pos1;
}

template <typename L, typename P>
concept Limit =
    Regular<L> and
    Position<P> and
    requires (P const& pos, L const& lim) {
        { precedes(pos, lim) } -> bool;
    };

template <Position P>
constexpr auto
successor(P x) -> P
{
    increment(x);
    return x;
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

template <Position P>
constexpr auto
operator+(P pos, Difference_type<P> dist) -> P
//[[expects axiom: dist >= Zero<Difference_type<P>>]]
//[[expects axiom: is_weak_range(pos, dist)]]
{
    while (!is_zero(dist)) {
        increment(pos);
        decrement(dist);
    }
    return pos;
}

template <Position P>
constexpr auto
operator+(Difference_type<P> dist, P pos) -> P
//[[expects axiom: dist >= Zero<Difference_type<P>>]]
//[[expects axiom: is_weak_range(pos, dist)]]
{
    return pos + dist;
}

template <Position P, Limit<P> L>
constexpr auto
operator-(L lim, P pos) -> Difference_type<P>
//[[expects axiom: is_bounded_range(pos, lim)]]
{
    auto n(Zero<Difference_type<P>>);
    while (precedes(pos, lim)) {
        increment(n);
        increment(pos);
    }
    return n;
}

template <typename P>
concept Forward_position =
    Regular<P> and
    Position<P>;

template <
    Position S0,
    Limit<S0> L0,
    Position S1,
    Forward_position D,
    Binary_operation S_add_op,
    Binary_operation S_mul_op>
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

template <typename P>
concept Bidirectional_position =
    Forward_position<P> and
    requires (P x) {
        decrement(x);
    };

template <Bidirectional_position P>
constexpr auto
predecessor(P x) -> P
{
    decrement(x);
    return x;
}

template <Constructible_from T>
constexpr auto
predecessor(Pointer_type<T> x) -> Pointer_type<T>
{
    decrement(x);
    return x;
}

template <Bidirectional_position P>
struct reverse_position
{
    P pos{};

    constexpr
    reverse_position() = default;

    explicit constexpr
    reverse_position(P const& pos_) : pos{pos_} {}
};

template <Bidirectional_position P>
struct value_type_t<reverse_position<P>>
{
    using type = Value_type<P>;
};

template <Bidirectional_position P>
struct difference_type_t<reverse_position<P>>
{
    using type = Difference_type<P>;
};

template <Bidirectional_position P>
constexpr auto
operator==(reverse_position<P> const& x, reverse_position<P> const& y) -> bool
{
    return x.pos == y.pos;
}

template <Bidirectional_position P>
constexpr void
increment(reverse_position<P>& pos)
{
    decrement(pos.pos);
}

template <Bidirectional_position P>
constexpr void
decrement(reverse_position<P>& pos)
{
    increment(pos.pos);
}

template <Bidirectional_position P>
constexpr auto
successor(reverse_position<P> const& pos) -> reverse_position<P>
{
    return reverse_position<P>{predecessor(pos.pos)};
}

template <Bidirectional_position P>
constexpr auto
predecessor(reverse_position<P> const& pos) -> reverse_position<P>
{
    return reverse_position<P>{successor(pos.pos)};
}

template <Bidirectional_position P>
requires Loadable<P>
constexpr auto
load(reverse_position<P> const& pos) -> Value_type<P> const&
{
    return load(predecessor(pos.pos));
}

template <Bidirectional_position P>
requires Storable<P>
constexpr void
store(reverse_position<P>& pos, Value_type<P> const& value)
{
    store(predecessor(pos.pos), value);
}

template <Bidirectional_position P>
requires Storable<P>
constexpr void
store(reverse_position<P>& pos, Value_type<P>&& value)
{
    store(predecessor(pos.pos), fw<Value_type<P>>(value));
}

template <Bidirectional_position P>
requires Mutable<P>
constexpr auto
at(reverse_position<P> const& pos) -> Value_type<P> const&
{
    return at(predecessor(pos.pos));
}

template <Bidirectional_position P>
requires Mutable<P>
constexpr auto
at(reverse_position<P>& pos) -> Value_type<P>&
{
    return at(predecessor(pos.pos));
}

template <Bidirectional_position P>
constexpr auto
precedes(reverse_position<P> const& pos0, reverse_position<P> const& pos1) -> bool
{
    return pos0.pos != pos1.pos;
}

template <typename T>
concept Indexed_position =
    Forward_position<T> and
    requires (T x, Difference_type<T> n) {
        { x + n } -> T;
        { x - x } -> Difference_type<T>;
    };

template <Position P>
requires Loadable<P>
struct loadable_position
{
    P pos;

    explicit loadable_position(P pos_) : pos{pos_} {}

    loadable_position(loadable_position const&) = delete;

    loadable_position(loadable_position&&) = default;

    loadable_position& operator=(loadable_position const&) = delete;

    loadable_position& operator=(loadable_position&&) = default;

    ~loadable_position() = default;
};

template <Position P>
requires Loadable<P>
struct value_type_t<loadable_position<P>>
{
    using type = Value_type<P>;
};

template <Position P>
requires Loadable<P>
struct difference_type_t<loadable_position<P>>
{
    using type = Difference_type<P>;
};

template <Position P>
constexpr void
increment(loadable_position<P>& pos)
{
    increment(pos.pos);
}

template <Position P>
constexpr void
decrement(loadable_position<P>& pos)
{
    decrement(pos.pos);
}

template <Position P>
constexpr auto
successor(loadable_position<P> const& pos) -> loadable_position<P>
{
    return loadable_position<P>(successor(pos.pos));
}

template <Position P>
constexpr auto
predecessor(loadable_position<P> const& pos) -> loadable_position<P>
{
    return loadable_position<P>(predecessor(pos.pos));
}

template <Position P>
requires Loadable<P>
constexpr auto
load(loadable_position<P> const& pos) -> Value_type<P> const&
{
    return load(pos.pos);
}

template <Position P>
constexpr auto
precedes(loadable_position<P> const& pos0, loadable_position<P> const& pos1) -> bool
{
    return pos0.pos != pos1.pos;
}

template <Position P>
struct counted_position
{
    P pos{};
    Difference_type<P> count = Zero<Difference_type<P>>;

    constexpr
    counted_position() = default;

    explicit constexpr
    counted_position(P const& pos_, Difference_type<P> const& count_ = Zero<Difference_type<P>>)
        : pos{pos_}
        , count{count_}
    {}
};

template <Position P>
struct value_type_t<counted_position<P>>
{
    using type = Value_type<P>;
};

template <Position P>
struct difference_type_t<counted_position<P>>
{
    using type = Difference_type<P>;
};

template <Position P>
constexpr auto
operator==(counted_position<P> const& x, counted_position<P> const& y) -> bool
{
    return x.pos == y.pos and x.count == y.count;
}

template <Position P>
constexpr void
increment(counted_position<P>& pos)
{
    increment(pos.pos);
    increment(pos.count);
}

template <Bidirectional_position P>
constexpr void
decrement(counted_position<P>& pos)
{
    decrement(pos.pos);
    decrement(pos.count);
}

template <Position P>
constexpr auto
successor(counted_position<P> const& pos) -> counted_position<P>
{
    return counted_position<P>{successor(pos.pos), successor(pos.count)};
}

template <Bidirectional_position P>
constexpr auto
predecessor(counted_position<P> const& pos) -> counted_position<P>
{
    return counted_position<P>{predecessor(pos.pos), predecessor(pos.count)};
}

template <Position P>
requires Loadable<P>
constexpr auto
load(counted_position<P> const& pos) -> Value_type<P> const&
{
    return load(pos.pos);
}

template <Position P>
requires Storable<P>
constexpr void
store(counted_position<P>& pos, Value_type<P> const& value)
{
    store(pos.pos, value);
}

template <Position P>
requires Storable<P>
constexpr void
store(counted_position<P>& pos, Value_type<P>&& value)
{
    store(pos.pos, fw<Value_type<P>>(value));
}

template <Position P>
requires Mutable<P>
constexpr auto
at(counted_position<P>& pos) -> Value_type<P>&
{
    return at(pos.pos);
}

template <Position P, Regular C>
requires Equality_comparable_with<Difference_type<P>, C>
constexpr auto
precedes(counted_position<P> const& pos, C const& count) -> bool
{
    return pos.count != count;
}

template <Position P>
requires Loadable<P>
struct position
{
    P pos;

    position() = default;

    explicit position(P pos_) : pos{pos_} {}

    position(position const&) = delete;

    position(position&&) = default;

    position& operator=(position const&) = delete;

    position& operator=(position&&) = default;

    ~position() = default;
};

template <Position P>
requires Loadable<P>
struct value_type_t<position<P>>
{
    using type = Value_type<P>;
};

template <Position P>
requires Loadable<P>
struct difference_type_t<position<P>>
{
    using type = Difference_type<P>;
};

template <Position P>
constexpr void
increment(position<P>& pos)
{
    increment(pos.pos);
}

template <Position P>
constexpr auto
successor(position<P> const& pos) -> position<P>
{
    return {successor(pos.pos)};
}

template <Position P>
requires Loadable<P>
constexpr auto
load(position<P> const& pos) -> Value_type<P> const&
{
    return load(pos.pos);
}

template <Position P>
requires Storable<P>
constexpr void
store(position<P>& pos, Value_type<P> const& value)
{
    store(pos.pos, value);
}

template <Position P>
requires Storable<P>
constexpr void
store(position<P>& pos, Value_type<P>&& value)
{
    store(pos.pos, std::forward<Value_type<P>>(value));
}

template <Position P>
requires Mutable<P>
constexpr auto
at(position<P> const& pos) -> Value_type<P> const&
{
    return at(pos.pos);
}

template <Position P>
requires Mutable<P>
constexpr auto
at(position<P>& pos) -> Value_type<P>&
{
    return at(pos.pos);
}

template <Position P>
constexpr auto
precedes(position<P> const& pos0, position<P> const& pos1) -> bool
{
    return pos0.pos != pos1.pos;
}

template <Position P, Limit<P> L>
constexpr auto
precedes(position<P> const& pos, L const& lim) -> bool
{
    return pos.pos != lim;
}

template <Forward_position P>
requires Loadable<P>
struct forward_position
{
    P pos;

    forward_position() = default;

    explicit forward_position(P pos_) : pos{pos_} {}
};

template <Forward_position P>
requires Loadable<P>
struct value_type_t<forward_position<P>>
{
    using type = Value_type<P>;
};

template <Forward_position P>
requires Loadable<P>
struct difference_type_t<forward_position<P>>
{
    using type = Difference_type<P>;
};

template <Forward_position P>
constexpr auto
operator==(forward_position<P> const& x, forward_position<P> const& y) -> bool
{
    return x.pos == y.pos;
}

template <Forward_position P>
constexpr auto
operator==(forward_position<P> const& x, P const& y) -> bool
{
    return x.pos == y;
}

template <Forward_position P>
constexpr auto
operator==(P const& x, forward_position<P> const& y) -> bool
{
    return x == y.pos;
}

template <Forward_position P>
constexpr void
increment(forward_position<P>& pos)
{
    increment(pos.pos);
}

template <Forward_position P>
constexpr auto
successor(forward_position<P> const& pos) -> forward_position<P>
{
    return {successor(pos.pos)};
}

template <Forward_position P>
requires Loadable<P>
constexpr auto
load(forward_position<P> const& pos) -> Value_type<P> const&
{
    return load(pos.pos);
}

template <Forward_position P>
requires Storable<P>
constexpr void
store(forward_position<P>& pos, Value_type<P> const& value)
{
    store(pos.pos, value);
}

template <Forward_position P>
requires Storable<P>
constexpr void
store(forward_position<P>& pos, Value_type<P>&& value)
{
    store(pos.pos, std::forward<Value_type<P>>(value));
}

template <Forward_position P>
requires Mutable<P>
constexpr auto
at(forward_position<P> const& pos) -> Value_type<P> const&
{
    return at(pos.pos);
}

template <Forward_position P>
requires Mutable<P>
constexpr auto
at(forward_position<P>& pos) -> Value_type<P>&
{
    return at(pos.pos);
}

template <Forward_position P>
constexpr auto
precedes(forward_position<P> const& pos0, forward_position<P> const& pos1) -> bool
{
    return pos0.pos != pos1.pos;
}

template <Forward_position P, Limit<P> L>
constexpr auto
precedes(forward_position<P> const& pos, L const& lim) -> bool
{
    return pos.pos != lim;
}

template <Bidirectional_position P>
constexpr auto
operator-(P pos, Difference_type<P> dist) -> P
//[[expects axiom: dist >= Zero<Difference_type<P>>]]
//[[expects axiom first: is_weak_range(first, dist)]]
//[[expects axiom first: pos = first + dist]]
{
    while (!is_zero(dist)) {
        decrement(pos);
        decrement(dist);
    }
    return pos;
}

template <Bidirectional_position P>
requires Loadable<P>
struct bidirectional_position
{
    P pos;

    bidirectional_position() = default;

    explicit bidirectional_position(P pos_) : pos{pos_} {}
};

template <Bidirectional_position P>
requires Loadable<P>
struct value_type_t<bidirectional_position<P>>
{
    using type = Value_type<P>;
};

template <Bidirectional_position P>
requires Loadable<P>
struct difference_type_t<bidirectional_position<P>>
{
    using type = Difference_type<P>;
};

template <Bidirectional_position P>
constexpr auto
operator==(bidirectional_position<P> const& x, bidirectional_position<P> const& y) -> bool
{
    return x.pos == y.pos;
}

template <Bidirectional_position P>
constexpr void
increment(bidirectional_position<P>& pos)
{
    increment(pos.pos);
}

template <Bidirectional_position P>
constexpr void
decrement(bidirectional_position<P>& pos)
{
    decrement(pos.pos);
}

template <Bidirectional_position P>
constexpr auto
successor(bidirectional_position<P> const& pos) -> bidirectional_position<P>
{
    return {successor(pos.pos)};
}

template <Bidirectional_position P>
constexpr auto
predecessor(bidirectional_position<P> const& pos) -> bidirectional_position<P>
{
    return {predecessor(pos.pos)};
}

template <Bidirectional_position P>
requires Loadable<P>
constexpr auto
load(bidirectional_position<P> const& pos) -> Value_type<P> const&
{
    return load(pos.pos);
}

template <Bidirectional_position P>
requires Storable<P>
constexpr void
store(bidirectional_position<P>& pos, Value_type<P> const& value)
{
    store(pos.pos, value);
}

template <Bidirectional_position P>
requires Storable<P>
constexpr void
store(bidirectional_position<P>& pos, Value_type<P>&& value)
{
    store(pos.pos, std::forward<Value_type<P>>(value));
}

template <Bidirectional_position P>
requires Mutable<P>
constexpr auto
at(bidirectional_position<P> const& pos) -> Value_type<P> const&
{
    return at(pos.pos);
}

template <Bidirectional_position P>
requires Mutable<P>
constexpr auto
at(bidirectional_position<P>& pos) -> Value_type<P>&
{
    return at(pos.pos);
}

template <Bidirectional_position P>
constexpr auto
precedes(bidirectional_position<P> const& pos0, bidirectional_position<P> const& pos1) -> bool
{
    return pos0.pos != pos1.pos;
}

template <typename P>
concept Linked_forward_position =
    Forward_position<P> and
    requires (P x) {
        next_link(x);
    };

template <typename L>
concept Forward_linker =
    requires (L linker, Position_type<L> pos) {
        linker(pos, pos);
    };

template <Linked_forward_position P>
struct forward_linker
{
    void operator()(P x, P y)
    {
        next_link(x) = y.pos;
    }
};

template <Linked_forward_position P>
struct position_type_t<forward_linker<P>>
{
    using type = P;
};

template <Linked_forward_position P>
constexpr void
set_link_forward(P x, P y)
{
    forward_linker<P>{}(x, y);
}

template <typename P>
concept Linked_bidirectional_position =
    Linked_forward_position<P> and
    requires (P x) {
        prev_link(x);
    };

template <Linked_bidirectional_position P>
struct bidirectional_linker
{
    void operator()(P x, P y)
    {
        next_link(x) = y.pos;
        prev_link(y) = x.pos;
    }
};

template <Linked_bidirectional_position P>
struct position_type_t<bidirectional_linker<P>>
{
    using type = P;
};

template <Linked_bidirectional_position P>
constexpr void
set_link_bidirectional(P x, P y)
{
    bidirectional_linker<P>{}(x, y);
}

template <typename>
struct index_position_type_t;

template <typename T>
using Index_position_type = typename index_position_type_t<T>::type;

template <typename>
struct segment_position_type_t;

template <typename T>
using Segment_position_type = typename segment_position_type_t<T>::type;

template <typename P>
concept Segmented_position =
    Movable<P> and
    requires (P x) {
        Position<Index_position_type<P>>;
        Position<Segment_position_type<P>>;
        { index_pos(x) } -> Index_position_type<P>;
        { segment_pos(x) } -> Segment_position_type<P>;
    };

}
