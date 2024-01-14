#include <string>
#include "parsing_result.h"
#include "oper.h"

namespace uthef {
    struct operation {
        char32_t chr;
        size_t left, right;
        unsigned int position;
        oper op;
        operation(char32_t chr, size_t left, unsigned int position, oper& op)
            : chr(chr), left(left), position(position), right(0), op(op) {}
    };

    static bool push_number(double number, operation* last_operation, std::vector<double>& operands) {
        if (last_operation == nullptr) {
            operands.push_back(number);

            if (operands.size() > 1) return false;

            return true;
        }

        if (last_operation->right != 0) {
            return false;
        }

        operands.push_back(number);
        last_operation->right = operands.size() - 1;

        return true;
    }

    static parsing_result<double> parse_number(const char32_t* exp, unsigned int& cur) {
        size_t start = cur;
        char32_t c = *(exp + cur++);

        std::string str;
        str.push_back((char)c);

        bool e = false,
             sign = false,
             num = isdigit((int)c),
             dot = c == '.',
             num_after_e = false;

         char32_t last_c = c;

        for (; (c = *(exp + cur)) != '\0'; cur++, last_c = c ) {

            if (isdigit((int)c)) {
                if (e) num_after_e = true;
                else num = true;

                str.push_back((char)c);

                continue;
            }

            switch (c) {
                case ',':
                    if (!e && last_c != ',' && !dot) continue;
                    else return {INVALID_NUMBER_FORMAT, start};
                case '.':
                    if (dot || e || last_c == ',') return {INVALID_NUMBER_FORMAT, start};

                    dot = true;
                    str.push_back((char)c);
                    continue;
                case 'e':
                    if (e) return {INVALID_NUMBER_FORMAT, start };

                    str.push_back((char)c);
                    e = true;
                    continue;
                case '+':
                case '-':
                    if (!e || sign || num_after_e) break;

                    str.push_back((char)c);
                    sign = true;
                    continue;
                default:
                    break;
            }

            break;
        }

        cur--;

        if ((dot && !num) || (e && !num_after_e) || last_c == ',') return {INVALID_NUMBER_FORMAT, start};

        return std::stod(str);
    }
}