#pragma once

#include "intrinsics.h"

namespace elements {

template <typename T, typename U>
concept Same_as = std::is_same<T, U>::value;

template <typename T, typename U>
concept Weakly_equality_comparable_with =
    requires(Remove_ref<T> const& x, Remove_ref<U> const& y) {
        { x == y } -> bool;
        { x != y } -> bool;
        { y == x } -> bool;
        { y != x } -> bool;
        // axiom equivalence_relation {
        //     x == x;
        //     x == y => y == x;
        //     x == y and y == z => x == z;
        // }
        // axiom equality {
        //     x == y <=> eq(x, y);
        // }
        // axiom complement {
        //     x != y <=> !(y == x);
        // }
        // complexity {
        //     O(areaof(min(areaof(x), areaof(y))));
        // }
    };

template <typename T>
concept Equality_comparable =
    Weakly_equality_comparable_with<T, T>;
    // axiom regular {
    //     (x == y) == (x == y);
    // }

template <Equality_comparable T>
struct equal
{
    constexpr auto
    operator()(T const& x, T const& y) -> bool
    {
        return x == y;
    }
};

template <Equality_comparable T>
struct equal_unary
{
    T const& x;

    constexpr
    equal_unary(T const& x_) : x{x_} {}

    constexpr auto
    operator()(T const& y) -> bool
    {
        return x == y;
    }
};

template <Equality_comparable T>
struct less
{
    constexpr auto
    operator()(T const& x, T const& y) -> bool
    {
        return x < y;
    }
};

template <typename T, typename U>
concept Equality_comparable_with =
    Equality_comparable<T> and
    Equality_comparable<U> and
    Weakly_equality_comparable_with<T, U>;

template <typename T>
concept Destructible =
    std::is_nothrow_destructible<T>::value;
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
    std::is_constructible<T, Args...>::value;

template <typename T>
concept Default_constructible = Constructible_from<T>;

template <typename T>
constexpr auto
allocate_raw() -> T*
{
    return reinterpret_cast<T*>(std::malloc(sizeof(T)));
}

template <typename T>
constexpr void
free_raw(T& raw)
    //[[expects axiom: raw_memory(*raw)]]
{
    std::free(&raw);
}

template <typename T>
requires Default_constructible<Remove_const<T>>
constexpr void
construct(T& raw)
    //[[expects axiom: raw_memory(raw)]]
    //[[ensures axiom: partially_formed(raw)]]
{
    new (&raw) T();
}

template <typename T, typename U>
requires Default_constructible<Remove_const<T>>
constexpr void
construct(T& raw, U&& initializer)
    //[[expects axiom: raw_memory(raw)]]
    //[[ensures axiom: raw == initializer]]
{
    new (&raw) T(fw<U>(initializer));
}

template <typename T, typename U>
concept Convertible_to =
    std::is_convertible<T, U>::value and
    requires(T (&x)()) {
        static_cast<U>(x());
    };

template <typename T>
concept Move_constructible =
    std::is_move_constructible<T>::value and
    Convertible_to<T, T>;
    // axiom move_semantics {
    //     eq(x, y) => eq(T{mv(x)}, y);
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
    requires(T x, U&& y) {
        { x = fw<U>(y) } -> T&;
        // axiom copy_semantics {
        //     is_lvalue_reference<decltype(y)> => eq(x = y, y);
        // }
        // axiom move_semantics {
        //     is_rvalue_reference<decltype(y)> => eq(x, y) => eq(x, z = mv(y));
        // }
        // complexity {
        //     is_lvalue_reference<decltype(y)> => O(areaof(y));
        //     is_rvalue_reference<decltype(y)> => O(sizeof(y));
        // }
    };

template <typename T>
concept Movable =
    std::is_object<T>::value and
    Move_constructible<T> and
    Assignable_from<T&, T>;
    // axiom partially_formed {
    //     // mv(x) not necessarily well-formed*
    //     T{mv(x)} => x = mv(y);
    //     T{mv(x)} => ~x;
    //     y = mv(x) => x = mv(y);
    //     y = mv(x) => ~x;
    // }

template <typename T>
concept Copyable =
    Copy_constructible<T> and
    Movable<T> and
    Assignable_from<T&, T const&>;
    // axiom partially_formed {
    //     // mv(a) not necessarily well-formed
    //     T{mv(a)} => a = b;
    //     b = mv(a) = a = b;
    // }

template <typename T>
concept Semiregular =
    Copyable<T> and
    Default_constructible<T>;

template <typename T>
concept Regular =
    Semiregular<T> and
    Equality_comparable<T>;
    // axiom partially_formed {
    //     // T x not necessarily well-formed
    //     T x => x = mv(y);
    //     T x => x = y;
    //     T x => ~x;
    // }
    // complexity default_constructor {
    //     O(1);
    // }

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
    z = std::move(y);
    if (z != x) return false;

    // Copy constructor
    T w(x);
    if (w != x) return false;

    // Move constructor
    T v(std::move(w));
    if (v != x) return false;

    // Destructor
    return true;
}

template <typename T>
concept Default_totally_ordered =
    Regular<T> and
    requires (T const& x, T const& y) {
        { less<T>{}(x, y) } -> bool;
        // axiom total_ordering {
        //    less<T>{}(x, y) and less<T>{}(y, z) => less<T>{}(x, z);
        //    less<T>{}(x, y) or less<T>{}(y, x) or x == y;
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
    if (!less<T>{}(x, x)) return false;

    return true;
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

template <typename T>
concept Totally_ordered =
    Regular<T> and
    requires (T const& x, T const& y) {
        { x < y } -> bool;
        { x > y } -> bool;
        { x <= y } -> bool;
        { x >= y } -> bool;
        // axiom total_ordering {
        //    x < y and y < z => x < z;
        //    x < y or y < x or x == y;
        // }
        // axiom derived_relations {
        //    x >= y <=> !(x < y);
        //    x > y <=> y < x;
        //    x <= y <=> !(y < x);
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
