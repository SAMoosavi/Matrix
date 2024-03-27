#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include <iostream>
#include <ranges>
#include <vector>

#include "concept.h"

using namespace std;

template <Elementable Element>
class Matrix {
private:
	typedef vector<Element> RowType;
	typedef vector<RowType> TableType;

public:
	Matrix();

	Matrix(size_t row, size_t col);

	template <template <Containerable> typename Container>
	Matrix(const Container<Container<Element>>& matrix);

	TableType get_table() const
	{
		return table;
	}

	Matrix<Element> sum(const Matrix<Element>& other) const;
	Matrix<Element> operator+(const Matrix<Element>& other) const;
	Matrix<Element>& operator+=(const Matrix<Element>& other);

	Matrix<Element> operator-() const;

	Matrix<Element> submission(const Matrix<Element>& other) const;
	Matrix<Element> operator-(const Matrix<Element>& other) const;
	Matrix<Element>& operator-=(const Matrix<Element>& other);

	Matrix<Element> multiple(const Matrix<Element>& other) const;
	Matrix<Element> operator*(const Matrix<Element>& other) const;
	Matrix<Element>& operator*=(const Matrix<Element>& other);

	Matrix<Element> multiple(const auto& other) const;
	Matrix<Element> operator*(const auto& other) const;
	Matrix<Element>& operator*=(const auto& other);

	Element at(size_t row_index, size_t col_index);

	RowType operator[](size_t idx) const;

	Element determinant() const;

private:
	RowType& operator[](size_t idx);

	size_t row;
	size_t col;
	TableType table;
};

template <Elementable Element>
Matrix<Element> operator*(const auto& number, const Matrix<Element>& matrix);

#include "matrix-tmp.h"

#endif
