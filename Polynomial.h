#ifndef MATRIX_POLYNOMIAL_H
#define MATRIX_POLYNOMIAL_H

#include <vector>
#include <string>
#include <cstdint>

class Polynomial {
public:
    typedef std::vector<double> PolynomialRoot;

    struct Variable {
        char variable;
        int64_t power;

        inline bool operator==(const Variable &another) const;

        explicit Variable(char variable, int64_t power = 1) :
                variable(variable),
                power(power) {}
    };

    struct Monomial {
        double constant;
        std::vector<Variable> variables;

        explicit Monomial(double constant, std::vector<Variable> variables = {}) :
                constant(constant),
                variables(std::move(variables)) {}
    };

    Polynomial(double constant, const Polynomial &polynomial, const int64_t &power = 1);

    Polynomial(double constant, const char variable, int64_t power);

    explicit Polynomial(Monomial monomial);

    explicit Polynomial(std::vector<Monomial> polynomial);

    explicit Polynomial(double constant);

    Polynomial &operator+=(const Polynomial &another);

    Polynomial &operator-=(const Polynomial &another);

    Polynomial &operator*=(const Polynomial &another);

    Polynomial &operator/=(const Polynomial &another);

    Polynomial &power_equal(const int64_t &power);

    Polynomial &operator=(const Polynomial &another) = default;

    Polynomial operator+(const Polynomial &another) const;

    Polynomial operator-(const Polynomial &another) const;

    Polynomial operator*(const Polynomial &another) const;

    Polynomial operator/(const Polynomial &another) const;

    Polynomial power(const int64_t &power) const;

    PolynomialRoot solve(double guess = 0) const;

    Polynomial derivate(uint64_t degree) const;

    long double set_value(const std::vector<std::pair<char, double>> &values) const;

    long double set_value(const std::pair<char, double> &values) const;

    friend inline std::ostream &operator<<(std::ostream &os, const Polynomial &polynomial);

private:
    class Expression {
    public:
        Expression(double constant, const char variable, int64_t power);

        Expression(double constant, std::vector<Variable> variables);

        explicit Expression(double constant);

        Expression &operator+=(const Expression &expression);

        Expression &operator-=(const Expression &expression);

        Expression &operator*=(const Expression &expression);

        Expression &operator/=(const Expression &expression);

        inline Expression &power_equal(const int64_t &pow);

        Expression operator+(const Expression &expression) const;

        Expression operator-(const Expression &expression) const;

        Expression operator*(const Expression &expression) const;

        Expression operator/(const Expression &expression) const;

        inline Expression power(const int64_t &pow) const;

        bool is_similar_terms(const Expression &expression) const;

        inline double get_constant() const;

        inline const std::vector<Variable> &get_variables() const;

        long double set_value(const std::vector<std::pair<char, double>> &values) const;

        long double set_value(const std::pair<char, double> &value) const;

        inline bool operator==(const Expression &expression) const;

    private:
        double constant;
        std::vector<Variable> variables;

        static inline constexpr bool is_alpha(const char &ch) noexcept;

        static inline constexpr bool is_number(const char &ch) noexcept;

        static int64_t find_number(size_t &index, const std::string &variables);

        void check_expression();
    };

    typedef std::vector<Polynomial::Variable> PolynomialVariableMaxPower;

    std::vector<Expression> all_expressions;

    explicit Polynomial(Expression expression);

    explicit Polynomial(std::vector<Expression> expressions);

    static void delete_repeated_expressions(std::vector<Expression> &expression);

    static PolynomialVariableMaxPower createvariables(const std::vector<int64_t> &alphabets);

    static inline bool compare_with_precision(const long double &num1, const long double &num2, const int &precision);

    static int32_t create_random_number(const int32_t &max_value = INT32_MAX);

    Expression *find_similar_expression(const Expression &expression) const;

    bool check_solve_validation(const PolynomialVariableMaxPower &variableMaxPower) const;

    PolynomialVariableMaxPower find_variables_and_max_power() const;

    Expression *find_expression_bypower(int64_t target_power) const;

    PolynomialRoot solve_linear_equation() const;

    PolynomialRoot solve_quardatic_equation() const;

    PolynomialRoot find_cubic_roots(double delta, double p, double q, double a) const;

    PolynomialRoot solve_cubic_equation() const;

    PolynomialRoot solve_by_newton_technique(double guess) const;

    int64_t calculate_constant_of_derivated(int64_t power, uint64_t degree) const;
};

#include "Polynomial-inl.h"

#endif //MATRIX_POLYNOMIAL_H