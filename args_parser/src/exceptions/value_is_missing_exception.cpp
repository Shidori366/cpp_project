/**
 * @author Dominik Milián
 * @Date 05.04.2025 
 */

#include "exceptions/value_is_missing_exception.h"

const char *value_is_missing_exception::what() const noexcept {
    return "Value is missing in provided arguments.";
}
