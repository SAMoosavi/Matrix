#include <iostream>
#include <ranges>
#include <vector>

#include "concept.h"
#include "matrix.h"

using namespace std;


template <Containerable Container, typename Element = typename Container::value_type>
	requires Multiplicationable<Element> && AssignSumable<Element>
constexpr Element multiplication(const Container& lhs, const Container& rhs)
{
	if (lhs.size() != rhs.size())
		throw invalid_argument("Cannot multiply spans of different sizes");
	Element result{};
	for (size_t i = 0; i < lhs.size(); ++i)
		result += lhs[i] * rhs[i];

	return result;
}

template <Containerable Container, typename Element = typename Container::value_type>
constexpr Element operator*(const Container& lhs, const Container& rhs)
{
	return multiplication(lhs, rhs);
}

template <Containerable Container, typename Element = typename Container::value_type>
	requires Sumable<Element>
constexpr Container sum(const Container& lhs, const Container& rhs)
{
	if (lhs.size() != rhs.size())
		throw invalid_argument("Cannot sum spans of different sizes");
	Container result(lhs.size());
	for (size_t index = 0; index < lhs.size(); ++index)
		result[index] = rhs[index] + lhs[index];
	return result;
}

template <Containerable Container, typename Element = typename Container::value_type>
constexpr Container operator+(const Container& lhs, const Container& rhs)
{
	return sum(lhs, rhs);
}


class W {
public:
	W()
		: num(0)
	{
	}

	W(int num)
		: num(num)
	{
	}

	W operator*(const W& other) const
	{
		return W{num * other.num};
	}

	W operator+(const W& other) const
	{
		return W{num + other.num};
	}

	W& operator+=(const W& other)
	{
		num += other.num;
		return *this;
	}

	W& operator*=(const W& other)
	{
		num *= other.num;
		return *this;
	}

	[[nodiscard]] int get() const
	{
		return num;
	}

private:
	int num;
};


template <typename T>
void print(const T& a)
{
	if constexpr (ranges::range<T>) {
		for (const auto& aa: a) {
			print(aa);
		}
		cout << endl;
	} else {
		cout << a.get() << " ";
	}
}

int main()
{
	vector<vector<W>> a = {
			{1, 2, 2, 2},
			{3, 2, 2, 4},
			{3, 2, 2, 4},
	};
	vector<vector<W>> b = {
			{1, 2, 2, 2},
			{3, 2, 2, 4},
			{3, 2, 2, 4},
			{3, 2, 2, 4},
	};
	Matrix<W> v1(a);
	Matrix<W> v2 = b;

	Matrix<W> po = v1 * v2;
	v1 *= v2;
	print(po.get_table());
	print(v1.get_table());
	Matrix<int> v4(vector<vector<int>>({{0, 2}, {1, 3}}));
	cout << v4.determinant();
	Matrix<W> v3 = 5 * v1 + v2 * 10;
	print(v3.get_table());

	return 0;
}
