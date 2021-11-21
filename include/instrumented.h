#pragma once

#include "intrinsics.h"
#include "fill.h"

namespace elements {

struct instrumented_base
{
    enum operations
    {
        n, default_construct, copy, assign, move, move_assign, destruct, equal, less
    };

    static size_t const n_ops = 10;
    static double counts[n_ops];

    static constexpr char const*
    counter_names[n_ops] =
    {
        "n",
        "default",
        "copy",
        "assign",
        "move",
        "move_assign",
        "destruct",
        "equal",
        "less"
    };

    static constexpr void
    initialize(size_t m = 0)
    {
        fill(counts, counts + n_ops, 0.0);
        counts[n] = double(m);
    }

};

double instrumented_base::counts[];

template <typename T>
struct instrumented : instrumented_base
{
    T value;

    constexpr
    instrumented()
    {
        ++counts[default_construct];
    }

    constexpr
    instrumented(instrumented const& x)
        : value{x.value}
    {
        ++counts[copy];
    }

    constexpr auto
    operator=(instrumented const& x) -> instrumented&
    {
        ++counts[assign];
        value = x.value;
        return *this;
    }

    constexpr
    instrumented(instrumented&& x)
        : value{mv(x.value)}
    {
        ++counts[move];
    }

    constexpr auto
    operator=(instrumented&& x) -> instrumented&
    {
        ++counts[move_assign];
        value = mv(x.value);
        return *this;
    }

    ~instrumented()
    {
        ++counts[destruct];
    }

    friend constexpr auto
    operator==(instrumented const& x, instrumented const& y) -> bool
    {
        ++counts[equal];
        return x.value == y.value;
    }

    friend constexpr auto
    operator<(instrumented const& x, instrumented const& y) -> bool
    {
        ++counts[less];
        return x.value < y.value;
    }
};

template <typename T>
struct value_type_t<instrumented<T>>
{
    using type = T;
};

}
