#include "streval.h"
#include "streval_utils.cpp"

namespace uthef {
    parsing_result<double> streval(const char32_t* expression, scope& scope, unsigned int& cur) {
        const int chain_count = 3;
        vector<operation> chains[chain_count];

        vector<double> operands;

        operation* last_op = nullptr;

        char32_t c;
        const unsigned int exp_start = cur;
        short multiplier = 1;

        // true if the closing parenthesis is either found or not needed
        bool parenthesis = cur == 0;

        for (; (c = *(expression + cur)) != '\0'; cur++) {
            int code = (int)c;
            const size_t it_start = cur;

            if (isspace(code)) continue;

            if (isdigit(code) || c == '.') {
               auto result = parse_number(expression, cur);
               if (!result.number().has_value()) return result;

               if (!push_number(result.number().value() * multiplier, last_op, operands)) {
                   return { OP_EXPECTED, it_start };
               }

               multiplier = 1;

               continue;
            }

            if (isalpha(code)) {
                auto result = parse_id(expression, scope, cur);
                if (result.error().has_value()) return result;

                if (!push_number(result.number().value() * multiplier, last_op, operands)) {
                    return { OP_EXPECTED, it_start };
                }

                multiplier = 1;

                continue;
            }

            if (operands.empty() && c == '-' && multiplier == 1) {
                multiplier = -1;
                continue;
            }

            if (c == '(') {
               cur++;
               auto result = streval(expression, scope, cur);
               if (result.error().has_value()) return result;

                if (!push_number(result.number().value() * multiplier, last_op, operands)) {
                    return { OP_EXPECTED, it_start };
                }

                multiplier = 1;

               continue;
            }

            if (c == ')') {
               if (parenthesis) return { UNEXPECTED_PARENTHESIS, cur };

               parenthesis = true;
               break;
            }

            auto result = scope.get_oper(c);

            if (result.has_value()) {
                if (operands.empty()) return { LEFT_MISSING, cur };

                auto chain_idx = result.value().priority_level();
                last_op = &chains[chain_idx].emplace_back(c, operands.size() - 1, cur, result.value());

                continue;
            }

            return { INVALID_CHAR, cur };
        }

        if (!parenthesis) {
            return {UNCLOSED_PARENTHESIS, exp_start - 1};
        }

        size_t erase_idx = operands.size() - 1;
        size_t offset = 0;

        for (int i = chain_count - 1; i >= 0; i--) {
            auto chain = chains[i];

            for (auto &operation: chain) {
                auto op = &operation;

                if (op->right == 0) return {RIGHT_MISSING, op->position};

                if (op->left >= erase_idx) {
                    if (offset > op->left) {
                        op->left = 0;
                        op->right = 1;
                    } else {
                        op->left -= offset;
                        op->right -= offset;
                    }
                }

                double value = op->op.calculate(operands[op->left], operands[op->right]);

                operands[op->right] = value;
                operands.erase(operands.begin() + (long)op->left);

                offset += 1;

                if (op->right < erase_idx) {
                    erase_idx = op->right;
                    erase_idx -= 1;
                }
            }
        }

        if (operands.empty() && multiplier == -1) return { LEFT_MISSING, cur - 1 };

        return operands.empty() ? 0 : operands[operands.size() - 1];
    }

    parsing_result<double> streval(const char32_t* expression, scope& scope) {
        unsigned int cursor = 0;
        return streval(expression, scope, cursor);
    }

    parsing_result<double> streval(const char32_t* expression) {
        auto scope = uthef::scope();
        return streval(expression, scope);
    }

    parsing_result<double> parse_id(const char32_t* exp, scope& scope, unsigned int& cur) {
        size_t start = cur;
        char32_t c = *(exp + cur++);

        std::string id;
        id.push_back((char)c);

        bool is_func = false;

        for (; (c = *(exp + cur)) != '\0'; cur++ ) {
            if (!isalnum((int)c)) {
                is_func = c == '(';
                break;
            }

            id.push_back((char)c);
        }

        if (is_func) {
            cur++;
            auto argres = streval(exp, scope, cur);

            if (argres.error().has_value()) {
                return argres;
            }

            auto funcres = scope.call(id.c_str(), argres.number().value());

            if (!funcres.has_value()) {
                return { UNKNOWN_FUNC, start, id };
            }

            return funcres.value();
        }

        auto res = scope.opt_const(id.c_str());

        cur--;

        if (res.has_value()) {
            return res.value();
        }
        else
            return { UNKNOWN_CONST, start, id };
    }
}
