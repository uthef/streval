#ifndef STREVAL_SCOPE_H
#define STREVAL_SCOPE_H

#include "identifier.h"
#include "oper.h"

#include <string>
#include <map>
#include <optional>
#include <functional>
#include <vector>
#include <iostream>

using std::string, std::function, std::optional, std::map, std::pair, std::vector;

namespace uthef {
    class scope {
    private:
        map<string, double> constants;
        map<string, function<double(double)>> functions;
        map<char32_t, oper> operators;
        static bool is_valid_op(char32_t c);
    public:
        explicit scope(bool empty = false);
        void put(identifier id, double value);
        optional<double> opt_const(identifier id);
        double get_const(identifier id);
        bool remove_const(identifier id);
        void clear_consts();
        void put(identifier id, double(*func)(double));
        optional<double> call(identifier id, double arg);
        optional<oper> get_oper(char32_t _operator);
        bool remove_func(identifier id);
        void clear_funcs();
        void clear_names();
        void put(vector<pair<identifier, double>>& pairs);
        void put(vector<pair<identifier, function<double(double)>>>& pairs);
        void put(char32_t _operator, double(*func)(double, double), unsigned short priority = level::LOW);
        bool remove_operator(char32_t _operator);
    };
}

#endif //STREVAL_SCOPE_H
