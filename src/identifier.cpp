#include "identifier.h"
#include <stdexcept>

using std::invalid_argument;

namespace uthef {
    identifier::identifier(const char* value) {
        int i = 0;
        char chr;

        while ((chr = *(value + i)) != '\0') {
            if (!std::isalnum(chr)) {
                throw invalid_argument("identifier num is allowed to contain only alphanumeric characters");
            }
            else if (i == 0 && std::isdigit(chr)) {
                throw invalid_argument("identifier num must begin with a letter");
            }

            i++;
        }

        if (i == 0) throw invalid_argument("identifier num must not be an empty string");

        _value = std::string(value);
    }

    std::string identifier::value() {
        return _value;
    }
}