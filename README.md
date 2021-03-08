Library contents
----------------

# Adapters

Adapters are type constructors that provide a different behavior and/or different interface than the given type.

## Cursor adapters

`cursor` takes a `Cursor` and provides the minimal interface required.

`forward_cursor` takes a `Forward_cursor` and provides the minimal interface required.

`counted_cursor` takes a `Cursor` and optionally a count of the `Difference_type` of the cursor, constructing a cursor type that can be compared against a limit that is `Equality_comparable_with` the count. `increment` increments both the cursor and the count, `decrement` decrements both the cursor and the count.

`loadable_cursor` takes a `Loadable` `Cursor` and provides the minimal interface required.

`reverse_cursor` takes a `Bidirectional_cursor` and implements a `Bidirectional_cursor` where `increment` decrements and `decrement` increments. Loading and storing is done from the predecessor of the original cursor. It is used for traversing ranges in reverse.

`filter_cursor` takes a `Storable` `Cursor` and a unary `Predicate`, constructing a cursor type that stores and increments when `store` is called, if the predicate is true for the current element. `filter_sink` takes a unary `Predicate`, constructing an `Invocable` type that returns a `filter_cursor` when invoked with a `Storable` `Cursor`.

`insert_cursor` takes a `Storable` `Cursor`, constructing a cursor type that applies the function `insert` on the current element when `store` is called. `insert_sink` in an `Invocable` type that returns an `insert_cursor` when invoked with a `Storable` `Cursor`.

`map_cursor` takes a `Storable` `Cursor` and a unary `Invocable`, constructing a cursor type that applies the function on the current element when `store` is called. `map_sink` takes a unary `Invocable`, constructing an `Invocable` type that returns a `map_cursor` when invoked with a `Storable` `Cursor`.

# Algorithms

## Algebra

### Accumulation

`power_left_associated` takes a left-associative binary `Operation` or `Accumulation_procedure`, a value of its `Return_type`, and  positive `Integer`. It raises the value to the power of the integer.
`power_right_associated` takes a right-associative binary `Operation` or `Accumulation_procedure`, a value of its `Return_type`, and  positive `Integer`. It raises the value to the power of the integer.
`power_semigroup` takes a `Semigroup` operation or `Semigroup_accumulator`, a value of its `Return type`, and a positive `Integer`. It raises the value to the power of the integer.
`power_monoid` takes a `Monoid` operation or `Monoid_accumulator`, a value of its `Return type`, and a non-negative `Integer`. It raises the value to the power of the integer, returning the `Identity_element` of the operation if the integer is zero.
`power_group` takes a `Group` operation or `Group_accumulator`, a value of its `Return type`, and an `Integer`. It raises the value to the power of the integer.

### Ordered algebra

`abs` returns the absolute value of a member of an `Ordered_group`, with respect to its inverse operation. It defaults to using negation, the inverse operation of `add`.

`cancel` performs cancellation (i.e. partial subtraction) on a member of a `Cancellable_monoid`.

`quotient` returns the quotient of a member of a `Halvable_monoid` with respect to another member and a given operation.

`remainder` returns the remainder of a member of a `Halvable_monoid` with respect to another member and a given operation.

`quotient_remainder` returns the quotient and remainder of a member of an `Archimedean_monoid` with respect to another member and a given operation. The default operation is `add`. If the members satisfy an `Archimedean_group`, it uses the `inverse_operation` instead of `cancel`. If called with `multiply` it calculates logarithm and remainder.

`gcd` returns the greatest common divisor of two members of an `Euclidean_semimodule`.

## Combinatorics

`choose` selects the n choose k binomial coefficent.

## Equivalence and ordering

`select_0_2` takes a weak ordering and two values of its domain, and returns the minimum value. `min` uses `lt` as the weak ordering.
`select_1_2` takes a weak ordering and two values of its domain, and returns the maximum value. `max` uses `lt` as the weak ordering.
`select_0_3` takes a weak ordering and three values, returning the minimum. `min` uses `lt` as the weak ordering.
`select_2_3` takes a weak ordering and three values, returning the maximum. `max` uses `lt` as the weak ordering.
`select_1_3_ab` takes a weak ordering and three values, where the first two are in increasing order, returning the median. `select_1_3` takes a weak ordering and three values, returning the median. `median` uses `lt` as the weak ordering.
`select_2_3` takes a weak ordering and three values, returning the maximum. `max` uses `lt` as the weak ordering.

`equivalent_lexicographical` takes two loadable ranges and an equivalence relation, comparing them for equivalence. `equal_lexicographical` uses `eq` as the equivalence relation.

`compare_lexicographical` takes two loadable ranges and a weak ordering, comparing them for lexicographical ordering. `less_lexicographical` uses `lt` as the ordering.

`equal_range` takes two `Range`s, comparing them for equality.
`less_range` takes two `Range`s, comparing them for `lt` lexicographical ordering.

## Assigning

`fill` takes a writable range and a value, filling the range with that value.

## Counting

`count_if` takes a loadable range, a unary predicate, and optionally an initial count. It counts the number of values in the range that satisfy the predicate.
`count_if_not` takes a loadable range, a unary predicate, and optionally an initial count. It counts the number of values in the range that do not satisfy the predicate.
`count` takes a loadable range, a value, and optionally an initial count. It counts the number of values in the range that equal the value.
`count_not` takes a loadable range, a value, and optionally an initial count. It counts the number of values in the range that do not equal the value.

## Copying

`copy` takes a loadable range as source and a storable cursor as destination. It performs copying from the first to the last element of the source, which implies that the range starting at the destination cursor can overlap with the source cursor, as long as no source cursor is read after an aliased destination cursor.

`copy_select` takes a loadable range as source, a storable cursor as destination, and a unary `Predicate` to determine which of the elements from the source that should be copied to the destination.

`copy_if` takes a loadable range as source, a storable cursor as destination, and a unary `Predicate` to determine which of the elements from the source that should be copied to the destination. The predicate is tested on the elements at the source cursors.

`copy_if_not` takes a loadable range as source, a storable cursor as destination, and a unary `Predicate` to determine which of the elements from the source that should not be copied to the destination. The predicate is tested on the elements at the source cursors.

## Map

The first version of `map` takes a loadable range as source, a storable cursor as destination, and a unary function to apply to every element in the source range, storing the result in the range starting at the destination cursor. It applies the function from the first to the last element of the source, which implies that the range starting at the destination cursor can overlap with the source cursor, as long as no source cursor is read after an aliased destination cursor.

The second version of `map` takes a loadable range and a loadable cursor as sources, a storable cursor as destination, and a binary function to apply to every corresponding pair of elements in the source ranges, storing the result in the range starting at the destination cursor. It applies the function from the first to the last element of the sources, which implies that the range starting at the destination cursor can overlap with any of the source cursors, as long as no source cursor is read after an aliased destination cursor.

## Reduce

`reduce` takes a loadable range, a binary operation, optionally a unary function, and a zero value. It starts with the zero value as the result, and, for each element in the range, applies the binary operation on the result and the return value of the unary function applied on the element. The default function is `load`.

`reduce_nonempty` takes a nonempty loadable range, a binary operation, and optionally a unary function. It starts with the result of applying the unary function on the first element of the range as the result, and, for each element in the rest of the range, applies the binary operation on the result and the return value of the unary function applied on the element. The default function is `load`.

`reduce_nonzeroes` takes a loadable range, a binary operation, optionally a unary function, and a zero value. It works the same as `reduce` except that the binary function is not applied when the
function applied on an element of the range returns the zero value. The default function is `load`.

`reduce_balanced` takes a loadable range, a binary operation, optionally a unary function, and a zero value. It performs balanced reduction, storing intermediate reductions in a `binary_counter`. The default function is `load`.
This algorithm minimizes the cost of applying the operation if the size of a reduced object is the sum of the sizes of the original objects and the complexity of applying the reduction operation grows linearly with the sizes of its arguments.

## Flat map

`flat_map` takes a loadable range as source and a unary function that returns a `Sequence`. It applies the function on every element in the source range, returning a `Sequence` of the concatenated sequences that the unary function returns.

## Partitioning

`is_partitioned` takes a loadable range and a unary predicate. It checks if the range is partitioned according to the predicate, with all elements not satisfying the predicate preceding the elements satisfying the predicate.

`partition_point_counted` takes a forward cursor, a distance, and a unary predicate, assuming that the range from the cursor to the cursor + distance is partitioned according to the predicate. Using bisection, it returns a cursor pointing to the first element satisfying the predicate.

`partition_point` takes a loadable forward range and a unary predicate, assuming that the range is partitioned according to the predicate. Using bisection, it returns a cursor pointing to the first element satisfying the predicate.

`partition_semistable` takes a mutable forward range and a unary prediate. It partitions the range such that all elements not satisfying the predicate precede the elements satisfying the predicate. It preserves the relative ordering of the elements not satisfying the predicate, but not the relative ordering of the elements satisfying the predicate. It returns the partition point, i.e. a cursor pointing to the first element satisfying the predicate.

`partition_with_buffer` takes a mutable forward range, a cursor pointing to the first element of a mutable buffer with a size not less than the size of the forward range, and a unary prediate. It partitions the range such that all elements not satisfying the predicate precede the elements satisfying the predicate. It preserves the relative ordering of the elements on both sides of the partition point. It returns the partition point.

`partition` takes a mutable bidirectional range and a unary prediate. It partitions the range such that all elements not satisfying the predicate precede the elements satisfying the predicate. It preserves the relative ordering of the elements on both sides of the partition point. It returns the partition point.

## Searching

### Binary search

The functions in `search.h` implement algorithms based on binary search, as described in [Knuth3](#Knuth3), Chapter 6.2.

`search_binary_lower` takes a loadable forward range and a relation defaulting to `lt`, assuming that the range is ordered in accordance with the relation. It returns a cursor pointing to the first element satisfying the relation.

`search_binary_upper` takes a loadable forward range and a relation defaulting to `lt`, assuming that the range is ordered in accordance with the relation. It returns the successor of a cursor pointing to the last element satisfying the relation.

`search_binary` takes a loadable forward range and a relation defaulting to `lt`, assuming that the range is ordered in accordance with the relation. It finds the `bounded_range` of elements satisfying the relation.

### Linear search

The functions in `search.h` implement algorithms based on linear search, as described in [Knuth3](#Knuth3), Chapter 6.1, and [KnuthMorrisPratt](#KnuthMorrisPratt).

`search` and `search_not` take a loadable range and a value that is equality-comparable with the elements of the range, stopping at the position of the first matching element, or at the limit of the range if no matching element is found.
For `search`, a match is defined as the first element `y` in the range for which the given element `x` equals `y`.
For `search_not`, a match is defined as the first element `y` in the range for which the given element `x` differs from `y`.

`search_if` and `search_if_not` take a unary predicate instead of an element.
For `search_if`, a match is defined as the first element `x` for which the given predicate is true.
For `search_not`, a match is defined as the first element `y` for which the given predicate is false.

`search_backward`, `search_backward_not`, `search_backward_if`, and `search_backward_if_not` are variations of the functions above that search backward through the loadable range.

`search_unguarded`, `search_not_unguarded`, `search_if_unguarded`, and `search_if_not_unguarded` are variations of the functions above that assum_ope an element satisfying the applied predicate is known to exist in the given range (which necessarily is nonempty). They take a cursor pointing to the first element to be tested instead of a range as they don't need to check for the limit of the range at each iteration. Iteration is faster because the end of the range doesn't need to be checked.

`search_guarded`, `search_not_guarded`, `search_if_guarded`, and `search_if_not_guarded` are variations of the functions above that temporarily put a `sentinel` value at the last position of the range, to enable search nearly as fast as for the unguarded variations. They require the range to be `Mutable` and take a cursor pointing to the last element of the range instead of its limit. `search_not_guarded`, `search_if_guarded`, and `search_if_not_guarded` also take a sentinel value to be used to ensure termination.

assum_ope an element satisfying the applied predicate is known to exist in the given range. They take a cursor pointing to the first element to be tested instead of a range as they don't need to check for the limit of the range at each iteration.

`search_match` and `search_mismatch` take two loadable ranges and an optional relation, simultaneously traversing the ranges and stopping at the first positions where a match or a mismatching element is found, respectively. The default relation is `eq`. If the second range is not shorter than the first, or if a match is known to exist before the end of the second range, the algorithm can be called with only a `cursor` to the first element of the second range.

`search_adjacent_match` and `search_adjacent_mismatch` take a loadable range and a relation,
stopping at the first position where an element and its successor satisfy the relation, or does not satisfy the relation, respectively. The default relation is `eq`.

### Subsequence search

`search_subsequence` takes two loadable ranges, a cursor pointing to the first element of a mutable buffer of the `Difference_type` of the range cursor, with a size not less than the size of the second range, and an optional relation. The default relation is `eq`
It will return a cursor pointing to the first subsequence in the first range that matches the second range, or the limit of the first range if no subsequence matching the second range is found.

## Permutations

### Reverse

`reverse` takes a mutable range and reverses the elements in the range.

### Rotate

`rotate` takes a mutable range and a cursor in the range, swapping elements such that the element at the given cursor's position becomes the first element in the range, and the element preceding it becomes the last element.

## Side effects

`for_each` takes a loadable range and a procedure of arity 1. It applies the procedure on each value in the range, returning a `for_each_result` containing a cursor and the procedure. The cursor points at the position where the iteration has stopped (which may not be reachable a second time), and the procedure is returned as it could have accumulated information during the traversal.

## Quantifiers

`each_of` takes a loadable range and a unary predicate. It checks if each value in the range satisfy the predicate.
`any_not_of` takes a loadable range and a unary predicate. It checks if any value in the range does not satisfy the predicate.
`none_of` takes a loadable range and a unary predicate. It checks if none of the values in the range satisfies the predicate.
`any_of` takes a loadable range and a unary predicate. It checks if any value in the range satisfies the predicate.

## Transformations and Actions

`power_unary` takes a `Transformation` or `Action`, a starting value of its `Return_type`, and a non-negative `Integer`. It invokes the `Transformation` or `Action` as many times as given by the `Integer`.
`distance` takes a `Transformation` or `Action` and two values of the its `Return_type`, where the second value is reachable from the first. It returns the number of invocations of the transformation it takes to go from the first to the second value.
`collision_point` takes a `Transformation` or `Action`, a starting value of its `Return_type`, and a unary `Predicate`. It returns the terminal point (if the transformation is terminating) or the collision point of the transformation (if the transformation is non-terminating). The collision point is the value where the transformation (starting with the given starting value) applied n times equals the transformation applied 2n + 1 times. The predicate guards against going outside of the transformation's definition space.
`collision_point_nonterminating_orbit` takes a `Transformation` or `Action` and a starting value of its `Return_type`. The transformation has to be total or nonterminating for the given value.
`is_terminating` takes a `Transformation` or `Action`, a starting value of its `Return_type`, and a unary `Predicate`. It determines if the given transformation is terminating.
`is_circular` takes a `Transformation` or `Action`, a starting value of its `Return_type`, and a unary `Predicate`. It determines if the given transformation is circular.
`is_circular_nonterminating_orbit` takes a `Transformation` or `Action` and a starting value of its `Return_type`. The transformation has to be total or nonterminating for the given value. It determines if the given transformation is circular.
`connection_point` takes a `Transformation` or `Action`, a starting value of its `Return_type`, and a unary `Predicate`. It returns the connection point of the transformation, i.e. the point where a cycle starts, or the terminal element for a terminating transformation.
`connection_point_nonterminating_orbit` takes a `Transformation` or `Action` and a starting value of its `Return_type`. It returns the connection point of the transformation, i.e. the point where a cycle starts, or the terminal element for a terminating transformation.
`orbit_structure` takes a `Transformation` or `Action`, a starting value of its `Return_type`, and a unary `Predicate`. It returns a structure that measures all orbit sizes.
`orbit_structure_nonterminating_orbit` takes a `Transformation` or `Action` and a starting value of its `Return_type`. It returns a structure that measures all orbit sizes.

## Zipping/Unzipping

`zip` takes two loadable ranges as sources and a storable cursor as destination. It performs copying from the first to the last element of the sources, starting with the first source and alternating between the sources for every element. If one of the source ranges is longer than the other, its remaining elements are appended at the end.

`unzip` takes one loadable range as source and two storable cursors as destinations. It performs copying from the first to the last element of the source, alternating between the destinations for each element.

# Data structures

## Algebra

`bit` implements a type supporting the algebra of Boole and propositional logic. It is a `Commutative_semiring`, with addition represented by "xor" and multiplication represented by "and". The 16 possible logical operations on two bits are implemented over this semiring by the following named functions, in binary counting order: `contradiction`, `conjunction`, `nonimplication`, `projection_left`, `converse_nonimplication`, `projection_right`, `nonequivalence`, `disjunction`, `nondisjunction`, `equivalence`, `nonprojection_right`, `converse_implication`, `nonprojection_left`, `implication`, `nonconjunction`, `tautology`.

`affine_point` implements an affine point type for use in linear algebra. It is an `Affine_space` over a `Vector_space` and a `Sequence`. By default it uses `array_k` for storage of the coordinates.

`affine_vector` implements an affine vector type for use in linear algebra. It is both a `Semimodule` over a `Semiring` and a `Sequence`. If the `Semiring` is a `Field` it supports all operations on a `Vector_space`. By default it uses `array_k` for storage of the elements. It takes two optional operators for addition and multiplication of the semiring elements.

`rational` implements a rational number type, forming a `Field` over any `Integral_domain`.

`polynomial` implements a polynomial type, forming an `Integral_domain` over any `Ring`. `degree` returns the degree of the polynomial, where an empty polynomial has degree -1. `evaluate` evaluates the polynomial at a given value, and `subderivative` calculates the k:th subderivate of a given polynomial and value k.

`binary counter` implements a binary counter of k elements, using a given binary operation and identity element. When calling it with an object, if the object is not equal to the identity element of the counter, it will be reduced with the existing elements in the counter, resetting each element to the identity element until either an identity element is found or the end of the elements are reached. If an identity element is found, the reduced value replaces it. If the end of the elements are reached, it is appended as the new last element.

## Memory management

`memory` implements a descriptor for a contiguous sequence of `byte`s, with a pointer to the first byte and the size of the sequence.

`empty_allocator` implements an `Allocator` that always fails.
`choice_allocator` implements an `Allocator` that composes an `Ownership_aware_allocator` with an `Allocator`. When allocating, it tries the first allocator, and if it fails it tries the second.
`static_allocator` implements a stateful `Allocator` that stores data in a locally allocated array. It can be queried with `available` for its remaining space. It only suppports deallocation of the last allocated `memory`.
`dynamic_allocator` implements a stateless `Allocator` that allocates using `allocate_dynamic`.
`affix_allocator` is an `Allocator` that stores another `Allocator` and adds the option to store a prefix and a postfix object to the allocated bytes. If any of the types are `void` no object is stored. By default no postfix is stored. `prefix` and `postfix` return a reference to the stored affix object.
`partition_allocator` implements an `Allocator` that stores two `Allocator`s and a `Pseudopredicate`. When allocating, it uses the first allocator if the pseudopredicate returns false and the second allocator if the pseudopredicate returns true.

A single instance `default_allocator` is defined. It can statically allocate up to 1KB and then dynamically allocates with suitable alignment for all built-in scalar types. To avoid exhausing the static buffer, deallocation needs to be done in reverse order of allocation.
A function `array_allocator` that returns a reference to `default_allocator` is also defined. It is used as the default allocator for all array data structures.

## Linear data structures

### Ranges

`bounded_range` implements a `Range` consisting of two `Loadable` cursors.

### Static sequences

`pair` implements a structure of two elements (that may differ in type) allocated on the stack. They can be accessed through the function `get`, either by cursor or by type. If the two elements
are of the same type, `pair` also provides a functor interface through the member function `.map`.

`array_k` implements an array of *k* elements contiguously allocated on the stack, like built-in C++ arrays, but with regular semantics, lexicographic comparison operators, and supporting functions and type functions for iteration and element access.
`array_k` is also a `Monad`.

### Dynamic sequences

#### Linked data structures

In linked data structures, the elements are stored in nodes that are permanently placed. During the lifetime of a linked data structure its nodes never move.

##### Lists

Lists have regular semantics, lexicographic comparison operators, and supporting functions and type functions for iteration and element access. Lists are also `Monad`s.

###### Singly-linked lists

Singly-linked lists provide a `Forward_cursor`.

`list_singly_linked_front` implements a singly-linked list that supports constant-time `insert` and `erase` at the front or after a given cursor. The header and cursor type are the size of a single pointer.

`list_singly_linked_front_back` implements a singly-linked list that supports constant-time `insert` and `erase` at the front or after a given cursor, and constant-time `insert` at the back. The header is the size of two pointers and the cursor type is the size of a single pointer.

`list_singly_linked_circular` implements a circular singly-linked list that supports constant-time
`insert` and `erase` at the front or after a given cursor, and constant-time `insert` at the back. The header is the size of a single pointer and the cursor type is the size of two pointers. Iteration is slower than for `list_singly_linked_front` and `list_singly_linked_front_back` because of special cases at the front and back of the list.

###### Doubly-linked lists

Doubly-linked lists support constant-time `insert` and `erase` at the front and back and constant-time `insert` before and after a given cursor.

`list_doubly_linked_circular` implements a circular doubly-linked list. It supports constant-time `erase` before and after a given cursor. The header is the size of a single pointer and the cursor type is the size of three pointers. Iteration is slower than for `list_doubly_linked_front_back`and `list_doubly_linked_sentinel` because of special cases at the front and back of the list. Construction of an empty list is cheaper than for `list_doubly_linked_sentinel`.

`list_doubly_linked_front_back` implements a doubly-linked list with both ends linked to its header. It supports constant-time `erase` at a given cursor. The header is the size of two pointers and the cursor type is the size of a single pointer. Construction of an empty list is cheaper than for `list_doubly_linked_sentinel`, but move construction, move assignment and `swap` is more expensive because of remote links to the header at the front and back of the list.

`list_doubly_linked_sentinel` implements a doubly-linked list with a sentinel node linked at both ends. It supports constant-time `erase` at a given cursor. The header and cursor type are the size of a single pointer. Iteration is faster than for `list_doubly_linked_circular` but construction of an empty list is more expensive as it requires allocation of the sentinel node.

#### Extent-based data structures

In extent-based data structures, the elements are stored in one or more *extents* that are allocated and deallocated on demand. During the lifetime of an extent-based data structure its elements may move.

##### Arrays

Arrays have regular semantics, lexicographic comparison operators, and supporting functions and type functions for iteration and element access.
Arrays are also `Monad`s.

`array_single_ended` implements an array of dynamically allocated elements. It stores a single pointer on the stack, keeping the array size and capacity in a header
to the array elements.
`array_single_ended` supports insertion at the back in amortized constant time using `emplace` and `push`. If the capacity is exceeded it reallocates and moves its elements.

`array_double_ended` implements an array of dynamically allocated elements. It stores a single pointer on the stack, keeping the array size and capacity in a header
to the array elements.
`array_double_ended` supports insertion at the back and the front in amortized constant time using `emplace`, `push`, `emplace_first`, and `push_first`. If the capacity is exceeded it reallocates and moves its elements.

`array_circular` implements an array of elements that are not necessarily contiguously allocated, as the elements are treated as if they may wrap around at the end of the reserved area.
It stores a single pointer on the stack, keeping the array size and capacity in a header to the array elements.
Cursors of `array_circular` elements are larger and element access is slower than for `array_single_ended` and `array_double_ended`.
`array_circular` supports insertion at the back and the front in amortized constant time using `emplace`, `push`, `emplace_first`, and `push_first`. If the capacity is exceeded it reallocates and moves its elements.

`array_segmented_single_ended` implements a segmented array of elements, where elements are dynamically allocated in multiple contiguously allocated blocks of a fixed size *k*, managed by an index of pointers, also dynamically allocated. All blocks in the array are full, except possibly the last one.
`array_segmented_single_ended` supports insertion at the back in amortized constant time using `push`. If the last block is full, a new block is allocated and appended to the index. Existing elements are never moved when new allocations occur. Erasure at the back using `pop` deallocates the last block if it becomes empty.

`array_segmented_double_ended` implements a segmented array of elements, where elements are dynamically allocated in multiple contiguously allocated blocks of a fixed size *k*, managed by an index of pointers, also dynamically allocated. All blocks in the array are full, except possibly the first and last one.
`array_segmented_double_ended` supports insertion at the front and at back in amortized constant time using `emplace`, `push`, `emplace_first`, and `push_first`. If the last block is full, a new block is allocated and appended to the index. Existing elements are never moved when new allocations occur. Erasure at the front and back and front using `pop_first` or `pop` deallocates the first and last block if they become empty.

### List pool

`list_pool` implements a pool of contiguously allocated singly linked elements. `allocate` inserts a new element after a given position and returns the position of the new element. `free` removes an element at a given position. free_pool removes all elements starting at a given position until the last reachable element.

## Sum types

`result` implements a type that carries either a value or an error. The presence of a value can be checked by boolean evaluation. An expected object is `Mutable` if its `Value_type` is.
`fail` constructs a `result` carrying an error.
`result` also provides a monadic interface through the member functions `.map` and `.flat_map`.

# Concepts

The concepts in this library are largely based on definitions in [StepanovMcJones](#StepanovMcJones), with some name changes and adaptations to modern C++ features, such as move semantics.

## Regular types

`Same_as` describes a pair of types that are the same.

`Convertible_to` describes a pair of types where the first is implicitly and explicitly convertible to the second.

`Boolean_testable` describes a type that is implicitly and explicitly convertible to `bool`.

`Equality_comparable` describes a type with equality comparison operators.
`Equality_comparable_with` describes two types with equality comparison operators defined between them.

`Destructible` describes a type with a destructor.
`Constructible` describes a type with a constructor.
`Default_initializable` describes a type for which the variable initialization `T t` is valid.
`Move_constructible` describes a type with a move constructor.
`Copy_constructible` describes a type with a copy constructor.
`Assignable` describes a type with assignment operator.
`Movable` describes a `Move_constructible` type with a move assignment operator.
`Copyable` describes a `Copy_constructible` type with a copy assignment operator.
`Semiregular` describes a type that is both `Default_constructible`and `Copyable`.
`Regular` describes a type that is `Semiregular` and `Equality_comparable`.
`axiom_Regular` checks the concept for one given value.
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

`Group` describes a `Monoid` with a unary `Operation` `inverse_operation` that when invoked returns the inverse of an object.
`Additive_group` describes an `Additive_monoid` where the inverse of an object is returned by the unary `operator-`, and subtraction is implemented by the binary `operator-`.
`axiom_Additive_group` checks the concept for three given values.
`Multiplicative_group` describes a `Multiplicative_monoid` where the inverse of an object is returned by inoking the unary `Operation` `reciprocal`, and division is implemented by `operator/`.
`axiom_Multiplicative_group` checks the concept for three given values.

### Ring-like

`Semiring` describes a type that is a `Monoid` in two operations `Add_op` and `Mul_op`, defaulted to the operators returned by the binary `Operation`s `add` and `multiply`, which unless specialized invoke `operator+` and `operator*`. `Add_op` is expected to be commutative, and `Mul_op` is expected to distribute over `Add_op`.
`axiom_Semiring` checks the concept for three given values.
`Commutative_semiring` descrivbes a `Semiring` where `Mul_op` is commutative.

`Ring` describes a `Semiring` where `Add_op` forms a `Group`.
`axiom_Ring` checks the concept for three given values.
`Commutative_ring` describes a `Ring` where `Mul_op` is commutative.

`Integral_domain` describes a `Commutative_ring` where there are no non-zero divisors.

`Field` describes an `Integral_domain` where `Mul_op` forms a group.

### Module-like

`Left_semimodule` describes a "vector" type that is a `Monoid` with a binary operation `V_add_op` defaulted to `add_op`, together with a "scalar" type that is a `Commutative_semiring` with two binary operations `S_add_op` and `S_mul_op` defaulted to `add`and `multiply`, and where an `operator*` with the scalar on the left side and the vector on the right side is defined, is commutative, distributive over `V_add_op`, compatible with `S_mul_op` and has an identity element `One`.
`Right_semimodule` describes a similar structure where `operator*` has the vector on the left side and the scalar on the right side.
`Semimodule` describes a pair of types that form both a `Left_semimodule` and a `Right_semimodule`.

`Left_module` describes a `Left_semimodule` where the "vector" type is a `Group` with `V_add_op` and where the "scalar" type is a `Ring` with `S_add_op` and `S_mul_op`.
`Right_module` describes a similar structure based on a `Right_semimodule`.
`Module` describes a pair of types that form both a `Left_module` and a `Right_module`.

### Linear algebra

`Vector_space` describes a `Module` where the "scalar" type is a `Field` and is defaulted to the `Value_type` of the "vector" type.

`Affine_space` describes a triple of types, representing a "point", a "vector", and a "scalar", where the vector and scalar form an `Affine_space` and the point is a `Regular` type where `operator+` and `operator-` are defined such that addition and subtraction of point and vector yields a point, and subtraction of point and point yields a vector.

## Functional concepts

`Functor` describes a `Movable` type with a related type constructor `functor_t`, that supports the type function `Constructor_type` and the function `fmap`, returning a new `Functor`.

`Monad` describes a type that is constructible from one value of its `Value_type` with the function `unit`, and where a value of the type and a unary `Invocable` that takes a value of the `Monad`'s `Value_type` can be composed with the function `chain`, returning an object of the same type.

## Integers

`Integral` describes a built-in binary integer type.

`Signed_integral` describes a built-in binary integer type.

`Unsigned_integral` describes a built-in binary integer type.

`Integer` describes a `Totally_ordered` integral type represented like a built-in machine type, with all its standard operators.

## Invocable concepts

`Invocable` describes a procedure that is not required to be equality-preserving.
`Regular_invocable` describes an equality-preserving `Invocable`.
`Operation` describes a `Regular_invocable` with arity > 0 and with the same domain and codomain.
`Predicate` describes a `Regular_invocable` where the codomain is `Boolean_testable`.
`Pseudopedicate` describes an `Invocable` where the codomain is `Boolean_testable`.
`Relation` describes a `Predicate` of arity 2 which accepts its arguments in any order.
`Transformation` describes a unary `Operation` with an `Integer` `Distance_type`.
`Action` describes a unary function that takes an object by non-constant reference. It is equivalent with a corresponding `Transformation` that returns a modified copy of the argument.
`Accumulation_procedure` describes a binary `Invocable` procedure that modifies its first argument, using itself and the second argument. It is equivalent with a corresponding binary `Operation`.
`Semigroup_accumulator` describes an associative `Accumulation_procedure`.
`Monoid_accumulator` describes a `Semigroup_accumulator` with an identity element, returned by the type function `Identity_element`.
`Group_accumulator` descrives a `Monoid_accumulator` with a unary `Operation` `inverse_operation` that when invoked returns the inverse of an object.

## Ordered algebraic concepts

`Ordered_semigroup` describes a `Semigroup` that is also `Totally_ordered`.
`Ordered_additive_semigroup` describes an `Additive_semigroup` that is also `Totally_ordered`.
`Ordered_monoid` describes a `Monoid` that is also `Totally_ordered`.
`Ordered_additive_monoid` describes an `Additive_monoid` that is also `Totally_ordered`.
`Ordered_group` describes a `Group` that is also `Totally_ordered`.
`Ordered_additive_group` describes an `Additive_group` that is also `Totally_ordered`.
`Cancellable_monoid` describes an `Ordered_monoid` that has a binary operation `cancel` that
implements cancellation.
`Archimedean_monoid` describes a `Cancellable_monoid` that has a `Quotient_type` that is an `Integral_domain`.
`Halvable_monoid` describes a `Archimedean_monoid` that has a `half` function.
`Archimedean_group` describes an `Archimedean_monoid` that is also a `Group`.
`Euclidean_semimodule` describes a `Semimodule` that has a `quotient` and `remainder` function, and where Euclid's algorithm terminates.
`Ordered_ring` describes a `Ring` that is also `Totally_ordered`.
`Ordered_integral_domain` describes an `Integral_domain` that is also `Totally_ordered`.

## Memory management

`Allocator` describes a `Semiregular` type managing blocks of raw memory. A function `allocate` that takes a reference to an `Allocator` and a desired number of `byte`s will try to allocate a contiguous sequence of `byte`s and return a `memory` descriptor to it. The caller must keep the descriptor around for deallocation when supported.
`Ownership_aware_allocator` describes an `Allocator` that supports the `Predicate` `is_owner`, which takes an `Allocator` and a `memory` block allocated from it, and checks if `allocate` was called with that `Allocator` to obtain it.

## Cursors

Cursors are types representing a point in a space, such as a number in a numeric type or a memory address to an element in a data structure.
When defined, cursors have successors and predecessors that can be accessed with the functions `increment` and `decrement`. The value that the cursor points to can often be accessed via the cursor using the functions `load` and `store`.

### Linear traversal

`Cursor` describes a `Movable` object representing a point in a space, with a function `increment` that moves it to the next point. Points are not necessarily visitable more than once.

`Forward_cursor` describes a `Regular` `Cursor` type where points may be visited multiple times.

`Bidirectional_cursor` describes a `Forward_cursor` type with a function `decrement` that moves it to the previous point. It is an `Affine_space` over its `Difference_type`.

#### Linear traversal with mutable successors

`Linked_forward_cursor` describes a `Forward_cursor` type with a function `next_link` that returns a reference to the next linked element.

`Forward_linker` describes a function object type that can be called with two `Linked_forward_cursor` objects and will link the first one to the second one.

`Linked_bidirectional_cursor` describes a `Bidirectional_cursor` type with functions `next_link` and `prev_link` that return a reference to the adjacent linked

`Bidirectional_linker` describes a function object type that can be called with two `Linked_bidirectional_cursor` objects and will link the first one to the second one.

#### Segmented traversal

`Segmented_cursor` describes a cursor in a segmented range, consisting of an index cursor and a segment cursor at the current index, both of which are `Cursor` types.

### Access

`Loadable` describes a type with a function `load` that returns a constant reference to its held object. For an object that represents the position of another object, `load` returns a reference to the other object. For other objects, load returns a reference to the object itself.

`Storable` describes a type with a function `store` that stores a given value at at a given position. If the given object does not represent a position, it stores the value at the position of the object itself.

`Mutable` describes a type that is both `Loadable` and `Storable`, and defines a function `at` that
returns either a reference or a constant reference to its held object.

### Ranges

`Limit` describes a `Cursor` type together with a type that can be used to check if the limit of the range into which the cursor points has been reached. The check is done with the binary `Predicate` `precedes`.

`Range` describes a type with two functions `first` and `limit`, returning the half-open range of cursors describing it.
`Mutable_range` describes a `Range` with a `Mutable` `Cursor_type`.

`Sequence` describes a `Range` that is `Totally_ordered`, and represents a composite object whose range of elements are its parts. It has functions `is_empty`. `size` and `operator[]` for element access.
`Dynamic_sequence` describes a `Sequence` that can change size at runtime. It has functions `insert` and `erase` to change elements at the back or the front of the range.

# Type functions

`Remove_const` is a type stripped of const and volatile qualifiers.

`Remove_ref` is a type stripped of reference qualifiers.

`Decay` is a type stripped of const, volatile, and reference qualifiers.

`Underlying_type` is the type describing the data storage of a given type.

`Element_type` is the type of an element in a `pair` with a given index.

`Error_type` is the type of the error that can be stored in a `result`.

## Algebra

`Scalar_type` is the associated scalar type of a `Module`-like object.

`Zero` is the additive identity element of a given type.

`One` is the multiplicative identity element of a given type.

## Invocable

`Return_type` is the return type of an `Invocable` type with a given list of argument types.

`Distance_type` is an `Integer` type large enough to count the number of invocations of a `Transformation`  or `Action` that is necessary to go from any return value to another.

## Cursors

`Value_type` is either a type directly, or, in the case of a `Cursor` type or a `Range` type, the type it is parameterized on.

`Pointer_type` is the type of a pointer to an object of a given type.

`Difference_type` is a type capable of storing the (signed) difference between two cursors of the given type.

`Cursor_type` is the type of an object describing a cursor to the given type.

`Size_type` is the type used to describe the size of an object of a given type.

`Size` is the size of a given type (when known at compile time).

`Index_cursor_type` and `Segment_cursor_type` describe the two `Cursor` types of a `Segment_cursor`

## Memory management

`Allocator_alignment` returns the alignment of a given `Allocator`.

## Integers

`Signed_type` returns a signed type of the same size as a given `Integral` type.

`Unsigned_type` returns an unsigned type of the same size as a given `Integral` type.

# Appendix A: On nomenclature

Concepts, types and functions in this library are named based on the following principles:

Consistency with the history and modern conventions of mathematics, in particular with computer science, is deemed more important than consistency with established names in popular programming languages. Consistency with popular programming languages is deemed more important than consistency with established names in C++.

Lexicographical ordering of names for closely related elements is deemed desirable, to facilitate searching. Multi-word names are separated by `_`.
Concepts and type functions are named with a leading upper case letter, while functions and types are all lower case.

Short names are deemed desirable, but abbreviations are generally avoided.

Index
-----

# Adapters

`cursor`
`forward_cursor`
`counted_cursor`
`loadable_cursor`
`reverse_cursor`

`filter_cursor`
`filter_sink`
`insert_cursor`
`insert_sink`
`map_cursor`
`map_sink`

# Algorithms

`power_left_associated`
`power_right_associated`
`power_semigroup`
`power_monoid`
`power_group`

`abs`
`cancel`
`quotient`
`remainder`
`quotient_remainder`
`gcd`

`choose`

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
`equivalent_lexicographical`
`equal_lexicographical`
`compare_lexicographical`
`less_lexicographical`
`equal_range`
`less_range`

`fill`

`count_if`
`count_if_not`
`count`
`count_not`

`copy`
`copy_select`
`copy_if`
`copy_if_not`

`map`

`reduce`
`reduce_nonempty`
`reduce_nonzeroes`
`reduce_balanced`

`flat_map`

`is_partitioned`
`partition_point_counted`
`partition_point`
`partition_semistable`
`partition_with_buffer`
`partition`

`search_binary_lower`
`search_binary_upper`
`search_binary`

`search`
`search_not`
`search_if`
`search_if_not`.

`search_backward`
`search_backward_not`
`search_backward_if`
`search_backward_if_not`

`search_unguarded`,
`search_not_unguarded`,
`search_if_unguarded`
`search_if_not_unguarded`

`search_guarded`,
`search_not_guarded`,
`search_if_guarded`
`search_if_not_guarded`

`search_match`
`search_mismatch`

`search_adjacent_match`
`search_adjacent_mismatch`

`reverse`
`rotate`

`for_each`
`for_each_result`

`each_of`
`any_not_of`
`none_of`
`any_of`

`zip`
`unzip`

`distance`
`collision_point`
`collision_point_nonterminating_orbit`
`is_terminating`
`is_circular`
`is_circular_nonterminating_orbit`
`connection_point`
`connection_point_nonterminating_orbit`
`orbit_structure`
`orbit_structure_nonterminating_orbit`

`contradiction`
`conjunction`
`nonimplication`
`projection_left`
`converse_nonimplication`
`projection_right`
`nonequivalence`
`disjunction`
`nondisjunction`
`equivalence`
`nonprojection_right`
`converse_implication`
`nonprojection_left`
`implication`
`nonconjunction`
`tautology`

# Data structures

`bit`
`affine_point`
`affine_vector`
`rational`
`polynomial`

`memory`
`static_allocator`
`dynamic_allocator`
`affix_allocator`
`partition_allocator`

`bounded_range`

`pair`
`array_k`

`list_singly_linked_front`
`list_singly_linked_front_back`
`list_singly_linked_circular`

`list_doubly_linked_circular`
`list_doubly_linked_front_back`
`list_doubly_linked_sentinel`

`array_single_ended`
`array_double_ended`
`array_circular`

`array_segmented_single_ended`
`array_segmented_double_ended`

`list_pool`

`result`

# Concepts

`Same_as`
`Equality_comparable`
`Equality_comparable_with`
`Destructible`
`Constructible`
`Default_initializable`
`Move_constructible`
`Copy_constructible`
`Assignable`
`Movable`
`Copyable`
`Semiregular`
`Regular`
`Totally_ordered`

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

`Integral`
`Signed_integral`
`Unsigned_integral`

`Invocable`
`Regular_invocable`
`Operation`
`Predicate`
`Relation`
`Transformation`
`Action`
`Accumulation_procedure`
`Semigroup_accumulator`
`Monoid_accumulator`
`Group_accumulator`

`Ordered_semigroup`
`Ordered_additive_semigroup`
`Ordered_monoid`
`Ordered_additive_monoid`
`Ordered_group`
`Ordered_additive_group`
`Cancellable_monoid`
`Archimedean_monoid`
`Halvable_monoid`
`Archimedean_group`
`Euclidean_semimodule`
`Ordered_ring`
`Ordered_integral_domain`

`Allocator`
`Ownership_aware_allocator`

`Cursor`
`Forward_cursor`
`Bidirectional_cursor`
`Linked_forward_cursor`
`Forward_linker`
`Linked_bidirectional_cursor`
`Bidirectional_linker`
`Loadable`
`Storable`
`Mutable`
`Limit`
`Range`
`Mutable_range`
`Sequence`
`Dynamic_sequence`

# Type functions

`Remove_const`
`Remove_ref`
`Decay`
`Underlying_type`
`Element_type`
`Error_type`

`Scalar_type`
`Zero`
`One`

`Return_type`
`Distance_type`

`Value_type`
`Pointer_type`
`Difference_type`
`Cursor_type`
`Size_type`
`Size`
`Index_cursor_type`
`Segment_cursor_type`

`Allocator_alignment`

`Signed_type`
`Unsigned_type`

References
----------

<a name="Knuth3"></a>
[Knuth3]
"The Art of Computer Programming, Volume 3, Sorting and Searching, Second Edition", ISBN 0-20189685-0

<a name="KnuthMorrisPratt"></a>
[KnuthMorrisPratt]
"Fast Pattern Matching in Strings, SIAM Journal on Computing, Volume 6, Issue 2, pp. 323-350"

<a name="StepanovMcJones"></a>
[StepanovMcJones]
"Elements of Programming", ISBN 0-321-63537-X
