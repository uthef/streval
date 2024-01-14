#include "streval.h"
#include <iostream>
#include "testcase.h"
#include <chrono>
#include <cmath>

using namespace std;

int main() {
    auto scope = uthef::scope();

    testcase valid_cases[] = {
            { "2 * 5 + 8 * 9", 82 },
            { "100 + 1 + 2 - 3", 100 },
            { "20 / 4 * 6 + 2", 32 },
            { "812 + 5 / 2 * 8", 832 },
            { "3.0 * 5. ^ .2e1 + 4", 79 },
            { "4 + 3 * 5 ^ 2 + 4", 83 },
            { "2 * PI", 2 * M_PI },
            { "sqrt(PI * 2) * 8e-1", sqrt(M_PI * 2) * 8e-1 },
            { "20 * 20 * 20", 8000 },
            { "sqrt( (-3 + 3.000 * 6.0) ^ 2 - 81) + 0.5000", 12.5 },
            { "-6,000+(-3,000)*(-2)", 0 },
            { "", 0 },
            { "945.123", 945.123 },
            { "3* (100 - 9)", 273 },
            { "(12 + (17 - 19))", 10 }
    };

    testcase error_cases[] = {
            { "* 2", { LEFT_MISSING, 0 } },
            { "5 + 5 * ", { RIGHT_MISSING, 6 } },
            { "2 + 5..", { INVALID_NUMBER_FORMAT, 4 } },
            { "2 + 5e", { INVALID_NUMBER_FORMAT, 4 } },
            { "5 * 40 40", { OP_EXPECTED, 7 } },
            { "12 + (5 + 1", { UNCLOSED_PARENTHESIS, 5 } },
            { ")(55 * 6.5)", { UNEXPECTED_PARENTHESIS, 0 } },
            { "d()", { UNKNOWN_FUNC, 0 } },
            { "14 / const", { UNKNOWN_CONST, 5} },
            { "123 + 123PI", { OP_EXPECTED, 9 } }
    };

    scope.put('^', [](double a, double b) { return pow(a, b); }, 2);
    scope.put("PI", M_PI);
    scope.put("sqrt", [](double x) { return sqrt(x); });

    auto start = chrono::high_resolution_clock::now();

    for (auto tcase : valid_cases) {
        tcase.check(scope);
    }

    for (auto tcase : error_cases) {
        tcase.check(scope);
    }

    auto duration = chrono::high_resolution_clock::now() - start;
    auto us = chrono::duration_cast<chrono::microseconds>(duration);

    cout << "Test took ~" << us.count() << "µs";

    return 0;
}