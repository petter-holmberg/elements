Library contents
----------------

# Algorithms

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

`array_k` implements an array of *k* elements contiguously allocated on the stack, like built-in C++ arrays, but with regular semantics, lexicographic comparison operators, and supporting functions and type functions for iteration and element access.

`coordinate_point` implements a coordinate point type for use in linear algebra. It is an `Affine_space` over a `Vector_space` and a `Sequence`. By default it uses `array_k` for storage of the coordinates.

`coordinate_vector` implements a coordinate vector type for use in linear algebra. It is both a `Semimodule` over a `Semiring` and a `Sequence`. If the `Semiring` is a `Field` it supports all operations on a `Vector_space`. By default it uses `array_k` for storage of the elements. It takes two optional operators for addition and multiplication of the semiring elements.

# Appendix A: On nomenclature

Concepts, types and functions in this library are named based on the following principles:

Consistency with the history and modern conventions of mathematics, in particular with computer science, is deemed more important than consistency with established names in popular programming languages. Consistency with popular programming languages is deemed more important than consistency with established names in C++.

Lexicographical ordering of names for closely related elements is deemed desirable, to facilitate searching. Multi-word names are separated by `_`.
Concepts and type functions are named with a leading upper case letter, while functions and types are all lower case.

Short names are deemed desirable, but abbreviations are generally avoided.

Index
-----

# Algorithms

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

`array_k`
`coordinate_point`
`coordinate_vector`

References
----------

<a name="Knuth3"></a>
[Knuth3]
"The Art of Computer Programming, Volume 3, Sorting and Searching, Second Edition", ISBN 0-20189685-0
