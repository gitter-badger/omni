#include <omni/take2/not_implemented_error.hpp>

omni::take2::not_implemented_error::not_implemented_error (std::string const & fileName, std::string const & functionName, int lineNumber) :
    omni::take2::logic_error (functionName, fileName, lineNumber, "Function is not (fully) implemented")
{
}
