#ifndef OMNI_CORE_VARIABLE_EXPRESSION_HPP
#define OMNI_CORE_VARIABLE_EXPRESSION_HPP

#include <omni/core/core.hpp>
#include <omni/core/pure_expression.hpp>

#include <memory>

namespace omni {
namespace core {
    class variable_declaration_statement;

    /**
    A `variable expression' is any mention of a variable that is used to take that variable's value.
    E.g. in "int foo = x;", the "x" is a variable_expression.
    To assign values to a variable, use the variable_assignment_expression.
    **/
    class OMNI_CORE_API variable_expression : public pure_expression {
    public:
        variable_expression (std::shared_ptr <variable_declaration_statement> variable);

        std::shared_ptr <type> getType () const override;

        const std::shared_ptr <variable_declaration_statement> getVariable () const ;

        llvm::Value * llvmValue (llvm::BasicBlock * llvmBasicBlock) override;

    private:
        std::shared_ptr <variable_declaration_statement> _variable;
    };

} // namespace core
} // namespace omni

#endif // include guard