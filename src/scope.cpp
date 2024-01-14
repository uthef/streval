#include "scope.h"
#include "oper.h"

#include <stdexcept>

namespace uthef {
    scope::scope(bool empty) {
        if (empty) return;

        put('+', [](double a, double b) { return a + b; }, level::LOW);
        put('-', [](double a, double b) { return a - b; }, level::LOW);
        put('*', [](double a, double b) { return a * b; }, level::HIGH);
        put('/', [](double a, double b) { return a / b; }, level::HIGH );
    }

    void scope::put(identifier id, double value) {
        constants[id.value()] = value;
    }

    std::optional<double> scope::opt_const(identifier id) {
        if (constants.count(id.value()) == 0) return {};
        return constants[id.value()];
    }

    double scope::get_const(identifier id) {
        return constants[id.value()];
    }

    bool scope::remove_const(identifier id) {
        return constants.erase(id.value()) > 0;
    }

    void scope::clear_consts() {
        constants.clear();
    }

    void scope::put(uthef::identifier id, double (*func)(double)) {
        functions[id.value()] = func;
    }

    std::optional<double> scope::call(identifier id, double arg) {
        if (functions.count(id.value()) == 0) return {};

        return functions[id.value()](arg);
    }

    bool scope::remove_func(uthef::identifier id) {
        return functions.erase(id.value()) > 0;
    }

    void scope::clear_funcs() {
        functions.clear();
    }

    void scope::clear_names() {
        clear_consts();
        clear_funcs();
    }

    void scope::put(vector<pair<identifier, double>>& pairs) {
        for (auto pair : pairs) {
            constants[pair.first.value()] = pair.second;
        }
    }

    void scope::put(vector<pair<identifier, function<double(double)>>>& pairs) {
        for (auto pair : pairs) {
            functions[pair.first.value()] = pair.second;
        }
    }

    void scope::put(char32_t _operator, double(*func)(double, double), unsigned short priority) {
        if (priority > 2) throw std::invalid_argument("operator priority level must be either 0 or less than 3");

        if (!is_valid_op(_operator))
            throw std::invalid_argument("unallowed operator character");

        remove_operator(_operator);

        operators.insert(std::make_pair(_operator, oper(func, (level)priority)));
    }

    bool scope::remove_operator(char32_t _operator) {
        auto iter = operators.find(_operator);
        bool found = iter != std::end(operators);

        if (found) operators.erase(_operator);

        return found;
    }

    optional<oper> scope::get_oper(char32_t _operator) {
        auto pair = operators.find(_operator);

        if (pair != end(operators)) {
            return pair->second;
        }

        return { };
    }

    bool scope::is_valid_op(char32_t c) {
        int code = (int)c;

        if (isspace(code)) return false;
        if (isalnum(code)) return false;

        if (c == '.' || c == ',' || c == '\0' || c == '(' || c == ')') return false;

        return true;
    }
}