#ifndef STREVAL_PARSING_RESULT_H
#define STREVAL_PARSING_RESULT_H

#include <cstddef>
#include <string>
#include <optional>
#include <type_traits>

#include "parsing_error.h"

namespace uthef {
    template<typename T>
    class parsing_result {
    private:
        bool successful;
        parsing_error error_val;
        size_t at;
        std::optional<std::string> id_val;

        T num;
    public:
        parsing_result(T value) : num(value), successful(true) {
            static_assert(std::is_arithmetic<T>());
        }

        parsing_result(parsing_error error, size_t at) :
                error_val(error), at(at), successful(false), id_val({}) {
            static_assert(std::is_arithmetic<T>());
        }

        parsing_result(parsing_error error, size_t at, std::string id) :
                error_val(error), at(at), successful(false), id_val(id) {
            static_assert(std::is_arithmetic<T>());
        }

        std::optional<parsing_error> error() {
            if (successful) return { };

            return error_val;
        }

        std::optional<size_t> error_at() {
            if (successful) return { };

            return at;
        }

        std::optional<T> number() {
            if (successful) return num;

            return { };
        }

        std::optional<std::string> id() {
            return id_val;
        }
    };
}

#endif //STREVAL_PARSING_RESULT_H
