#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include <iostream>
#include <ranges>
#include <vector>

#include "concept.h"

template <IsMatrixableElement Element>
class Matrix
{
private:
	typedef std::vector<Element> RowType;
	typedef std::vector<RowType> TableType;

public:
	Matrix() = default;

	Matrix(size_t row, size_t col);

	Matrix(const std::initializer_list<std::initializer_list<Element>>& matrix);

	template <template <Containerable> typename Container>
	explicit Matrix(const Container<Container<Element>>& matrix);

	[[nodiscard]] TableType get_table() const;
	[[nodiscard]] size_t get_number_of_row() const;
	[[nodiscard]] size_t get_number_of_col() const;

	template <typename OtherElement>
		requires SumableDifferentType<Element, OtherElement>
	Matrix<Element> sum(const Matrix<OtherElement>& other) const;
	template <typename OtherElement>
	Matrix<Element> operator+(const Matrix<OtherElement>& other) const;
	template <typename OtherElement>
	Matrix<Element>& operator+=(const Matrix<OtherElement>& other);

	Matrix<Element> operator-() const;

	template <typename OtherElement>
	Matrix<Element> submission(const Matrix<OtherElement>& other) const;
	template <typename OtherElement>
	Matrix<Element> operator-(const Matrix<OtherElement>& other) const;
	template <typename OtherElement>
	Matrix<Element>& operator-=(const Matrix<OtherElement>& other);

	template <typename OtherElement>
		requires MultiplableDifferentTypeReturnFirstType<Element, OtherElement>
	Matrix<Element> multiple(const Matrix<OtherElement>& other) const;
	template <typename OtherElement>
		requires(not IsMatrixable<OtherElement>) and MultiplableDifferentType<Element, OtherElement>
	Matrix<Element> multiple(const OtherElement& other) const;
	template <typename OtherElement>
	Matrix<Element> operator*(const OtherElement& other) const;
	template <typename OtherElement>
	Matrix<Element>& operator*=(const OtherElement& other);

	template <typename OtherElement>
	bool operator==(const Matrix<OtherElement>& other) const;

	Element at(size_t row_index, size_t col_index);

	RowType operator[](size_t idx) const;

	Element determinant() const;

	Matrix<Element> transpose() const noexcept;
	Matrix<Element> inverse() const;

	[[nodiscard]] inline std::string to_string() const noexcept;
	[[nodiscard]] inline operator std::string() const noexcept;

private:
	RowType& operator[](size_t idx);

	size_t number_of_row;
	size_t number_of_col;
	TableType table;
};

template <IsMatrixableElement Element>
std::ostream& operator<<(std::ostream& os, const Matrix<Element>& matrix);

template <typename Element, typename OtherElement>
	requires(not IsMatrixable<Element>) and
		(not IsMatrixable<OtherElement>) and MultiplableDifferentType<Element, OtherElement>
Matrix<Element> operator*(const OtherElement& number, const Matrix<Element>& matrix);

#include "matrix-tmp.h"

#endif
