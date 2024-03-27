#include <gtest/gtest.h>

#include "matrix.h"

TEST(MatrixFunctionality, default_values_of_table_in_matrix_shuld_be_zero)
{
	Matrix<int> matrix(3, 3);
	auto table = matrix.get_table();
	for (const auto& row: table) {
		EXPECT_EQ(row.size(), 3);
		for (const auto& cell: row)
			EXPECT_EQ(cell, 0);
	}
}
