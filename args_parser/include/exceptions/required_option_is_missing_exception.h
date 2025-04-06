/**
 * @author Dominik Milián
 * @Date 05.04.2025 
 */

#ifndef ARGS_PARSER_REQUIRED_OPTION_IS_MISSING_EXCEPTION_H
#define ARGS_PARSER_REQUIRED_OPTION_IS_MISSING_EXCEPTION_H

#include <exception>

class required_option_is_missing_exception : std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override;
};


#endif //ARGS_PARSER_REQUIRED_OPTION_IS_MISSING_EXCEPTION_H
