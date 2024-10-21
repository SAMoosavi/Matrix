#ifndef MATRIX_MATRIX_TMP_H
#define MATRIX_MATRIX_TMP_H

#include <cmath>

#include <ranges>

#include "matrix.h"

template <IsMatrixableElement Element>
Matrix<Element>::Matrix(size_t row, size_t col)
: number_of_row(row)
, number_of_col(col)
, table(row, RowType(col, Element(0)))
{
}

template <IsMatrixableElement Element>
template <template <Containerable> typename Container>
Matrix<Element>::Matrix(const Container<Container<Element>>& matrix)
: number_of_row(matrix.size())
, number_of_col(matrix.begin()->size())
, table(0)
{
	for (const auto& row_of_matrix : matrix)
	{
		if (row_of_matrix.size() != number_of_col)
			throw std::invalid_argument("Cannot creat matrix with different column size.");

		table.emplace_back(row_of_matrix);
	}
}

template <IsMatrixableElement Element>
Matrix<Element>::Matrix(const std::initializer_list<std::initializer_list<Element>>& matrix)
: number_of_row(matrix.size())
, number_of_col(matrix.begin()->size())
, table(0)
{
	for (const auto& row_of_matrix : matrix)
	{
		if (row_of_matrix.size() != number_of_col)
			throw std::invalid_argument("Cannot creat matrix with different column size.");

		table.emplace_back(row_of_matrix);
	}
}

template <IsMatrixableElement Element>
template <typename OtherElement>
	requires SumableDifferentType<Element, OtherElement>
Matrix<Element> Matrix<Element>::sum(const Matrix<OtherElement>& other) const
{
	if (number_of_row != other.get_number_of_row() or number_of_col != other.get_number_of_col())
		throw std::invalid_argument("Cannot sum spans of different sizes");

	Matrix<Element> result(number_of_row, number_of_col);
	for (size_t row_index = 0; row_index < number_of_row; ++row_index)
		for (size_t col_index = 0; col_index < number_of_col; ++col_index)
			result[row_index][col_index] = table[row_index][col_index] + other[row_index][col_index];

	return result;
}

template <IsMatrixableElement Element>
template <typename OtherElement>
Matrix<Element> Matrix<Element>::operator+(const Matrix<OtherElement>& other) const
{
	return sum(std::move(other));
}

template <IsMatrixableElement Element>
template <typename OtherElement>
Matrix<Element>& Matrix<Element>::operator+=(const Matrix<OtherElement>& other)
{
	*this = sum(other);
	return *this;
}

template <IsMatrixableElement Element>
Matrix<Element> Matrix<Element>::operator-() const
{
	Matrix tmp(*this);

	for (RowType& row_of_tmp : tmp.table)
		for (Element& element_of_tmp : row_of_tmp)
			element_of_tmp = -element_of_tmp;

	return tmp;
}

template <IsMatrixableElement Element>
template <typename OtherElement>
Matrix<Element> Matrix<Element>::submission(const Matrix<OtherElement>& other) const
{
	return sum(-other);
}

template <IsMatrixableElement Element>
template <typename OtherElement>
Matrix<Element> Matrix<Element>::operator-(const Matrix<OtherElement>& other) const
{
	return submission(other);
}

template <IsMatrixableElement Element>
template <typename OtherElement>
Matrix<Element>& Matrix<Element>::operator-=(const Matrix<OtherElement>& other)
{
	*this = submission(other);
	return *this;
}

template <IsMatrixableElement Element>
template <typename OtherElement>
	requires MultiplableDifferentTypeReturnFirstType<Element, OtherElement>
Matrix<Element> Matrix<Element>::multiple(const Matrix<OtherElement>& other) const
{
	if (number_of_col != other.get_number_of_row())
		throw std::invalid_argument("the number of rows must match the number of columns.");

	Matrix<Element> result(number_of_row, number_of_col);

	for (size_t i = 0; i < number_of_row; ++i)
		for (size_t k = 0; k < number_of_col; ++k)
			for (size_t j = 0; j < other.get_number_of_col(); ++j)
				result[i][j] += table[i][k] * other[k][j];

	return result;
}

template <IsMatrixableElement Element>
template <typename OtherElement>
	requires(not IsMatrixable<OtherElement>) and MultiplableDifferentType<Element, OtherElement>
Matrix<Element> Matrix<Element>::multiple(const OtherElement& other) const
{
	Matrix<Element> result = *this;
	for (RowType& row_of_table : result.table)
	{
		for (Element& element : row_of_table)
		{
			if constexpr (MultipleAssignableDifferentType<Element, OtherElement>)
				element *= other;
			else if constexpr (MultiplableDifferentTypeReturnFirstType<Element, OtherElement>)
				element = element * other;
			else if constexpr (MultiplableDifferentTypeReturnSecondType<Element, OtherElement>)
				element = other * element;
		}
	}
	return result;
}

template <IsMatrixableElement Element>
template <typename OtherElement>
Matrix<Element> Matrix<Element>::operator*(const OtherElement& other) const
{
	return multiple(std::move(other));
}

template <IsMatrixableElement Element>
template <typename OtherElement>
Matrix<Element>& Matrix<Element>::operator*=(const OtherElement& other)
{
	*this = multiple(other);
	return *this;
}

template <typename Element, typename OtherElement>
	requires(not IsMatrixable<Element>) and
		(not IsMatrixable<OtherElement>) and MultiplableDifferentType<Element, OtherElement>
Matrix<Element> operator*(const OtherElement& number, const Matrix<Element>& matrix)
{
	return std::move(matrix * number);
}

template <IsMatrixableElement Element>
Matrix<Element>::RowType Matrix<Element>::operator[](size_t idx) const
{
	return table[idx];
}

template <IsMatrixableElement Element>
Matrix<Element>::RowType& Matrix<Element>::operator[](size_t idx)
{
	return table[idx];
}

template <IsMatrixableElement Element>
Element Matrix<Element>::at(size_t row_index, size_t col_index)
{
	return table[row_index][col_index];
}

template <IsMatrixableElement Element>
Element Matrix<Element>::determinant() const
{
	if (number_of_col != number_of_row)
		throw std::invalid_argument("Matrix<Element>::determinant: column and number_of_row must be equal");

	TableType tmp_table = table;
	size_t number_of_swap = 0;
	for (size_t col_index = 0; col_index < number_of_col; col_index++)
	{
		size_t swap_row_index = col_index;
		Element base_of_column = tmp_table[swap_row_index][col_index];
		// TODO: create concept for check exit Element == 0
		while (base_of_column == 0 and swap_row_index < number_of_row)
		{
			base_of_column = tmp_table[swap_row_index][col_index];
			++swap_row_index;
		}

		if (base_of_column == 0)
			return 0;
		else if (swap_row_index != col_index)
		{
			std::swap(tmp_table[swap_row_index - 1], tmp_table[col_index]);
			++number_of_swap;
		}

		for (size_t row_index = col_index + 1; row_index < number_of_row; row_index++)
		{
			if (tmp_table[row_index][col_index] == 0)
				continue;

			Element ratio = tmp_table[row_index][col_index] / base_of_column;
			for (size_t i = col_index; i < number_of_col; i++)
				tmp_table[row_index][i] -= ratio * tmp_table[col_index][i];
		}
	}

	Element det = 1;
	for (size_t i = 0; i < number_of_row; i++)
		det *= tmp_table[i][i];

	if (number_of_swap % 2 == 0)
		return det;
	else
		return -det;
}

template <IsMatrixableElement Element>
auto Matrix<Element>::get_table() const -> TableType
{
	return table;
}

template <IsMatrixableElement Element>
size_t Matrix<Element>::get_number_of_row() const
{
	return number_of_row;
}

template <IsMatrixableElement Element>
size_t Matrix<Element>::get_number_of_col() const
{
	return number_of_col;
}

template <IsMatrixableElement Element>
template <typename OtherElement>
bool Matrix<Element>::operator==(const Matrix<OtherElement>& other) const
{
	if (this->number_of_col != other.number_of_col or this->number_of_row != other.number_of_row)
		return false;

	for (size_t i = 0; i < number_of_row; ++i)
	{
		for (int j = 0; j < number_of_col; ++j)
		{
			if (this->table[i][j] != other.table[i][j])
				return false;
		}
	}

	return true;
}

template <IsMatrixableElement Element>
std::string Matrix<Element>::to_string() const noexcept
{
	const std::string NEW_LINE = "\n";
	const std::string TAB = "\t";
	const std::string OPEN_ACCOLADE = "{";
	const std::string CLOSE_ACCOLADE = "}";
	const std::string COLON = ",";
	const std::string SPACE = " ";

	const std::string START_OF_TABLE = OPEN_ACCOLADE + NEW_LINE;
	const std::string START_OF_ROW = TAB + OPEN_ACCOLADE;
	const std::string SEPARATE_COLUMN = COLON + SPACE;
	const std::string END_OF_ROW = CLOSE_ACCOLADE + COLON + NEW_LINE;
	const std::string END_OF_TABLE = NEW_LINE + CLOSE_ACCOLADE;

	std::string result = START_OF_TABLE;
	for (const auto& row_of_table : table)
	{
		result += START_OF_ROW;

		for (const auto& elem : row_of_table)
			result += std::to_string(elem) + SEPARATE_COLUMN;

		result.erase(result.end() - 2);
		result += END_OF_ROW;
	}
	result.erase(result.end() - 2);
	result += END_OF_TABLE;
	return result;
}

template <IsMatrixableElement Element>
Matrix<Element>::operator std::string() const noexcept
{
	return to_string();
}

template <IsMatrixableElement Element>
std::ostream& operator<<(std::ostream& os, const Matrix<Element>& matrix)
{
	os << matrix.to_string();
	return os;
}

template <IsMatrixableElement Element>
Matrix<Element> Matrix<Element>::transpose() const noexcept
{
	TableType ans_table(number_of_col, RowType(number_of_row));
	for (int i = 0; i < number_of_row; ++i)
		for (int j = 0; j < number_of_col; ++j)
			ans_table[j][i] = table[i][j];
	return Matrix<Element>(ans_table);
}

template <IsMatrixableElement Element>
Matrix<Element> Matrix<Element>::inverse() const
{
	if (number_of_row != number_of_col)
		throw std::invalid_argument("the matrix should be square!");

	TableType gauss_table = table;
	TableType inverse_table(number_of_row, RowType(number_of_col, Element()));
	for (size_t i : std::views::iota(0LLU, number_of_col))
		inverse_table[i][i] = Element(1);

	for (size_t col_index : std::views::iota(0LLU, number_of_col))
	{
		// Select row
		size_t non_zero_row_index;
		bool can_find_non_zero_row = false;
		for (size_t i : std::views::iota(col_index, number_of_row))
		{
			if (gauss_table[i][col_index] != 0)
			{
				non_zero_row_index = i;
				can_find_non_zero_row = true;
				break;
			}
		}
		if (!can_find_non_zero_row)
		{
			throw std::invalid_argument("the matrix should not be the determinant equal to zero!");
		}
		std::swap(gauss_table.at(col_index), gauss_table.at(non_zero_row_index));
		std::swap(inverse_table.at(col_index), inverse_table.at(non_zero_row_index));

		const size_t SELECTED_ROW_INDEX = col_index;
		const RowType& SELECTED_GAUSS_ROW = gauss_table[SELECTED_ROW_INDEX];
		const RowType& SELECTED_INVERSE_ROW = inverse_table[SELECTED_ROW_INDEX];

		// update other row
		for (size_t row_index : std::views::iota(0LLU, number_of_col) |
						std::views::filter([col_index](size_t i) { return i != col_index; }))
		{
			RowType& current_gauss_row = gauss_table[row_index];
			RowType& current_inverse_row = inverse_table[row_index];
			Element coefficient = -current_gauss_row[col_index] / SELECTED_GAUSS_ROW[col_index];

			if (coefficient == 0)
				continue;

			for (size_t i : std::views::iota(0LLU, number_of_col))
			{
				current_gauss_row[i] += coefficient * SELECTED_GAUSS_ROW[i];
				current_inverse_row[i] += coefficient * SELECTED_INVERSE_ROW[i];
			}
		}

		// update selected row
		RowType& selected_gauss_row = gauss_table[SELECTED_ROW_INDEX];
		RowType& selected_inverse_row = inverse_table[SELECTED_ROW_INDEX];
		Element coefficient = 1 / selected_gauss_row[col_index];
		for (size_t i : std::views::iota(0LLU, number_of_col))
		{
			selected_gauss_row[i] *= coefficient;
			selected_inverse_row[i] *= coefficient;
		}
	}

	return Matrix<Element>(inverse_table);
}

#endif
