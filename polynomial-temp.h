#ifndef MATRIX_POLYNOMIAL_TEMP_H
#define MATRIX_POLYNOMIAL_TEMP_H

#include <cmath>

#include "polynomial.h"

template <Polynomialable Element>
Polynomial<Element>::Polynomial(const vector<Element> &coefficients)
	: coefficients(coefficients)
{
}

template <Polynomialable Element>
Polynomial<Element>::Polynomial(const Polynomial &other)
	: coefficients(other, coefficients)
{
}

template <Polynomialable Element>
Polynomial<Element>::Polynomial(Polynomial &&other) noexcept
	: coefficients(std::move(other.coefficients))
{
}

template <Polynomialable Element>
template <typename Float>
inline std::enable_if_t<std::is_floating_point_v<Float>, bool>
Polynomial<Element>::compare_with_precision(Float number1, Float number2, uint16_t precision)
{
	Float diff = std::abs(number1 - number2);
	Float epsilon = std::pow(10, -precision);

	return diff < epsilon;
}

template <Polynomialable Element>
int64_t Polynomial<Element>::create_random_number(int64_t begin, int64_t end)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<int> distribution(begin, end);

	return distribution(gen);
}

template <Polynomialable Element>
inline Polynomial<Element>::NewtonOutput Polynomial<Element>::solve_by_newton(double guess, uint16_t max_iteration, uint16_t precision) const
{
	NewtonOutput result;
	Polynomial<Element> derived = derivate();
	double previous_answer = NOT_FOUND, current_answer = guess;
	double polynomial_answer, derived_answer;
	uint16_t iteration = 0;
	while (!compare_with_precision(previous_answer, current_answer, precision) && iteration < max_iteration) {
		previous_answer = current_answer;
		polynomial_answer = set_value(previous_answer);
		derived_answer = derived.set_value(previous_answer);
		if (compare_with_precision(polynomial_answer, 0.0, precision))
			break;
		else if (compare_with_precision(derived_answer, 0.0, precision / 2)) {
			// it could replace with 2 or three as precision. check this
			if (compare_with_precision(polynomial_answer, 0.0, precision / 2))
				result.second = true;
			else
				current_answer = create_random_number(guess - guess * 0.1, guess + guess * 0.1);
		} else {
			current_answer = previous_answer - (polynomial_answer / derived_answer);
		}
		++iteration;
	}
	if (compare_with_precision(set_value(current_answer), 0.0, precision / 2))
		result.first = round(current_answer, precision);
	return result;
}

template <Polynomialable Element>
inline void Polynomial<Element>::simplify_by_horner(NewtonOutput info)
{
	Element temp;
	for (size_t i = coefficients.size() - 1; i > 0; --i) {
		if (i == coefficients.size() - 1)
			temp = coefficients[i];
		else {
			temp = temp * info.first + coefficients[i];
			coefficients[i] = temp;
		}
	}
	coefficients.erase(coefficients.begin());
}

template <Polynomialable Element>
Polynomial<Element>::PolynomialRoot Polynomial<Element>::solve_quadratic_equation(uint16_t precision) const
{
	PolynomialRoot result;
	Element coefficient = coefficients.at(0);
	Element one_power = coefficients.at(1);
	Element two_power = coefficients.at(2);

	Element delta = pow(one_power, 2) - 4 * two_power * coefficient;

	if (delta >= 0) {
		result.emplace_back(round(((-1 * one_power) + sqrt(delta)) / (2 * two_power), precision));
		result.emplace_back(round(((-1 * one_power) - sqrt(delta)) / (2 * two_power), precision));
	}

	return result;
}

template <Polynomialable Element>
Polynomial<Element>::PolynomialRoot Polynomial<Element>::solve_greater_power(double guess, uint16_t max_iteration,
																			 uint16_t precision) const
{
	PolynomialRoot result;
	Polynomial<Element> temp_polynomial(*this);
	NewtonOutput temp_newton;
	while (temp_polynomial.coefficients.size() > 3) {
		temp_newton = solve_by_newton(guess, max_iteration, precision);
		if (temp_newton.first == NOT_FOUND)
			guess = create_random_number(guess - guess * 0.1, guess + guess * 0.1);
		else{
			temp_polynomial.simplify_by_horner(temp_newton);
			result.emplace_back(temp_newton.first);
			if (temp_newton.second)
				result.emplace_back(temp_newton.first);
		}
	}
	PolynomialRoot temp = solve(guess, max_iteration, precision);
	result.insert(result.end(), temp.begin(), temp.end());
	return result;
}

template <Polynomialable Element>
template <typename Number>
inline std::enable_if_t<std::is_arithmetic_v<Number>, Number>
Polynomial<Element>::round(Number number, uint16_t precision)
{
	double precision_number = std::pow(10.0, precision);
	return static_cast<Number>(std::round(number * precision_number) / precision_number);
}

template <Polynomialable Element>
template <typename OtherElement>
	requires SamableDifferentType<Element, OtherElement>
Polynomial<Element> Polynomial<Element>::sum(const Polynomial<OtherElement> &other) const
{
	uint64_t max_length = std::max(coefficients.size(), other.coefficients.size());
	std::vector<Element> temp(max_length);
	for (uint64_t i = 0; i < max_length; ++i) {
		if (coefficients.size() <= i)
			temp[i] = other.coefficients[i];
		else if (other.coefficients.size() <= i)
			temp[i] = coefficients[i];
		else
			temp[i] = coefficients[i] + other.coefficients[i];
	}
	return Polynomial(std::move(temp));
}

template <Polynomialable Element>
template <typename OtherElement>
Polynomial<Element> Polynomial<Element>::operator+(const Polynomial<OtherElement> &other) const
{
	return sum(other);
}

template <Polynomialable Element>
template <typename OtherElement>
	requires SamableDifferentType<Element, OtherElement>
Polynomial<Element> Polynomial<Element>::operator+(const OtherElement &other) const
{
	Polynomial new_polynomial(*this);
	new_polynomial.coefficients[0] += other;
	return new_polynomial;
}

template <Polynomialable Element>
template <typename OtherElement>
Polynomial<Element> &Polynomial<Element>::operator+=(const Polynomial<OtherElement> &other)
{
	*this = sum(other);
	return this;
}

template <Polynomialable Element>
Polynomial<Element> Polynomial<Element>::operator-() const
{
	Polynomial new_polynomial(*this);
	for (auto &coeff: new_polynomial.coefficients)
		coeff = -coeff;
	return new_polynomial;
}

template <Polynomialable Element>
template <typename OtherElement>
Polynomial<Element> Polynomial<Element>::submission(const Polynomial<OtherElement> &other) const
{
	return sum(-other);
}

template <Polynomialable Element>
template <typename OtherElement>
Polynomial<Element> Polynomial<Element>::operator-(const Polynomial<OtherElement> &other) const
{
	return submission(other);
}

template <Polynomialable Element>
template <typename OtherElement>
	requires SamableDifferentType<Element, OtherElement>
Polynomial<Element> Polynomial<Element>::operator-(const OtherElement &other) const
{
	Polynomial new_polynomial(*this);
	new_polynomial[0] += -other;
	return new_polynomial;
}

template <Polynomialable Element>
template <typename OtherElement>
Polynomial<Element> &Polynomial<Element>::operator-=(const Polynomial<OtherElement> &other)
{
	*this = submission(other);
	return *this;
}

template <Polynomialable Element>
template <typename OtherElement>
	requires MultipleableDifferentTypeReturnFirstType<Element, OtherElement>
Polynomial<Element> Polynomial<Element>::multiple(const Polynomial<OtherElement> &other) const
{
	uint64_t max_length = std::max(coefficients.size(), other.coefficients.size());
	Polynomial result(std::vector<Element>(0));
	for (const auto &coeff: coefficients) {
		for (const auto &other_coeff: other.coefficients) {
			result += coeff * other_coeff;
		}
	}
	return Polynomial(std::move(result));
}

template <Polynomialable Element>
template <typename OtherElement>
Polynomial<Element> Polynomial<Element>::operator*(const OtherElement &other) const
{
	Polynomial new_polynomial(*this);
	for (const auto &coeff: new_polynomial.coefficients)
		coeff *= other;
	return new_polynomial;
}

template <Polynomialable Element>
template <typename OtherElement>
Polynomial<Element> &Polynomial<Element>::operator*=(const OtherElement &other)
{
	*this = multiple(other);
	return this;
}

template <Polynomialable Element>
template <typename Number>
std::enable_if_t<std::is_arithmetic_v<Number>, Number>
Polynomial<Element>::set_value(Number value) const
{
	Number result{};
	Number temp = static_cast<Number>(1);
	for (const auto &coeff: coefficients) {
		result += temp * coeff;
		temp *= value;
	}
	return result;
}

template <Polynomialable Element>
Polynomial<Element>::PolynomialRoot Polynomial<Element>::solve(double guess, uint16_t max_iteration, uint16_t precision) const
{
	PolynomialRoot result;
	// check zero condition in constructor
	if (coefficients.size() == 1)
		result.emplace_back(coefficients.begin());
	else if (coefficients.size() == 2)
		result.emplace_back(-coefficients.begin() / coefficients.end());
	else if (coefficients.size() == 3)
		result = solve_quadratic_equation();
	else
		result = solve_greater_power();

	return result;
}

template <Polynomialable Element>
Polynomial<Element> Polynomial<Element>::power(uint64_t number) const
{
	if (number == 0)
		return Polynomial(std::vector<Element>(0));
	Polynomial result(*this);
	for (uint64_t i = 0; i < number - 1; ++i) {
		result *= *this;
	}
	return result;
}

template <Polynomialable Element>
Polynomial<Element> &Polynomial<Element>::power_equal(uint64_t number)
{
	*this = power(number);
}

template <Polynomialable Element>
Polynomial<Element> Polynomial<Element>::derivate() const
{
	Polynomial result(*this);
	result.coefficients.erase(result.coefficients.begin());
	for (size_t i = 0; i < result.coefficients.size(); ++i)
		result.coefficients[i] *= i + 1;
	return std::move(result);
}

template <Polynomialable Element>
Polynomial<Element> &Polynomial<Element>::derivate_equal()
{
	*this = derivate();
	return this;
}

template <Polynomialable Element>
Element &Polynomial<Element>::at(size_t index)
{
	return coefficients[index];
}

template <Polynomialable Element>
Element &Polynomial<Element>::operator[](size_t index)
{
	return at(index);
}

#endif//MATRIX_POLYNOMIAL_TEMP_H