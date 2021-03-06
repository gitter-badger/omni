#include <omni/core/model/type.hpp>
#include <omni/core/context.hpp>
#include <omni/core/not_implemented_error.hpp>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>

#include <sstream>

// See meta_info.cpp for initialization.
namespace omniMetaImpl {
extern omni::core::model::meta_info typeMetaInfo;
}

namespace {
/*
Returns the type that points to baseType with the given indirectionLevel. For example, if baseType is int32 and indirectionLevel is 1,
the result is int32*. If indirection is 2, the result is int32** and if indirection is 0, the result is int32.
*/
llvm::Type * addIndirection (llvm::Type * baseType, unsigned int indirectionLevel)
{
    llvm::Type * result = baseType;
    for (unsigned i = 0; i < indirectionLevel; ++ i) {
        result = result->getPointerTo ();
    }
    return result;
}
} // anonymous namespace

/**
@brief Initializes this type with the given type_class and indirectionLevel.

Neither the typeClass, nor the indirectionLevel can be changed later. If you need a type with a different typeClass or indirectionLevel,
use another call to sharedBasicType for built-in types.

<b>Important:</b> You should always use sharedBasicType instead of instantiating your own type object.

@param context The context that this type is used in.
@param typeClass The type class of the desired type. @see type_class
@param indirectionLevel The level of pointer-indirection. This is only for internal usage and for representing types imported from external C code.
The Omni Language itself does not use types with an indiretionLevel > 0. Instead, "optionals" and "references" are used. (Not yet implemented.)
**/
omni::core::model::type::type (omni::core::context & context, omni::core::model::type_class typeClass, unsigned int indirectionLevel) :
    entity (),
    _context (context),
    _typeClass (typeClass),
    _indirectionLevel (indirectionLevel)
{
    std::stringstream typeName;
    typeName << typeClass;
    setName (typeName.str ());
}

omni::core::model::type::~ type ()
{
}

omni::core::model::meta_info & omni::core::model::type::getStaticMetaInfo ()
{
    return omniMetaImpl::typeMetaInfo;
}

omni::core::model::meta_info & omni::core::model::type::getMetaInfo () const
{
    return getStaticMetaInfo ();
}

/**
Returns a human-readable text that represents this type.
@param fullyQualified Specifies, whether the type should be returned fully qualified, or in canonical form. The exact meaning of this depends on the actualy type.
@return A human-readable text that represents this type.
**/
std::string omni::core::model::type::toString (bool fullyQualified)
{
    return toString (_typeClass, fullyQualified);
}

/**
Returns a human-readable text that represents `typeClass'. Only suitable for built-in types (no classes, no enums).
@param typeClass The type-class that will be converted to text.
@param fullyQualified Specifies, whether the type should be returned fully qualified, or in canonical form. The exact meaning of this depends on the actualy type.
@return A human-readable text that represents `typeClass'.
**/
std::string omni::core::model::type::toString (type_class typeClass, bool fullyQualified)
{
    switch (typeClass) {
    case type_class::t_void:
        return "void";
    case type_class::t_boolean:
        return "boolean";
    case type_class::t_unsignedByte:
        return "unsigned byte";
    case type_class::t_signedByte:
        return "signed byte";
    case type_class::t_unsignedShort:
        return "unsigned short";
    case type_class::t_signedShort:
        return "signed short";
    case type_class::t_unsignedInt:
        return "unsigned int";
    case type_class::t_signedInt:
        return "signed int";
    case type_class::t_unsignedLong:
        return "unsigned long";
    case type_class::t_signedLong:
        return "signed long";
    case type_class::t_unsignedLongLong:
        return "unsigned long long";
    case type_class::t_signedLongLong:
        return "signed long long";
    case type_class::t_char:
        return "character";
    default:
        throw not_implemented_error (__FILE__, __FUNCTION__, __LINE__);
    }
}

omni::core::domain omni::core::model::type::getDomain () const
{
    return domain::type;
}

omni::core::context * omni::core::model::type::getContext ()
{
    return & _context;
}

const omni::core::context * omni::core::model::type::getContext () const
{
    return & _context;
}

omni::core::model::module * omni::core::model::type::getModule ()
{
    return nullptr;
}

const omni::core::model::module * omni::core::model::type::getModule () const
{
    return nullptr;
}

void omni::core::model::type::setComponent (omni::core::domain domain, std::string name, std::shared_ptr <omni::core::model::entity> entity)
{
    // At this time, a type does not contain components.
}

/**
@rief Returns the type_class of this type.
@return The type_class of this type.
@see type_class
**/
omni::core::model::type_class omni::core::model::type::getTypeClass () const
{
    return _typeClass;
}

/**
@brief Returns the pointer indirection-level of this type.
E.g. when the type_class is t_signedInt and the indirectionLevel is 2, the underlying C-type would be "int**".

@return The pointer indirection-level of this type.
**/
unsigned int omni::core::model::type::getIndirectionLevel () const
{
    return _indirectionLevel;
}
/**
@brief Returns a shared_ptr for a instance of type for the desired context and type_class.

Short-hand for creating a type with new type(context, typeClass, indirectionLevel); with the additional benefit that types are cached in the context.
You should always use sharedBasicType instead of instantiating your own type object.

@param context The context that this type is used in.
@param typeClass The type class of the desired type. @see type_class
@param indirectionLevel The level of pointer-indirection. This is only for internal usage and for representing types imported from external C code.
The Omni Language itself does not use types with an indiretionLevel > 0. Instead, "optionals" and "references" are used. (Not yet implemented.)
@see context::sharedBasicType(type_class);
**/
std::shared_ptr <omni::core::model::type> omni::core::model::type::sharedBasicType (context & context, type_class typeClass, unsigned int indirectionLevel)
{
    return context.sharedBasicType (typeClass, indirectionLevel);
}


/*
Returns an llvm::Type that corresponds to this Omni type. For built-in types, these directly map to i1, i8, i16, i32, and so on.
A pointer-asterisk is added per indirectionLevel. (E.g. type_class t_signedInt with indirectionLevel 2 becomes an i32** on x86).
*/
llvm::Type * omni::core::model::type::llvmType ()
{
    switch (_typeClass) {
    case type_class::t_void:
        return addIndirection (llvm::Type::getVoidTy (getContext ()->llvmContext ()), getIndirectionLevel ());
        break;
    case type_class::t_boolean:
        return addIndirection (llvm::Type::getInt1Ty (getContext ()->llvmContext ()), getIndirectionLevel ());
        break;
    case type_class::t_char:
    case type_class::t_unsignedByte:
    case type_class::t_signedByte:
        return addIndirection (llvm::Type::getInt8Ty (getContext ()->llvmContext ()), getIndirectionLevel ());
        break;
    case type_class::t_unsignedShort:
    case type_class::t_signedShort:
        return addIndirection (llvm::Type::getInt16Ty (getContext ()->llvmContext ()), getIndirectionLevel ());
        break;
    case type_class::t_unsignedLong:
    case type_class::t_signedLong:
#ifdef ENVIRONMENT64
        return addIndirection (llvm::Type::getInt64Ty (getContext ()->llvmContext ()), getIndirectionLevel ());
#endif
    case type_class::t_unsignedInt:
    case type_class::t_signedInt:
        return addIndirection (llvm::Type::getInt32Ty (getContext ()->llvmContext ()), getIndirectionLevel ());
        break;
    case type_class::t_unsignedLongLong:
    case type_class::t_signedLongLong:
        return addIndirection (llvm::Type::getInt64Ty (getContext ()->llvmContext ()), getIndirectionLevel ());
        break;
    default:
        throw not_implemented_error (__FILE__, __FUNCTION__, __LINE__);
    }
}
