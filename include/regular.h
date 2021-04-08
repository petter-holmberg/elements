#pragma once

#include "intrinsics.h"

namespace elements {

template <typename T = void, typename U = T>
struct eq
{
    constexpr decltype(auto)
    operator()(T const& x, U const& y) const
    {
        return x == y;
    }
};

template <>
struct eq<void>
{
    template <typename T, typename U>
    constexpr decltype(auto)
    operator()(T&& x, U&& y) const
    {
        return eq<Decay<T>, Decay<U>>{}(fw<T>(x), fw<U>(y));
    }
};

template <typename T = void, typename U = T>
struct ne
{
    constexpr decltype(auto)
    operator()(T const& x, U const& y) const
    {
        return x != y;
    }
};

template <>
struct ne<void>
{
    template <typename T, typename U>
    constexpr decltype(auto)
    operator()(T&& x, U&& y) const
    {
        return ne<Decay<T>, Decay<U>>{}(fw<T>(x), fw<U>(y));
    }
};

template <Equality_comparable T>
struct eq_unary
{
    T const& x;

    constexpr
    eq_unary(T const& x_) : x{x_} {}

    constexpr auto
    operator()(T const& y) -> bool
    {
        return x == y;
    }
};

template <typename T = void, typename U = T>
struct lt
{
    constexpr decltype(auto)
    operator()(T const& x, U const& y) const
    {
        return x < y;
    }
};

template <>
struct lt<void>
{
    template <typename T, typename U>
    constexpr decltype(auto)
    operator()(T&& x, U&& y) const
    {
        return lt<Decay<T>, Decay<U>>{}(fw<T>(x), fw<U>(y));
    }
};

template <typename T = void, typename U = T>
struct ge
{
    constexpr decltype(auto)
    operator()(T const& x, U const& y) const
    {
        return x >= y;
    }
};

template <>
struct ge<void>
{
    template <typename T, typename U>
    constexpr decltype(auto)
    operator()(T&& x, U&& y) const
    {
        return ge<Decay<T>, Decay<U>>{}(fw<T>(x), fw<U>(y));
    }
};

template <typename T = void, typename U = T>
struct gt
{
    constexpr decltype(auto)
    operator()(T const& x, U const& y) const
    {
        return x > y;
    }
};

template <>
struct gt<void>
{
    template <typename T, typename U>
    constexpr decltype(auto)
    operator()(T&& x, U&& y) const
    {
        return gt<Decay<T>, Decay<U>>{}(fw<T>(x), fw<U>(y));
    }
};

template <typename T = void, typename U = T>
struct le
{
    constexpr decltype(auto)
    operator()(T const& x, U const& y) const
    {
        return x <= y;
    }
};

template <>
struct le<void>
{
    template <typename T, typename U>
    constexpr decltype(auto)
    operator()(T&& x, U&& y) const
    {
        return le<Decay<T>, Decay<U>>{}(fw<T>(x), fw<U>(y));
    }
};

template <typename T>
requires Destructible<T>
constexpr void
destroy(T& x)
    //[[expects axiom: partially_formed(x)]]
    //[[ensures axiom: raw_memory(x)]]
{
    destroy_at(addressof(x));
}

template <typename T, typename... Args>
requires Constructible_from<T, Args...>
constexpr void
construct(T& raw, Args&&... args)
    //[[expects axiom: raw_memory(raw)]]
    //[[ensures axiom: raw == T{args...}]]
{
    construct_at(addressof(raw), fw<Args>(args)...);
}

template <Semiregular T>
constexpr auto
operator!=(T const& x, T const& y) -> bool
{
    return !(x == y);
}

template <Regular T>
constexpr auto
operator>=(T const& x, T const& y) -> bool
{
    return !(x < y);
}

template <Regular T>
constexpr auto
operator>(T const& x, T const& y) -> bool
{
    return y < x;
}

template <Regular T>
constexpr auto
operator<=(T const& x, T const& y) -> bool
{
    return !(y < x);
}

template <Regular T>
constexpr auto
axiom_Regular(T const& x) noexcept -> bool
{
    // Default constructor
    T y;
    T z;

    // Equality
    if (!(x == x)) return false;
    if (x != x) return false;

    // Copy assignment
    y = x;
    if (y != x) return false;

    // Move assignment
    z = mv(y);
    if (z != x) return false;

    // Copy constructor
    T w(x);
    if (w != x) return false;

    // Move constructor
    T v(mv(w));
    if (v != x) return false;

    // Destructor
    return true;
}

template <typename T, typename U>
concept Default_totally_ordered_with =
    Equality_comparable_with<T, U> and
    requires (Remove_ref<T>& x, Remove_ref<U>& y) {
        { lt<Remove_ref<T>, Remove_ref<U>>{}(x, y) } -> Boolean_testable;
        // axiom total_ordering {
        //    implication(
        //        lt<Remove_ref<T>, Remove_ref<U>>{}(x, y) and lt<Remove_ref<T>, Remove_ref<U>>{}(y, z),
        //        lt<Remove_ref<T>, Remove_ref<U>>{}(x, z));
        //    lt<Remove_ref<T>, Remove_ref<U>>{}(x, y) xor lt<Remove_ref<T>, Remove_ref<U>>{}(y, x) xor x == y;
        // }
        // complexity {
        //     O(areaof(x));
        // }
    };

template <typename T>
concept Default_totally_ordered =
    Default_totally_ordered_with<T, T>;

template <Regular T>
constexpr auto
axiom_Default_totally_ordered(T const& x) noexcept -> bool
{
    if (!axiom_Regular(x)) return false;

    // Default total ordering
    if (!lt{}(x, x)) return false;

    return true;
}

template <Totally_ordered T>
constexpr auto
axiom_Totally_ordered(T const& x, T const& y) noexcept -> bool
    //[[expects: x < y]]
{
    if (!axiom_Regular(x)) return false;

    // Natural total ordering
    if (x < x) return false;
    if (!(x < y)) return false;
    if (!(y > x)) return false;
    if (!(x <= y)) return false;
    if (!(y >= x)) return false;
    if (y < x) return false;
    if (x > y) return false;
    if (y <= x) return false;
    if (x >= y) return false;

    return true;
}

}
