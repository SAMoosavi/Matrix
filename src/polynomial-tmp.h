#ifndef MATRIX_POLYNOMIAL_TEMP_H
#define MATRIX_POLYNOMIAL_TEMP_H

#include <algorithm>
#include <random>
#include <ranges>

#include "polynomial-helper.h"
#include "polynomial.h"

template <Polynomialable Element>
Polynomial<Element>::Polynomial(const Coefficient &coefficients)
: coefficients(coefficients)
{
}

template <Polynomialable Element>
constexpr Polynomial<Element>::NewtonOutput Polynomial<Element>::solve_by_newton(double guess, uint16_t max_iteration,
		uint16_t precision) const
{
	NewtonOutput result;
	const Polynomial<Element> derived = derivative();
	double previous_answer = NOT_FOUND;
	double current_answer = guess;
	double polynomial_answer = NAN;
	double derived_answer = NAN;
	uint16_t iteration = 0;
	while (!polynomial_helper::compare_with_precision(previous_answer, current_answer, precision) &&
			iteration < max_iteration)
	{
		previous_answer = current_answer;
		polynomial_answer = set_value(previous_answer);
		derived_answer = derived.set_value(previous_answer);
		if (polynomial_helper::compare_with_precision(polynomial_answer, 0.0, precision))
			break;
		if (polynomial_helper::compare_with_precision(derived_answer, 0.0, precision / 2))
		{
			// TODO: it could replace with 2 or three as precision. check this
			if (polynomial_helper::compare_with_precision(polynomial_answer, 0.0, precision / 2))
				result.second = true;
			else
				current_answer = polynomial_helper::create_random_number(static_cast<int64_t>(guess - guess * 0.1),
						static_cast<int64_t>(guess + guess * 0.1));
		}
		else
		{
			current_answer = previous_answer - (polynomial_answer / derived_answer);
		}
		++iteration;
	}
	if (polynomial_helper::compare_with_precision(set_value(current_answer), 0.0, precision / 2))
		result.first = polynomial_helper::round(current_answer, precision);
	return result;
}

template <Polynomialable Element>
constexpr void Polynomial<Element>::simplify_by_horner(NewtonOutput info)
{
	Element current_coefficient = *coefficients.begin();

	std::ranges::for_each(coefficients.begin() + 1, coefficients.end(),
			[&info , & current_coefficient](Element &coeff)
			{
				current_coefficient = current_coefficient * info.first + coeff;
				coeff = current_coefficient;
			});

	coefficients.pop_back();
}

template <Polynomialable Element>
constexpr Polynomial<Element> Polynomial<Element>::sum_smaller_degree_with_greater(
		Coefficient greater_degree_coefficients, const Coefficient &smaller_degree_coefficients) const noexcept
{
	size_t difference_of_sizes = greater_degree_coefficients.size() - smaller_degree_coefficients.size();

	for (size_t i = smaller_degree_coefficients.size() - 1; i > 0; i--)
		greater_degree_coefficients[i + difference_of_sizes] =
				greater_degree_coefficients[i + difference_of_sizes] + smaller_degree_coefficients[i];

	greater_degree_coefficients[difference_of_sizes] =
			greater_degree_coefficients[difference_of_sizes] + smaller_degree_coefficients.front();

	return Polynomial(greater_degree_coefficients);
}

template <Polynomialable Element>
constexpr auto Polynomial<Element>::solve_quadratic_equation(uint16_t precision) const -> PolynomialRoot
{
	PolynomialRoot result;
	const size_t last_index = coefficients.size() - 1;
	Element coefficient = at(last_index);
	Element one_power = at(last_index - 1);
	Element two_power = at(last_index - 2);

	// need tp support sqrt
	Element delta = (one_power * one_power) - (4 * two_power * coefficient);

	if (delta >= 0)
	{
		result.emplace_back(polynomial_helper::round(((-1 * one_power) + sqrt(delta)) / (2 * two_power), precision));
		result.emplace_back(polynomial_helper::round(((-1 * one_power) - sqrt(delta)) / (2 * two_power), precision));
	}

	return result;
}

template <Polynomialable Element>
constexpr Polynomial<Element>::PolynomialRoot Polynomial<Element>::solve_greater_power(double guess,
		uint16_t max_iteration, uint16_t precision) const
{
	PolynomialRoot result;
	Polynomial<Element> temp_polynomial(*this);
	NewtonOutput temp_newton;
	while (temp_polynomial.coefficients.size() > 3)
	{
		temp_newton = temp_polynomial.solve_by_newton(guess, max_iteration, precision);
		if (temp_newton.first == NOT_FOUND)
			guess = polynomial_helper::create_random_number(static_cast<int64_t>(guess - guess * 0.1),
					static_cast<int64_t>(guess + guess * 0.1));
		else
		{
			temp_polynomial.simplify_by_horner(temp_newton);
			result.emplace_back(temp_newton.first);
			if (temp_newton.second)
				result.emplace_back(temp_newton.first);
		}
	}
	PolynomialRoot temp = temp_polynomial.solve(guess, max_iteration, precision);
	result.insert(result.end(), temp.begin(), temp.end());
	return result;
}

template <Polynomialable Element>
template <typename OtherElement>
	requires SumableDifferentType<Element, OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::sum(const Polynomial<OtherElement> &other) const
{
	return coefficients.size() > other.coefficients.size() ?
			sum_smaller_degree_with_greater(coefficients, other.coefficients) :
			sum_smaller_degree_with_greater(other.coefficients, coefficients);
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::operator+(const Polynomial<OtherElement> &other) const
{
	return sum(other);
}

template <Polynomialable Element>
template <typename OtherElement>
	requires SumableDifferentType<Element, OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::operator+(const OtherElement &other) const
{
	Polynomial new_polynomial(*this);
	Element &zero_degree_coefficient = new_polynomial.at(coefficients.size() - 1);
	zero_degree_coefficient += other;
	return new_polynomial;
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> &Polynomial<Element>::operator+=(const Polynomial<OtherElement> &other)
{
	*this = sum(other);
	return *this;
}

template <Polynomialable Element>
constexpr Polynomial<Element> &Polynomial<Element>::operator+=(const Element &new_coefficient)
{
	Element &zero_degree_coefficient = at(coefficients.size() - 1);
	zero_degree_coefficient += new_coefficient;
	return *this;
}

template <Polynomialable Element>
constexpr Polynomial<Element> Polynomial<Element>::operator-() const
{
	Polynomial new_polynomial(*this);
	std::ranges::for_each(new_polynomial.coefficients, [](Element &coeff) { coeff = coeff * -1; });
	return new_polynomial;
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::submission(const Polynomial<OtherElement> &other) const
{
	return sum(-other);
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::operator-(const Polynomial<OtherElement> &other) const
{
	return submission(other);
}

template <Polynomialable Element>
template <typename OtherElement>
	requires SumableDifferentType<Element, OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::operator-(const OtherElement &other) const
{
	Polynomial new_polynomial(*this);
	Element &zero_degree_coefficient = new_polynomial.at(coefficients.size() - 1);
	zero_degree_coefficient -= other;
	return new_polynomial;
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> &Polynomial<Element>::operator-=(const Polynomial<OtherElement> &other)
{
	*this = submission(other);
	return *this;
}

template <Polynomialable Element>
template <typename OtherElement>
	requires MultiplableDifferentTypeReturnFirstType<Element, OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::multiple(const Polynomial<OtherElement> &other) const
{
	Coefficient multiple_result_coefficients(coefficients.size() + other.coefficients.size() - 1, Element());
	for (size_t i = 0; i < coefficients.size(); i++)
		for (size_t j = 0; j < other.coefficients.size(); j++)
			multiple_result_coefficients[i + j] += coefficients[i] * other.coefficients[j];

	return Polynomial(multiple_result_coefficients);
}

template <Polynomialable Element>
template <typename OtherElement>
	requires MultiplableDifferentType<Element, OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::operator*(const OtherElement &other) const
{
	Polynomial new_polynomial(*this);
	for (auto &coeff : new_polynomial.coefficients)
		coeff *= other;
	return new_polynomial;
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::operator*(const Polynomial<OtherElement> &other) const
{
	return multiple(other);
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> &Polynomial<Element>::operator*=(const OtherElement &other)
{
	*this = *this * other;
	return *this;
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> &Polynomial<Element>::operator*=(const Polynomial<OtherElement> &other)
{
	*this = multiple(other);
	return *this;
}

template <Polynomialable Element>
template <typename OtherElement>
	requires DivisionableDifferentTypeReturnFirstType<Element, OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::divide(const Polynomial<OtherElement> &other) const
{
	if (!is_divide_valid(other))
		return *this;

	Coefficient copy_of_this_polynomial_coefficients(coefficients);
	Coefficient coefficients_of_result;
	while (copy_of_this_polynomial_coefficients.size() >= other.coefficients.size())
	{
		coefficients_of_result.emplace_back(copy_of_this_polynomial_coefficients.front() / other.coefficients.front());
		for (size_t i = 0; i < other.coefficients.size(); i++)
			copy_of_this_polynomial_coefficients[i] =
					copy_of_this_polynomial_coefficients[i] - (other.coefficients[i] * coefficients_of_result.back());

		copy_of_this_polynomial_coefficients.erase(copy_of_this_polynomial_coefficients.begin());
	}

	return Polynomial(coefficients_of_result);
}

template <Polynomialable Element>
template <typename OtherElement>
	requires DivisionableDifferentType<Element, OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::operator/(const OtherElement &other) const
{
	Coefficient divided_coefficients(coefficients);
	for (auto &coeff : divided_coefficients)
		coeff = coeff / other;

	return Polynomial(divided_coefficients);
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> Polynomial<Element>::operator/(const Polynomial<OtherElement> &other) const
{
	return divide(other);
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr Polynomial<Element> &Polynomial<Element>::operator/=(const OtherElement &other)
{
	*this = *this / other;
	return *this;
}

template <Polynomialable Element>
template <typename OtherElement>
inline constexpr Polynomial<Element> &Polynomial<Element>::operator/=(const Polynomial<OtherElement> &other)
{
	*this = divide(other);
	return *this;
}

template <Polynomialable Element>
template <Numberable Number>
constexpr Number Polynomial<Element>::set_value(Number value) const
{
	Number result{};
	Number variable_nth_power = 1;
	std::ranges::for_each(coefficients.crbegin(), coefficients.crend(), [&value, &result, &variable_nth_power](const Element& coeff)
	{
		result += coeff * variable_nth_power;
		variable_nth_power *= value;
	});
	return result;
}

template <Polynomialable Element>
template <typename OtherElement>
constexpr bool Polynomial<Element>::is_divide_valid(const Polynomial<OtherElement> &other) const
{
	if (coefficients.size() < other.coefficients.size())
		return false;

	if (other.coefficients.empty())
		throw std::invalid_argument("divide on empty polynomial is not valid.");

	return true;
}

template <Polynomialable Element>
constexpr Polynomial<Element>::PolynomialRoot Polynomial<Element>::solve(double guess, uint16_t max_iteration,
		uint16_t precision) const
{
	PolynomialRoot result;
	switch (coefficients.size())
	{
		case 0:
			break;
		case 1:
			result.emplace_back(coefficients.back());
			break;
		case 2:
			result.emplace_back((coefficients[1] * -1) / coefficients[0]);
			break;
		case 3:
			result = solve_quadratic_equation();
			break;
		default:
			result = solve_greater_power(guess, max_iteration, precision);
			break;
	}

	return result;
}

template <Polynomialable Element>
constexpr Polynomial<Element> &Polynomial<Element>::operator-=(const Element &new_coefficient)
{
	Element &zero_degree_coefficient = at(coefficients.size() - 1);
	zero_degree_coefficient = zero_degree_coefficient - new_coefficient;
	return *this;
}

template <Polynomialable Element>
constexpr Polynomial<Element> Polynomial<Element>::power(uint64_t number) const
{
	if (number == 0)
		return Polynomial();

	Polynomial result(*this);
	for (uint64_t i = 0; i < number - 1; ++i)
		result *= *this;

	return result;
}

template <Polynomialable Element>
constexpr Polynomial<Element> &Polynomial<Element>::power_equal(uint64_t number)
{
	*this = power(number);
	return *this;
}

template <Polynomialable Element>
constexpr Polynomial<Element> Polynomial<Element>::derivative() const
{
	Coefficient derived_coefficients(coefficients.begin(), coefficients.end() - 1);
	size_t power_degree = derived_coefficients.size();
	std::for_each(derived_coefficients.begin(), derived_coefficients.end(),
			[&power_degree](Element &coeff)
			{
				coeff *= power_degree;
				--power_degree;
			});
	return Polynomial(derived_coefficients);
}

template <Polynomialable Element>
constexpr Polynomial<Element> &Polynomial<Element>::derivative_equal()
{
	*this = derivative();
	return *this;
}

template <Polynomialable Element>
constexpr Element &Polynomial<Element>::at(size_t index)
{
	return coefficients[index];
}

template <Polynomialable Element>
constexpr const Element &Polynomial<Element>::at(size_t index) const
{
	return coefficients[index];
}

template <Polynomialable Element>
constexpr Element &Polynomial<Element>::operator[](size_t index)
{
	return at(index);
}

template <Polynomialable Element>
constexpr const Element &Polynomial<Element>::operator[](size_t index) const
{
	return at(index);
}

template <typename Element, typename OtherElement>
	requires MultiplableDifferentTypeReturnFirstType<Element, OtherElement>
constexpr Polynomial<Element> operator*(const OtherElement &other, const Polynomial<Element> &polynomial)
{
	return polynomial * other;
}

template <typename Element, typename OtherElement>
	requires MultiplableDifferentTypeReturnSecondType<Element, OtherElement> and Polynomialable<OtherElement>
constexpr Polynomial<OtherElement> operator*(const OtherElement &other, const Polynomial<Element> &polynomial)
{
	Polynomial other_polynomial(other);
	return other_polynomial * polynomial;
}

#endif
