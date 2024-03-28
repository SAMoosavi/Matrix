#include <stdexcept>
#include <cmath>
#include <format>
#include <cfloat>
#include "Polynomial.h"

// Expression
int64_t Polynomial::Expression::find_number(size_t &index, const std::string &variables) {
    std::string result;
    if (variables[index] == '-') {
        result += '-';
        ++index;
    }
    while (index < variables.size() && is_number(variables[index])) {
        result += variables[index];
        ++index;
    }
    return std::stoi(result);
}

bool Polynomial::Expression::is_similar_terms(const Expression &expression) const {
    bool result = true;
    if (variables.size() != expression.variables.size())
        result = false;
    else {
        std::vector<int16_t> all_alphabets(26, -1);
        for (int16_t i = 0; i < variables.size(); ++i)
            all_alphabets[variables[i].variable - 'a'] = i;
        int16_t temp;
        for (const auto &var: expression.variables) {
            temp = all_alphabets[var.variable - 'a'];
            if ((temp == -1) || (variables[temp].variable != var.variable ||
                                 (variables[temp].power) != (var.power))) {
                result = false;
                break;
            }
        }
    }

    return result;
}


Polynomial::Expression::Expression(double constant, const char variable, int64_t power) :
        constant(constant) {
    if (constant != 0 && power != 0)
        variables.emplace_back(variable, power);
}

Polynomial::Expression::Expression(double constant, std::vector<Variable> variables) :
        constant(constant) {
    if (constant != 0)
        this->variables = std::move(variables);
}

Polynomial::Expression::Expression(double constant) :
        constant(constant) {}

Polynomial::Expression &Polynomial::Expression::operator+=(const Expression &expression) {
    if (is_similar_terms(expression)) {
        constant += expression.constant;
        check_expression();
    } else
        throw std::invalid_argument("two expressions should have same variables and same powers.");

    return *this;
}

Polynomial::Expression &Polynomial::Expression::operator-=(const Expression &expression) {
    if (is_similar_terms(expression)) {
        constant -= expression.constant;
        check_expression();
    } else
        throw std::invalid_argument("two expressions should have same variables and same powers.");
    return *this;
}

Polynomial::Expression &Polynomial::Expression::operator*=(const Expression &expression) {
    constant *= expression.constant;
    std::vector<int16_t> alphas(26, -1);
    for (int16_t i = 0; i < variables.size(); ++i)
        alphas[variables[i].variable - 'a'] = i;

    for (const auto &var: expression.variables) {
        if (alphas[var.variable - 'a'] == -1)
            variables.emplace_back(var.variable, var.power);
        else
            variables[alphas[var.variable - 'a']].power += var.power;
    }
    check_expression();
    return *this;
}

Polynomial::Expression &Polynomial::Expression::operator/=(const Expression &expression) {
    if (expression.get_constant() != 0) {
        constant /= expression.constant;
        std::vector<int16_t> alphas(26, -1);
        for (int16_t i = 0; i < variables.size(); ++i)
            alphas[variables[i].variable - 'a'] = i;
        for (const auto &var: expression.variables) {
            if (alphas[var.variable - 'a'] == -1)
                variables.emplace_back(var.variable, -1 * var.power);
            else
                variables[alphas[var.variable - 'a']].power -= var.power;
        }
        check_expression();
    }
    return *this;
}

void Polynomial::Expression::check_expression() {
    if (constant == 0)
        variables.clear();
    else {
        size_t i = 0;
        while (i < variables.size()) {
            if (variables[i].power == 0) {
                std::swap(variables[i], variables.back());
                variables.pop_back();
            } else
                ++i;
        }
    }
}

Polynomial::Expression Polynomial::Expression::operator+(const Expression &expression) const {
    return Expression(*this) += expression;
}

Polynomial::Expression Polynomial::Expression::operator-(const Expression &expression) const {
    return Expression(*this) -= expression;
}

Polynomial::Expression Polynomial::Expression::operator*(const Expression &expression) const {
    return Expression(*this) *= expression;
}

Polynomial::Expression Polynomial::Expression::operator/(const Expression &expression) const {
    return Expression(*this) /= expression;
}


long double Polynomial::Expression::set_value(const std::vector<std::pair<char, double>> &values) const {
    if (variables.size() > values.size())
        throw std::invalid_argument(std::format("there should be {} at least argument.", variables.size()));
    std::vector<double> alphabets(26, INT64_MIN);
    for (const auto &pair: values)
        alphabets[pair.first - 'a'] = pair.second;

    long double result = 0;
    double temp;
    for (auto &var: variables) {
        temp = alphabets['a' + var.variable];
        if (temp == INT64_MIN)
            throw std::invalid_argument(std::format("not specified {} in values.", var.variable));
        result += powf64x(temp, var.power);
    }
    result *= constant;

    return result;
}

long double Polynomial::Expression::set_value(const std::pair<char, double> &value) const {
    if (variables.size() > 1)
        throw std::invalid_argument(std::format("there should be {} at least argument.", variables.size()));

    long double result = 0;
    if (!variables.empty()) {
        for (auto &var: variables) {
            if (var.variable != value.first)
                throw std::invalid_argument(std::format("not specified {} in values.", var.variable));
            result += powf64x(value.second, var.power);
        }
        result *= constant;
    } else
        result = constant;

    return result;
}


// Polynomial
void Polynomial::delete_repeated_expressions(std::vector<Expression> &expressions) {
    for (size_t i = 0; i < expressions.size(); ++i) {
        size_t j = i + 1;
        while (j < expressions.size()) {
            if (expressions[i].is_similar_terms(expressions[j])) {
                expressions[i] += expressions[j];
                std::swap(expressions[j], expressions.back());
                expressions.pop_back();
            } else
                ++j;
        }
    }
}

Polynomial::PolynomialVariableMaxPower Polynomial::createvariables(const std::vector<int64_t> &alphabets) {
    Polynomial::PolynomialVariableMaxPower result;
    for (size_t i = 0; i < alphabets.size(); ++i) {
        if (alphabets[i] != INT64_MIN)
            result.emplace_back('a' + i, alphabets[i]);
    }
    return result;
}

int32_t Polynomial::create_random_number(const int32_t &max_value) {
    srand(time(0));
    return rand() % max_value;
}

Polynomial::Polynomial(double constant, const Polynomial &polynomial, const int64_t &power) :
        all_expressions(polynomial.all_expressions) {
    if (constant != 0 || power != 0) {
        this->power_equal(power);
        Expression temp(constant);
        if (constant != 1) {
            for (auto &expr: all_expressions)
                expr *= temp;
        }
    }
}

Polynomial::Polynomial(double constant, const char variable, int64_t power) :
        all_expressions(std::vector<Polynomial::Expression>{{constant, variable, power}}) {}

Polynomial::Polynomial(double constant):
        all_expressions(std::vector<Expression>{Expression(constant)}) {}

Polynomial::Polynomial(Polynomial::Expression expression):
        all_expressions(std::vector<Expression>{std::move(expression)}) {};

Polynomial::Polynomial(std::vector<Expression> expressions) :
        all_expressions(std::move(expressions)) {}

Polynomial::Polynomial(Polynomial::Monomial monomial) {
    if (monomial.constant != 0)
        all_expressions.emplace_back(monomial.constant, std::move(monomial.variables));
}

Polynomial::Polynomial(std::vector<Monomial> polynomial) {
    for (auto & monomial: polynomial) {
        if (monomial.constant != 0)
            all_expressions.emplace_back(monomial.constant, std::move(monomial.variables));
    }
}

Polynomial::Expression *Polynomial::find_similar_expression(const Expression &expression) const {
    Expression *result = nullptr;
    for (auto &expr: all_expressions) {
        if (expr.is_similar_terms(expression)) {
            result = (Expression *) &expr;
            break;
        }
    }

    return result;
}

Polynomial &Polynomial::operator+=(const Polynomial &another) {
    Expression *temp;
    for (auto &expr: another.all_expressions) {
        temp = find_similar_expression(expr);
        if (temp)
            *temp += expr;
        else
            all_expressions.emplace_back(expr);
    }
    return *this;
}

Polynomial &Polynomial::operator-=(const Polynomial &another) {
    Expression *temp;
    for (auto &expr: another.all_expressions) {
        temp = find_similar_expression(expr);
        if (temp)
            *temp -= expr;
        else {
            all_expressions.emplace_back(Expression(-1) *= expr);
        }
    }

    return *this;
}

Polynomial &Polynomial::operator*=(const Polynomial &another) {
    Polynomial result = Polynomial(0);
    Expression temp(0);
    for (const auto &expr: another.all_expressions) {
        for (const auto &temp_expr: all_expressions) {
            temp = temp_expr * expr;
            result += Polynomial(std::move(temp));
        }
    }
    *this = result;
    return *this;
}

Polynomial &Polynomial::operator/=(const Polynomial &another) {
    Polynomial result = Polynomial(0);
    Expression temp(0);
    for (const auto &expr: another.all_expressions) {
        for (const auto &temp_expr: all_expressions) {
            temp = temp_expr / expr;
            result += Polynomial(std::move(temp));
        }
    }
    *this = result;
    return *this;
}

Polynomial &Polynomial::power_equal(const int64_t &power) {
    Polynomial another = *this;
    size_t i = 1;
    while (i * 2 <= std::abs(power)) {
        *this *= *this;
        i *= 2;
    }
    while (i < std::abs(power)) {
        *this *= another;
        ++i;
    }

    if (power < 0) {
        for (auto &expr: all_expressions)
            expr.power_equal(-1);
    }
    return *this;
}

Polynomial Polynomial::operator+(const Polynomial &another) const {
    return Polynomial(*this) += another;
}

Polynomial Polynomial::operator-(const Polynomial &another) const {
    return Polynomial(*this) -= another;
}

Polynomial Polynomial::operator*(const Polynomial &another) const {
    return Polynomial(*this) *= another;
}

Polynomial Polynomial::operator/(const Polynomial &another) const {
    return Polynomial(*this) /= another;
}

Polynomial Polynomial::power(const int64_t &power) const {
    Polynomial another = *this;
    another.power_equal(power);
    return another;
}

bool Polynomial::check_solve_validation(const Polynomial::PolynomialVariableMaxPower &variableMaxPower) const {
    bool result = false;
    if (variableMaxPower.empty()) {
        // need to test when it completed
        if (all_expressions.begin()->get_constant() == 0)
            throw std::runtime_error("this Polynomial has infinity answers.");
        else
            throw std::runtime_error("there is no answer.");
    } else if (variableMaxPower.size() > 1)
        throw std::runtime_error("there is no support to solving multi-variable polynomials.");
    else
        result = true;

    return result;
}

Polynomial::PolynomialRoot Polynomial::solve(double guess) const {
    PolynomialVariableMaxPower variableMaxPower = find_variables_and_max_power();
    PolynomialRoot roots;
    if (check_solve_validation(variableMaxPower)) {
        // it need to test that the polynomial save unique expressions
        if (variableMaxPower.begin()->power == 1)
            roots = solve_linear_equation();
        else if (variableMaxPower.begin()->power == 2)
            roots = solve_quardatic_equation();
        else if (variableMaxPower.begin()->power == 3)
            roots = solve_cubic_equation();
        else
            roots = solve_by_newton_technique(guess);
    }
    return roots;
}

Polynomial::PolynomialVariableMaxPower Polynomial::find_variables_and_max_power() const {
    // check  when there is constant only
    PolynomialVariableMaxPower result;
    std::vector<int64_t> alphabets(26, INT64_MIN);
    for (const auto &expr: all_expressions) {
        if (expr.get_constant() != 0) {
            for (const auto &var: expr.get_variables()) {
                if (var.power > alphabets[var.variable - 'a'])
                    alphabets[var.variable - 'a'] = var.power;
            }
        }
    }

    result = createvariables(alphabets);

    return result;
}

Polynomial::Expression *Polynomial::find_expression_bypower(int64_t target_power) const {
    Expression *result = nullptr;
    if (target_power == 0) {
        for (auto &expr: all_expressions) {
            if (expr.get_variables().empty() || (expr.get_variables().begin()->power == target_power)) {
                result = (Expression *) &expr;
                break;
            }
        }
    } else {
        for (auto &expr: all_expressions) {
            if (!expr.get_variables().empty() && (expr.get_variables().begin()->power == target_power)) {
                result = (Expression *) &expr;
                break;
            }
        }
    }
    return result;
}

Polynomial::PolynomialRoot Polynomial::solve_linear_equation() const {
    // expressions need to be simplified
    PolynomialRoot result(1);
    Expression *constant = find_expression_bypower(0);
    Expression *onepower = find_expression_bypower(1);

    result[0] = -1 * constant->get_constant() / onepower->get_constant();

    return result;
}

Polynomial::PolynomialRoot Polynomial::solve_quardatic_equation() const {
    PolynomialRoot result;
    Expression *constant = find_expression_bypower(0);
    Expression *onepower = find_expression_bypower(1);
    Expression *twopower = find_expression_bypower(2);

    double delta = pow(onepower->get_constant(), 2) - 4 * twopower->get_constant() * constant->get_constant();

    if (compare_with_precision(delta, 0, 6)) {
        result.emplace_back(-1 * onepower->get_constant() / 2 * twopower->get_constant());
    } else if (delta > 0) {
        result.emplace_back((-1 * onepower->get_constant() + sqrt(delta)) / 2 * twopower->get_constant());
        result.emplace_back((-1 * onepower->get_constant() - sqrt(delta)) / 2 * twopower->get_constant());
    }

    return result;
}

Polynomial::PolynomialRoot Polynomial::solve_cubic_equation() const {
    Expression *constant = find_expression_bypower(0);
    Expression *onepower = find_expression_bypower(1);
    Expression *twopower = find_expression_bypower(2);
    Expression *threepower = find_expression_bypower(3);

    double a = twopower->get_constant() / threepower->get_constant();
    double b = onepower->get_constant() / threepower->get_constant();
    double c = constant->get_constant() / threepower->get_constant();

    double p = b - pow(a, 2) / 3;
    double q = 2 * pow(a, 3) / 27 - a * b / 3 + c;
    double delta = pow(q, 2) / 4 + pow(p, 3) / 27;

    return find_cubic_roots(delta, p, q, a);
}

Polynomial::PolynomialRoot Polynomial::find_cubic_roots(double delta, double p, double q, double a) const {
    PolynomialRoot result;
    double temp;
    // right now i just find Real answers not complexes
    if (delta > 0) {
        temp = pow((-1 * q / 2 + sqrt(delta)), 1.0 / 3) +
               pow((-1 * q / 2 - sqrt(delta)), 1.0 / 3) - a / 3;
        result.emplace_back(temp);
    } else if (compare_with_precision(delta, 0, 6)) {
        temp = pow(q / 2, 1.0 / 3) - a / 3;
        result.emplace_back(temp);
        temp = -2 * pow(q / 2, 1.0 / 3) - a / 3;
        result.emplace_back(temp);
    } else {
        double temp1 = 2.0 / sqrt(3) * sqrt(-1 * p);
        double temp2 = (1.0 / 3) * asin((3 * sqrt(3) * q / (2 * pow(-1 * p, 3.0 / 2))));
        temp = temp1 * sin(temp2) - a / 3;
        result.emplace_back(temp);
        temp = -1 * temp1 * sin(temp2 + M_PI / 3) - a / 3;
        result.emplace_back(temp);
        temp = temp1 * cos(temp2 + M_PI / 6) - a / 3;
        result.emplace_back(temp);
    }
    return result;
}

Polynomial::PolynomialRoot Polynomial::solve_by_newton_technique(double guess) const {
    Polynomial derivated = derivate(1);
    const char variable = find_variables_and_max_power().begin()->variable;
    long double previous_answer = LDBL_MIN_10_EXP, current_answer = guess;
    long double polynomial_answer, derivated_answer;
    while (!compare_with_precision(previous_answer, current_answer, 6)) {
        previous_answer = current_answer;
        polynomial_answer = this->set_value(std::make_pair(variable, previous_answer));
        derivated_answer = derivated.set_value(std::make_pair(variable, previous_answer));
        if (polynomial_answer == 0)
            break;
        else if (derivated_answer == 0) {
            // bug
            current_answer = create_random_number();
        } else {
            current_answer = previous_answer - (polynomial_answer / derivated_answer);
        }
    }

    return PolynomialRoot{static_cast<double>(current_answer)};
}

Polynomial Polynomial::derivate(uint64_t degree) const {
    // just derivate for single variables
    if (find_variables_and_max_power().size() > 1)
        throw std::invalid_argument("it can not calculate derivate of multi variable polynomials.");
    else {
        // need to check that a positive power can not be a negative power after derivation
        std::vector<Expression> expressions;
        double constant;
        Variable *temp;
        for (auto &expr: all_expressions) {
            temp = (Variable *) &expr.get_variables()[0];
            if (temp != nullptr) {
                constant = expr.get_constant() *
                           calculate_constant_of_derivated(temp->power, degree);
                expressions.emplace_back(constant, temp->variable, temp->power - degree);
            }
        }
        return Polynomial(std::move(expressions));
    }
}

int64_t Polynomial::calculate_constant_of_derivated(int64_t power, uint64_t degree) const {
    int64_t result = 1;
    while (degree > 0) {
        result *= power;
        --power;
        --degree;
    }
    return result;
}

long double Polynomial::set_value(const std::vector<std::pair<char, double>> &values) const {
    auto size = find_variables_and_max_power().size();
    if (size != values.size())
        throw std::invalid_argument(std::format("there should exist {} pair in values.", size));
    long double result = 0;
    for (auto &expr: all_expressions)
        result += expr.set_value(values);

    return result;
}

long double Polynomial::set_value(const std::pair<char, double> &value) const {
    auto size = find_variables_and_max_power().size();
    if (size > 1)
        throw std::invalid_argument(std::format("there should exist {} pair in values.", size));
    long double result = 0;
    for (auto &expr: all_expressions)
        result += expr.set_value(value);

    return result;
}

