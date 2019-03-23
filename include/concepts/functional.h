#pragma once

#include "invocable.h"

namespace elements {

template <typename T, typename Fun>
concept Functor =
    Movable<T> and
    Unary_function<Fun> and
    Same<Decay<Value_type<T>>, Decay<Domain<Fun>>> and
    requires (T x, Fun fun) {
        x.map(fun);
    };

template <typename T, typename Fun>
concept Monad =
    Functor<T, Fun> and
    Default_constructible<T> and
    requires (T x, Fun fun) {
        x.bind(fun);
    };

}
