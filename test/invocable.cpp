#include "catch.hpp"

#include "elements.h"

namespace e = elements;

int fn(int x, double y) { return x + static_cast<int>(y); }

struct fn_struct
{
    int fn(int x, double y) { return x + static_cast<int>(y); }
    int cfn(int x, double y) const { return x + static_cast<int>(y); }
    int mo;
};

struct fo_struct
{
    int operator()(int x, double y) { return x + static_cast<int>(y); }
};

SCENARIO ("Using invocable types", "[invocable]")
{
    SECTION ("free function")
    {
        static_assert(e::Same<e::Domain<decltype(fn)>, int>);
        static_assert(e::Same<e::Codomain<decltype(fn)>, int>);
        static_assert(e::Arity<decltype(fn)> == 2);
        static_assert(e::Same<e::Input_type<decltype(fn), 0>, int>);
        static_assert(e::Same<e::Input_type<decltype(fn), 1>, double>);
    }

    SECTION ("function pointer")
    {
        static_assert(e::Same<e::Domain<decltype(&fn)>, int>);
        static_assert(e::Same<e::Codomain<decltype(&fn)>, int>);
        static_assert(e::Arity<decltype(&fn)> == 2);
        static_assert(e::Same<e::Input_type<decltype(&fn), 0>, int>);
        static_assert(e::Same<e::Input_type<decltype(&fn), 1>, double>);
    }

    SECTION ("std::function")
    {
        static_assert(e::Same<e::Domain<std::function<int(int, double)>>, int>);
        static_assert(e::Same<e::Codomain<std::function<int(int, double)>>, int>);
        static_assert(e::Arity<std::function<int(int, double)>> == 2);
        static_assert(e::Same<e::Input_type<std::function<int(int, double)>, 0>, int>);
        static_assert(e::Same<e::Input_type<std::function<int(int, double)>, 1>, double>);
    }

    SECTION ("member function pointer")
    {
        static_assert(e::Same<e::Domain<decltype(&fn_struct::fn)>, int>);
        static_assert(e::Same<e::Codomain<decltype(&fn_struct::fn)>, int>);
        static_assert(e::Arity<decltype(&fn_struct::fn)> == 3);
        static_assert(e::Same<e::Input_type<decltype(&fn_struct::fn), 1>, int>);
        static_assert(e::Same<e::Input_type<decltype(&fn_struct::fn), 2>, double>);
    }

    SECTION ("const member function pointer")
    {
        static_assert(e::Same<e::Domain<decltype(&fn_struct::cfn)>, int>);
        static_assert(e::Same<e::Codomain<decltype(&fn_struct::cfn)>, int>);
        static_assert(e::Arity<decltype(&fn_struct::cfn)> == 3);
        static_assert(e::Same<e::Input_type<decltype(&fn_struct::cfn), 1>, int>);
        static_assert(e::Same<e::Input_type<decltype(&fn_struct::cfn), 2>, double>);
    }

    SECTION ("member object pointer")
    {
        static_assert(e::Same<e::Domain<decltype(&fn_struct::mo)>, fn_struct>);
        static_assert(e::Same<e::Codomain<decltype(&fn_struct::mo)>, int>);
        static_assert(e::Arity<decltype(&fn_struct::mo)> == 1);
        static_assert(e::Same<e::Input_type<decltype(&fn_struct::mo), 0>, fn_struct&>);
    }

    SECTION ("function object")
    {
        fo_struct fo;

        static_assert(e::Same<e::Domain<decltype(fo)>, int>);
        static_assert(e::Same<e::Codomain<decltype(fo)>, int>);
        static_assert(e::Arity<decltype(fo)> == 2);
        static_assert(e::Same<e::Input_type<decltype(fo), 0>, int>);
        static_assert(e::Same<e::Input_type<decltype(fo), 1>, double>);
    }

    SECTION ("lambda")
    {
        auto l = [](int x, double y){ return x + static_cast<int>(y); };

        static_assert(e::Same<e::Domain<decltype(l)>, int>);
        static_assert(e::Same<e::Codomain<decltype(l)>, int>);
        static_assert(e::Arity<decltype(l)> == 2);
        static_assert(e::Same<e::Input_type<decltype(l), 0>, int>);
        static_assert(e::Same<e::Input_type<decltype(l), 1>, double>);
    }

    SECTION ("reference")
    {
        auto& fn_ref = fn;

        static_assert(e::Same<e::Domain<decltype(fn_ref)>, int>);
        static_assert(e::Same<e::Codomain<decltype(fn_ref)>, int>);
        static_assert(e::Arity<decltype(fn_ref)> == 2);
        static_assert(e::Same<e::Input_type<decltype(fn_ref), 0>, int>);
        static_assert(e::Same<e::Input_type<decltype(fn_ref), 1>, double>);
    }
}
