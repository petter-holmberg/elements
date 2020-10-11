#pragma once

#include "intrinsics.h"

namespace elements {

template <typename T, typename U>
concept Same_as = exposition_only::Same_as<T, U> and exposition_only::Same_as<U, T>;

template <typename T, typename U>
concept Convertible_to =
    exposition_only::Is_convertible<T, U> and
    requires (Add_rv_ref<T>(&f)()) {
        static_cast<U>(f());
    };

namespace exposition_only {

template <typename T>
concept Boolean_testable = Convertible_to<T, bool>;

}

template <typename T>
concept Boolean_testable =
    exposition_only::Boolean_testable<T> and
    requires (T&& t) {
        { !fw<T>(t) } -> exposition_only::Boolean_testable;
    };

namespace exposition_only {

template <typename T, typename U>
concept Weakly_equality_comparable_with =
    requires(Remove_ref<T> const& x, Remove_ref<U> const& y) {
        { x == y } -> Boolean_testable;
        { x != y } -> Boolean_testable;
        { y == x } -> Boolean_testable;
        { y != x } -> Boolean_testable;
        // axiom equivalence_relation {
        //     x == x;
        //     implication(x == y, y == x);
        //     implication(x == y and y == z, x == z);
        // }
        // axiom equality {
        //     equivalence(x == y, eq(x, y));
        // }
        // axiom complement {
        //     equivalence(x != y, !(y == x));
        // }
        // complexity {
        //     O(areaof(min(areaof(x), areaof(y))));
        // }
    };

}

template <typename T>
concept Equality_comparable =
    exposition_only::Weakly_equality_comparable_with<T, T>;
    // axiom regular {
    //     bool(x == y) == bool(y == x);
    // }

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

template <typename T, typename U>
concept Equality_comparable_with =
    Equality_comparable<T> and
    Equality_comparable<U> and
    exposition_only::Weakly_equality_comparable_with<T, U>;

template <typename T>
concept Destructible =
    exposition_only::Is_nothrow_destructible<T>;
    // axiom end_of_object_lifetimes {}
    // complexity {
    //     O(areaof(x));
    // }

template <typename T>
requires Destructible<Remove_const<T>>
constexpr void
destroy(T& x)
    //[[expects axiom: partially_formed(x)]]
    //[[ensures axiom: raw_memory(x)]]
{
    x.~T();
}

template <typename T, typename ...Args>
concept Constructible_from =
    Destructible<T> and
    exposition_only::Is_constructible<T, Args...>;

template <typename T>
concept Default_initializable =
    Constructible_from<T> and
    requires {
        T{};
    };

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

template <typename T>
concept Move_constructible =
    Constructible_from<T, T> and
    Convertible_to<T, T>;
    // axiom move_semantics {
    //     implication(eq(x, y), eq(T{mv(x)}, y));
    // }
    // complexity {
    //     O(sizeof(x));
    // }

template <typename T>
concept Copy_constructible =
    Move_constructible<T> and
    Constructible_from<T, T&> and
    Convertible_to<T&, T> and
    Constructible_from<T, T const&> and
    Convertible_to<T const&, T> and
    Constructible_from<T, T const> and
    Convertible_to<T const, T>;
    // axiom copy_semantics {
    //     eq(T{x}, y);
    // }
    // complexity {
    //     O(sizeof(y));
    // }


template <typename T, typename U>
concept Assignable_from =
    exposition_only::Is_lvalue_ref<T> and
    requires(T x, U&& y) {
        { x = fw<U>(y) } -> Same_as<T>;
        // axiom copy_semantics {
        //     implication(exposition_only::Is_lvalue_ref<decltype(y)>, eq(x = y, y));
        // }
        // axiom move_semantics {
        //     implication(implication(exposition_only::Is_rvalue_ref<decltype(y)>, eq(x, y)), eq(x, z = mv(y)));
        // }
        // complexity {
        //     implication(exposition_only::Is_lvalue_ref<decltype(y)>, O(areaof(y)));
        //     implication(exposition_only::Is_rvalue_ref<decltype(y)>, O(sizeof(y)));
        // }
    };

template <typename T>
concept Movable =
    std::is_object_v<T> and
    Move_constructible<T> and
    Assignable_from<T&, T>;

template <typename T>
concept Copyable =
    Copy_constructible<T> and
    Movable<T> and
    Assignable_from<T&, T&> and
    Assignable_from<T&, T const&> and
    Assignable_from<T&, T const>;

template <typename T>
concept Semiregular =
    Copyable<T> and
    Default_initializable<T>;

template <Semiregular T>
constexpr auto
operator!=(T const& x, T const& y) -> bool
{
    return !(x == y);
}

template <typename T>
concept Regular =
    Semiregular<T> and
    Equality_comparable<T>;
    // complexity default_constructor {
    //     O(1);
    // }

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

template <typename T>
concept Totally_ordered =
    Equality_comparable<T> and
    requires (T const& x, T const& y) {
        { x < y } -> Boolean_testable;
        { x > y } -> Boolean_testable;
        { x <= y } -> Boolean_testable;
        { x >= y } -> Boolean_testable;
        // axiom total_ordering {
        //    implication(bool(x < y) and bool(y < z), x < z);
        //    bool(x < y) xor bool(y < x) xor bool(x == y);
        // }
        // axiom derived_relations {
        //    equivalence(x >= y, !(x < y));
        //    equivalence(x > y, y < x);
        //    equivalence(x <= y, !(y < x));
        // }
        // complexity {
        //     O(areaof(x));
        // }
    };

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
