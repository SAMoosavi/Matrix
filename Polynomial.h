#ifndef MATRIX_POLYNOMIAL_H
#define MATRIX_POLYNOMIAL_H

#include <vector>
#include <string>
#include <cstdint>
#include <cfloat>

class Polynomial {
public:
    typedef std::vector<long double> PolynomialRoot;

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

    Polynomial(const Polynomial& another);

    Polynomial(Polynomial &&another) noexcept;

    Polynomial &operator+=(const Polynomial &another);

    Polynomial &operator-=(const Polynomial &another);

    Polynomial &operator*=(const Polynomial &another);

    Polynomial &operator/=(const Monomial &another);

    Polynomial &power_equal(const uint64_t &power);

    Polynomial &operator=(const Polynomial &another) = default;

    inline Polynomial operator+(const Polynomial &another) const;

    inline Polynomial operator-(const Polynomial &another) const;

    inline Polynomial operator*(const Polynomial &another) const;

    inline Polynomial operator/(const Monomial &another) const;

    inline Polynomial power(const uint64_t &power) const;

    PolynomialRoot solve(double guess = 0, const uint16_t &max_iteration = 100, const uint16_t &precision = 6) const;

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

        Expression(const Expression& another);

        Expression(Expression&& another) noexcept;

        Expression& operator=(const Expression& anohter) = default;

        Expression &operator+=(const Expression &expression);

        Expression &operator-=(const Expression &expression);

        Expression &operator*=(const Expression &expression);

        Expression &operator/=(const Expression &expression);

        inline Expression &power_equal(const int64_t &pow);

        inline Expression operator+(const Expression &expression) const;

        inline Expression operator-(const Expression &expression) const;

        inline Expression operator*(const Expression &expression) const;

        inline Expression operator/(const Expression &expression) const;

        inline Expression power(const int64_t &pow) const;

        bool is_similar_terms(const Expression &expression) const;

        inline double get_constant() const;

//        it needs to added set_variables
        inline void set_constant(double constant);

        inline void increase_power();

        inline void decrease_power();

        inline const std::vector<Variable> &get_variables() const;

        long double set_value(const std::vector<std::pair<char, double>> &values) const;

        long double set_value(const std::pair<char, double> &value) const;

        inline bool operator==(const Expression &expression) const;

        static bool compare_expressions_by_power(const Expression &first, const Expression &second);

    private:
        double constant;
        std::vector<Variable> variables;

        static inline constexpr bool is_alpha(const char &ch) noexcept;

        static inline constexpr bool is_number(const char &ch) noexcept;

        static int64_t find_number(size_t &index, const std::string &variables);

        void check_expression();
    };

    struct NewtonOutput {
        bool is_repeated;
        long double root;

        explicit NewtonOutput(long double root = LDBL_MAX, bool is_repeated = false) :
                root(root),
                is_repeated(is_repeated) {}
    };

    typedef std::vector<Polynomial::Variable> PolynomialVariableMaxPower;

    std::vector<Expression> all_expressions;

    explicit Polynomial(Expression expression);

    explicit Polynomial(std::vector<Expression> expressions);

    static void delete_repeated_expressions(std::vector<Expression> &expression);

    static PolynomialVariableMaxPower createvariables(const std::vector<int64_t> &alphabets);

    static inline bool compare_with_precision(const long double &num1, const long double &num2, const int &precision);

    static int32_t create_random_number(const int32_t &max_value = INT32_MAX);

    std::vector<Expression> calculate_quotient(const long double &root) const;

    static const Expression &find_expression(const std::vector<Expression> &expressions, const char &variable,
                                             const int64_t &power = INT64_MIN);

    static size_t find_index(const std::vector<Expression> &expressions, const char &variable,
                             const int64_t &power = INT64_MIN);

    // it needs to have an enable_if
    template<typename T>
    static inline T round(const T &number, const uint16_t &precision);

    Expression *find_similar_expression(const Expression &expression) const;

    bool check_solve_validation(const PolynomialVariableMaxPower &variableMaxPower) const;

    PolynomialVariableMaxPower find_variables_and_max_power() const;

    Expression *find_expression_by_power(int64_t target_power) const;

    PolynomialRoot solve_linear_equation(const uint16_t &precision) const;

    PolynomialRoot solve_quardatic_equation(const uint16_t &precision) const;

    PolynomialRoot solve_greater_equation(double guess, const uint16_t &max_iteration, const uint16_t &precision) const;

    NewtonOutput solve_by_newton_technique(double guess, const uint16_t &max_iteration,
                                           const uint16_t &precision) const;

    long double solve_by_fixed_point_technique(double guess,
                                               const uint16_t &max_iteration,
                                               const uint16_t &precision) const;

    int64_t calculate_constant_of_derivated(int64_t power, uint64_t degree) const;

    inline Polynomial create_g_function() const;

    inline void save_newton_answer(PolynomialRoot &result, const Polynomial::NewtonOutput &answer,
                                   Polynomial &polynomial) const;

    inline void save_fixed_point_answer(PolynomialRoot &result, long double answer, Polynomial &polynomial) const;
};

#include "Polynomial-inl.h"

#endif //MATRIX_POLYNOMIAL_H