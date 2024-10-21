#ifndef MATRIX_POLYNOMIAL_H
#define MATRIX_POLYNOMIAL_H

#include <cinttypes>

#include <numeric>
#include <vector>

#include "concept.h"

template <Polynomialable Element>
class Polynomial
{
private:
	using Coefficient = std::vector<Element>;
	using NewtonOutput = std::pair<double, bool>;
	using PolynomialRoot = std::vector<double>;

public:
	Polynomial() = default;

	explicit Polynomial(const Coefficient& coefficients);

	Polynomial(const Polynomial& other) = default;
	Polynomial(Polynomial&& other) noexcept = default;

	~Polynomial() = default;

	Polynomial& operator=(const Polynomial& another) = default;
	Polynomial& operator=(Polynomial&& another) noexcept = default;

	constexpr bool operator==(const Polynomial<Element>& other) const = default;

	template <typename OtherElement>
		requires SumableDifferentType<Element, OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> sum(const Polynomial<OtherElement>& other) const;
	template <typename OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> operator+(const Polynomial<OtherElement>& other) const;
	template <typename OtherElement>
		requires SumableDifferentType<Element, OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> operator+(const OtherElement& other) const;
	template <typename OtherElement>
	constexpr Polynomial<Element>& operator+=(const Polynomial<OtherElement>& other);
	constexpr Polynomial<Element>& operator+=(const Element& new_coefficient);

	[[nodiscard]] constexpr Polynomial<Element> operator-() const;

	template <typename OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> submission(const Polynomial<OtherElement>& other) const;
	template <typename OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> operator-(const Polynomial<OtherElement>& other) const;
	template <typename OtherElement>
		requires SumableDifferentType<Element, OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> operator-(const OtherElement& other) const;
	template <typename OtherElement>
	constexpr Polynomial<Element>& operator-=(const Polynomial<OtherElement>& other);
	constexpr Polynomial<Element>& operator-=(const Element& new_coefficient);

	template <typename OtherElement>
		requires MultiplableDifferentTypeReturnFirstType<Element, OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> multiple(const Polynomial<OtherElement>& other) const;
	template <typename OtherElement>
		requires MultiplableDifferentType<Element, OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> operator*(const OtherElement& other) const;
	template <typename OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> operator*(const Polynomial<OtherElement>& other) const;
	template <typename OtherElement>
	constexpr Polynomial<Element>& operator*=(const OtherElement& other);
	template <typename OtherElement>
	constexpr Polynomial<Element>& operator*=(const Polynomial<OtherElement>& other);

	template <typename OtherElement>
		requires DivisionableDifferentTypeReturnFirstType<Element, OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> divide(const Polynomial<OtherElement>& other) const;
	template <typename OtherElement>
		requires DivisionableDifferentType<Element, OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> operator/(const OtherElement& other) const;
	template <typename OtherElement>
	[[nodiscard]] constexpr Polynomial<Element> operator/(const Polynomial<OtherElement>& other) const;
	template <typename OtherElement>
	constexpr Polynomial<Element>& operator/=(const OtherElement& other);
	template <typename OtherElement>
	constexpr Polynomial<Element>& operator/=(const Polynomial<OtherElement>& other);

	[[nodiscard]] constexpr Polynomial<Element> power(uint64_t number) const;
	constexpr Polynomial<Element>& power_equal(uint64_t number);

	[[nodiscard]] constexpr Polynomial<Element> derivative() const;
	constexpr Polynomial<Element>& derivative_equal();

	template <Numberable Number>
	[[nodiscard]] constexpr Number set_value(Number value) const;

	[[nodiscard]] constexpr PolynomialRoot solve(double guess = 0, uint16_t max_iteration = 100,
			uint16_t precision = 6) const;

	[[nodiscard]] constexpr Element& at(size_t index);
	[[nodiscard]] constexpr const Element& at(size_t index) const;

	[[nodiscard]] constexpr Element& operator[](size_t index);
	[[nodiscard]] constexpr const Element& operator[](size_t index) const;

private:
	[[nodiscard]] constexpr Polynomial<Element> sum_smaller_degree_with_greater(Coefficient greater_degree_coefficients,
			const Coefficient& smaller_degree_coefficients) const noexcept;

	[[nodiscard]] constexpr PolynomialRoot solve_quadratic_equation(uint16_t precision = 6) const;

	[[nodiscard]] constexpr PolynomialRoot solve_greater_power(double guess, uint16_t max_iteration, uint16_t precision) const;

	[[nodiscard]] constexpr NewtonOutput solve_by_newton(double guess, uint16_t max_iteration, uint16_t precision) const;

	constexpr void simplify_by_horner(NewtonOutput info);

	template <typename OtherElement>
	[[nodiscard]] constexpr bool is_divide_valid(const Polynomial<OtherElement>& other) const;

	static constexpr double NOT_FOUND = std::numeric_limits<double>::min_exponent10;
	// Biggest to smallest degree
	Coefficient coefficients;
};

template <typename Element, typename OtherElement>
	requires MultiplableDifferentTypeReturnFirstType<Element, OtherElement>
constexpr Polynomial<Element> operator*(const OtherElement& other, const Polynomial<Element>& polynomial);

template <typename Element, typename OtherElement>
	requires MultiplableDifferentTypeReturnSecondType<Element, OtherElement> and Polynomialable<OtherElement>
constexpr Polynomial<OtherElement> operator*(const OtherElement& other, const Polynomial<Element>& polynomial);

#include "polynomial-tmp.h"

#endif
