#include <iostream>
#include <ranges>
#include <vector>

#include "Matrix.h"
#include "concept.h"
#include "Polynomial.h"

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

	template <typename T>
	W operator*(const T& other) const
	{
		return W{num * other};
	}


	template <typename T>
	W operator+(const T& other) const
	{
		return W{num + other};
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

	bool operator==(const int& other) const
	{
		return num == other;
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
	Matrix<W> v3 = v2 * 10;
	print(v3.get_table());
	Matrix<int> v4(vector<vector<int>>({
			{1, 2, 2, 2},
			{3, 2, 2, 4},
			{3, 2, 2, 4},
			{3, 2, 2, 4},
	}));
	cout << v4.determinant() << endl;
	Matrix<W> v5 = v2 + v4;
	print(v5.get_table());
	Matrix<W> v6 = v2 * v4;
	print(v6.get_table());
	Matrix<W> v7 = 10 * v2;
	print(v7.get_table());

    Polynomial::Monomial two(2);
    Polynomial::Monomial three(3);
    Polynomial::Monomial x(1, std::vector<Polynomial::Variable> {Polynomial::Variable('x')});

    Polynomial polynomial(std::vector<Polynomial::Monomial> {x, two});
    Polynomial polynomial1(std::vector<Polynomial::Monomial> {x, three});
    Polynomial polynomial2(1, polynomial, 2);
    Polynomial polynomial3(1, polynomial1, 2);
    Polynomial polynomial4(1, polynomial2 * polynomial3);

    std::cout << polynomial4 << '\n';

    auto result = polynomial4.solve(1000000);
    std::cout << polynomial4.set_value(make_pair('x', -3)) << '\n';
    for (const auto& ans: result) {
        std::cout << ans << '\t';
    }

  return 0;
}
