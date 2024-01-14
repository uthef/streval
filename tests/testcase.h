#ifndef STREVAL_TESTCASE_H
#define STREVAL_TESTCASE_H

#include "parsing_result.h"

struct testcase {
private:
    const char* expression;
    uthef::parsing_result<double> expected_result;
public:
    testcase(const char* expression, uthef::parsing_result<double> expected_result)
            : expression(expression), expected_result(expected_result) {

    }

    testcase(const char* expression, double value)
            : expression(expression), expected_result(uthef::parsing_result<double>(value)) {

    }

    void check(uthef::scope& scope) {
        auto xpstr = string(expression);
        auto result = uthef::streval(std::u32string(xpstr.begin(), xpstr.end()).c_str(), scope);

        if (result.error().has_value() && !expected_result.error().has_value()) {
            std::cerr << "ERROR: ";
            std::cerr << "code " << result.error().value() << " at " << result.error_at().value();
            std::cerr << " (" << xpstr << ")" << std::endl;
            throw;
        }

        if (result.error().has_value()) {
            if (result.error().value() != expected_result.error().value()) {
                std::cerr << "ERROR: ";
                std::cerr << "error code " << result.error().value() << " != " << expected_result.error().value();
                std::cerr << " (" << xpstr << ")" << std::endl;
                throw;
            }

            if (result.error_at().value() != expected_result.error_at().value()) {
                std::cerr << "ERROR: ";
                std::cerr << "error position " << result.error_at().value() << " != " << expected_result.error_at().value();
                std::cerr << " (" << xpstr << ")" << std::endl;
                throw;
            }
        }


        if (!result.number().has_value()) return;

        if (!expected_result.number().has_value()) {
            std::cerr << "ERROR: ";
            std::cerr << "expected a parsing error" << " (" << xpstr << ")" << std::endl;
            throw;
        }

        if (result.number().value() != expected_result.number().value()) {
            std::cerr << "ERROR: ";
            std::cerr << xpstr << " != " << expected_result.number().value() << std::endl;
            throw;
        }
    }
};

#endif //STREVAL_TESTCASE_H
