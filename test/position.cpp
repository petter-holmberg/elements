#include "catch.hpp"

#include "elements.h"

namespace e = elements;

struct storable_position
{
    int value;

    storable_position() = delete;

    explicit storable_position(int value_) : value{value_} {}

    storable_position(storable_position const&) = delete;

    storable_position(storable_position&&) = default;

    storable_position& operator=(storable_position const&) = delete;

    storable_position& operator=(storable_position&&) = default;

    ~storable_position() = default;
};

struct forward_position
{
    int value = 0;

    forward_position() = default;

    explicit forward_position(int value_) : value{value_} {}
};

namespace elements {

template <>
struct value_type_t<storable_position>
{
    using type = int;
};

template <>
struct value_type_t<forward_position>
{
    using type = int;
};

constexpr auto
operator==(forward_position const& x, forward_position const& y) -> bool
{
    return x.value == y.value;
}

template <>
constexpr void
store(storable_position& x, Value_type<storable_position> const& value)
{
    x.value = value;
}

template <>
constexpr void
store(storable_position& x, Value_type<storable_position>&& value)
{
    x.value = std::forward<Value_type<storable_position>>(value);
}

template <>
constexpr auto
load(forward_position const& x) -> int const&
{
    return x.value;
}

template <>
constexpr void
increment(storable_position& x)
{
    ++x.value;
}

template <>
constexpr void
increment(forward_position& x)
{
    ++x.value;
}

constexpr auto
precedes(storable_position const& pos, int const& lim) -> bool
{
    return pos.value < lim;
}

template <>
constexpr auto
precedes(forward_position const& pos, int const& lim) -> bool
{
    return pos.value < lim;
}

template <>
constexpr auto
precedes(int const& pos, int const& lim) -> bool
{
    return pos < lim;
}

}

SCENARIO ("Using access functions", "[position]")
{
    int x = 0;
    int xs[] = {0, 1, 2};

    SECTION ("Loadable")
    {
        static_assert(e::Loadable<e::loadable_position<int>>);
        static_assert(e::Loadable<forward_position>);
        static_assert(e::Loadable<int>);
        static_assert(e::Loadable<int*>);

        e::loadable_position lp{0};
        REQUIRE(lp.pos == 0);
        REQUIRE(e::predecessor(lp).pos == -1);
        REQUIRE(e::load(lp) == 0);

        forward_position fp{0};
        REQUIRE(e::load(fp) == 0);

        REQUIRE (e::load(x) == x);

        REQUIRE (e::load(xs) == x);

        auto y = e::pointer_to(x);
        REQUIRE(e::load(y) == x);
    }

    SECTION ("Storable")
    {
        static_assert(e::Storable<storable_position>);
        static_assert(e::Storable<int>);
        static_assert(e::Storable<int*>);

        storable_position sp{0};
        e::store(sp, 1);
        REQUIRE (sp.value == 1);

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

SCENARIO ("Using linear traversal functions", "[position]")
{
    SECTION ("Position")
    {
        static_assert(e::Position<int>);
        static_assert(e::Position<int*>);
        static_assert(e::Position<e::loadable_position<int>>);

        e::loadable_position p0{0};
        e::loadable_position p1{0};
        e::increment(p0);
        REQUIRE (p0.pos == e::successor(std::move(p1)).pos);
    }

    SECTION ("Forward_position")
    {
        static_assert(e::Forward_position<int>);
        static_assert(e::Forward_position<int*>);
        static_assert(e::Forward_position<e::loadable_forward_position<int>>);
    }
}
