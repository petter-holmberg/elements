#pragma once

#include "algebra.h"
#include "array_k.h"
#include "array_single_ended.h"
#include "fill.h"
#include "map.h"
#include "range.h"
#include "reduce.h"

namespace elements {

template <typename V, typename S = Scalar_type<V>>
concept Vector_space =
    Module<V, add_op<V>, S, add_op<S>, mul_op<S>> and
    Field<S, add_op<S>, mul_op<S>>;

template <typename T, pointer_diff k>
struct static_vector_engine
{
    array_k<T, k> elements;

    constexpr
    static_vector_engine() = default;

    explicit constexpr
    static_vector_engine(array_k<T, k>&& x)
        : elements{fw<array_k<T, k>>(x)}
    {}

    constexpr auto
    operator[](Size_type<array_k<T, k>> i) -> T&
    {
        return elements[i];
    }

    constexpr auto
    operator[](Size_type<array_k<T, k>> i) const -> T const&
    {
        return elements[i];
    }

    constexpr auto
    operator()(Size_type<array_k<T, k>> i) -> T&
    {
        return elements[i];
    }

    constexpr auto
    operator()(Size_type<array_k<T, k>> i) const -> T const&
    {
        return elements[i];
    }
};

template <typename T, pointer_diff k>
static_vector_engine(array_k<T, k>) -> static_vector_engine<T, k>;

template <typename T, pointer_diff k>
struct value_type_t<static_vector_engine<T, k>>
{
    using type = Remove_const<T>;
};

template <typename T, pointer_diff k>
struct scalar_type_t<static_vector_engine<T, k>>
{
    using type = T;
};

template <typename T, pointer_diff k>
struct scalar_type_t<static_vector_engine<T, k> const>
{
    using type = T const;
};

template <typename T, pointer_diff k>
struct cursor_type_t<static_vector_engine<T, k>>
{
    using type = Pointer_type<T>;
};

template <typename T, pointer_diff k>
struct cursor_type_t<static_vector_engine<T, k> const>
{
    using type = Pointer_type<T const>;
};

template <typename T, pointer_diff k>
struct size_type_t<static_vector_engine<T, k>>
{
    using type = Difference_type<Pointer_type<T>>;
    static constexpr auto value = k;
};

template <typename T, pointer_diff k>
constexpr auto
operator==(static_vector_engine<T, k> const& x, static_vector_engine<T, k> const& y) -> bool
{
    return x.elements == y.elements;
}

template <typename T, pointer_diff k>
struct lt<static_vector_engine<T, k>>
{
    constexpr auto
    operator()(static_vector_engine<T, k> const& x, static_vector_engine<T, k> const& y) -> bool
    {
        return lt{}(x.elements, y.elements);
    }
};

template <typename T, pointer_diff k>
constexpr auto
first(static_vector_engine<T, k>& x) -> Cursor_type<static_vector_engine<T, k>>
{
    return first(x.elements);
}

template <typename T, pointer_diff k>
constexpr auto
first(static_vector_engine<T, k> const& x) -> Cursor_type<static_vector_engine<T, k> const>
{
    return first(x.elements);
}

template <typename T, pointer_diff k>
constexpr auto
limit(static_vector_engine<T, k>& x) -> Cursor_type<static_vector_engine<T, k>>
{
    return limit(x.elements);
}

template <typename T, pointer_diff k>
constexpr auto
limit(static_vector_engine<T, k> const& x) -> Cursor_type<static_vector_engine<T, k> const>
{
    return limit(x.elements);
}

template <typename T, pointer_diff k>
constexpr auto
is_empty(static_vector_engine<T, k> const& x) -> bool
{
    return is_empty(x.elements);
}

template <typename T, pointer_diff k>
constexpr auto
size(static_vector_engine<T, k> const& x) -> Size_type<static_vector_engine<T, k>>
{
    return size(x.elements);
}

template <typename T, Invocable auto alloc>
struct dynamic_vector_engine
{
    array_single_ended<T, alloc> elements;

    constexpr
    dynamic_vector_engine() = default;

    constexpr
    dynamic_vector_engine(Size_type<array_single_ended<T, alloc>> capacity)
        : elements{capacity}
    {}

    constexpr
    dynamic_vector_engine(Size_type<array_single_ended<T, alloc>> size, T const& x)
        : elements(size, size, x)
    {}

    constexpr
    dynamic_vector_engine(Size_type<array_single_ended<T, alloc>> capacity, Size_type<array_single_ended<T, alloc>> size, T const& x)
        : elements(capacity, size, x)
    {}

    constexpr auto
    operator()(Size_type<array_single_ended<T, alloc>> i) -> T&
    {
        return elements[i];
    }

    constexpr auto
    operator()(Size_type<array_single_ended<T, alloc>> i) const -> T const&
    {
        return elements[i];
    }
};

template <typename T, Invocable auto alloc>
struct value_type_t<dynamic_vector_engine<T, alloc>>
{
    using type = Remove_const<T>;
};

template <typename T, Invocable auto alloc>
struct scalar_type_t<dynamic_vector_engine<T, alloc>>
{
    using type = T;
};

template <typename T, Invocable auto alloc>
struct scalar_type_t<dynamic_vector_engine<T, alloc> const>
{
    using type = T const;
};

template <typename T, Invocable auto alloc>
struct cursor_type_t<dynamic_vector_engine<T, alloc>>
{
    using type = Pointer_type<T>;
};

template <typename T, Invocable auto alloc>
struct cursor_type_t<dynamic_vector_engine<T, alloc> const>
{
    using type = Pointer_type<T const>;
};

template <typename T, Invocable auto alloc>
struct size_type_t<dynamic_vector_engine<T, alloc>>
{
    using type = Difference_type<Pointer_type<T>>;
};

template <typename T, Invocable auto alloc>
constexpr auto
operator==(dynamic_vector_engine<T, alloc> const& x, dynamic_vector_engine<T, alloc> const& y) -> bool
{
    return x.elements == y.elements;
}

template <typename T, Invocable auto alloc>
struct lt<dynamic_vector_engine<T, alloc>>
{
    constexpr auto
    operator()(dynamic_vector_engine<T, alloc> const& x, dynamic_vector_engine<T, alloc> const& y) -> bool
    {
        return lt{}(x.elements, y.elements);
    }
};

template <typename T, Invocable auto alloc>
constexpr auto
first(dynamic_vector_engine<T, alloc>& x) -> Cursor_type<dynamic_vector_engine<T, alloc>>
{
    return first(x.elements);
}

template <typename T, Invocable auto alloc>
constexpr auto
first(dynamic_vector_engine<T, alloc> const& x) -> Cursor_type<dynamic_vector_engine<T, alloc> const>
{
    return first(x.elements);
}

template <typename T, Invocable auto alloc>
constexpr auto
limit(dynamic_vector_engine<T, alloc>& x) -> Cursor_type<dynamic_vector_engine<T, alloc>>
{
    return limit(x.elements);
}

template <typename T, Invocable auto alloc>
constexpr auto
limit(dynamic_vector_engine<T, alloc> const& x) -> Cursor_type<dynamic_vector_engine<T, alloc> const>
{
    return limit(x.elements);
}

template <typename T, Invocable auto alloc>
constexpr auto
is_empty(dynamic_vector_engine<T, alloc> const& x) -> bool
{
    return is_empty(x.elements);
}

template <typename T, Invocable auto alloc>
constexpr auto
size(dynamic_vector_engine<T, alloc> const& x) -> Size_type<dynamic_vector_engine<T, alloc>>
{
    return size(x.elements);
}

template <typename T, Invocable auto alloc>
constexpr auto
capacity(dynamic_vector_engine<T, alloc> const& x) -> Size_type<dynamic_vector_engine<T, alloc>>
{
    return capacity(x.elements);
}

template <typename T, Invocable auto alloc>
constexpr void
reserve(dynamic_vector_engine<T, alloc>& x, Size_type<dynamic_vector_engine<T, alloc>> capacity)
{
    reserve(x.elements, capacity);
}

template <typename>
struct engine_type_t;

template <typename T>
using Engine_type = typename engine_type_t<T>::type;

template <typename>
struct transpose_type_t;

template <typename T>
using Transpose_type = typename transpose_type_t<T>::type;

template <typename ET, typename OT>
struct vector
{
    ET engine;

    constexpr
    vector() = default;

    constexpr
    vector(Size_type<ET> capacity)
        : engine{capacity}
    {}

    constexpr
    vector(Size_type<ET> size, Scalar_type<ET> const& x)
        : engine(size, size, x)
    {}

    constexpr
    vector(Size_type<ET> capacity, Size_type<ET> size, Scalar_type<ET> const& x)
        : engine(capacity, size, x)
    {}

    constexpr auto
    operator[](Size_type<ET> i) -> Value_type<ET>&
    {
        return engine[i];
    }

    constexpr auto
    operator[](Size_type<ET> i) const -> Value_type<ET> const&
    {
        return engine[i];
    }

    constexpr auto
    operator()(Size_type<ET> i) -> Value_type<ET>&
    {
        return engine(i);
    }

    constexpr auto
    operator()(Size_type<ET> i) const -> Value_type<ET> const&
    {
        return engine(i);
    }
};

template <typename ET, typename OT>
struct engine_type_t<vector<ET, OT>>
{
    using type = ET;
};

template <typename ET, typename OT>
struct value_type_t<vector<ET, OT>>
{
    using type = Value_type<ET>;
};

template <typename ET, typename OT>
struct scalar_type_t<vector<ET, OT>>
{
    using type = Scalar_type<ET>;
};

template <typename ET, typename OT>
struct scalar_type_t<vector<ET, OT> const>
{
    using type = Scalar_type<ET const>;
};

template <typename ET, typename OT>
struct cursor_type_t<vector<ET, OT>>
{
    using type = Cursor_type<ET>;
};

template <typename ET, typename OT>
struct cursor_type_t<vector<ET, OT> const>
{
    using type = Cursor_type<ET const>;
};

template <typename ET, typename OT>
struct transpose_type_t<vector<ET, OT>>
{
    using type = vector<ET, OT>;
};

template <typename ET, typename OT>
struct size_type_t<vector<ET, OT>>
{
    using type = Size_type<ET>;
};

template <typename ET, typename OT>
struct transpose_type_t<vector<ET, OT> const>
{
    using type = vector<ET, OT> const;
};

template <typename ET, typename OT>
constexpr auto
operator==(vector<ET, OT> const& x, vector<ET, OT> const& y) -> bool
{
    return x.engine == y.engine;
}

template <typename ET, typename OT>
struct lt<vector<ET, OT>>
{
    constexpr auto
    operator()(vector<ET, OT> const& x, vector<ET, OT> const& y) -> bool
    {
        return lt{}(x.engine, y.engine);
    }
};

template <typename ET, typename OT>
constexpr auto
first(vector<ET, OT>& x) -> Cursor_type<vector<ET, OT>>
{
    return first(x.engine);
}

template <typename ET, typename OT>
constexpr auto
first(vector<ET, OT> const& x) -> Cursor_type<vector<ET, OT> const>
{
    return first(x.engine);
}

template <typename ET, typename OT>
constexpr auto
limit(vector<ET, OT>& x) -> Cursor_type<vector<ET, OT>>
{
    return limit(x.engine);
}

template <typename ET, typename OT>
constexpr auto
limit(vector<ET, OT> const& x) -> Cursor_type<vector<ET, OT> const>
{
    return limit(x.engine);
}

template <typename ET, typename OT>
constexpr auto
is_empty(vector<ET, OT> const& x) -> bool
{
    return is_empty(x.engine);
}

template <typename ET, typename OT>
constexpr auto
size(vector<ET, OT> const& x) -> Size_type<vector<ET, OT>>
{
    return size(x.engine);
}

template <typename ET, typename OT>
constexpr auto
capacity(vector<ET, OT> const& x) -> Size_type<vector<ET, OT>>
{
    return capacity(x.engine);
}

template <typename T>
constexpr auto Is_dynamic_vector = requires (T x, Size_type<T> s) { reserve(engine(x), s); };

template <typename ET, typename OT>
requires Is_dynamic_vector<vector<ET, OT>>
constexpr void
reserve(vector<ET, OT>& x, Size_type<vector<ET, OT>> capacity)
{
    reserve(x.engine, capacity);
}

template <typename ET, typename OT>
constexpr auto
engine(vector<ET, OT>& x) -> Engine_type<vector<ET, OT>>&
{
    return x.engine;
}

template <typename ET, typename OT>
constexpr auto
engine(vector<ET, OT> const& x) -> Engine_type<vector<ET, OT>> const&
{
    return x.engine;
}

template <typename ET, typename OT>
constexpr auto
transpose(vector<ET, OT> const& x) -> Transpose_type<vector<ET, OT>>
{
    return {x};
}

struct matrix_operation_t;

template <typename T, pointer_diff k>
using static_vector = vector<static_vector_engine<T, k>, matrix_operation_t>;

template <typename T, Invocable auto alloc = array_allocator<T>>
using dynamic_vector = vector<dynamic_vector_engine<T, alloc>, matrix_operation_t>;

template <typename T0, typename T1>
struct matrix_addition_scalar_t
{
    using type = decltype(declval<T0>() + declval<T1>());
};

template <typename OT, typename T0, typename T1>
using Matrix_addition_scalar_t_type = typename OT::scalar_addition_t<T0, T1>;

template <typename T>
struct matrix_negation_scalar_t
{
    using type = decltype(-declval<T>());
};

template <typename OT, typename T>
using Matrix_negation_scalar_t_type = typename OT::scalar_negation_t<T>;

template <typename T0, typename T1>
struct matrix_subtraction_scalar_t
{
    using type = decltype(declval<T0>() - declval<T1>());
};

template <typename OT, typename T0, typename T1>
using Matrix_subtraction_scalar_t_type = typename OT::scalar_subtraction_t<T0, T1>;

template <typename T0, typename T1>
struct matrix_multiplication_scalar_t
{
    using type = decltype(declval<T0>() * declval<T1>());
};

template <typename OT, typename T0, typename T1>
using Matrix_multiplication_scalar_t_type = typename OT::scalar_multiplication_t<T0, T1>;

template <typename OT, typename ET0, typename ET1>
struct matrix_addition_engine_t
{
    using scalar_t = Matrix_addition_scalar_t_type<OT, Scalar_type<ET0>, Scalar_type<ET1>>;
    using engine_type = ET0;
};

template <typename OT, typename ET0, typename ET1>
using Matrix_addition_engine_type = typename OT::engine_addition_t<OT, ET0, ET1>::engine_type;

template <typename OT, typename ET>
struct matrix_negation_engine_t
{
    using scalar_t = Matrix_negation_scalar_t_type<OT, Scalar_type<ET>>;
    using engine_type = ET;
};

template <typename OT, typename ET>
using Matrix_negation_engine_type = typename OT::engine_negation_t<OT, ET>::engine_type;

template <typename OT, typename ET0, typename ET1>
struct matrix_subtraction_engine_t
{
    using scalar_t = Matrix_subtraction_scalar_t_type<OT, Scalar_type<ET0>, Scalar_type<ET1>>;
    using engine_type = ET0;
};

template <typename OT, typename ET0, typename ET1>
using Matrix_subtraction_engine_type = typename OT::engine_subtraction_t<OT, ET0, ET1>::engine_type;

template <typename OT, typename ET0, typename ET1>
struct matrix_multiplication_engine_t
{
    using scalar_t = Matrix_multiplication_scalar_t_type<OT, Scalar_type<ET0>, Scalar_type<ET1>>;
    using engine_type = ET0;
};

template <typename OT, typename ET0, typename ET1>
using Matrix_multiplication_engine_type = typename OT::engine_multiplication_t<OT, ET0, ET1>::engine_type;

template <typename, typename>
struct matrix_operation_t_selector_t;

template <typename T>
struct matrix_operation_t_selector_t<T, T>
{
    using type = T;
};

template <typename T>
struct matrix_operation_t_selector_t<T, matrix_operation_t>
{
    using type = T;
};

template <typename T>
struct matrix_operation_t_selector_t<matrix_operation_t, T>
{
    using type = T;
};

template <>
struct matrix_operation_t_selector_t<matrix_operation_t, matrix_operation_t>
{
    using type = matrix_operation_t;
};

template <typename T0, typename T1>
using Matrix_operation_selector_type = typename matrix_operation_t_selector_t<T0, T1>::type;

template <typename OT, typename OP0, typename OP1>
struct matrix_addition_t;

template <typename OT, typename ET0, typename OT0, typename ET1, typename OT1>
struct matrix_addition_t<OT, vector<ET0, OT0>, vector<ET1, OT1>>
{
    using engine_type = Matrix_addition_engine_type<OT, ET0, ET1>;
    using op_t = OT;
    using return_type = vector<engine_type, op_t>;

    static auto
    add(vector<ET0, OT0> const& x, vector<ET1, OT1> const& y) -> return_type
    {
        return_type z;
        if constexpr (Is_dynamic_vector<return_type>) {
            reserve(engine(z), size(x));
            map(first(x), limit(x), first(y), insert_sink{}(back{engine(z).elements}), add_op<Scalar_type<ET0>>{});
        } else {
            map(first(x), limit(x), first(y), first(z), add_op<Scalar_type<ET0>>{});
        }
        return z;
    }
};

template <typename OT, typename OP0, typename OP1>
using Matrix_addition_t_type = typename OT::addition_t<OT, OP0, OP1>;

template <typename ET0, typename OT0, typename ET1, typename OT1>
inline auto
operator+(vector<ET0, OT0> const& x, vector<ET1, OT1> const& y)
{
    using op_traits = Matrix_operation_selector_type<OT0, OT1>;
    using op0_type = vector<ET0, OT0>;
    using op1_type = vector<ET1, OT1>;
    using add_traits = Matrix_addition_t_type<op_traits, op0_type, op1_type>;

    return add_traits::add(x, y);
}

template <typename OT, typename OP>
struct matrix_negation_t;

template <typename OT, typename ET0, typename OT0>
struct matrix_negation_t<OT, vector<ET0, OT0>>
{
    using engine_type = Matrix_negation_engine_type<OT, ET0>;
    using op_t = OT;
    using return_type = vector<engine_type, op_t>;

    static auto
    neg(vector<ET0, OT0> const& x) -> return_type
    {
        return_type y;
        if constexpr (Is_dynamic_vector<return_type>) {
            reserve(y, size(x));
            map(first(x), limit(x), insert_sink{}(back{engine(y).elements}), neg_op<Scalar_type<ET0>>{});
        } else {
            map(first(x), limit(x), first(y), neg_op<Scalar_type<ET0>>{});
        }
        return y;
    }
};

template <typename OT, typename OP>
using Matrix_negation_t_type = typename OT::negation_t<OT, OP>;

template <typename ET, typename OT>
inline auto
operator-(vector<ET, OT> const& x)
{
    using op_t = OT;
    using op_type = vector<ET, OT>;
    using neg_traits = Matrix_negation_t_type<op_t, op_type>;
    return neg_traits::neg(x);
}

template <typename OT, typename OP0, typename OP1>
struct matrix_subtraction_t;

template <typename OT, typename ET0, typename OT0, typename ET1, typename OT1>
struct matrix_subtraction_t<OT, vector<ET0, OT0>, vector<ET1, OT1>>
{
    using engine_type = Matrix_subtraction_engine_type<OT, ET0, ET1>;
    using op_t = OT;
    using return_type = vector<engine_type, op_t>;

    static auto
    sub(vector<ET0, OT0> const& x, vector<ET1, OT1> const& y) -> return_type
    {
        return_type z;
        if constexpr (Is_dynamic_vector<return_type>) {
            reserve(z, size(x));
            map(first(x), limit(x), first(y), insert_sink{}(back{engine(z).elements}), sub_op<Scalar_type<ET0>>{});
        } else {
            map(first(x), limit(x), first(y), first(z), sub_op<Scalar_type<ET0>>{});
        }
        return z;
    }
};

template <typename OT, typename OP0, typename OP1>
using Matrix_subtraction_t_type = typename OT::subtraction_t<OT, OP0, OP1>;

template <typename ET0, typename OT0, typename ET1, typename OT1>
inline auto
operator-(vector<ET0, OT0> const& x, vector<ET1, OT1> const& y)
{
    using op_traits  = Matrix_operation_selector_type<OT0, OT1>;
    using op1_type   = vector<ET0, OT0>;
    using op2_type   = vector<ET1, OT1>;
    using sub_traits = Matrix_subtraction_t_type<op_traits, op1_type, op2_type>;

    return sub_traits::sub(x, y);
}

template <typename OT, typename OP0, typename OP1>
struct matrix_multiplication_t;

template <typename OT, typename ET0, typename OT0, typename S1>
struct matrix_multiplication_t<OT, vector<ET0, OT0>, S1>
{
    using engine_type = Matrix_multiplication_engine_type<OT, ET0, S1>;
    using op_t = OT;
    using return_type = vector<engine_type, op_t>;

    static auto
    mul(vector<ET0, OT0> const& x, S1 const& s) -> return_type
    {
        return_type y;
        if constexpr (Is_dynamic_vector<return_type>) {
            reserve(y, size(x));
            map(first(x), limit(x), insert_sink{}(back{engine(y).elements}), mul_unary<Scalar_type<vector<ET0, OT0>>>{s});
        } else {
            map(first(x), limit(x), first(y), mul_unary<Scalar_type<vector<ET0, OT0>>>{s});
        }
        return y;
    }
};

template <typename OT, typename S0, typename ET1, typename OT1>
struct matrix_multiplication_t<OT, S0, vector<ET1, OT1>>
{
    using engine_type = Matrix_multiplication_engine_type<OT, ET1, S0>;
    using op_t = OT;
    using return_type = vector<engine_type, op_t>;

    static auto
    mul(S0 const& s, vector<ET1, OT1> const& x) -> return_type
    {
        return_type y;
        if constexpr (Is_dynamic_vector<return_type>) {
            reserve(y, size(x));
            map(first(x), limit(x), insert_sink{}(back{engine(y).elements}), mul_unary<Scalar_type<vector<ET1, OT1>>>{s});
        } else {
            map(first(x), limit(x), first(y), mul_unary<Scalar_type<vector<ET1, OT1>>>{s});
        }
        return y;
    }
};

template <typename OT, typename ET0, typename OT0, typename ET1, typename OT1>
struct matrix_multiplication_t<OT, vector<ET0, OT0>, vector<ET1, OT1>>
{
    using op_t = OT;
    using scalar0_type = Scalar_type<vector<ET0, OT0>>;
    using scalar1_type = Scalar_type<vector<ET1, OT1>>;
    using return_type = typename Matrix_multiplication_scalar_t_type<op_t, scalar0_type, scalar1_type>::type;

    static auto
    mul(vector<ET0, OT0> const& x, vector<ET0, OT0> const& y) -> return_type
    //[[expects: size(x) == size(y)]]
    {
        vector<ET0, OT0> z;
        if constexpr (Is_dynamic_vector<vector<ET0, OT0>>) {
            reserve(z, size(x));
            map(first(x), limit(x), first(y), insert_sink{}(back{engine(z).elements}), mul_op<Scalar_type<vector<ET0, OT0>>>{});
            return reduce(
                first(z),
                limit(z),
                add_op<Scalar_type<vector<ET0, OT0>>>{},
                Zero<Scalar_type<vector<ET0, OT0>>>);
        } else {
            return reduce(
                first(z),
                map(first(x), limit(x), first(y), first(z), mul_op<Scalar_type<vector<ET0, OT0>>>{}),
                add_op<Scalar_type<vector<ET0, OT0>>>{},
                Zero<Scalar_type<vector<ET0, OT0>>>);
        }
    }
};

template <typename OT, typename OP0, typename OP1>
using Matrix_multiplication_t_type = typename OT::multiplication_t<OT, OP0, OP1>;

template <typename ET0, typename OT0, typename S1>
inline auto
operator*(vector<ET0, OT0> const& x0, S1 const& s1)
{
    return Matrix_multiplication_t_type<OT0, vector<ET0, OT0>, S1>::mul(x0, s1);
}

template <typename S0, typename ET1, typename OT1>
inline auto
operator*(S0 const& s0, vector<ET1, OT1> const& x1)
{
    return Matrix_multiplication_t_type<OT1, S0, vector<ET1, OT1>>::mul(s0, x1);
}

template <typename ET0, typename OT0, typename ET1, typename OT1>
inline auto
operator*(vector<ET0, OT0> const& x, vector<ET1, OT1> const& y)
{
    return Matrix_multiplication_t_type<Matrix_operation_selector_type<OT0, OT1>, vector<ET0, OT0>, vector<ET1, OT1>>::mul(x, y);
}

struct matrix_operation_t
{
    template <typename T0, typename T1>
    using scalar_addition_t = matrix_addition_scalar_t<T0, T1>;

    template <typename T>
    using scalar_negation_t = matrix_negation_scalar_t<T>;

    template <typename T0, typename T1>
    using scalar_subtraction_t = matrix_subtraction_scalar_t<T0, T1>;

    template <typename T0, typename T1>
    using scalar_multiplication_t = matrix_multiplication_scalar_t<T0, T1>;

    template <typename OTR, typename ET0, typename ET1>
    using engine_addition_t = matrix_addition_engine_t<OTR, ET0, ET1>;

    template <typename OTR, typename ET>
    using engine_negation_t = matrix_negation_engine_t<OTR, ET>;

    template <typename OTR, typename ET0, typename ET1>
    using engine_subtraction_t = matrix_subtraction_engine_t<OTR, ET0, ET1>;

    template <typename OTR, typename ET0, typename ET1>
    using engine_multiplication_t = matrix_multiplication_engine_t<OTR, ET0, ET1>;

    template <typename OTR, typename OP0, typename OP1>
    using addition_t = matrix_addition_t<OTR, OP0, OP1>;

    template <typename OTR, typename OP>
    using negation_t = matrix_negation_t<OTR, OP>;

    template <typename OTR, typename OP0, typename OP1>
    using subtraction_t = matrix_subtraction_t<OTR, OP0, OP1>;

    template <typename OTR, typename OP0, typename OP1>
    using multiplication_t = matrix_multiplication_t<OTR, OP0, OP1>;
};

}
