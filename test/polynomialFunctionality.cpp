#include <algorithm>
#include <functional>
#include <tuple>
#include <vector>

#include "polynomial.h"
#include <gtest/gtest.h>

using namespace std;

using Coefficient = vector<int>;
using Root = vector<double>;
using SolveParameter = tuple<Coefficient, Root>;

class SharePolynomial: public ::testing::Test
{
protected:
	Coefficient coefficients1;
	Coefficient coefficients2;		/* its size must be greater than coefficients1 */
	int element;

	void SetUp() override
	{
		coefficients1 = Coefficient{2, -3, 1};
		coefficients2 = Coefficient{3, 2, -3, 1};
		element = 10;
	}
};

TEST_F(SharePolynomial, PolynomialSumWithPolynomialTest)
{
	Coefficient expected_result = coefficients2;
	for (size_t i = 0; i < coefficients1.size(); i++)
		expected_result[i] += coefficients1[i];
	
	auto result = Polynomial(coefficients1) + Polynomial(coefficients2);
	EXPECT_EQ(Polynomial(expected_result), result);
}

TEST_F(SharePolynomial, PolynomialSumWithElementTest)
{
	Coefficient expected_result = coefficients1;
	expected_result[0] += element;

	auto result = Polynomial(coefficients1) + element;
	EXPECT_EQ(Polynomial(expected_result), result);
}

TEST_F(SharePolynomial, PolynomialSumEqualWithPolynomialTest)
{
	Coefficient expected_result = coefficients2;
	for (size_t i = 0; i < coefficients1.size(); i++)
		expected_result[i] += coefficients1[i];

	auto polynomial = Polynomial(coefficients1);
	polynomial += Polynomial(coefficients2);

	EXPECT_EQ(Polynomial(expected_result), polynomial);
}

TEST_F(SharePolynomial, PolynomialSumEqualWithElementTest)
{
	Coefficient expected_result = coefficients1;
	expected_result[0] += element;

	auto polynomial = Polynomial(coefficients1);
	polynomial += element;
	EXPECT_EQ(Polynomial(expected_result), polynomial);
}

class PolynomialTest: public testing::TestWithParam<SolveParameter> {
};

TEST_P(PolynomialTest, PolynomialSolveTest)
{
	const Coefficient coefficients = std::get<0>(GetParam());
	const Root expected_roots = std::get<1>(GetParam());

	const auto polynomial = Polynomial(coefficients);
	Root roots = polynomial.solve(100);
	std::sort(roots.begin(), roots.end());

	EXPECT_EQ(roots,expected_roots);
}

INSTANTIATE_TEST_SUITE_P(SolveParameters, PolynomialTest, testing::Values(
	make_tuple(vector<int>{2, -3, 1}, vector<double>{1, 2}),
	make_tuple(vector<int>{0, 2, -3, 1}, vector<double>{0, 1, 2}),
	make_tuple(vector<int>{0, -6, 11, -6, 1}, vector<double>{0, 1, 2, 3})
));