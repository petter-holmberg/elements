#include "catch.hpp"

#include "memory.h"

namespace e = elements;

SCENARIO ("Using allocators", "[allocator]")
{
    SECTION ("Empty allocator")
    {
        e::empty_allocator a;
        e::empty_allocator b;

        static_assert(e::Allocator<decltype(a)>);
        static_assert(e::Allocator_alignment<decltype(a)> == 32768);

        REQUIRE (a == b);

        REQUIRE (e::good_size(a, 256) == 0);
        auto x = e::allocate(a, 256);
        REQUIRE (!x);
        e::deallocate(a, x);
    }

    SECTION ("Static allocator")
    {
        e::static_allocator<1024, 1> a;
        e::static_allocator<1024, 2> b;

        static_assert(e::Allocator<decltype(a)>);
        static_assert(e::Allocator_alignment<decltype(a)> == 1);

        REQUIRE (a != b);

        REQUIRE (e::good_size(a, 256) == 256);
        REQUIRE (e::good_size(b, 255) == 256);
        REQUIRE (e::available(a) == 1024);
        auto x = e::allocate(a, 256);
        REQUIRE (e::available(a) == 768);
        auto y = e::allocate(a, 768);
        auto z = e::allocate(a, 0);
        auto w = e::allocate(a, 1);
        REQUIRE (x);
        REQUIRE (y);
        REQUIRE (x != y);
        REQUIRE (z);
        REQUIRE (y != z);
        REQUIRE (!w);
        e::deallocate(a, x);
        REQUIRE (e::available(a) == 0);
        e::deallocate(a, z);
        REQUIRE (e::available(a) == 0);
        e::deallocate(a, y);
        REQUIRE (e::available(a) == 768);
        e::deallocate(a, x);
        REQUIRE (e::available(a) == 1024);
    }

    SECTION ("Dynamic allocator")
    {
        e::dynamic_allocator a;
        e::dynamic_allocator b;

        static_assert(e::Allocator<decltype(a)>);
        static_assert(e::Allocator_alignment<decltype(a)> == alignof(double));

        REQUIRE (a == b);

        REQUIRE (e::good_size(a, 1) == alignof(double));
        REQUIRE (e::good_size(a, alignof(double) + 1) == alignof(double) * 2);
        auto x = e::allocate(a, 256);
        auto y = e::allocate(b, 512);
        REQUIRE (x);
        REQUIRE (y);
        REQUIRE (x != y);
        e::deallocate(a, x);
        e::deallocate(a, y);
    }

    SECTION ("Choice allocator")
    {
        e::choice_allocator<e::static_allocator<1024, 2>, e::dynamic_allocator> a;

        static_assert(e::Allocator<decltype(a)>);
        static_assert(e::Allocator_alignment<decltype(a)> == 2);

        REQUIRE (e::good_size(a, 256) == 256);
        auto x = e::allocate(a, 256);
        auto y = e::allocate(a, 768);
        auto z = e::allocate(a, 0);
        auto w = e::allocate(a, 1);
        REQUIRE (x);
        REQUIRE (y);
        REQUIRE (x != y);
        REQUIRE (z);
        REQUIRE (y != z);
        REQUIRE (w);
        e::deallocate(a, w);
        e::deallocate(a, y);
        e::deallocate(a, x);
    }

    SECTION ("Affix allocator")
    {
        using int_fsa = e::affix_allocator<int, decltype(e::default_allocator)>;

        static_assert(e::Allocator<int_fsa>);
        static_assert(e::Allocator_alignment<int_fsa> == e::Allocator_alignment<decltype(e::default_allocator)>);

        int_fsa a;
        int_fsa b;
        REQUIRE (e::good_size(a, 1) == alignof(double));
        REQUIRE (e::good_size(a, alignof(double) + 1) == alignof(double) * 2);
        auto x = e::allocate(a, 256);
        auto y = e::allocate(b, 512);
        prefix(a, x) = 1;
        REQUIRE(prefix(a, x) == 1);
        REQUIRE (x);
        REQUIRE (y);
        REQUIRE (x != y);
        e::deallocate(b, y);
        e::deallocate(a, x);
    }

    SECTION ("Partition allocator")
    {
        auto pred = [](e::Size_type<e::memory> n){ return n <= 8; };
        e::partition_allocator a{e::default_allocator, e::static_allocator<20>{}, pred};

        static_assert(e::Allocator<decltype(a)>);
        static_assert(e::Allocator_alignment<decltype(a)> == e::Allocator_alignment<decltype(e::default_allocator)>);

        auto x = e::allocate(a, 4);
        auto y = e::allocate(a, 4);
        REQUIRE (x);
        REQUIRE (x.size == 4);
        REQUIRE (good_size(a, x.size) == alignof(double));
        REQUIRE (y);
        REQUIRE (y.size == 4);
        REQUIRE (y.first == x.first + alignof(double));
        REQUIRE (good_size(a, 10) == alignof(double) * 2);
        auto z = e::allocate(a, 4);
        REQUIRE (!z);
        e::deallocate(a, y);
        e::deallocate(a, x);
        e::deallocate_all(a.a1);
        auto w = e::allocate(a, 10);
        REQUIRE (w.first != x.first);
    }
}
