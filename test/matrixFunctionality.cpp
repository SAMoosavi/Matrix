#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "matrix.h"

using namespace ::testing;

class MatrixFunctionality : public Test
{
};

TEST_F(MatrixFunctionality, CreatMatrixWithSizeShouldCreatMatrixWithSizeAndSetValueOfEachCellZero)
{
	constexpr size_t NUMBER_OF_RAW = 4;
	constexpr size_t NUMBER_OF_COL = 5;
	const int DEFAULT_VALUE_OF_MATRIX_CONSTRUCTOR = 0;

	Matrix<int> matrix(NUMBER_OF_RAW, NUMBER_OF_COL);
	std::vector<std::vector<int>> TABLE_OF_MATRIX(NUMBER_OF_RAW,
			std::vector<int>(NUMBER_OF_COL, DEFAULT_VALUE_OF_MATRIX_CONSTRUCTOR));
	EXPECT_EQ(matrix.get_table(), TABLE_OF_MATRIX);
}

TEST_F(MatrixFunctionality, CreatMatrixWithVector)
{
	const std::vector<std::vector<double>> INITIALIZE_DATA_OF_MATRIX = {{1.2, 2.3, 3, 4}, {1, 2, 3.5, 4},
			{5, 6, 7, 8.9}};

	Matrix<double> matrix(INITIALIZE_DATA_OF_MATRIX);
	EXPECT_EQ(matrix.get_table(), INITIALIZE_DATA_OF_MATRIX);
}

TEST_F(MatrixFunctionality, CreatMatrixWithIntializerList)
{
	const std::initializer_list<std::initializer_list<uint32_t>> INITIALIZE_LIST_OF_MATRIX = {{1, 2, 3, 4},
			{1, 2, 3, 4}, {5, 6, 7, 8}};

	std::vector<std::vector<uint32_t>> table_of_matrix;
	for (const auto& row_of_matrix : INITIALIZE_LIST_OF_MATRIX)
		table_of_matrix.emplace_back(row_of_matrix);

	Matrix<uint32_t> matrix(INITIALIZE_LIST_OF_MATRIX);
	EXPECT_THAT(matrix.get_table(), Eq(table_of_matrix));
}

TEST_F(MatrixFunctionality, CreatMatrixWithDifferentColumnSizeShouldBeGetTrow)
{
	EXPECT_THROW(Matrix<int>({{1, 2, 3}, {1, 4}, {5, 6, 7, 8}}), std::invalid_argument);
}

TEST_F(MatrixFunctionality, TheSumFunctionSuoldBeTrowWhenFirstAndSecondMatrixIsNotSameColumnNumber)
{
	constexpr size_t ROW_NUMBER = 1;
	constexpr size_t FIRST_COL_NUMBER = 1;
	constexpr size_t SECOND_COL_NUMBER = 2;
	const Matrix<int> FIRST_MATRIX(ROW_NUMBER, FIRST_COL_NUMBER);
	const Matrix<int> SECOND_MATRIX(ROW_NUMBER, SECOND_COL_NUMBER);
	EXPECT_THROW(FIRST_MATRIX.sum(SECOND_MATRIX), std::invalid_argument);
}

TEST_F(MatrixFunctionality, TheSumFunctionSuoldBeTrowWhenFirstAndSecondMatrixIsNotSameRowNumber)
{
	constexpr size_t FIRST_ROW_NUMBER = 1;
	constexpr size_t SECOND_ROW_NUMBER = 2;
	constexpr size_t COL_NUMBER = 1;
	const Matrix<int> FIRST_MATRIX(FIRST_ROW_NUMBER, COL_NUMBER);
	const Matrix<int> SECOND_MATRIX(SECOND_ROW_NUMBER, COL_NUMBER);
	EXPECT_THROW(FIRST_MATRIX.sum(SECOND_MATRIX), std::invalid_argument);
}

TEST_F(MatrixFunctionality, TheSubmissionFunctionSuoldBeTrowWhenFirstAndSecondMatrixIsNotSameColumnNumber)
{
	constexpr size_t ROW_NUMBER = 1;
	constexpr size_t FIRST_COL_NUMBER = 1;
	constexpr size_t SECOND_COL_NUMBER = 2;
	const Matrix<int> FIRST_MATRIX(ROW_NUMBER, FIRST_COL_NUMBER);
	const Matrix<int> SECOND_MATRIX(ROW_NUMBER, SECOND_COL_NUMBER);
	EXPECT_THROW(FIRST_MATRIX.submission(SECOND_MATRIX), std::invalid_argument);
}

TEST_F(MatrixFunctionality, TheSubmissionFunctionSuoldBeTrowWhenFirstAndSecondMatrixIsNotSameRowNumber)
{
	constexpr size_t FIRST_ROW_NUMBER = 1;
	constexpr size_t SECOND_ROW_NUMBER = 2;
	constexpr size_t COL_NUMBER = 1;
	const Matrix<int> FIRST_MATRIX(FIRST_ROW_NUMBER, COL_NUMBER);
	const Matrix<int> SECOND_MATRIX(SECOND_ROW_NUMBER, COL_NUMBER);
	EXPECT_THROW(FIRST_MATRIX.submission(SECOND_MATRIX), std::invalid_argument);
}

TEST_F(MatrixFunctionality, TheMultipleFunctionSuoldBeTrowWhenColNumberOfFirstAndRowNumberOfSecondMatrixIsNotSame)
{
	constexpr size_t FIRST_ROW_NUMBER = 1;
	constexpr size_t FIRST_COL_NUMBER = 1;
	constexpr size_t SECOND_ROW_NUMBER = 2;
	constexpr size_t SECOND_COL_NUMBER = 1;
	const Matrix<int> FIRST_MATRIX(FIRST_ROW_NUMBER, FIRST_COL_NUMBER);
	const Matrix<int> SECOND_MATRIX(SECOND_ROW_NUMBER, SECOND_COL_NUMBER);
	EXPECT_THROW(FIRST_MATRIX.multiple(SECOND_MATRIX), std::invalid_argument);
}

TEST_F(MatrixFunctionality, TheDeterminantFunctionSuoldBeTrowWhenTheMatrixIsNotSqurt)
{
	constexpr size_t ROW_NUMBER = 1;
	constexpr size_t COL_NUMBER = 2;
	const Matrix<int> MATRIX(ROW_NUMBER, COL_NUMBER);
	EXPECT_THROW(MATRIX.determinant(), std::invalid_argument);
}

TEST_F(MatrixFunctionality, TheEigenvaluesFunctionSuoldBeTrowWhenTheMatrixIsNotSqurt)
{
	constexpr size_t ROW_NUMBER = 1;
	constexpr size_t COL_NUMBER = 2;
	const Matrix<double> MATRIX(ROW_NUMBER, COL_NUMBER);
	EXPECT_THROW(MATRIX.eigenvalues(), std::invalid_argument);
}

TEST_F(MatrixFunctionality, TheEigenvaluesFunctionSuoldReturnEigenvaluesNumberOfMatrix)
{
	const Matrix<double> MATRIX({{-26, -33, -25}, {31, 42, 23}, {-11, -15, -4}});
	EXPECT_THAT(MATRIX.eigenvalues(), ElementsAre(-6, 3, 15));
}

TEST_F(MatrixFunctionality, TheEigenvectorFunctionSuoldBeTrowWhenTheMatrixIsNotSqurt)
{
	constexpr size_t ROW_NUMBER = 1;
	constexpr size_t COL_NUMBER = 2;
	const Matrix<double> MATRIX(ROW_NUMBER, COL_NUMBER);
	EXPECT_THROW(MATRIX.eigenvector(), std::invalid_argument);
}

TEST_F(MatrixFunctionality, TheEigenvectorFunctionSuoldReturnEigenvectorNumberOfMatrix)
{
	const Matrix<double> matrix({{-26, -33, -25}, {31, 42, 23}, {-11, -15, -4}});
	const std::vector<std::vector<double>> ans =  {{7, -5, 1}, {-2, 1, 1}, {1, -2, 1}};
	const std::vector<std::vector<double>> eigenvector = matrix.eigenvector();

	for (int i =0; i < 3; ++i)
		for (int j =0; j < 3; ++j)
			EXPECT_THAT(eigenvector[i][j], DoubleEq(ans[i][j]));
}

using BaineryOperatorType = std::tuple<Matrix<int>, Matrix<int>, Matrix<int>>;

class SumOfTwoMatrix : public ::testing::TestWithParam<BaineryOperatorType>
{
};

TEST_P(SumOfTwoMatrix, TheSumFunctionShouldReturnThirdParamWhenSumFirstAndSecondParams)
{
	const Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	const Matrix<int> THIRD_MATRIX = std::get<2>(GetParam());

	EXPECT_EQ(FIRST_MATRIX.sum(SECOND_MATRIX), THIRD_MATRIX);
}

TEST_P(SumOfTwoMatrix, TheSumOperatorShouldReturnThirdParamWhenSumFirstAndSecondParams)
{
	const Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	const Matrix<int> THIRD_MATRIX = std::get<2>(GetParam());

	EXPECT_EQ(FIRST_MATRIX + SECOND_MATRIX, THIRD_MATRIX);
}

TEST_P(SumOfTwoMatrix, TheSumEqualOperatorShouldSetFirstMatrixEqualToThirdParamWhenSumFirstAndSecondParams)
{
	Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	const Matrix<int> THIRD_MATRIX = std::get<2>(GetParam());
	FIRST_MATRIX += SECOND_MATRIX;
	EXPECT_EQ(FIRST_MATRIX, THIRD_MATRIX);
}

INSTANTIATE_TEST_SUITE_P(SumData, SumOfTwoMatrix,
		Values(std::make_tuple(Matrix<int>({{1, 2, 3}, {1, 2, 3}}), Matrix<int>({{6, 5, 4}, {6, 5, 4}}),
					   Matrix<int>({{7, 7, 7}, {7, 7, 7}})),
				std::make_tuple(Matrix<int>({{1, 2, 3, 4, 5, 6, 7}}), Matrix<int>({{7, 6, 5, 4, 3, 2, 1}}),
						Matrix<int>({{8, 8, 8, 8, 8, 8, 8}}))));

class SubmissionOfTwoMatrix : public ::testing::TestWithParam<BaineryOperatorType>
{
};

TEST_P(SubmissionOfTwoMatrix, TheSubmissionFunctionShouldReturnThirdParamWhenSubmissionFirstAndSecondParams)
{
	const Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	const Matrix<int> THIRD_MATRIX = std::get<2>(GetParam());

	EXPECT_EQ(FIRST_MATRIX.submission(SECOND_MATRIX), THIRD_MATRIX);
}

TEST_P(SubmissionOfTwoMatrix, TheSubmissionOperatorShouldReturnThirdParamWhenSubmissionFirstAndSecondParams)
{
	const Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	const Matrix<int> THIRD_MATRIX = std::get<2>(GetParam());

	EXPECT_EQ(FIRST_MATRIX - SECOND_MATRIX, THIRD_MATRIX);
}

TEST_P(SubmissionOfTwoMatrix,
		TheSubmissionEqualOperatorShouldSetFirstMatrixEqualToThirdParamWhenSubmissionFirstAndSecondParams)
{
	Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	const Matrix<int> THIRD_MATRIX = std::get<2>(GetParam());
	FIRST_MATRIX -= SECOND_MATRIX;
	EXPECT_EQ(FIRST_MATRIX, THIRD_MATRIX);
}

INSTANTIATE_TEST_SUITE_P(SubmissionData, SubmissionOfTwoMatrix,
		Values(std::make_tuple(Matrix<int>({{1, 2, 3}, {1, 2, 3}}), Matrix<int>({{6, 5, 4}, {6, 5, 4}}),
					   Matrix<int>({{-5, -3, -1}, {-5, -3, -1}})),
				std::make_tuple(Matrix<int>({{1, 2, 3, 4, 5, 6, 7}}), Matrix<int>({{7, 6, 5, 4, 3, 2, 1}}),
						Matrix<int>({{-6, -4, -2, 0, 2, 4, 6}}))));

class MultipleOfTwoMatrix : public ::testing::TestWithParam<BaineryOperatorType>
{
};

TEST_P(MultipleOfTwoMatrix, TheMultipleFunctionShouldReturnThirdParamWhenMultipleFirstAndSecondParams)
{
	const Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	const Matrix<int> THIRD_MATRIX = std::get<2>(GetParam());

	EXPECT_EQ(FIRST_MATRIX.multiple(SECOND_MATRIX), THIRD_MATRIX);
}

TEST_P(MultipleOfTwoMatrix, TheMultipleOperatorShouldReturnThirdParamWhenMultipleFirstAndSecondParams)
{
	const Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	const Matrix<int> THIRD_MATRIX = std::get<2>(GetParam());

	EXPECT_EQ(FIRST_MATRIX * SECOND_MATRIX, THIRD_MATRIX);
}

TEST_P(MultipleOfTwoMatrix,
		TheMultipleEqualOperatorShouldSetFirstMatrixEqualToThirdParamWhenMultipleFirstAndSecondParams)
{
	Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	const Matrix<int> THIRD_MATRIX = std::get<2>(GetParam());
	FIRST_MATRIX *= SECOND_MATRIX;
	EXPECT_EQ(FIRST_MATRIX, THIRD_MATRIX);
}

INSTANTIATE_TEST_SUITE_P(MultipleData, MultipleOfTwoMatrix,
		Values(std::make_tuple(Matrix<int>({{1, 2}, {1, 2}}), Matrix<int>({{1, 2}, {1, 2}}),
					   Matrix<int>({{3, 6}, {3, 6}})),
				std::make_tuple(Matrix<int>({{1, 2, 3, 4, 5, 6, 7}}), Matrix<int>({{7}, {6}, {5}, {4}, {3}, {2}, {1}}),
						Matrix<int>({{84, 0, 0, 0, 0, 0, 0}})),
				std::make_tuple(Matrix<int>({{1, 2, 3, 4, 5, 6, 7}, {1, 2, 3, 4, 5, 6, 7}, {1, 2, 3, 4, 5, 6, 7},
										{1, 2, 3, 4, 5, 6, 7}, {1, 2, 3, 4, 5, 6, 7}}),
						Matrix<int>({{7, 2, 3, 4, 5, 6, 9, 7}, {6, 2, 3, 4, 5, 6, 9, 7}, {5, 2, 3, 4, 5, 6, 9, 7},
								{4, 2, 3, 4, 5, 6, 9, 7}, {3, 2, 3, 4, 5, 6, 9, 7}, {2, 2, 3, 4, 5, 6, 9, 7},
								{1, 2, 3, 4, 5, 6, 9, 7}}),
						Matrix<int>({{84, 56, 84, 112, 140, 168, 252}, {84, 56, 84, 112, 140, 168, 252},
								{84, 56, 84, 112, 140, 168, 252}, {84, 56, 84, 112, 140, 168, 252},
								{84, 56, 84, 112, 140, 168, 252}}))));

class OppositeOfMatrix : public ::testing::TestWithParam<std::tuple<Matrix<int>, Matrix<int>>>
{
};

TEST_P(OppositeOfMatrix, TheOppositeOperatorShouldReturnSecondParamWhenOppositFirstParams)
{
	const Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const Matrix<int> SECOND_MATRIX = std::get<1>(GetParam());
	EXPECT_EQ(-FIRST_MATRIX, SECOND_MATRIX);
}

INSTANTIATE_TEST_SUITE_P(OppositeData, OppositeOfMatrix,
		Values(std::make_tuple(Matrix<int>({{1, 2, 3}, {1, 2, 3}}), Matrix<int>({{-1, -2, -3}, {-1, -2, -3}})),
				std::make_tuple(Matrix<int>({{1, 2, 3, 4, 5, 6, 7}}), Matrix<int>({{-1, -2, -3, -4, -5, -6, -7}}))));

class DeterminantOfMatrix : public ::testing::TestWithParam<std::tuple<Matrix<int>, int>>
{
};

TEST_P(DeterminantOfMatrix, TheDeterminantShouldReturnSecondParamWhenDeterminantFirstParams)
{
	const Matrix<int> FIRST_MATRIX = std::get<0>(GetParam());
	const int DETERMINANT = std::get<1>(GetParam());
	EXPECT_EQ(FIRST_MATRIX.determinant(), DETERMINANT);
}

INSTANTIATE_TEST_SUITE_P(DeterminantData, DeterminantOfMatrix,
		Values(std::make_tuple(Matrix<int>({{1, 2}, {0, 2}}), 2),
				std::make_tuple(Matrix<int>({{1, 2, 3}, {0, 2, 3}, {1, 2, 0}}), -6),
				std::make_tuple(Matrix<int>({{1, 2, 3, 4}, {0, 2, 3, 5}, {1, 2, 0, 5}, {1, 2, 8, 5}}), -12),
				std::make_tuple(Matrix<int>({{1}}), 1)));