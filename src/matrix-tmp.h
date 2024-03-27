#ifndef MATRIX_MATRIX_TMP_H
#define MATRIX_MATRIX_TMP_H

#include <format>

#include "matrix.h"

template <Elementable Element>
Matrix<Element>::Matrix()
	: row(0), col(0), table({{}}){};

template <Elementable Element>
Matrix<Element>::Matrix(size_t row, size_t col)
	: row(row), col(col), table(row, RowType(col, 0)){};

template <Elementable Element>
template <template <Containerable> typename Container>
Matrix<Element>::Matrix(const Container<Container<Element>>& matrix)
	: row(matrix.size()), col(matrix[0].size()), table(row, RowType(col, 0))
{
	for (int row_index = 0; row_index < row; ++row_index)
		for (int col_index = 0; col_index < col; ++col_index)
			table[row_index][col_index] = matrix[row_index][col_index];
};

template <Elementable Element>
Matrix<Element> Matrix<Element>::sum(const Matrix<Element>& other) const
{
	if (row != other.row && col != other.col)
		throw invalid_argument("Cannot sum spans of different sizes");

	Matrix<Element> result(row, col);
	for (size_t row_index = 0; row_index < row; ++row_index)
		for (size_t col_index = 0; col_index < col; ++col_index)
			result[row_index][col_index] = other[row_index][col_index] + table[row_index][col_index];

	return result;
}

template <Elementable Element>
Matrix<Element> Matrix<Element>::operator+(const Matrix<Element>& other) const
{
	return sum(std::move(other));
}

template <Elementable Element>
Matrix<Element>& Matrix<Element>::operator+=(const Matrix<Element>& other)
{
	*this = sum(other);
	return *this;
}

template <Elementable Element>
Matrix<Element> Matrix<Element>::operator-() const
{
	Matrix tmp(*this);

	for (RowType& row_of_tmp: tmp.table)
		for (Element& element_of_tmp: row_of_tmp)
			element_of_tmp = -element_of_tmp;
}

template <Elementable Element>
Matrix<Element> Matrix<Element>::submission(const Matrix<Element>& other) const
{
	return sum(-other);
}

template <Elementable Element>
Matrix<Element> Matrix<Element>::operator-(const Matrix<Element>& other) const
{
	return multiple(std::move(other));
}

template <Elementable Element>
Matrix<Element>& Matrix<Element>::operator-=(const Matrix<Element>& other)
{
	*this = submission(other);
	return *this;
}

template <Elementable Element>
Matrix<Element> Matrix<Element>::multiple(const Matrix<Element>& other) const
{
	if (col != other.row)
		throw std::runtime_error(std::format("Must be number of col in first Matrix equal to row of second Matrix but {} not equal to {}", col, other.row));

	Matrix<Element> result(row, col);

	for (size_t i = 0; i < row; ++i)
		for (size_t k = 0; k < col; ++k)
			for (size_t j = 0; j < other.col; ++j)
				result[i][j] += table[i][k] * other[k][j];

	return result;
}

template <Elementable Element>
Matrix<Element> Matrix<Element>::operator*(const Matrix<Element>& other) const
{
	return multiple(std::move(other));
}

template <Elementable Element>
Matrix<Element>& Matrix<Element>::operator*=(const Matrix<Element>& other)
{
	*this = multiple(other);
	return *this;
}

template <Elementable Element>
Matrix<Element> Matrix<Element>::multiple(const auto& other) const
{
	Matrix<Element> result = *this;
	for (RowType& row_of_table: result.table) {
		for (Element& element: row_of_table) {
			if constexpr (MultipleAssignableDifferentType<Element, decltype(other)>) {
				element *= other;
			} else if constexpr (MultipleableDifferentTypeReturnFirstType<Element, decltype(other)>) {
				element = element * other;
			} else if constexpr (MultipleableDifferentTypeReturnSecondType<Element, decltype(other)>) {
				element = other * element;
			} else {
				throw std::invalid_argument("cannot calculate multiplication");
			}
		}
	}
	return result;
}

template <Elementable Element>
Matrix<Element> Matrix<Element>::operator*(const auto& other) const
{
	return multiple(std::move(other));
}

template <Elementable Element>
Matrix<Element>& Matrix<Element>::operator*=(const auto& other)
{
	*this = multiple(std::move(other));
	return *this;
}

template <Elementable Element>
Matrix<Element> operator*(const auto& number, const Matrix<Element>& matrix)
{
	return std::move(matrix * number);
}

template <Elementable Element>
Matrix<Element>::RowType Matrix<Element>::operator[](size_t idx) const
{
	return table[idx];
}

template <Elementable Element>
Matrix<Element>::RowType& Matrix<Element>::operator[](size_t idx)
{
	return table[idx];
}

template <Elementable Element>
Element Matrix<Element>::at(size_t row_index, size_t col_index)
{
	return table[row_index][col_index];
}

template <Elementable Element>
Element Matrix<Element>::determinant() const
{
	if (col != row)
		throw std::invalid_argument("Matrix<Element>::determinant: column and row must be equal");

	TableType tmp_table = table;
	size_t number_of_swap = 0;
	for (size_t col_index = 0; col_index < col; col_index++) {
		size_t swap_row_index = col_index;
		Element base_of_column = tmp_table[swap_row_index][col_index];
		while (base_of_column == 0 && swap_row_index < row) {
			base_of_column = tmp_table[swap_row_index][col_index];
			++swap_row_index;
		}

		if (base_of_column == 0)
			return 0;
		else if (swap_row_index != col_index) {
			std::swap(tmp_table[swap_row_index - 1], tmp_table[col_index]);
			++number_of_swap;
		}

		for (size_t row_index = col_index + 1; row_index < row; row_index++) {
			if (tmp_table[row_index][col_index] == 0)
				continue;

			Element ratio = tmp_table[row_index][col_index] / base_of_column;
			for (size_t i = col_index; i < col; i++)
				tmp_table[row_index][i] -= ratio * tmp_table[col_index][i];
		}
	}

	Element det = 1;
	for (size_t i = 0; i < row; i++)
		det *= tmp_table[i][i];

	if (number_of_swap % 2 == 0)
		return det;
	else
		return -det;
}

#endif