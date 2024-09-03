#include "matrix.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

class MatrixFunctionality: public Test {};

TEST_F(MatrixFunctionality, creat_matrix_with_size_should_creat_matrix_with_size_and_set_value_of_each_cell_zero)
{
	constexpr size_t NUMBER_OF_RAW = 4;
	constexpr size_t NUMBER_OF_COL = 5;
	const int DEFAULT_VALUE_OF_MATRIX_CONSTRUCTOR = 0;

	Matrix<int> matrix(NUMBER_OF_RAW, NUMBER_OF_COL);
	std::vector<std::vector<int>> TABLE_OF_MATRIX(NUMBER_OF_RAW, std::vector<int>(NUMBER_OF_COL,
		DEFAULT_VALUE_OF_MATRIX_CONSTRUCTOR));
	EXPECT_EQ(matrix.get_table(), TABLE_OF_MATRIX);
}

TEST_F(MatrixFunctionality, creat_matrix_with_vector)
{
	const std::vector<std::vector<double>> INITIALIZE_DATA_OF_MATRIX = {
			{1.2, 2.3, 3, 4},
			{1, 2, 3.5, 4},
			{5, 6, 7, 8.9}
	};

	Matrix<double> matrix(INITIALIZE_DATA_OF_MATRIX);
	EXPECT_EQ(matrix.get_table(), INITIALIZE_DATA_OF_MATRIX);
}

TEST_F(MatrixFunctionality, creat_matrix_with_intializer_list)
{
	const std::initializer_list<std::initializer_list<uint32_t>> INITIALIZE_LIST_OF_MATRIX = {
			{1, 2, 3, 4},
			{1, 2, 3, 4},
			{5, 6, 7, 8}
	};

	std::vector<std::vector<uint32_t>> table_of_matrix;
	for (const auto& row_of_matrix: INITIALIZE_LIST_OF_MATRIX)
		table_of_matrix.emplace_back(row_of_matrix);

	Matrix<uint32_t> matrix(INITIALIZE_LIST_OF_MATRIX);
	EXPECT_THAT(matrix.get_table(), Eq(table_of_matrix));
}

TEST_F(MatrixFunctionality, creat_matrix_with_different_column_size_should_be_get_trow)
{
	EXPECT_THROW(Matrix<int>({{1, 2, 3}, {1, 4}, {5, 6, 7, 8}}), std::invalid_argument);
}