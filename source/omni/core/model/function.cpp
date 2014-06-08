#include <omni/core/model/function.hpp>
#include <omni/core/context.hpp>
#include <omni/core/model/module.hpp>
#include <omni/core/model/type.hpp>
#include <omni/core/model/block.hpp>
#include <omni/core/model/statement.hpp>
#include <omni/core/model/parameter.hpp>
#include <omni/core/no_context_error.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Module.h>

/**
@brief Initializes a function implementation with the given name, returnType, body, parameters and visibility.

The body's and the parameter's parent will be set to this function.

@param name The name of this function.
@param returnType The return type of this function. Use a type constucted with the type_class omni::core::type_class::t_void for functions that have no return value.
@param body The body (implementation) of this function.
@param parameters An optional list of parameters that the function receives.
@param isExported Specifies, whether this function is visible from outside the module it is defined in. @see isExported().
**/
omni::core::model::function::function (std::string const & name,
                                       std::shared_ptr <type> returnType,
                                       std::shared_ptr <block> body,
                                       std::vector <std::shared_ptr <parameter>> parameters,
                                       bool isExported) :
    function_prototype (name, returnType, parameters),
    _llvmFunction (),
    _isExported (isExported)
{
    setComponent (domain::block, "body", body);
}

omni::core::domain omni::core::model::function::getDomain () const
{
    return domain::function;
}

/**
Returns true, if this function is exported from the module it is defined in. Depending on what kind of module it is, exporting
has different meanings.
If isExported () returns false, the function is only locally visible in the module it is defined in.
**/
bool omni::core::model::function::isExported () const
{
    return _isExported;
}

/**
Returns the body of this function in a const form.
**/
const std::shared_ptr <omni::core::model::block> omni::core::model::function::getBody () const
{
    return getComponentAs <block> (domain::block, "body");
}

/**
Returns the body of this function in a modifyable form.
**/
std::shared_ptr <omni::core::model::block> omni::core::model::function::getBody ()
{
    return getComponentAs <block> (domain::block, "body");
}


llvm::Function * omni::core::model::function::llvmFunction ()
{
    if (getContext () == nullptr) {
        throw no_context_error (domain::function, getName ());
    }
    if (_llvmFunction != nullptr) {
        return _llvmFunction;
    } else {
        llvm::FunctionType * funcType = llvmFunctionType ();

        llvm::Function::LinkageTypes linkageType;
        if (isExported ()) {
            // TODO: Handle static and dynamic libraries differently - if omni will have static libraries, that is.
            linkageType = llvm::Function::ExternalLinkage;
        } else {
            linkageType = llvm::Function::InternalLinkage;
        }
        _llvmFunction = llvm::Function::Create (funcType, linkageType, getName (), & getModule ()->llvmModule ());
        if (isExported ()) {
            _llvmFunction->setDLLStorageClass (llvm::GlobalValue::DLLExportStorageClass);
        }

        auto mainBlock = llvm::BasicBlock::Create (getContext ()->llvmContext (), "__entry__", _llvmFunction);
        getBody ()->llvmEmitIntoExistingBlock (mainBlock);

        return _llvmFunction;
    }
}
