#include <cinttypes>
#include <cstddef>

#include <algorithm>
#include <tuple>
#include <variant>
#include <vector>

#include <gtest/gtest.h>

#include "polynomial.h"

using namespace std;

using Coefficient = vector<int>;
using Root = vector<double>;
using SolveParameter = tuple<Coefficient, Root>;

class SharedCoefficients : public ::testing::Test
{
protected:
	const Polynomial<int> first_polynomial;
	const Polynomial<int> second_polynomial;
	const Polynomial<int> expected_result;

	SharedCoefficients(const Polynomial<int>& expected_result)
	: first_polynomial({1, -1})
	, second_polynomial({1, -4, 6, -4, 1})
	, expected_result(expected_result)
	{
	}
};

class SharedCoefficientAndElement : public ::testing::Test
{
protected:
	const Polynomial<int> primary_polynomial;
	const int element;
	const Polynomial<int> expected_result;

	SharedCoefficientAndElement(const Polynomial<int>& expected_result)
	: primary_polynomial({4, -4})
	, element(4)
	, expected_result(expected_result)
	{
	}
};

class PolynomialSumWithPolynomial : public SharedCoefficients
{
protected:
	PolynomialSumWithPolynomial()
	: SharedCoefficients(Polynomial(Coefficient({1, -4, 6, -3, 0})))
	{
	}
};

TEST_F(PolynomialSumWithPolynomial, Sum)
{
	const Polynomial<int> result = first_polynomial + second_polynomial;
	EXPECT_EQ(expected_result, result);
}

TEST_F(PolynomialSumWithPolynomial, SumEqual)
{
	Polynomial<int> result = first_polynomial;
	result += second_polynomial;

	EXPECT_EQ(expected_result, result);
}

class PolynomialSumWithElement : public SharedCoefficientAndElement
{
protected:
	PolynomialSumWithElement()
	: SharedCoefficientAndElement(Polynomial(Coefficient({4, 0})))
	{
	}
};

TEST_F(PolynomialSumWithElement, Sum)
{
	const Polynomial<int> result = primary_polynomial + element;
	EXPECT_EQ(expected_result, result);
}

TEST_F(PolynomialSumWithElement, SumEqual)
{
	Polynomial<int> polynomial = primary_polynomial;
	polynomial += element;
	EXPECT_EQ(expected_result, polynomial);
}

class PolynomialSymmetry : public SharedCoefficients
{
protected:
	PolynomialSymmetry()
	: SharedCoefficients(Polynomial(Coefficient({-1, 4, -6, 4, -1})))
	{
	}
};

TEST_F(PolynomialSymmetry, PolynomialSymmetry)
{
	EXPECT_EQ(-second_polynomial, expected_result);
}

class PolynomialSubmissionWithPolynomial : public SharedCoefficients
{
protected:
	PolynomialSubmissionWithPolynomial()
	: SharedCoefficients(Polynomial(Coefficient({-1, 4, -6, 5, -2})))
	{
	}
};

TEST_F(PolynomialSubmissionWithPolynomial, Submission)
{
	const Polynomial<int> result = first_polynomial - second_polynomial;
	EXPECT_EQ(expected_result, result);
}

TEST_F(PolynomialSubmissionWithPolynomial, SubmissionEqual)
{
	Polynomial<int> polynomial = first_polynomial;
	polynomial -= second_polynomial;

	EXPECT_EQ(expected_result, polynomial);
}

class PolynomialSubmissionWithElement : public SharedCoefficientAndElement
{
protected:
	PolynomialSubmissionWithElement()
	: SharedCoefficientAndElement(Polynomial(Coefficient({4, -8})))
	{
	}
};

TEST_F(PolynomialSubmissionWithElement, Submission)
{
	const Polynomial<int> result = primary_polynomial - element;
	EXPECT_EQ(expected_result, result);
}

TEST_F(PolynomialSubmissionWithElement, SubmissionEqual)
{
	Polynomial<int> polynomial = primary_polynomial;
	polynomial -= element;
	EXPECT_EQ(expected_result, polynomial);
}

class PolynomialMultipleWithPolynomial : public SharedCoefficients
{
protected:
	PolynomialMultipleWithPolynomial()
	: SharedCoefficients(Polynomial(Coefficient({1, -5, 10, -10, 5, -1})))
	{
	}
};

TEST_F(PolynomialMultipleWithPolynomial, Multiple)
{
	const Polynomial<int> result = first_polynomial * second_polynomial;
	EXPECT_EQ(expected_result, result);
}

TEST_F(PolynomialMultipleWithPolynomial, MultipleEqual)
{
	Polynomial<int> result = first_polynomial;
	result *= second_polynomial;
	EXPECT_EQ(expected_result, result);
}

class PolynomialMultipleWithElement : public SharedCoefficientAndElement
{
protected:
	PolynomialMultipleWithElement()
	: SharedCoefficientAndElement(Polynomial(Coefficient({16, -16})))
	{
	}
};

TEST_F(PolynomialMultipleWithElement, Multiple)
{
	const Polynomial<int> result = primary_polynomial * element;
	EXPECT_EQ(expected_result, result);
}

TEST_F(PolynomialMultipleWithElement, MultipleEqual)
{
	Polynomial<int> result = primary_polynomial;
	result *= element;
	EXPECT_EQ(expected_result, result);
}

class PolynomialDivideWithPolynomial : public SharedCoefficients
{
protected:
	PolynomialDivideWithPolynomial()
	: SharedCoefficients(Polynomial(Coefficient({1, -3, 3, -1})))
	{
	}
};

TEST_F(PolynomialDivideWithPolynomial, Divide)
{
	const Polynomial<int> result = second_polynomial / first_polynomial;
	EXPECT_EQ(expected_result, result);
}

TEST_F(PolynomialDivideWithPolynomial, DivideEqual)
{
	Polynomial<int> result = second_polynomial;
	result /= first_polynomial;
	EXPECT_EQ(expected_result, result);
}

class PolynomialDivideWithElement : public SharedCoefficientAndElement
{
protected:
	PolynomialDivideWithElement()
	: SharedCoefficientAndElement(Polynomial(Coefficient({1, -1})))
	{
	}
};

TEST_F(PolynomialDivideWithElement, Divide)
{
	const Polynomial<int> result = primary_polynomial / element;
	EXPECT_EQ(expected_result, result);
}

TEST_F(PolynomialDivideWithElement, DivideEqual)
{
	Polynomial<int> result = primary_polynomial;
	result /= element;
	EXPECT_EQ(expected_result, result);
}

class PowerCoefficients : public ::testing::Test
{
protected:
	Coefficient base;
	Polynomial<int> fourth_power;
	uint8_t degree;

	void SetUp() override
	{
		base = Coefficient{1, -1};
		fourth_power = Polynomial(Coefficient{1, -4, 6, -4, 1});
		degree = 4;
	}
};

TEST_F(PowerCoefficients, Power)
{
	const Polynomial<int> result = Polynomial(base).power(degree);
	EXPECT_EQ(fourth_power, result);
}

TEST_F(PowerCoefficients, PowerEqual)
{
	Polynomial<int> result(base);
	result.power_equal(degree);
	EXPECT_EQ(fourth_power, result);
}

class DerivativeCoefficients : public ::testing::Test
{
protected:
	Coefficient base;
	Polynomial<int> derivative;

	void SetUp() override
	{
		base = Coefficient{10, 0, -3, 0, 5, 0, -9};
		derivative = Polynomial(Coefficient{60, 0, -12, 0, 10, 0});
	}
};

TEST_F(DerivativeCoefficients, Derivate)
{
	const Polynomial<int> result = Polynomial(base).derivative();
	EXPECT_EQ(derivative, result);
}

TEST_F(DerivativeCoefficients, DerivateEqual)
{
	Polynomial<int> result(base);
	result.derivative_equal();
	EXPECT_EQ(derivative, result);
}

class SetValueTest : public ::testing::Test
{
protected:
	Polynomial<int32_t> polynomial;

	void SetUp() override
	{
		polynomial = Polynomial(Coefficient{1, -3, 3, -1});
	}
};

TEST_F(SetValueTest, InputInt32)
{
	constexpr const int32_t input = 5;
	constexpr const int32_t output = 64;

	const int32_t result = polynomial.set_value(input);

	EXPECT_EQ(result, output);
}

TEST_F(SetValueTest, Double)
{
	constexpr const double input = 3.75;
	constexpr const double output = 20.796875;

	const double result = polynomial.set_value(input);

	EXPECT_EQ(result, output);
}

class PolynomialTest : public testing::TestWithParam<SolveParameter>
{
};

TEST_P(PolynomialTest, PolynomialSolveTest)
{
	const Coefficient coefficients = std::get<0>(GetParam());
	const Root expected_roots = std::get<1>(GetParam());

	const Polynomial<int> polynomial = Polynomial(coefficients);
	Root roots = polynomial.solve(100);
	std::sort(roots.begin(), roots.end());

	EXPECT_EQ(roots, expected_roots);
}

INSTANTIATE_TEST_SUITE_P(SolveParameters, PolynomialTest,
		testing::Values(make_tuple(vector<int>{1, -3, 2}, vector<double>{1, 2}),
				make_tuple(vector<int>{1, -3, 2, 0}, vector<double>{0, 1, 2}),
				make_tuple(vector<int>{1, -6, 11, -6, 0}, vector<double>{0, 1, 2, 3})));

class CoefficientTest : public ::testing::Test
{
protected:
	const Coefficient coefficients;
	const Polynomial<int32_t> polynomial;

	CoefficientTest()
	: coefficients({1, -3, 3, -1})
	, polynomial(coefficients)
	{
	}
};

TEST_F(CoefficientTest, BracketOperator)
{
	for (size_t i = 0; i < coefficients.size(); i++)
		EXPECT_EQ(coefficients[i], polynomial[i]);
}

TEST_F(CoefficientTest, At)
{
	for (size_t i = 0; i < coefficients.size(); i++)
		EXPECT_EQ(coefficients.at(i), polynomial.at(i));
}
