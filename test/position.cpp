#include "catch.hpp"

#include "elements.h"

namespace e = elements;

struct loadable_position
{
    int value;

    loadable_position() = delete;

    explicit loadable_position(int value_) : value{value_} {};

    loadable_position(loadable_position const&) = delete;

    loadable_position(loadable_position&&) = default;

    loadable_position& operator=(loadable_position const&) = delete;

    loadable_position& operator=(loadable_position&&) = default;

    ~loadable_position() = default;
};

namespace elements {

template <>
struct value_type_t<loadable_position>
{
    using type = int;
};

template <>
constexpr auto
load(loadable_position const& x) -> int const&
{
    return x.value;
}

template <>
constexpr void
increment(loadable_position& x)
{
    ++x.value;
}

template <>
constexpr auto
precedes(loadable_position const& pos, int const& lim) -> bool
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
        static_assert(e::Loadable<loadable_position>);
        static_assert(e::Loadable<int>);
        static_assert(e::Loadable<int*>);

        loadable_position p{0};
        REQUIRE(e::load(p) == 0);

        REQUIRE (e::load(x) == x);

        REQUIRE (e::load(xs) == x);

        auto y = e::pointer_to(x);
        REQUIRE(e::load(y) == x);
    }

    SECTION ("Storable")
    {
        e::store(x, 1);
        REQUIRE (x == 1);

        auto y = e::pointer_to(x);
        e::store(y, 2);
        REQUIRE (x == 2);
    }

    SECTION ("Mutable")
    {
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
        static_assert(e::Position<loadable_position>);
        static_assert(e::Position<int>);
        static_assert(e::Position<int*>);

        loadable_position p0{0};
        loadable_position p1{0};
        e::increment(p0);
        REQUIRE (p0.value == e::successor(std::move(p1)).value);
    }
}
