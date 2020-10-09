#pragma once

#include "swap.h"

namespace elements {

template <Movable F>
struct failure
{
    F value;

    explicit
    failure(F const& value_)
        : value(value_)
    {}
};

template <Movable, Movable>
struct result;

template <Movable T, Movable E>
constexpr auto
fail(E const& error) -> result<T, E>
{
    return result<T, E>(failure(error));
}

template <Movable T, Movable E>
constexpr auto
fail(E&& error) -> result<T, E>
{
    return result<T, E>(failure(fw<E>(error)));
}

template <typename>
struct error_type_t;

template <typename T>
requires Movable<Decay<T>>
using Error_type = typename error_type_t<T>::type;

template <Movable T, Movable E>
struct result
{
    union {
        T value;
        E error;
    } data = { T() };
    bool has_value = true;

    constexpr
    result() noexcept
    {
        construct(data.value);
    }

    constexpr
    result(result const& x) noexcept
        : has_value(x.has_value)
    {
        if (has_value) {
            construct(data.value, x.data.value);
        } else {
            construct(data.error, x.data.error);
        }
    }

    constexpr
    result(result&& x) noexcept
        : has_value(x.has_value)
    {
        if (has_value) {
            construct(data.value, mv(x.data.value));
        } else {
            construct(data.error, mv(x.data.error));
        }
    }

    explicit
    result(T const& value)
    {
        construct(data.value, value);
    }

    explicit
    result(T&& value)
    {
        construct(data.value, fw<T>(value));
    }

    explicit
    result(failure<E> const& error)
        : has_value(false)
    {
        construct(data.error, error.value);
    }

    explicit
    result(failure<E>&& error)
        : has_value(false)
    {
        construct(data.error, fw<E>(error.value));
    }

    ~result()
    {
        if (has_value) {
            destroy(data.value);
        } else {
            destroy(data.error);
        }
    }

    constexpr auto
    operator=(result<T, E> const& x) -> result<T, E>&
    {
        result<T, E> temp(x);
        swap(at(this), temp);
        return at(this);
    }

    constexpr auto
    operator=(result<T, E>&& x) noexcept -> result<T, E>&
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
    operator=(failure<E> const& x) -> result<T, E>&
    {
        result<T, E> temp(x);
        swap(at(this), temp);
        return at(this);
    }

    explicit constexpr
    operator bool() const noexcept
    {
        return has_value;
    }

    template <Operation<T> Op>
    constexpr auto
    fmap(Op op) -> result<T, E>&
    {
        if (has_value) data.value = op(data.value);
        return at(this);
    }

    template <Regular_invocable<T> F>
    constexpr auto
    fmap(F fun) const -> result<Return_type<F, T>, E>
    {
        if (has_value) {
            return result<Return_type<F, T>, E>(fun(data.value));
        } else {
            return result<Return_type<F, T>, E>(failure<E>(data.error));
        }
    }
};

template <Movable T, Movable E>
struct value_type_t<result<T, E>>
{
    using type = T;
};

template <Movable T, Movable E>
struct error_type_t<result<T, E>>
{
    using type = E;
};

template <Movable T, Movable E>
constexpr auto
has_value(result<T, E> const& x) noexcept -> bool
{
    return x.has_value;
}

template <Movable T, Movable E>
constexpr auto
has_error(result<T, E> const& x) noexcept -> bool
{
    return !has_value(x);
}

template <typename T, typename E>
requires
    Regular<Remove_const<T>> and
    Regular<Remove_const<E>>
constexpr auto
operator==(result<T, E> const& x, result<T, E> const& y) -> bool
{
    if (x and y) return x.data.value == y.data.value;
    if (!x and !y) return x.data.error == y.data.error;
    return false;
}

template <typename T, Movable E>
requires Regular<Remove_const<T>>
constexpr auto
operator==(result<T, E> const& x, T const& y) -> bool
{
    if (x) return x.data.value == y.data.value;
    return false;
}

template <typename T, Movable E>
requires Regular<Remove_const<T>>
constexpr auto
operator==(T const& x, result<T, E> const& y) -> bool
{
    return y == x;
}

template <typename T, Movable E>
requires Regular<Remove_const<T>>
constexpr auto
operator!=(result<T, E> const& x, T const& y) -> bool
{
    return !(x == y);
}

template <typename T, Movable E>
requires Regular<Remove_const<T>>
constexpr auto
operator!=(T const& x, result<T, E> const& y) -> bool
{
    return !(y == x);
}

template <Movable T, typename E>
requires Regular<Remove_const<E>>
constexpr auto
operator==(result<T, E> const& x, failure<E> const& y) -> bool
{
    if (!x) return x.data.error == y.value.data.error;
    return false;
}

template <Movable T, typename E>
requires Regular<Remove_const<E>>
constexpr auto
operator==(failure<E> const& x, result<T, E> const& y) -> bool
{
    return y == x;
}

template <Movable T, typename E>
requires Regular<Remove_const<E>>
constexpr auto
operator!=(result<T, E> const& x, failure<E> const& y) -> bool
{
    return !(x == y);
}

template <Movable T, typename E>
requires Regular<Remove_const<E>>
constexpr auto
operator!=(failure<E> const& x, result<T, E> const& y) -> bool
{
    return !(y == x);
}

template <typename T, typename E>
requires
    Totally_ordered<Remove_const<T>> and
    Totally_ordered<Remove_const<E>>
constexpr auto
operator<(result<T, E> const& x, result<T, E> const& y) -> bool
{
    if (x and y) return x.data.value < y.data.value;
    if (x and !y) return false;
    if (y) return true;
    return x.data.error < y.data.error;
}

template <Movable T, Movable E>
constexpr void
swap_value_error(result<T, E>& x, result<T, E>& y)
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
swap(result<T, E>& x, result<T, E>& y)
{
    if (x and y) return swap(x.data.value, y.data.value);
    if (x and !y) return swap_value_error(x, y);
    if (y) return swap_value_error(y, x);
    return swap(x.data.error, y.data.error);
}

template <Movable T, Movable E>
requires Loadable<T>
constexpr auto
load(result<T, E> const& x) -> T const&
    //[[expects: has_value(x)]]
{
    return x.data.value;
}

template <Movable T, Movable E>
requires Storable<T>
constexpr void
store(result<T, E>& x, T const& value)
{
    x.data.value = value;
    x.has_value = true;
}

template <Movable T, Movable E>
requires Storable<T>
constexpr void
store(result<T, E>& x, T&& value)
{
    x.data.value = fw<T>(value);
    x.has_value = true;
}

template <Movable T, Movable E>
requires Mutable<T>
constexpr auto
at(result<T, E>& x) -> T&
    //[[expects: has_value(x)]]
{
    return x.data.value;
}

template <Movable T, Movable E, Regular_invocable<T> F>
requires Same_as<Decay<E>, Error_type<Return_type<F, T>>>
constexpr auto
operator&(result<T, E> const& x, F fun) -> Return_type<F, T>
{
    if (x.has_value) {
        return fun(x.data.value);
    } else {
        return Return_type<F, T>(failure<E>(x.data.error));
    }
}

template <Movable T, Movable E, Regular_invocable<T> F>
requires Same_as<Decay<E>, Error_type<Return_type<F, T>>>
constexpr auto
operator&(result<T, E>& x, F fun) -> Return_type<F, T>
{
    if (x.has_value) {
        return fun(x.data.value);
    } else {
        return Return_type<F, T>(failure<E>(x.data.error));
    }
}

template <Movable T, Movable E>
constexpr auto
error(result<T, E> const& x) -> E const&
    //[[expects: !has_value(x)]]
{
    return x.data.error;
}

template <Movable T, Movable E>
constexpr auto
error(result<T, E>&& x) -> E&&
    //[[expects: !has_value(x)]]
{
    return fw<E>(x.data.error);
}

template <Movable T, Movable E, Movable U>
requires Loadable<T>
constexpr auto
load_or(result<T, E> const& x, U&& y) noexcept -> T const&
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
load_or(result<T, E>&& x, U&& y) noexcept -> T const&
{
    if (has_value(x)) {
        return fw<T>(load(x));
    } else {
        return fw<U>(y);
    }
}

}
