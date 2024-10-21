#ifndef MATRIX_CONCEPT_H
#define MATRIX_CONCEPT_H

#include <concepts>

#include <iostream>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

template <typename T>
concept Multiplicationable = requires(T t) {
	{
		t* t
	} -> std::same_as<T>;
};
template <typename T>
concept AssignMultiplicationable = requires(T t) {
	{
		t *= t
	} -> std::same_as<T&>;
};

template <typename T, typename U>
concept MultipleAssignableDifferentType = requires(T t, U u) {
	{
		t *= u
	} -> std::same_as<T&>;
};

template <typename T, typename U>
concept MultiplableDifferentTypeReturnFirstType = requires(T t, U u) {
	{
		t* u
	} -> std::same_as<T>;
};

template <typename T, typename U>
concept MultiplableDifferentTypeReturnSecondType = requires(T t, U u) {
	{
		u* t
	} -> std::same_as<T>;
};

template <typename T, typename U>
concept MultiplableDifferentType = requires(T, U) {
	requires MultiplableDifferentTypeReturnSecondType<T, U> or MultiplableDifferentTypeReturnFirstType<T, U> or
			MultipleAssignableDifferentType<T, U>;
};

template <typename T>
concept Divisionable = requires(T t) {
	{
		t* t
	} -> std::same_as<T>;
};

template <typename T, typename U>
concept DivisionAssignableDifferentType = requires(T t, U u) {
	{
		t /= u
	} -> std::same_as<T&>;
};

template <typename T, typename U>
concept DivisionableDifferentTypeReturnFirstType = requires(T t, U u) {
	{
		t/ u
	} -> std::same_as<T>;
};

template <typename T, typename U>
concept DivisionableDifferentTypeReturnSecondType = requires(T t, U u) {
	{
		u/ t
	} -> std::same_as<T>;
};

template <typename T, typename U>
concept DivisionableDifferentType = requires(T, U) {
	requires DivisionableDifferentTypeReturnSecondType<T, U> or DivisionableDifferentTypeReturnFirstType<T, U> or
			DivisionAssignableDifferentType<T, U>;
};

template <typename T>
concept Sumable = requires(T t) {
	{
		t + t
	} -> std::same_as<T>;
};
template <typename T>
concept Symmetryable = requires(T t) {
	{
		-t
	} -> std::same_as<T>;
	{
		t * -1
	} -> std::same_as<T>;
	-t == t * -1;
};

template <typename T>
concept AssignSumable = requires(T t) {
	{
		t += t
	} -> std::same_as<T&>;
};

template <typename T, typename U>
concept SamAssignableDifferentType = requires(T t, U u) {
	{
		t += u
	} -> std::same_as<T&>;
};

template <typename T, typename U>
concept SumableDifferentTypeReturnFirstType = requires(T t, U u) {
	{
		t + u
	} -> std::same_as<T>;
};

template <typename T, typename U>
concept SumableDifferentTypeReturnSecondType = requires(T t, U u) {
	{
		u + t
	} -> std::same_as<T>;
};

template <typename T, typename U>
concept SumableDifferentType = requires(T, U) {
	requires SumableDifferentTypeReturnSecondType<T, U> or SumableDifferentTypeReturnFirstType<T, U> or
			SamAssignableDifferentType<T, U>;
};

template <typename Container, typename Element = typename Container::value_type>
concept Containerable = requires(Container c, Element e) {
	requires std::ranges::range<Container>;
	{
		c.size()
	} -> std::same_as<std::ranges::range_size_t<Container>>;
	typename Element;
};

template <typename Element>
concept Elementable = requires(Element e) {
	requires Multiplicationable<Element>;
	requires Divisionable<Element>;
	requires Sumable<Element>;
	requires Symmetryable<Element>;
};

template <typename Element>
concept IsMatrixableElement = requires(Element e) {
	requires Elementable<Element>;
	{
		std::to_string(e)
	} -> std::same_as<std::string>;
};

template <typename Matrix>
concept IsMatrixable = requires(Matrix m) {
	{
		m.get_number_of_row()
	} -> std::same_as<size_t>;
	{
		m.get_number_of_col()
	} -> std::same_as<size_t>;
	{
		m.get_table()
	};
};

template <typename Coefficientable>
concept Polynomialable = requires(Coefficientable first_coefficient, Coefficientable second_coefficient) {
	requires Elementable<Coefficientable>;
	requires MultiplableDifferentType<Coefficientable, int>;
	requires MultiplableDifferentType<Coefficientable, float>;
	{
		first_coefficient < 0.0
	} -> std::same_as<bool>;
	{
		first_coefficient == second_coefficient
	} -> std::same_as<bool>;
};

template <typename Element>
concept Numberable = std::is_arithmetic_v<Element>;

template <typename Element>
concept Integrable = std::is_integral_v<Element>;

template <typename Element, typename OtherElement>
concept AnotherElementMultiplable = requires() {
	requires MultiplableDifferentTypeReturnFirstType<Element, OtherElement> or
			(MultiplableDifferentTypeReturnSecondType<Element, OtherElement> and Polynomialable<OtherElement>);
};

#endif