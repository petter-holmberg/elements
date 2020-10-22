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
requires Destructible<Remove_const<T>>
constexpr void
destroy(T& x)
    //[[expects axiom: partially_formed(x)]]
    //[[ensures axiom: raw_memory(x)]]
{
    x.~T();
}

template <typename T>
requires Default_initializable<Remove_const<T>>
constexpr void
construct(T& raw)
    //[[expects axiom: raw_memory(raw)]]
    //[[ensures axiom: partially_formed(raw)]]
{
    new (&raw) T();
}

template <typename T, typename U>
requires Default_initializable<Remove_const<T>>
constexpr void
construct(T& raw, U&& initializer)
    //[[expects axiom: raw_memory(raw)]]
    //[[ensures axiom: raw == initializer]]
{
    new (&raw) T(fw<U>(initializer));
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

template <typename T>
concept Default_totally_ordered =
    Equality_comparable<T> and
    requires (T const& x, T const& y) {
        { lt<T>{}(x, y) } -> Boolean_testable;
        // axiom total_ordering {
        //    implication(lt<T>{}(x, y) and lt<T>{}(y, z), lt<T>{}(x, z));
        //    lt<T>{}(x, y) xor lt<T>{}(y, x) xor x == y;
        // }
        // complexity {
        //     O(areaof(x));
        // }
    };

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
