#ifndef STREVAL_STREVAL_H
#define STREVAL_STREVAL_H

#include "scope.h"
#include "parsing_result.h"

namespace uthef {
    parsing_result<double> parse_id(const char32_t* exp, scope& scope, unsigned int& cur);
    parsing_result<double> streval(const char32_t* expression, uthef::scope& scope);
    parsing_result<double> streval(const char32_t* expression);
}


#endif //STREVAL_STREVAL_H