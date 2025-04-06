/**
 * @author Dominik Milián
 * @Date 05.04.2025 
 */

#ifndef ARGS_PARSER_VALUE_IS_MISSING_EXCEPTION_H
#define ARGS_PARSER_VALUE_IS_MISSING_EXCEPTION_H

#include <exception>


class value_is_missing_exception : std::exception {
public:
    [[nodiscard]] const char *what() const noexcept override;
};


#endif //ARGS_PARSER_VALUE_IS_MISSING_EXCEPTION_H
