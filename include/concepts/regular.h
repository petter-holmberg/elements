#pragma once

#include <type_traits>
#include <utility>

namespace elements {

template <typename T, typename U>
concept Same = std::is_same<T, U>::value;

template <typename T, typename U>
concept Weakly_equality_comparable =
    requires (T const& x, U const& y) {
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
    Weakly_equality_comparable<T, T>;
    // axiom regular {
    //     (x == y) == (x == y);
    // }

template <typename T, typename U>
concept Equality_comparable_with =
    Equality_comparable<T> and
    Equality_comparable<U> and
    Weakly_equality_comparable<T, U>;

template <typename T>
concept Destructible =
    std::is_nothrow_destructible<T>::value;
    // axiom end_of_object_lifetimes {}
    // complexity {
    //     O(areaof(x));
    // }

template <typename T, typename ...Args>
concept Constructible =
    Destructible<T> and
    std::is_constructible<T, Args...>::value;

template <typename T>
concept Default_constructible = Constructible<T>;

template <typename T>
concept Move_constructible =
    std::is_convertible<T, T&&>::value and
    Constructible<T, T&&>;
    // axiom move_semantics {
    //     eq(x, y) => eq(T{std::move(x)}, y);
    // }
    // complexity {
    //     O(sizeof(x));
    // }

template <typename T>
concept Copy_constructible =
    Move_constructible<T> and
    Constructible<T, T const&>;
    // axiom copy_semantics {
    //     eq(T{x}, y);
    // }
    // complexity {
    //     O(sizeof(y));
    // }

template <typename T, typename U>
concept Assignable =
    requires (T x, U&& y) {
        { x = std::forward<U>(y) } -> T&;
        // axiom copy_semantics {
        //     is_lvalue_reference<decltype(y)> => eq(x = y, y);
        // }
        // axiom move_semantics {
        //     is_rvalue_reference<decltype(y)> => eq(x, y) => eq(x, z = std::move(y));
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
    Assignable<T, T&&>;
    // axiom partially_formed {
    //     // std::move(x) not necessarily well-formed*
    //     T{std::move(x)} => x = std::move(y);
    //     T{std::move(x)} => ~x;
    //     y = std::move(x) => x = std::move(y);
    //     y = std::move(x) => ~x;
    // }

template <typename T>
concept Copyable =
    Movable<T> and
    Copy_constructible<T> and
    Assignable<T, T const&>;
    // axiom partially_formed {
    //     // std::move(a) not necessarily well-formed
    //     T{std::move(a)} => a = b;
    //     b = std::move(a) = a = b;
    // }

template <typename T>
concept Semiregular =
    Default_constructible<T> and
    Copyable<T>;

template <typename T>
concept Regular =
    Semiregular<T> and
    Equality_comparable<T>;
    // axiom partially_formed {
    //     // T x not necessarily well-formed
    //     T x => x = std::move(y);
    //     T x => x = y;
    //     T x => ~x;
    // }
    // complexity default_constructor {
    //     O(1);
    // }

template <typename T>
requires Equality_comparable<T>
struct less;

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

}
