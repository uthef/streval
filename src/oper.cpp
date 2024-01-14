#include "oper.h"
#include "level.h"

#include <utility>

namespace uthef {
    oper::oper(double(*func)(double, double), level priority) : func(func), lvl(priority) {}
    level oper::priority_level() const {
        return lvl;
    }

    double oper::calculate(double a, double b) {
        return func(a, b);
    }
}