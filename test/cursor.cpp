#include "catch.hpp"

#include "affine_space.h"
#include "cursor.h"

namespace e = elements;

struct storable_cursor
{
    int value;

    storable_cursor() = delete;

    explicit storable_cursor(int value_) : value{value_} {}

    storable_cursor(storable_cursor const&) = delete;

    storable_cursor(storable_cursor&&) = default;

    storable_cursor& operator=(storable_cursor const&) = delete;

    storable_cursor& operator=(storable_cursor&&) = default;

    ~storable_cursor() = default;
};

namespace elements {

template <>
struct value_type_t<storable_cursor>
{
    using type = int;
};

constexpr void
store(storable_cursor& cur, Value_type<storable_cursor> const& value)
{
    cur.value = value;
}

constexpr void
store(storable_cursor& cur, Value_type<storable_cursor>&& value)
{
    cur.value = std::forward<Value_type<storable_cursor>>(value);
}

constexpr void
increment(storable_cursor& cur)
{
    ++cur.value;
}

constexpr auto
precedes(storable_cursor const& cur, int const& lim) -> bool
{
    return cur.value < lim;
}

constexpr auto
precedes(int const& cur, int const& lim) -> bool
{
    return cur < lim;
}

}

SCENARIO ("Using access functions", "[cursor]")
{
    int x = 0;
    int xs[] = {0, 1, 2};

    SECTION ("Loadable")
    {
        static_assert(e::Loadable<e::loadable_cursor<int>>);
        static_assert(e::Loadable<e::forward_cursor<int>>);
        static_assert(e::Loadable<int>);
        static_assert(e::Loadable<int*>);

        e::loadable_cursor lc{0};
        REQUIRE(lc.cur == 0);
        REQUIRE(e::predecessor(lc).cur == -1);
        REQUIRE(e::load(lc) == 0);

        e::forward_cursor fc{0};
        REQUIRE(e::load(fc) == 0);

        REQUIRE (e::load(x) == x);

        REQUIRE (e::load(xs) == x);

        auto y = e::pointer_to(x);
        REQUIRE(e::load(y) == x);
    }

    SECTION ("Storable")
    {
        static_assert(e::Storable<storable_cursor>);
        static_assert(e::Storable<int>);
        static_assert(e::Storable<int*>);

        storable_cursor sc{0};
        e::store(sc, 1);
        REQUIRE (sc.value == 1);

        e::store(x, 1);
        REQUIRE (x == 1);

        auto y = e::pointer_to(x);
        e::store(y, 2);
        REQUIRE (x == 2);
    }

    SECTION ("Mutable")
    {
        static_assert(e::Mutable<int>);
        static_assert(e::Mutable<int*>);

        auto y = e::at(x);
        REQUIRE (e::at(y) == 0);

        e::at(y) = 1;
        REQUIRE (e::at(y) == 1);
    }
}

SCENARIO ("Using linear traversal functions", "[cursor]")
{
    SECTION ("Cursor")
    {
        static_assert(e::Cursor<int>);
        static_assert(e::Cursor<int*>);
        static_assert(e::Cursor<e::loadable_cursor<int>>);

        e::loadable_cursor c0{0};
        e::loadable_cursor c1{0};
        e::increment(c0);
        REQUIRE (c0.cur == e::successor(std::move(c1)).cur);
    }

    SECTION ("Forward_cursor")
    {
        static_assert(e::Forward_cursor<int>);
        static_assert(e::Forward_cursor<int*>);
        static_assert(e::Cursor<e::loadable_cursor<int>>);
        static_assert(e::Forward_cursor<e::forward_cursor<int>>);
        static_assert(!e::Bidirectional_cursor<e::forward_cursor<int>>);
    }

    SECTION ("Bidirectional_cursor")
    {
        static_assert(e::Bidirectional_cursor<int>);
        static_assert(e::Bidirectional_cursor<int*>);
        static_assert(e::Bidirectional_cursor<e::bidirectional_cursor<int>>);
    }
}

SCENARIO ("Reversing cursors", "[reverse_cursor]")
{
    int xs[] = {0, 1};

    SECTION ("Bidirectional traversal")
    {
        auto cx = e::reverse_cursor(xs + 2);
        static_assert(e::Bidirectional_cursor<decltype(cx)>);
        static_assert(e::Affine_space<decltype(cx)>);

        REQUIRE (load(cx) == 1);
        REQUIRE (successor(cx).cur == xs + 1);

        increment(cx);
        REQUIRE (load(cx) == 0);
        REQUIRE (predecessor(cx).cur == xs + 2);

        decrement(cx);
        REQUIRE (cx.cur == xs + 2);
    }
}

SCENARIO ("Counted cursors", "[counted_cursor]")
{
    int xs[] = {0, 1};

    SECTION ("Counted traversal")
    {
        auto cx = e::counted_cursor(xs + 0);
        static_assert(e::Bidirectional_cursor<decltype(cx)>);

        REQUIRE(load(cx) == 0);
        REQUIRE(cx.cur == xs);
        REQUIRE(cx.count == 0);
        REQUIRE(successor(cx).cur == xs + 1);
        REQUIRE(successor(cx).count == 1);
        REQUIRE(precedes(cx, 2));

        increment(cx);
        REQUIRE(load(cx) == 1);
        REQUIRE(cx.cur == xs + 1);
        REQUIRE(cx.count == 1);
        REQUIRE(successor(cx).cur == xs + 2);
        REQUIRE(successor(cx).count == 2);
        REQUIRE(precedes(cx, 2));

        increment(cx);
        REQUIRE(cx.cur == xs + 2);
        REQUIRE(cx.count == 2);
        REQUIRE(predecessor(cx).cur == xs + 1);
        REQUIRE(predecessor(cx).count == 1);
        REQUIRE(!precedes(cx, 2));

        decrement(cx);
        REQUIRE(load(cx) == 1);
        REQUIRE(cx.cur == xs + 1);
        REQUIRE(cx.count == 1);
        REQUIRE(successor(cx).cur == xs + 2);
        REQUIRE(successor(cx).count == 2);
        REQUIRE(predecessor(cx).cur == xs);
        REQUIRE(predecessor(cx).count == 0);
        REQUIRE(precedes(cx, 2));
    }
}
