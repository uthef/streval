#ifndef STREVAL_OPER_H
#define STREVAL_OPER_H

#include <functional>
#include "level.h"

namespace uthef {
    class oper {
    private:
        level lvl = level::LOW;
        double(*func)(double, double) = nullptr;

    public:
        oper(double(*func)(double, double), level level);

        level priority_level() const;
        double calculate(double a, double b);
    };
}


#endif //STREVAL_OP