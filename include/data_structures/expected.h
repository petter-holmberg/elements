#pragma once

#include "algorithms/regular.h"

namespace elements {

template <Movable E>
struct unexpected
{
    E value;

    explicit
    unexpected(E const& value_)
        : value(value_)
    {}
};

template <Movable, Movable>
struct expected;

template <Movable T, Movable E>
constexpr auto
make_unexpected(E const& error) -> expected<T, E>
{
    return expected<T, E>(unexpected(error));
}

template <Movable T, Movable E>
constexpr auto
make_unexpected(E&& error) -> expected<T, E>
{
    return expected<T, E>(unexpected(fw<E>(error)));
}

template <typename>
struct error_type_t;

template <typename T>
requires Movable<Decay<T>>
using Error_type = typename error_type_t<T>::type;

template <Movable T, Movable E>
struct expected
{
    union {
        T value;
        E error;
    } data = { T() };
    bool has_value = true;

    constexpr
    expected() noexcept
    {
        construct(data.value);
    }

    constexpr
    expected(expected const& x) noexcept
        : has_value(x.has_value)
    {
        if (has_value) {
            construct(data.value, x.data.value);
        } else {
            construct(data.error, x.data.error);
        }
    }

    constexpr
    expected (expected&& x) noexcept
        : has_value(x.has_value)
    {
        if (has_value) {
            construct(data.value, mv(x.data.value));
        } else {
            construct(data.error, mv(x.data.error));
        }
    }

    explicit
    expected(T const& value)
    {
        construct(data.value, value);
    }

    explicit
    expected(T&& value)
    {
        construct(data.value, fw<T>(value));
    }

    explicit
    expected(unexpected<E> const& error)
        : has_value(false)
    {
        construct(data.error, error.value);
    }

    explicit
    expected(unexpected<E>&& error)
        : has_value(false)
    {
        construct(data.error, fw<E>(error.value));
    }

    ~expected()
    {
        if (has_value) {
            destroy(data.value);
        } else {
            destroy(data.error);
        }
    }

    constexpr auto
    operator=(expected<T, E> const& x) -> expected<T, E>&
    {
        expected<T, E> temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(expected<T, E>&& x) noexcept -> expected<T, E>&
    {
        has_value = x.has_value;
        if (has_value) {
            construct(data.value, mv(x.data.value));
        } else {
            construct(data.error, mv(x.data.error));
        }
        return at(this);
    }

    constexpr auto
    operator=(unexpected<E> const& x) -> expected<T, E>&
    {
        expected<T, E> temp(x);
        swap(at(this), temp);
        return at(this);
    }

    explicit constexpr
    operator bool() const noexcept
    {
        return has_value;
    }

    template <Unary_operation Op>
    requires Same<Decay<T>, Domain<Op>>
    constexpr auto
    map(Op op) -> expected<T, E>&
    {
        if (has_value) data.value = op(data.value);
        return at(this);
    }

    template <Unary_function Fun>
    requires Same<Decay<T>, Domain<Fun>>
    constexpr auto
    map(Fun fun) const -> expected<Codomain<Fun>, E>
    {
        if (has_value) {
            return expected<Codomain<Fun>, E>(fun(data.value));
        } else {
            return expected<Codomain<Fun>, E>(unexpected<E>(data.error));
        }
    }

    template <Unary_function Fun>
    requires
        Same<Decay<T>, Domain<Fun>> and
        Same<Decay<E>, Error_type<Codomain<Fun>>>
    constexpr auto
    bind(Fun fun) const -> Codomain<Fun>
    {
        if (has_value) {
            return fun(data.value);
        } else {
            return Codomain<Fun>(unexpected<E>(data.error));
        }
    }
};

template <Movable T, Movable E>
struct value_type_t<expected<T, E>>
{
    using type = T;
};

template <Movable T, Movable E>
struct error_type_t<expected<T, E>>
{
    using type = E;
};

template <Movable T, Movable E>
constexpr auto
has_value(expected<T, E> const& x) noexcept -> bool
{
    return x.has_value;
}

template <Movable T, Movable E>
constexpr auto
has_error(expected<T, E> const& x) noexcept -> bool
{
    return !has_value(x);
}

template <typename T, typename E>
requires
    Regular<Remove_cv<T>> and
    Regular<Remove_cv<E>>
constexpr auto
operator==(expected<T, E> const& x, expected<T, E> const& y) -> bool
{
    if (x and y) return x.data.value == y.data.value;
    if (!x and !y) return x.data.error == y.data.error;
    return false;
}

template <typename T, Movable E>
requires Regular<Remove_cv<T>>
constexpr auto
operator==(expected<T, E> const& x, T const& y) -> bool
{
    if (x) return x.data.value == y.data.value;
    return false;
}

template <typename T, Movable E>
requires Regular<Remove_cv<T>>
constexpr auto
operator==(T const& x, expected<T, E> const& y) -> bool
{
    return y == x;
}

template <typename T, Movable E>
requires Regular<Remove_cv<T>>
constexpr auto
operator!=(expected<T, E> const& x, T const& y) -> bool
{
    return !(x == y);
}

template <typename T, Movable E>
requires Regular<Remove_cv<T>>
constexpr auto
operator!=(T const& x, expected<T, E> const& y) -> bool
{
    return !(y == x);
}

template <Movable T, typename E>
requires Regular<Remove_cv<E>>
constexpr auto
operator==(expected<T, E> const& x, unexpected<E> const& y) -> bool
{
    if (!x) return x.data.error == y.value.data.error;
    return false;
}

template <Movable T, typename E>
requires Regular<Remove_cv<E>>
constexpr auto
operator==(unexpected<E> const& x, expected<T, E> const& y) -> bool
{
    return y == x;
}

template <Movable T, typename E>
requires Regular<Remove_cv<E>>
constexpr auto
operator!=(expected<T, E> const& x, unexpected<E> const& y) -> bool
{
    return !(x == y);
}

template <Movable T, typename E>
requires Regular<Remove_cv<E>>
constexpr auto
operator!=(unexpected<E> const& x, expected<T, E> const& y) -> bool
{
    return !(y == x);
}

template <typename T, typename E>
requires
    Totally_ordered<Remove_cv<T>> and
    Totally_ordered<Remove_cv<E>>
constexpr auto
operator<(expected<T, E> const& x, expected<T, E> const& y) -> bool
{
    if (x and y) return x.data.value < y.data.value;
    if (x and !y) return false;
    if (y) return true;
    return x.data.error < y.data.error;
}

template <Movable T, Movable E>
constexpr void
swap_value_error(expected<T, E>& x, expected<T, E>& y)
    //[[expects: has_value(x)]]
    //[[expects: !has_value(y)]]
{
    T temp(mv(x.data.value));
    destroy(x.data.value);
    construct(x.data.error, mv(y.data.error));
    x.has_value = false;
    construct(y.data.value, mv(temp));
    y.has_value = true;
}

template <Movable T, Movable E>
constexpr void
swap(expected<T, E>& x, expected<T, E>& y)
{
    if (x and y) return swap(x.data.value, y.data.value);
    if (x and !y) return swap_value_error(x, y);
    if (y) return swap_value_error(y, x);
    return swap(x.data.error, y.data.error);
}

template <Movable T, Movable E>
requires Loadable<T>
constexpr auto
load(expected<T, E> const& x) -> T const&
    //[[expects: has_value(x)]]
{
    return x.data.value;
}

template <Movable T, Movable E>
requires Storable<T>
constexpr void
store(expected<T, E>& x, T const& value)
{
    x.data.value = value;
    x.has_value = true;
}

template <Movable T, Movable E>
requires Storable<T>
constexpr void
store(expected<T, E>& x, T&& value)
{
    x.data.value = fw<T>(value);
    x.has_value = true;
}

template <Movable T, Movable E>
requires Mutable<T>
constexpr auto
at(expected<T, E>& x) -> T&
    //[[expects: has_value(x)]]
{
    return x.data.value;
}

template <Movable T, Movable E>
constexpr auto
error(expected<T, E> const& x) -> E const&
    //[[expects: !has_value(x)]]
{
    return x.data.error;
}

template <Movable T, Movable E>
constexpr auto
error(expected<T, E>&& x) -> E&&
    //[[expects: !has_value(x)]]
{
    return fw<E>(x.data.error);
}

template <Movable T, Movable E, Movable U>
requires Loadable<T>
constexpr auto
load_or(expected<T, E> const& x, U&& y) noexcept -> T const&
{
    if (has_value(x)) {
        return load(x);
    } else {
        return fw<U>(y);
    }
}

template <Movable T, Movable E, Movable U>
requires Loadable<T>
constexpr auto
load_or(expected<T, E>&& x, U&& y) noexcept -> T const&
{
    if (has_value(x)) {
        return fw<T>(load(x));
    } else {
        return fw<U>(y);
    }
}

}
