#pragma once

#include "ordered_algebra.h"
#include "cursor.h"

namespace elements {

struct memory
{
    Pointer_type<byte> first;
    Difference_type<Pointer_type<byte>> size;

    constexpr
    operator bool() const
    {
        return first != nullptr;
    }
};

template <>
struct size_type_t<memory>
{
    using type = Difference_type<Pointer_type<byte>>;
};

constexpr auto
operator==(memory const& x, memory const& y) -> bool
{
    return x.first == y.first and x.size == y.size;
}

template <typename A>
concept Allocator =
    Semiregular<A> and
    requires (A& a, Size_type<memory> n) {
        { allocate(a, n) } -> Same_as<memory>;
    };

template <typename A>
concept Ownership_aware_allocator =
    Allocator<A> and
    requires (A const& a, memory const& x) {
        { is_owner(a, x) } -> Boolean_testable;
    };

template <typename>
struct allocator_t;

template <typename T>
struct allocator_t
{
    static_assert(Allocator<T>);
    static constexpr N<16> alignment = Max_integral<N<16>>;
};

template <Allocator A>
constexpr auto Allocator_alignment = allocator_t<A>::alignment;

template <Allocator A>
constexpr auto
good_size(A const&, Size_type<memory> n) -> Size_type<memory>
{
    return round_up_to_multiple(n, Size_type<memory>{Allocator_alignment<A>});
}

template <Allocator A>
constexpr auto Is_expandable = requires (A& a, memory& x, Size_type<memory> n) { { expand(a, x, n) } -> Same_as<bool>; };

template <Allocator A>
constexpr auto Is_deallocatable = requires (A& a, memory const& x) { { deallocate(a, x) } -> Same_as<void>; };

template <Allocator A>
constexpr auto Is_erasable = requires (A& a) { { deallocate_all(a) } -> Same_as<void>; };

template <Allocator A>
constexpr auto
reallocate(A& a, memory& x, Size_type<memory> n) -> bool
{
    if (x.size == n) return true;
    if constexpr (Is_expandable<A>) {
        if (x.size < n and expand(a, x, n - x.size)) return true;
    }
    auto y{allocate(a, n)};
    copy(x.first, x.first + min(x.size, y.size), y.first);
    if constexpr (Is_deallocatable<A>) {
        deallocate(a, x);
    }
    x = y;
    return true;
}

template <Allocator A0, Allocator A1>
constexpr auto
allocator_move(A0& src, A1& dst, memory& x, Size_type<memory> n) -> bool
{
    auto y{allocate(dst, n)};
    if (!y) return false;
    copy(x.first, x.first + min(x.size, n), y.first);
    if constexpr (Is_deallocatable<A0>) {
        deallocate(src, x);
    }
    x = y;
    return true;
}

struct empty_allocator
{
};

template <>
struct allocator_t<empty_allocator>
{
    static constexpr N<16> alignment = 32768;
};

constexpr auto
operator==(empty_allocator const&, empty_allocator const&) -> bool
{
    return true;
}

constexpr auto
good_size(empty_allocator const&, Size_type<memory>) -> Size_type<memory>
{
    return 0;
}

constexpr auto
allocate(empty_allocator&, Size_type<memory>) -> memory
{
    return {nullptr, 0};
}

constexpr auto
expand(empty_allocator&, memory&, Size_type<memory>) -> bool
{
    return false;
}

constexpr auto
reallocate(empty_allocator&, memory&, Size_type<memory>) -> bool
{
    return false;
}

constexpr void
deallocate(empty_allocator&, memory const&)
{}

template <Ownership_aware_allocator A0, Allocator A1>
struct choice_allocator
{
    A0 a0;
    A1 a1;
};

template <Ownership_aware_allocator A0, Allocator A1>
struct allocator_t<choice_allocator<A0, A1>>
{
    static constexpr N<16> alignment = min(Allocator_alignment<A0>, Allocator_alignment<A1>);
};

template <Ownership_aware_allocator A0, Allocator A1>
constexpr auto
operator==(choice_allocator<A0, A1> const& x, choice_allocator<A0, A1> const& y) -> bool
{
    return x.a0 == y.a0 and x.a1 == y.a1;
}

template <Ownership_aware_allocator A0, Allocator A1>
constexpr auto
allocate(choice_allocator<A0, A1>& a, Size_type<memory> n) -> memory
{
    memory x{allocate(a.a0, n)};
    if (x) {
        return x;
    } else {
        return allocate(a.a1, n);
    }
}

template <Ownership_aware_allocator A0, Allocator A1>
constexpr void
deallocate(choice_allocator<A0, A1>& a, memory const& x)
{
    if (is_owner(a.a0, x)) {
        if constexpr (Is_deallocatable<A0>) {
            deallocate(a.a0, x);
        }
    } else {
        if constexpr (Is_deallocatable<A1>) {
            deallocate(a.a1, x);
        }
    }
}

template <Ownership_aware_allocator A0, Ownership_aware_allocator A1>
constexpr auto
is_owner(choice_allocator<A0, A1> const& a, memory const& x) -> bool
{
    return is_owner(a.a0, x) or is_owner(a.a1, x);
}

template <Ownership_aware_allocator A0, Allocator A1>
requires Is_expandable<A0> or Is_expandable<A1>
constexpr auto
expand(choice_allocator<A0, A1>& a, memory& x, Size_type<memory> n) -> bool
{
    if (is_owner(a.a0, x)) {
        if constexpr (Is_expandable<A0>) {
            return expand(a.a0, x, n);
        } else {
            return false;
        }
    } else {
        if constexpr (Is_expandable<A1>) {
            return expand(a.a1, x, n);
        } else {
            return false;
        }
    }
}

template <Ownership_aware_allocator A0, Allocator A1>
constexpr auto
reallocate(choice_allocator<A0, A1>& a, memory& x, Size_type<memory> n) -> bool
{
    if (is_owner(a.a0, x)) {
        if (reallocate(a.a0, x, n)) {
            return true;
        } else {
            return allocator_move(a.a0, a.a1, x, n);
        }
    } else {
        if (reallocate(a.a1, x, n)) {
            return true;
        } else {
            return allocator_move(a.a1, a.a0, x, n);
        }
    }
}

template <Size_type<memory> size, N<16> min_alignment = alignof(double)>
requires (size != 0 and min_alignment != 0 and size >= min_alignment)
struct static_allocator
{
    Size_type<memory> free_size{size};
    alignas(min_alignment) byte stack[Unsigned_type<Size_type<memory>>{size}];
};

template <Size_type<memory> size, N<16> min_alignment>
struct allocator_t<static_allocator<size, min_alignment>>
{
    static constexpr N<16> alignment = min_alignment;
};

template <Size_type<memory> size0, N<16> min_alignment0, Size_type<memory> size1, N<16> min_alignment1>
constexpr auto
operator==(static_allocator<size0, min_alignment0> const&, static_allocator<size1, min_alignment1> const&) -> bool
{
    return size0 == size1 and min_alignment0 == min_alignment1;
}

template <Size_type<memory> size, N<16> min_alignment>
constexpr auto
allocate(static_allocator<size, min_alignment>& a, Size_type<memory> n) -> memory
{
    auto alloc_size{good_size(a, n)};
    if (a.free_size < alloc_size) {
        return {nullptr, 0};
    } else {
        auto cur{a.stack + size - a.free_size};
        a.free_size = a.free_size - alloc_size;
        return {cur, n};
    }
}

template <Size_type<memory> size, N<16> min_alignment>
constexpr void
deallocate(static_allocator<size, min_alignment>& a, memory const& x)
{
    auto alloc_size{good_size(a, x.size)};
    if (x.first + alloc_size == a.stack + size - a.free_size) {
        a.free_size = a.free_size + alloc_size;
    }
}

template <Size_type<memory> size, N<16> min_alignment>
constexpr auto
is_owner(static_allocator<size, min_alignment> const& a, memory const& x) -> bool
//[[require: x.first != nullptr]]
{
    return (
        ge_address(reinterpret_cast<Pointer_type<void const>>(x.first), reinterpret_cast<Pointer_type<void const>>(a.stack)) and
        lt_address(reinterpret_cast<Pointer_type<void const>>(x.first), reinterpret_cast<Pointer_type<void const>>(a.stack + size))
    );
}

template <Size_type<memory> size, N<16> min_alignment>
constexpr auto
allocate_all(static_allocator<size, min_alignment>& a) -> memory
{
    a.free_size = 0;
    return {a.stack, size};
}

template <Size_type<memory> size, N<16> min_alignment>
constexpr void
deallocate_all(static_allocator<size, min_alignment>& a)
{
    a.free_size = size;
}

template <Size_type<memory> size, N<16> min_alignment>
constexpr auto
available(static_allocator<size, min_alignment>& a) -> Size_type<memory>
{
    return a.free_size;
}

struct dynamic_allocator
{
};

template <>
struct allocator_t<dynamic_allocator>
{
    static constexpr N<16> alignment = alignof(double);
};

constexpr auto
operator==(dynamic_allocator const&, dynamic_allocator const&) -> bool
{
    return true;
}

constexpr auto
allocate(dynamic_allocator&, Size_type<memory> n) -> memory
{
    if (n == 0) return {nullptr, 0};
    return {reinterpret_cast<Pointer_type<byte>>(allocate_dynamic(static_cast<Unsigned_type<Size_type<memory>>>(n))), n};
}

constexpr auto
reallocate(dynamic_allocator&, memory& x, Size_type<memory> n) -> memory
{
    if (n == 0) return {nullptr, 0};
    return {reinterpret_cast<Pointer_type<byte>>(reallocate_dynamic(x.first, static_cast<Unsigned_type<Size_type<memory>>>(n))), n};
}

constexpr void
deallocate(dynamic_allocator&, memory const& x)
{
    deallocate_dynamic(x.first);
}

template <typename Prefix, Allocator A, typename Suffix = void>
requires (sizeof(Prefix) or Allocator_alignment<A> >= alignof(Prefix))
struct affix_allocator
{
    A underlying_allocator;
};

template <typename Prefix, Allocator A, typename Suffix>
struct allocator_t<affix_allocator<Prefix, A, Suffix>>
{
    static constexpr auto alignment = Allocator_alignment<A>;
};

template <typename Prefix, Allocator A, typename Suffix>
constexpr auto
operator==(affix_allocator<Prefix, A, Suffix> const& x, affix_allocator<Prefix, A, Suffix> const& y)
{
    return x.underlying_allocator == y.underlying_allocator;
}

template <typename Prefix, Allocator A, typename Suffix>
constexpr auto
underlying_allocator_size(affix_allocator<Prefix, A, Suffix> const&, Size_type<memory> n) -> Size_type<memory>
{
    if constexpr (Same_as<Suffix, void>) {
        return Size_type<memory>{sizeof(Prefix)} + n;
    } else {
        return round_up_to_multiple(Size_type<memory>{sizeof(Prefix)} + n, Size_type<memory>{alignof(Suffix)}) + Size_type<memory>{sizeof(Suffix)};
    }
}

template <typename Prefix, Allocator A, typename Suffix>
constexpr auto
underlying_allocator_allocation(affix_allocator<Prefix, A, Suffix>& a, memory const& x) -> memory
{
    return {x.first - sizeof(Prefix), underlying_allocator_size(a, x.size)};
}

template <typename Prefix, Allocator A, typename Suffix>
constexpr auto
good_size(affix_allocator<Prefix, A, Suffix> const& a, Size_type<memory> n) -> Size_type<memory>
{
    return good_size(a.underlying_allocator, underlying_allocator_size(a, n));
}

template <typename Prefix, Allocator A, typename Suffix>
constexpr auto
allocate(affix_allocator<Prefix, A, Suffix>& a, Size_type<memory> n) -> memory
{
    memory x{allocate(a.underlying_allocator, underlying_allocator_size(a, n))};
    if (x) {
        if constexpr (Default_initializable<Prefix>) {
            construct_at<Prefix>(reinterpret_cast<Pointer_type<Prefix>>(x.first));
        }
        if constexpr (Default_initializable<Suffix>) {
            construct_at<Suffix>(reinterpret_cast<Pointer_type<Suffix>>(x.first + x.size - sizeof(Suffix)));
        }
        return {x.first + sizeof(Prefix), n};
    } else {
        return {nullptr, 0};
    }
}

template <typename Prefix, Allocator A>
requires Is_expandable<A>
constexpr auto
expand(affix_allocator<Prefix, A, void>& a, memory& x, Size_type<memory> n) -> bool
{
    auto y{underlying_allocator_allocation(a, x)};
    if (!expand(a.underlying_allocator, y, n)) return false;
    x.size = x.size + n;
    return true;
}

template <typename Prefix, Allocator A, typename Suffix>
constexpr auto
reallocate(affix_allocator<Prefix, A, Suffix>& a, memory& x, Size_type<memory> n) -> bool
{
    auto y{underlying_allocator_allocation(a, x)};
    if (!reallocate(a.underlying_allocator, y, underlying_allocator_size(a, n))) return false;
    x = {y.first + sizeof(Prefix), n};
    return true;
}

template <typename Prefix, Allocator A, typename Suffix>
requires Is_deallocatable<A>
constexpr void
deallocate(affix_allocator<Prefix, A, Suffix>& a, memory const& x)
{
    if constexpr (Destructible<Suffix>) {
        destroy_at<Suffix>(reinterpret_cast<Pointer_type<Suffix>>(x.first + x.size - sizeof(Suffix)));
    }
    if constexpr (Destructible<Prefix>) {
        destroy_at<Prefix>(reinterpret_cast<Pointer_type<Prefix>>(x.first));
    }
    auto y{underlying_allocator_allocation(a, x)};
    deallocate(a.underlying_allocator, y);
}

template <typename Prefix, Allocator A, typename Suffix>
requires Is_erasable<A>
constexpr void
deallocate_all(affix_allocator<Prefix, A, Suffix>& a)
{
    deallocate_all(a.underlying_allocator);
}

template <typename Prefix, Ownership_aware_allocator A, typename Suffix>
constexpr auto
is_owner(affix_allocator<Prefix, A, Suffix> const& a, memory const& x) -> bool
{
    if (!x) {
        return true;
    } else {
        return is_owner(a.underlying_allocator, parent_allocation(a, x));
    }
}

template <typename Prefix, Allocator A, typename Suffix>
requires (!Same_as<Prefix, void>)
constexpr auto
prefix(affix_allocator<Prefix, A, Suffix>&, memory const& x) -> Prefix&
{
    return at(reinterpret_cast<Pointer_type<Prefix>>(x.first - sizeof(Prefix)));
}

template <typename Prefix, Allocator A, typename Suffix>
requires (!Same_as<Prefix, void>)
constexpr auto
prefix(affix_allocator<Prefix, A, Suffix> const& a, memory const& x) -> Prefix const&
{
    return load(reinterpret_cast<Pointer_type<Prefix>>(x.first - sizeof(Prefix)));
}

template <typename Prefix, Allocator A, typename Suffix>
requires (!Same_as<Suffix, void>)
constexpr auto
suffix(affix_allocator<Prefix, A, Suffix>& a) -> Prefix&
{
    return at(reinterpret_cast<Pointer_type<Suffix>>(a.parent.first + underlying_allocator_size(a, a.parent.size) - sizeof(Suffix)));
}

template <typename Prefix, Allocator A, typename Suffix>
requires (!Same_as<Suffix, void>)
constexpr auto
suffix(affix_allocator<Prefix, A, Suffix> const& a) -> Prefix const&
{
    return load(reinterpret_cast<Pointer_type<Suffix>>(a.parent.first + underlying_allocator_size(a, a.parent.size) - sizeof(Suffix)));
}

template <Allocator A0, Allocator A1, Pseudopredicate<Size_type<memory>> P>
requires (Allocator_alignment<A0> == Allocator_alignment<A1>)
struct partition_allocator
{
    A0 a0;
    A1 a1;
    P pred;
};

template <Allocator A0, Allocator A1, Pseudopredicate<Size_type<memory>> P>
requires (Allocator_alignment<A0> == Allocator_alignment<A1>)
struct allocator_t<partition_allocator<A0, A1, P>>
{
    static constexpr auto alignment = Allocator_alignment<A0>;
};

template <Allocator A0, Allocator A1, Pseudopredicate<Size_type<memory>> P>
requires (Allocator_alignment<A0> == Allocator_alignment<A1>)
constexpr auto
operator==(partition_allocator<A0, A1, P> const& x, partition_allocator<A0, A1, P> const& y)
{
    return x.a0 == y.a0 and x.a1 == y.a1 and x.pred == y.pred;
}

template <Allocator A0, Allocator A1, Pseudopredicate<Size_type<memory>> P>
constexpr auto
good_size(partition_allocator<A0, A1, P> const& a, Size_type<memory> n) -> Size_type<memory>
{
    if (a.pred(n)) {
        return good_size(a.a1, n);
    } else {
        return good_size(a.a0, n);
    }
}

template <Allocator A0, Allocator A1, Pseudopredicate<Size_type<memory>> P>
constexpr auto
allocate(partition_allocator<A0, A1, P>& a, Size_type<memory> n) -> memory
{
    if (a.pred(n)) {
        return allocate(a.a1, n);
    } else {
        return allocate(a.a0, n);
    }
}

template <Allocator A0, Allocator A1, Pseudopredicate<Size_type<memory>> P>
requires Is_expandable<A1> or Is_expandable<A0>
constexpr auto
expand(partition_allocator<A0, A1, P>& a, memory& x, Size_type<memory> n) -> bool
{
    if (a.pred(x.size + n)) {
        if constexpr(Is_expandable<A1>) {
            return expand(a.a1, x, n);
        } else {
            return false;
        }
    }
    if (!a.pred(x.size)) {
        if constexpr(Is_expandable<A0>) {
            return expand(a.a0, x, n);
        } else {
            return false;
        }
    }
    return false;
}

template <Allocator A0, Allocator A1, Pseudopredicate<Size_type<memory>> P>
constexpr auto
reallocate(partition_allocator<A0, A1, P>& a, memory& x, Size_type<memory> n) -> bool
{
    if (a.pred(x.size) and a.pred(n)) return reallocate(a.a1, x, n);
    if (!a.pred(x.size) and !a.pred(n)) return reallocate(a.a0, x, n);
    return reallocate(a, x, n);
}

template <Allocator A0, Allocator A1, Pseudopredicate<Size_type<memory>> P>
requires Is_deallocatable<A0> and Is_deallocatable<A1>
constexpr void
deallocate(partition_allocator<A0, A1, P>& a, memory const& x)
{
    if (a.pred(x.size)) {
        deallocate(a.a1, x);
    } else {
        deallocate(a.a0, x);
    }
}

template <Allocator A0, Allocator A1, Pseudopredicate<Size_type<memory>> P>
requires Is_erasable<A0> and Is_erasable<A1>
constexpr void
deallocate_all(partition_allocator<A0, A1, P>& a)
{
    deallocate_all(a.a0);
    deallocate_all(a.a1);
}

template <Ownership_aware_allocator A0, Ownership_aware_allocator A1, Pseudopredicate<Size_type<memory>> P>
constexpr auto
is_owner(partition_allocator<A0, A1, P> const& a, memory const& x) -> bool
{
    if (a.pred(x.size)) {
        return is_owner(a.a1);
    } else {
        return is_owner(a.a0);
    }
}

inline auto default_allocator = choice_allocator{static_allocator<1024>{}, dynamic_allocator{}};

template <typename T>
constexpr auto array_allocator = []() -> Allocator auto& { return default_allocator; };

}
