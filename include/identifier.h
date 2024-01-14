#ifndef STREVAL_IDENTIFIER_H
#define STREVAL_IDENTIFIER_H

#include <string>

namespace uthef {
    class identifier {
    private:
        std::string _value;
    public:
        identifier(const char *value);
        std::string value();
    };
}

#endif //STREVAL_IDENTIFIER_H
