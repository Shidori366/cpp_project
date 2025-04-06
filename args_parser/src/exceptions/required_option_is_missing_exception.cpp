/**
 * @author Dominik Milián
 * @Date 05.04.2025 
 */

#include "exceptions/required_option_is_missing_exception.h"

const char *required_option_is_missing_exception::what() const noexcept {
    return "Required option_descriptor is missing";
}
