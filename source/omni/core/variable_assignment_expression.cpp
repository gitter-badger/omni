#include <omni/core/variable_assignment_expression.hpp>
#include <omni/core/variable_declaration_statement.hpp>
#include <omni/core/type_mismatch_error.hpp>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/NoFolder.h>

/**
Initializes this variabe_assignment_expresion to assign `value' to `variable'.
variable and value need to be of exactly the same type.
**/
omni::core::variable_assignment_expression::variable_assignment_expression (std::shared_ptr <variable_declaration_statement> variable, std::shared_ptr <expression> value) :
    _variable (variable),
    _value (value),
    _llvmValue (nullptr)
{
    if (_variable->getVariableType () != _value->getType ()) {
        throw type_mismatch_error (* _variable->getVariableType (), * _value->getType ());
    }
}

/**
Returns the variable that is being assigned to by this expression.
**/
std::shared_ptr <omni::core::variable_declaration_statement> omni::core::variable_assignment_expression::getVariable () const
{
    return _variable;
}

/**
Returns the value that is being assigned to a variable by this expression.
**/
std::shared_ptr <omni::core::expression> omni::core::variable_assignment_expression::getValue () const
{
    return _value;
}

std::shared_ptr <omni::core::type> omni::core::variable_assignment_expression::getType () const
{
    return _variable->getVariableType ();
}

/**
Internal
**/
llvm::Value * omni::core::variable_assignment_expression::llvmValue (llvm::BasicBlock * llvmBasicBlock)
{
    return omni::core::variable_assignment_expression::llvmEmitImpl (llvmBasicBlock, * _variable, * _value);
}

/**
Internal
**/
llvm::Value * omni::core::variable_assignment_expression::llvmEmitImpl (llvm::BasicBlock * llvmBasicBlock, variable_declaration_statement & variable, expression & value)
{
    llvm::IRBuilder <true, llvm::NoFolder> builder (llvmBasicBlock);
    llvm::Value * result = value.llvmValue (llvmBasicBlock);
    builder.CreateStore (result, variable.llvmPointerValue ());
    return result;
}