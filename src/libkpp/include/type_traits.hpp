/*
	Copyright 2016 Matthew Fosdick

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
#ifndef __TYPE_TRAITS_HPP
#define __TYPE_TRAITS_HPP
#include <cstddef>
/*
NOTE: does not implement:
    is_function
    is_member_object_pointer
    is_member_function_pointer
    is_scaler
    is_object
    is_member_pointer
    is_constructible
    is_trivially_constructible
    is_nothrow_constructible
    is_default_constructible
    is_trivially_default_constructible
    is_nothrow_default_constructible
    is_copy_constructible
    is_trivially_copy_constructible
    is_nothrow_copy_constructible
    is_move_constructible
    is_trivially_move_constructible
    is_nothrow_move_constructible
    is_assignable
    is_trivially_assignable
    is_nothrow_assignable
    is_default_assignable
    is_trivially_default_assignable
    is_nothrow_default_assignable
    is_copy_assignable
    is_trivially_copy_assignable
    is_nothrow_copy_assignable
    is_move_assignable
    is_trivially_move_assignable
    is_nothrow_move_assignable
    is_destructible
    is_trivially_destructible
    is_nothrow_destructible
    add_pointer
    aligned_union
    decay
    common_type
    result_of
 */
namespace std {
template <class T, T v> struct integral_constant {
	static constexpr T value = v;
	typedef T value_type;
	typedef integral_constant type;
	constexpr operator value_type() const { return value; }
	constexpr value_type operator()() const { return value; }
};

template <bool B> using bool_constant = integral_constant<bool, B>;
struct true_type : bool_constant<true> {};
struct false_type : bool_constant<false> {};

template <class T> struct remove_const { typedef T type; };
template <class T> struct remove_const<const T> { typedef T type; };
template <class T> using remove_const_t = typename std::remove_const<T>::type;
template <class T> struct remove_volatile { typedef T type; };
template <class T> struct remove_volatile<volatile T> { typedef T type; };
template <class T> using remove_volatile_t = typename std::remove_volatile<T>::type;
template <class T> struct remove_cv {
	typedef typename std::remove_volatile<typename std::remove_const<T>::type>::type type;
};
template <class T> using remove_cv_t = typename std::remove_cv<T>::type;

template <class T, class U> struct is_same : std::false_type {};
template <class T> struct is_same<T, T> : std::true_type {};
#if __cplusplus >= 201500L
template <class T, class U> constexpr bool is_same_v = is_same<T, U>::value;
#endif
template <class T> struct is_void : std::is_same<void, typename std::remove_cv<T>::type> {};

template <class T> struct is_nullptr : std::is_same<nullptr_t, typename std::remove_cv<T>::type> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_null_pointer_v = is_nullptr<T>::value;
#endif

template <class T>
struct is_integral
    : std::bool_constant<
          std::is_same<bool, typename std::remove_cv<T>::type>::value ||
          std::is_same<char, typename std::remove_cv<T>::type>::value ||
          std::is_same<int, typename std::remove_cv<T>::type>::value ||
          std::is_same<long, typename std::remove_cv<T>::type>::value ||
          std::is_same<long long, typename std::remove_cv<T>::type>::value ||
          std::is_same<unsigned char, typename std::remove_cv<T>::type>::value ||
          std::is_same<unsigned int, typename std::remove_cv<T>::type>::value ||
          std::is_same<unsigned long, typename std::remove_cv<T>::type>::value ||
          std::is_same<unsigned long long, typename std::remove_cv<T>::type>::value> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_integral_v = is_integral<T>::value;
#endif
template <class T>
struct is_floating_point
    : std::bool_constant<std::is_same<float, typename std::remove_cv<T>::type>::value ||
                         std::is_same<double, typename std::remove_cv<T>::type>::value ||
                         std::is_same<long double, typename std::remove_cv<T>::type>::value> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_floating_point_v = is_floating_point<T>::value;
#endif

template <class T> struct is_array : std::false_type {};
template <class T> struct is_array<T[]> : std::true_type {};
template <class T, std::size_t N> struct is_array<T[N]> : std::true_type {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_array_v = is_array<T>::value;
#endif

template <class T> struct is_enum : std::bool_constant<__is_enum(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_enum_v = is_enum<T>::value;
#endif

template <class T> struct is_union : std::bool_constant<__is_union(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_union_v = is_union<T>::value;
#endif

template <class T> struct is_class : std::bool_constant<__is_class(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_class_v = is_class<T>::value;
#endif

template <class T> struct is_pointer_helper : std::false_type {};
template <class T> struct is_pointer_helper<T *> : std::true_type {};
template <class T> struct is_pointer : std::is_pointer_helper<typename std::remove_cv<T>::type> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_pointer_v = is_pointer<T>::value;
#endif

template <class T> struct is_lvalue_reference : std::false_type {};
template <class T> struct is_lvalue_reference<T &> : std::true_type {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;
#endif

template <class T> struct is_rvalue_reference : std::false_type {};
template <class T> struct is_rvalue_reference<T &&> : std::true_type {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;
#endif

template <class T> struct is_member_pointer_helper : std::false_type {};
template <class T, class U> struct is_member_pointer_helper<T U::*> : std::true_type {};
template <class T>
struct is_member_pointer : is_member_pointer_helper<typename std::remove_cv<T>::type> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_member_pointer_v = is_member_pointer<T>::value;
#endif

template <class T>
struct is_arithmetic
    : std::bool_constant<std::is_integral<T>::value || std::is_floating_point<T>::value> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
#endif

template <class T>
struct is_fundamental
    : std::bool_constant<std::is_arithmetic<T>::value || std::is_void<T>::value ||
                         std::is_same<nullptr_t, typename std::remove_cv<T>::type>::value> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_fundamental_v = is_fundamental<T>::value;
#endif

template <class T> struct is_compound : std::bool_constant<!std::is_fundamental<T>::value> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_compound_v = is_compound<T>::value;
#endif

template <class T> struct is_reference : std::false_type {};
template <class T> struct is_reference<T &> : std::true_type {};
template <class T> struct is_reference<T &&> : std::true_type {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_reference_v = is_reference<T>::value;
#endif

template <class T> struct is_const : std::false_type {};
template <class T> struct is_const<const T> : std::true_type {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_const_v = is_const<T>::value;
#endif

template <class T> struct is_volatile : std::false_type {};
template <class T> struct is_volatile<volatile T> : std::true_type {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_volatile_v = is_volatile<T>::value;
#endif

template <class T> struct is_trivial : std::bool_constant<__is_trivial(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_trivial_v = is_trivial<T>::value;
#endif

template <class T> struct is_trivial_copyable : std::bool_constant<__has_trivial_copy(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_trivial_copyable_v = is_trivial_copyable<T>::value;
#endif

template <class T> struct is_standard_layout : std::bool_constant<__is_standard_layout(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_standard_layout_v = is_standard_layout<T>::value;
#endif

template <class T> struct is_pod : std::bool_constant<__is_pod(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_pod_v = is_pod<T>::value;
#endif

template <class T> struct is_literal_type : std::bool_constant<__is_literal_type(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_literal_type_v = is_literal_type<T>::value;
#endif

template <class T> struct is_empty : std::bool_constant<__is_empty(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_empty_v = is_empty<T>::value;
#endif

template <class T> struct is_polymorphic : std::bool_constant<__is_polymorphic(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_polymorphic_v = is_polymorphic<T>::value;
#endif

template <class T> struct is_abstract : std::bool_constant<__is_abstract(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_abstract_v = is_abstract<T>::value;
#endif

namespace detail {
template <typename T, bool = std::is_arithmetic<T>::value>
struct is_signed : std::bool_constant<T(-1) < T(0)> {};
template <typename T> struct is_signed<T, false> : std::false_type {};

template <typename T, bool = std::is_arithmetic<T>::value>
struct is_unsigned : std::bool_constant<T(0) < T(-1)> {};
template <typename T> struct is_unsigned<T, false> : std::false_type {};
}
template <typename T> struct is_signed : detail::is_signed<T>::type {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_signed_v = is_signed<T>::value;
#endif
template <typename T> struct is_unsigned : detail::is_unsigned<T>::type {};
#if __cplusplus >= 201500L
template <class T> constexpr bool is_unsigned_v = is_unsigned<T>::value;
#endif

template <class T>
struct has_virtual_destructor : std::bool_constant<__has_virtual_destructor(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;
#endif

template <class T> struct alignment_of : std::integral_constant<std::size_t, alignof(T)> {};
#if __cplusplus >= 201500L
template <class T> constexpr bool alignment_of_v = alignment_of<T>::value;
#endif

template <class T> struct rank : public std::integral_constant<std::size_t, 0> {};
template <class T>
struct rank<T[]> : public std::integral_constant<std::size_t, rank<T>::value + 1> {};
template <class T, std::size_t N>
struct rank<T[N]> : public std::integral_constant<std::size_t, rank<T>::value + 1> {};

#if __cplusplus >= 201500L
template <class T> constexpr std::size_t rank_v = rank<T>::value;
#endif

template <class T, unsigned N = 0> struct extent : std::integral_constant<std::size_t, 0> {};
template <class T> struct extent<T[], 0> : std::integral_constant<std::size_t, 0> {};
template <class T, unsigned N>
struct extent<T[], N> : std::integral_constant<std::size_t, std::extent<T, N - 1>::value> {};
template <class T, std::size_t N>
struct extent<T[N], 0> : std::integral_constant<std::size_t, N> {};
template <class T, std::size_t I, unsigned N>
struct extent<T[I], N> : std::integral_constant<std::size_t, std::extent<T, N - 1>::value> {};
#if __cplusplus >= 201500L
template <class T, unsigned N = 0> constexpr std::size_t extent_v = extent<T, N>::value;
#endif

template <class T, class U> struct is_base_of : std::bool_constant<__is_base_of(T, U)> {};
#if __cplusplus >= 201500L
template <class T, class U> constexpr bool is_base_of_v = is_base_of<T, U>::value;
#endif

template <class T, class U> struct is_convertible : std::bool_constant<__is_convertible(T, U)> {};
#if __cplusplus >= 201500L
template <class T, class U> constexpr bool is_convertible_v = is_convertible<T, U>::value;
#endif

template <class T> struct add_const { typedef const T type; };
template <class T> struct add_volatile { typedef volatile T type; };
template <class T> struct add_cv {
	typedef typename std::add_volatile<typename std::add_const<T>::type>::type type;
};
template <class T> using add_cv_t = typename add_cv<T>::type;
template <class T> using add_const_t = typename add_const<T>::type;
template <class T> using add_volatile_t = typename add_volatile<T>::type;

template <class T> struct remove_reference { typedef T type; };
template <class T> struct remove_reference<T &> { typedef T type; };
template <class T> struct remove_reference<T &&> { typedef T type; };
template <class T> using remove_reference_t = typename remove_reference<T>::type;

template <class T> struct add_lvalue_reference { typedef T &type; };
template <class T> struct add_lvalue_reference<T &> { typedef T &type; };
template <class T> struct add_lvalue_reference<T &&> { typedef T &&type; };
template <class T> struct add_rvalue_reference { typedef T &&type; };
template <class T> struct add_rvalue_reference<T &> { typedef T &type; };
template <class T> struct add_rvalue_reference<T &&> { typedef T &&type; };
template <class T> using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
template <class T> using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template <class T> struct remove_pointer { typedef T type; };
template <class T> struct remove_pointer<T *> { typedef T type; };
template <class T> struct remove_pointer<T *const> { typedef T type; };
template <class T> struct remove_pointer<T *volatile> { typedef T type; };
template <class T> struct remove_pointer<T *const volatile> { typedef T type; };
template <class T> using remove_pointer_t = typename remove_pointer<T>::type;

template <class T> struct make_signed { typedef T type; };
template <> struct make_signed<unsigned char> { typedef char type; };
template <> struct make_signed<unsigned short> { typedef short type; };
template <> struct make_signed<unsigned int> { typedef int type; };
template <> struct make_signed<unsigned long> { typedef long type; };
template <> struct make_signed<unsigned long long> { typedef long long type; };
template <> struct make_signed<unsigned const char> { typedef const char type; };
template <> struct make_signed<unsigned const short> { typedef const short type; };
template <> struct make_signed<unsigned const int> { typedef const int type; };
template <> struct make_signed<unsigned const long> { typedef const long type; };
template <> struct make_signed<unsigned const long long> { typedef const long long type; };
template <> struct make_signed<unsigned volatile char> { typedef volatile char type; };
template <> struct make_signed<unsigned volatile short> { typedef volatile short type; };
template <> struct make_signed<unsigned volatile int> { typedef volatile int type; };
template <> struct make_signed<unsigned volatile long> { typedef volatile long type; };
template <> struct make_signed<unsigned volatile long long> { typedef volatile long long type; };
template <> struct make_signed<unsigned const volatile char> { typedef const volatile char type; };
template <> struct make_signed<unsigned const volatile short> {
	typedef const volatile short type;
};
template <> struct make_signed<unsigned const volatile int> { typedef const volatile int type; };
template <> struct make_signed<unsigned const volatile long> { typedef const volatile long type; };
template <> struct make_signed<unsigned const volatile long long> {
	typedef const volatile long long type;
};
template <class T> using make_signed_t = typename make_signed<T>::type;

template <class T> struct make_unsigned { typedef T type; };
template <> struct make_unsigned<char> { typedef unsigned char type; };
template <> struct make_unsigned<short> { typedef unsigned short type; };
template <> struct make_unsigned<int> { typedef unsigned int type; };
template <> struct make_unsigned<long> { typedef unsigned long type; };
template <> struct make_unsigned<long long> { typedef unsigned long long type; };
template <> struct make_unsigned<const char> { typedef unsigned const char type; };
template <> struct make_unsigned<const short> { typedef unsigned const short type; };
template <> struct make_unsigned<const int> { typedef unsigned const int type; };
template <> struct make_unsigned<const long> { typedef unsigned const long type; };
template <> struct make_unsigned<const long long> { typedef unsigned const long long type; };
template <> struct make_unsigned<volatile char> { typedef unsigned volatile char type; };
template <> struct make_unsigned<volatile short> { typedef unsigned volatile short type; };
template <> struct make_unsigned<volatile int> { typedef unsigned volatile int type; };
template <> struct make_unsigned<volatile long> { typedef unsigned volatile long type; };
template <> struct make_unsigned<volatile long long> { typedef unsigned volatile long long type; };
template <> struct make_unsigned<const volatile char> {
	typedef unsigned const volatile char type;
};
template <> struct make_unsigned<const volatile short> {
	typedef unsigned const volatile short type;
};
template <> struct make_unsigned<const volatile int> { typedef unsigned const volatile int type; };
template <> struct make_unsigned<const volatile long> {
	typedef unsigned const volatile long type;
};
template <> struct make_unsigned<const volatile long long> {
	typedef unsigned const volatile long long type;
};
template <class T> using make_unsigned_t = typename make_unsigned<T>::type;

template <class T> struct remove_extent { typedef T type; };
template <class T> struct remove_extent<T[]> { typedef T type; };
template <class T, std::size_t N> struct remove_extent<T[N]> { typedef T type; };
template <class T> using remove_extent_t = typename remove_extent<T>::type;

template <class T> struct remove_all_extents { typedef T type; };
template <class T> struct remove_all_extents<T[]> {
	typedef typename remove_all_extents<T>::type type;
};
template <class T, std::size_t N> struct remove_all_extents<T[N]> {
	typedef typename remove_all_extents<T>::type type;
};
template <class T> using remove_all_extents_t = typename remove_all_extents<T>::type;

template <std::size_t Len, std::size_t Align = 8> struct aligned_storage {
	typedef struct { alignas(Align) unsigned char data[Len]; } type;
};
template <std::size_t Len, std::size_t Align = 8>
using aligned_storage_t = typename aligned_storage<Len, Align>::type;

template <bool B, class T = void> struct enable_if {};
template <class T> struct enable_if<true, T> { typedef T type; };
template <bool B, class T = void> using enable_if_t = typename enable_if<B, T>::type;

template <bool B, class T, class F> struct conditional { typedef T type; };
template <class T, class F> struct conditional<false, T, F> { typedef F type; };
template <bool B, class T, class F> using conditional_t = typename conditional<B, T, F>::type;

template <class T> struct underlying_type { typedef __underlying_type(T) type; };
template <class T> using underlying_type_t = typename underlying_type<T>::type;
}
#endif
