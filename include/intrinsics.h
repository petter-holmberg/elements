#pragma once

#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

namespace elements {

template <typename T, typename U>
concept Same_as = std::same_as<T, U>;

template <typename T, typename U>
concept Convertible_to = std::convertible_to<T, U>;

template <typename T>
concept Equality_comparable = std::equality_comparable<T>;
// axiom regular {
//     bool(x == y) == bool(y == x);
// }

template <typename T, typename U>
concept Equality_comparable_with = std::equality_comparable_with<T, U>;

template <typename T>
concept Destructible = std::destructible<T>;
// axiom end_of_object_lifetimes {}
// complexity {
//     O(areaof(x));
// }

template <typename T, typename ...Args>
concept Constructible_from = std::constructible_from<T, Args...>;

template <typename T>
concept Default_initializable = std::default_initializable<T>;

template <typename T>
concept Move_constructible = std::move_constructible<T>;
// axiom move_semantics {
//     implication(eq(x, y), eq(T{mv(x)}, y));
// }
// complexity {
//     O(sizeof(x));
// }

template <typename T>
concept Copy_constructible = std::copy_constructible<T>;
// axiom copy_semantics {
//     eq(T{x}, y);
// }
// complexity {
//     O(sizeof(y));
// }

template <typename T, typename U>
concept Assignable_from = std::assignable_from<T, U>;

template <typename T>
concept Movable =
    std::is_object_v<T> and
    Move_constructible<T> and
    Assignable_from<T&, T>;

template <typename T>
concept Copyable = std::copyable<T>;

template <typename T>
concept Semiregular = std::semiregular<T>;

template <typename T>
concept Regular = std::regular<T>;

template <typename T>
concept Totally_ordered = std::totally_ordered<T>;
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

template <typename T>
concept Integral = std::is_integral_v<T>;

template <typename F, typename... Args>
concept Invocable = std::invocable<F, Args...>;

template <typename F, typename... Args>
requires Invocable<F, Args...>
using Return_type = std::invoke_result_t<F, Args...>;

template <typename F, typename... Args>
concept Regular_invocable = std::regular_invocable<F, Args...>;

template <typename F, typename... Args>
concept Predicate = std::predicate<F, Args...>;

template <typename R, typename T, typename U>
concept Relation = std::relation<R, T, U>;

template <typename T>
using Add_rv_ref = std::add_rvalue_reference_t<T>;

template <typename T>
using Remove_const = std::remove_cv_t<T>;

template <typename T>
using Remove_ref = std::remove_reference_t<T>;

template <typename T>
using Decay = typename std::decay<T>::type;

template <typename T>
constexpr auto
fw(Remove_ref<T>& x) noexcept -> T&&
{
    return std::forward<T>(x);
}

template <typename T>
constexpr auto
fw(Remove_ref<T>&& x) noexcept -> T&&
{
    return std::forward<T>(x);
}

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

template <typename T>
constexpr auto
mv(T&& x) noexcept -> Remove_ref<T>&&
{
    return std::move(x);
}

template <typename F, typename... Args>
constexpr auto
invoke(F&& f, Args&&... args) noexcept(std::is_nothrow_invocable_v<F, Args...>) -> Return_type<F, Args...>
{
    return std::invoke(fw<F>(f), fw<Args>(args)...);
}

template <typename T>
constexpr auto
first(std::initializer_list<T> const& x) -> T const*
{
    return std::cbegin(x);
}

template <typename T>
constexpr auto
limit(std::initializer_list<T> const& x) -> T const*
{
    return std::cend(x);
}

}
