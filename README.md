Library contents
----------------

# Algorithms

## Algebra

`choose` selects the n choose k binomial coefficent.

## Searching

### Linear search

The functions in `search.h` implement algorithms based on linear search, as described in [Knuth3](#Knuth3), Chapter 6.1.

`search` and `search_not` take a loadable range and a value that is equality-comparable with the elements of the range, stopping at the position of the first matching element, or at the limit of the range if no matching element is found.
For `search`, a match is defined as the first element `y` in the range for which the given element `x` equals `y`.
For `search_not`, a match is defined as the first element `y` in the range for which the given element `x` differs from `y`.

`search_if` and `search_if_not` take a unary predicate instead of an element.
For `search_if`, a match is defined as the first element `x` for which the given predicate is true.
For `search_not`, a match is defined as the first element `y` for which the given predicate is false.

`search_unguarded`, `search_not_unguarded`, `search_if_unguarded`, and `search_if_not_unguarded` are variations of the functions above that assume an element satisfying the applied predicate is known to exist in the given range. They take a position to the first element to be tested instead of a range as they don't need to check for the limit of the range at each iteration.

`search_match` and `search_mismatch` take two loadable ranges and an optional relation, simultaneously traversing the ranges and stopping at the first positions where a match or a mismatching element is found, respectively. The default relation is `equal`.

## Equivalence and ordering

`select_0_2` takes a weak ordering and two values of its domain, and returns the minimum value. `min` uses `less` as the weak ordering.
`select_1_2` takes a weak ordering and two values of its domain, and returns the maximum value. `max` uses `less` as the weak ordering.
`select_0_3` takes a weak ordering and three values, returning the minimum. `min` uses `less` as the weak ordering.
`select_2_3` takes a weak ordering and three values, returning the maximum. `max` uses `less` as the weak ordering.
`select_1_3_ab` takes a weak ordering and three values, where the first two are in increasing order, returning the median. `select_1_3` takes a weak ordering and three values, returning the median. `median` uses `less` as the weak ordering.
`select_2_3` takes a weak ordering and three values, returning the maximum. `max` uses `less` as the weak ordering.

`lexicographical_equivalent` take two loadable ranges and an equivalence relation, comparing them for equivalence. `lexicographical_equal` uses `equal` as the equivalence relation.

`lexicographical_compare` take two loadable ranges and a weak ordering, comparing them for lexicographical ordering. `lexicographical_less` uses `less` as the ordering.

## Assigning

`fill` takes a writable range and a value, filling the range with that value.

## Copying

`copy` takes a loadable range as source and a storable position as destination. It performs copying from the first to the last element of the source, which implies that the range starting at the destination position can overlap with the source position, as long as no source position is read after an aliased destination position.

## Map/Reduce

The first version of `map` takes a loadable range as source, a storable position as destination, and a unary function to apply to every element in the source range, storing the result in the range starting at the destination position. It applies the function from the first to the last element of the source, which implies that the range starting at the destination position can overlap with the source position, as long as no source position is read after an aliased destination position.

The second version of `map` takes a loadable range and a loadable position as sources, a storable position as destination, and a binary function to apply to every corresponding pair of elements in the source ranges, storing the result in the range starting at the destination position. It applies the function from the first to the last element of the sources, which implies that the range starting at the destination position can overlap with any of the source positions, as long as no source position is read after an aliased destination position.

`reduce` takes a loadable range, a binary operation, optionally a unary function, and a zero value. It starts with the zero value as the result, and, for each element in the range, applies the binary operation on the result and the return value of the unary function applied on the element. The default function is `load`.

`reduce_nonempty` takes a nonempty loadable range, a binary operation, and optionally a unary function. It starts with the result of applying the unary function on the first element of the range as the result, and, for each element in the rest of the range, applies the binary operation on the result and the return value of the unary function applied on the element. The default function is `load`.

# Data structures

## Linear data structures

`pair` implements a structure of two elements (that may differ in type) allocated on the stack. They can be accessed through the function `get`, either by position or by type. If the two elements
are of the same type, `pair` also provides a functor interface through the member function `.map`.

`array` implements an array of elements contiguously allocated on the free store, like `std::vector`. It stores a single pointer on the stack, keeping the array size and capacity in a header
to the array elements. `array` has regular semantics, lexicographic comparison operators, and supporting functions and type functions for iteration and element access.
`array` also provides a monadic interface through the member functions `.map` and `.bind`.

`array_k` implements an array of *k* elements contiguously allocated on the stack, like built-in C++ arrays, but with regular semantics, lexicographic comparison operators, and supporting functions and type functions for iteration and element access.
`array_k` also provides a monadic interface through the member functions `.map` and `.bind`.

## Algebra

`coordinate_point` implements a coordinate point type for use in linear algebra. It is an `Affine_space` over a `Vector_space` and a `Sequence`. By default it uses `array_k` for storage of the coordinates.

`coordinate_vector` implements a coordinate vector type for use in linear algebra. It is both a `Semimodule` over a `Semiring` and a `Sequence`. If the `Semiring` is a `Field` it supports all operations on a `Vector_space`. By default it uses `array_k` for storage of the elements. It takes two optional operators for addition and multiplication of the semiring elements.

`rational` implements a rational number type, forming a `Field` over any `Integral_domain`.

`polynomial` implements a polynomial type, forming an `Integral_domain` over any `Ring`. `degree` returns the degree of the polynomial, where an empty polynomial has degree -1. `evaluate` evaluates the polynomial at a given value, and `subderivative` calculates the k:th subderivate of a given polynomial and value k.

# Concepts

The concepts in this library are largely based on definitions in [StepanovMcJones](#StepanovMcJones), with some name changes and adaptations to modern C++ features, such as move semantics.

### Regular types

`Same` describes a pair of types that are the same.

`Weakly_equality_comparable` describes a type with equality comparison operators that may not necessarily be callable twice.
`Equality_comparable` describes a type with equality comparison operators that can be called multiple times.
`Equality_comparable_with` describes two types with equality comparison operators defined between them.

`Destructible` describes a type with a destructor.
`Constructible` describes a type with a constructor.
`Default_constructible` describes a type with a default constructor.
`Move_constructible` describes a type with a move constructor.
`Copy_constructible` describes a type with a copy constructor.
`Assignable` describes a type with assignment operator.
`Movable` describes a `Move_constructible` type with a move assignment operator.
`Copyable` describes a `Copy_constructible` type with a copy assignment operator.
`Semiregular` describes a type that is both `Default_constructible`and `Copyable`.
`Regular` describes a type that is `Semiregular` and `Equality_comparable`.
`axiom_Regular` checks the concept for one given value.
`Default_totally_ordered` describes a type that is `Regular` and implements a function object `less` that provides a default total ordering for the type.
`axiom_Default_totally_ordered` checks the concept for one given value.
`Totally_ordered` describes a type where the relational operators provide the natural total ordering for the type.
`axiom_Totally_ordered` checks the concept for two given values where the first must be less than the second.

## Algebra

### Group-like

`Semigroup` describes a `Regular` type together with an associative binary operation.
`Additive_semigroup` describes a commutative `Semigroup` where the binary operation is `operator+`.
`axiom_Additive_semigroup` checks the concept for three given values.
`Multiplicative_semigroup` describes a `Semigroup` where the binary operation is `operator*`.
`axiom_Multiplicative_semigroup` checks the concept for three given values.

`Monoid` describes a `Semigroup` with an identity element, returned by the type function `Identity_element`.
`Additive_monoid` describes an `Additive_semigroup` with an identity element, returned by the type function `Zero`.
`axiom_Additive_monoid` checks the concept for three given values.
`Multiplicative_monoid` describes an `Multiplicative_semigroup` with an identity element, returned by the type function `One`.
`axiom_Multiplicative_monoid` checks the concept for three given values.

`Group` describes a `Monoid` with a `Unary_operation` `inverse_operation` that when invoked returns the inverse of an object.
`Additive_group` describes an `Additive_monoid` where the inverse of an object is returned by the unary `operator-`, and subtraction is implemented by the binary `operator-`.
`axiom_Additive_group` checks the concept for three given values.
`Multiplicative_group` describes a `Multiplicative_monoid` where the inverse of an object is returned by inoking the `Unary_operation` `reciprocal`, and division is implemented by `operator/`.
`axiom_Multiplicative_group` checks the concept for three given values.

### Ring-like

`Semiring` describes a type that is a `Monoid` in two operations `Add_op` and `Mul_op`, defaulted to the operators returned by the `Binary_operation`s `sum` and `product`, which unless specialized invoke `operator+` and `operator*`. `Add_op` is expected to be commutative, and `Mul_op` is expected to distribute over `Add_op`.
`axiom_Semiring` checks the concept for three given values.
`Commutative_semiring` descrivbes a `Semiring` where `Mul_op` is commutative.

`Ring` describes a `Semiring` where `Add_op` forms a `Group`.
`axiom_Ring` checks the concept for three given values.
`Commutative_ring` describes a `Ring` where `Mul_op` is commutative.

`Integral_domain` describes a `Commutative_ring` where there are no non-zero divisors.

`Field` describes an `Integral_domain` where `Mul_op` forms a group.

### Module-like

`Left_semimodule` describes a "vector" type that is a `Monoid` with a binary operation `V_add_op` defaulted to `sum`, together with a "scalar" type that is a `Commutative_semiring` with two binary operations `S_add_op` and `S_mul_op` defaulted to `sum`and `product`, and where an `operator*` with the scalar on the left side and the vector on the right side is defined, is commutative, distributive over `V_add_op`, compatible with `S_mul_op` and has an identity element `One`.
`Right_semimodule` describes a similar structure where `operator*` has the vector on the left side and the scalar on the right side.
`Semimodule` describes a pair of types that form both a `Left_semimodule` and a `Right_semimodule`.

`Left_module` describes a `Left_semimodule` where the "vector" type is a `Group` with `V_add_op` and where the "scalar" type is a `Ring` with `S_add_op` and `S_mul_op`.
`Right_module` describes a similar structure based on a `Right_semimodule`.
`Module` describes a pair of types that form both a `Left_module` and a `Right_module`.

### Linear algebra

`Vector_space` describes a `Module` where the "scalar" type is a `Field` and is defaulted to the `Value_type` of the "vector" type.

`Affine_space` describes a triple of types, representing a "position", a "vector", and a "scalar", where the vector and scalar form an `Affine_space` and the point is a `Regular` type where `operator+` and `operator-` are defined such that addition and subtraction of point and vector yields a point, and subtraction of point and point yields a vector.

## Functional concepts

`Functor` describes a `Movable` type together with a `Unary_function` type, where the `Value_type` of the type is the same as the `Domain` of the function type, and a member function `.map` is defined and applies the function onto the element(s) it contains, returning an object of the same type.

`Monad` describes a `Functor` that is also default constructible, and where the member function `.bind` is defined and takes a `Unary_function` returning an object of the same type.

## Integers

`Binary_integer` describes a `Totally_ordered` integral type represented like a built-in machine type, with all its standard operators.

## Invocable concepts

`Procedure` describes an invocable type with n input parameters.
`Functional_procedure` describes a `Procedure` where all input parameters are `Regular`.
`Unary_function` describes a `Functional_procedure` of `Arity` 1.
`Binary_function` describes a `Functional_procedure` of `Arity` 2.
`Homogeneous_function` describes a `Functional_procedure` of arity > 0, where all input parameters are of the same type.

`Operation` describes a `Homogeneous_function` with the same `Domain` and `Codomain`.
`Unary_operation` describes an `Operation` of `Arity` 1.
`Binary_operation` describes an `Operation` of `Arity` 2.

`Predicate` describes a `Functional_procedure` where the `Codomain` is `bool`.
`Unary_predicate` describes a `Predicate` of `Arity` 1.
`Binary_predicate` describes a `Predicate` of `Arity` 2.

`Homogeneous_predicate` describes a `Predicate` that is also a `Homogeneous_function`.
`Relation` describes a `Homogeneous_predicate` of `Arity` 2.

## Ordered algebraic concpets

`Ordered_additive_semigroup` describes an `Additive_semigroup` that is also `Totally_ordered`.
`Ordered_additive_monoid` describes an `Additive_monoid` that is also `Totally_ordered`.
`Ordered_additive_group` describes an `Additive_group` that is also `Totally_ordered`.
`Ordered_ring` describes a `Ring` that is also `Totally_ordered`.
`Ordered_integral_domain` describes an `Integral_domain` that is also `Totally_ordered`.

## Positions

### Access

`Loadable` describes a type with a function `load` that returns a constant reference to its held object. For an object that represents the position of another object, `load` returns a reference to the other object. For other objects, load returns a reference to the object itself.

`Storable` describes a type with a function `store` that stores a given value at at a given position. If the object given as a position does not represent a position, it stores the value at the position of the object itself.

`Mutable` describes a type that is both `Loadable` and `Storable`, and defines a function `at` that
returns either a reference or a constant reference to its held object.

### Linear traversal

`Position` describes a `Movable` object representing the position in a range, with a function `increment` that moves it to the next position in a range. Elements in the range are not necessarily visitable more than once.

`Forward_position` describes a `Regular` `Position` object where elements in the range may be visited multiple times.

`Bidirectional_position` describes a `Forward_position` type with a function `decrement` that moves it to the previous position in a range.

### Ranges

`Limit` describes a `Position` type together with a type that can be used to check if the limit of the range into which the position points has been reached. The check is done with the `Binary_predicate` `precedes`.

`Range` describes a type with two functions `first` and `limit`, returning the half-open range of positions describing it.
`Mutable_range` describes a `Range` with a `Mutable` `Position_type`.

`Sequence` describes a `Range` that is `Totally_ordered`, and represents a composite object whose range of elements are its parts. It has functions `is_empty`. `size` and `operator[]` for element access.
`Dynamic_sequence` describes a `Sequence` that can change size at runtime. It has functions `insert` and `erase` to change elements at the back of the range.

# Adapters

`loadable_position` takes a `Loadable` `Position` and provides the minimal interface required.

`reverse_position` takes a `Bidirectional_position` and implements a `Bidirectional_position` where `increment` decrements and `decrement` increments. Loading and storing is done from the predecessor of the original position. It is used for traversing ranges in reverse.

# Type functions

`Remove_cv` is a type stripped of const and volatile qualifiers.

`Remove_ref` is a type stripped of reference qualifiers.

`Decay` is a type stripped of const, volatile, and reference qualifiers.

`Underlying_type` is the type describing the data storage of a given type.

## Invocable

`Codomain` is the return type of an invocable type.

`Input_type` is the type of an invocable's parameter at a given index.

`Arity` is the number of parameters of an invocable.

`Domain` is the type of a homogeneous invocable's parameters.

## Positions

`Pointer_type` is the type of a pointer to an object of a given type.

`Value_type` is either a type directly, or, in the case of a `Position` type, or a `Range` type, the type it is parameterized on.

`Difference_type` is a type capable of storing the (signed) difference between two objects of the given type.

`Distance_type` is a type capable of storing the (unsigned) distance between two objects of the given type.

`Position_type` is the type of an object describing a position to the given type.

`Size_type` is the type used to describe the size of an object of a given type.

`Size` is the size of a given type (when known at compile time).

## Algebra

`Zero` is the additive identity element of a given type.

`One` is the multiplicative identity element of a given type.

# Appendix A: On nomenclature

Concepts, types and functions in this library are named based on the following principles:

Consistency with the history and modern conventions of mathematics, in particular with computer science, is deemed more important than consistency with established names in popular programming languages. Consistency with popular programming languages is deemed more important than consistency with established names in C++.

Lexicographical ordering of names for closely related elements is deemed desirable, to facilitate searching. Multi-word names are separated by `_`.
Concepts and type functions are named with a leading upper case letter, while functions and types are all lower case.

Short names are deemed desirable, but abbreviations are generally avoided.

Index
-----

# Algorithms

`choose`

`search`
`search_not`
`search_if`
`search_if_not`.
`search_unguarded`,
`search_not_unguarded`,
`search_if_unguarded`
`search_if_not_unguarded`
`search_match`
`search_mismatch`

`select_0_2`
`min`
`select_1_2`
`max`
`select_0_3`
`select_2_3`
`max`
`select_1_3_ab`
`select_1_3`
`median`
`lexicographical_equivalent`
`lexicographical_equal`
`lexicographical_compare`
`lexicographical_less`

`fill`

`copy`

`map`
`reduce`

# Data structures

`pair`
`array`
`array_k`

`coordinate_point`
`coordinate_vector`
`rational`
`polynomial`

# Concepts

`Semigroup`
`Additive_semigroup`
`Multiplicative_semigroup`
`Monoid`
`Additive_monoid`
`Multiplicative_monoid`
`Group`
`Additive_group`
`Multiplicative_group`
`Semiring`
`Commutative_semiring`
`Ring`
`Commutative_ring`
`Integral_domain`
`Field`
`Left_semimodule`
`Right_semimodule`
`Semimodule`
`Left_module`
`Right_module`
`Module`
`Vector_space`
`Affine_space`

`Functor`
`Monad`

`Binary_integer`

`Procedure`
`Functional_procedure`
`Unary_function`
`Binary_function`
`Homogeneous_function`
`Operation`
`Unary_operation`
`Binary_operation`
`Predicate`
`Unary_predicate`
`Binary_predicate`
`Homogeneous_predicate`
`Relation`

`Ordered_additive_semigroup`
`Ordered_additive_monoid`
`Ordered_additive_group`
`Ordered_ring`
`Ordered_integral_domain`

`Loadable`
`Storable`
`Mutable`
`Position`
`Forward_position`
`Bidirectional_position`
`Limit`
`Range`
`Mutable_range`
`Sequence`
`Dynamic_sequence`

`Same`
`Weakly_equality_comparable`
`Equality_comparable`
`Equality_comparable_with`
`Destructible`
`Constructible`
`Default_constructible`
`Move_constructible`
`Copy_constructible`
`Assignable`
`Movable`
`Copyable`
`Semiregular`
`Regular`
`Default_totally_ordered`
`Totally_ordered`

# Adapters

`loadable_position`
`reverse_position`

# Type functions

`Remove_cv`
`Remove_ref`
`Decay`
`Underlying_type`

`Codomain`
`Input_type`
`Arity`
`Domain`

`Pointer_type`
`Value_type`
`Difference_type`
`Distance_type`
`Position_type`
`Size_type`
`Size`

`Zero`
`One`

References
----------

<a name="Knuth3"></a>
[Knuth3]
"The Art of Computer Programming, Volume 3, Sorting and Searching, Second Edition", ISBN 0-20189685-0

<a name="StepanovMcJones"></a>
[StepanovMcJones]
"Elements of Programming", ISBN 0-321-63537-X
