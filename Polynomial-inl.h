#ifndef MATRIX_POLYNOMIAL_INL_H
#define MATRIX_POLYNOMIAL_INL_H

#include "Polynomial.h"
#include <cmath>
#include <iostream>

// Expression

constexpr bool Polynomial::Expression::is_alpha(const char &ch) noexcept {
    return ch - 'a' <= 'z' - 'a';
}

constexpr bool Polynomial::Expression::is_number(const char &ch) noexcept {
    return ch - '0' <= '9' - '0';
}

double Polynomial::Expression::get_constant() const {
    return constant;
}

const std::vector<Polynomial::Variable> &Polynomial::Expression::get_variables() const {
    return variables;
}

bool Polynomial::Expression::operator==(const Expression &expression) const {
    bool result = true;
    if (constant != expression.constant)
        result = false;
    else
        result = is_similar_terms(expression);
    return result;
}

Polynomial::Expression &Polynomial::Expression::power_equal(const int64_t &pow) {
    for (auto & var: variables)
        var.power *= pow;
    return *this;
}

Polynomial::Expression Polynomial::Expression::power(const int64_t &pow) const {
    Expression another = *this;
    another.power_equal(pow);
    return another;
}


bool Polynomial::Variable::operator==(const Polynomial::Variable &another) const {
    return (this->variable == another.variable) && (this->power == another.power);
}


// Polynomial
bool Polynomial::compare_with_precision(const long double &num1, const long double &num2, const int &precision) {
    long double diff = std::abs(num1 - num2);
    long double epsilon = std::pow(10, -precision);

    return diff < epsilon;
}

std::ostream &operator<<(std::ostream &os, const Polynomial &polynomial) {
    bool flag = false;
    for (auto& expr: polynomial.all_expressions){
        if (expr.get_constant() < 0)
            os << '-';
        else
            os << '+';
        if (flag)
            os << ' ';
        os << std::abs(expr.get_constant());
        for (auto &var: expr.get_variables())
            os << '*' << var.variable << '^' << var.power;

        os << ' ';
        flag = true;
    }
    return os;
}

#endif //MATRIX_POLYNOMIAL_INL_H
