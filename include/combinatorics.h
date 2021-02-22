#pragma once

#include "integer.h"
#include "swap.h"

namespace elements {

template <Integer I>
constexpr auto
choose(I n, I k) -> I
{
    if (n < k) return Zero<I>;
    n = n - k;
    if (n < k) swap(n, k);
    I r{One<I>};
    I i{Zero<I>};
    while (i < k) {
        increment(i);
        increment(n);
        r = r * n;
        r = r / i;
    }
    return r;
}

}
