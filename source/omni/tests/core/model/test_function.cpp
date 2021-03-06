#include <omni/core/context.hpp>
#include <omni/core/model/meta_info.hpp>
#include <omni/core/model/module.hpp>
#include <omni/core/model/function.hpp>
#include <omni/core/model/block.hpp>
#include <omni/core/model/type.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (funcionTests)

BOOST_AUTO_TEST_CASE (ctor)
{
    using namespace omni::core;
    context c;
    model::module mod (c, "test");
    std::shared_ptr <model::type> returnType (model::type::sharedBasicType (c, model::type_class::t_signedInt));
    model::function func ("hello", returnType, nullptr);
}

BOOST_AUTO_TEST_CASE (getReturnType)
{
    using namespace omni::core;
    context c;
    model::module mod (c, "test");
    std::shared_ptr <model::type> returnType (model::type::sharedBasicType (c, model::type_class::t_signedInt));
    model::function func ("hello", returnType, nullptr);
    BOOST_CHECK (func.getReturnType ()->getTypeClass () == model::type_class::t_signedInt);
}

BOOST_AUTO_TEST_CASE (metaInfo)
{
    using namespace omni::core::model;

    meta_info & meta = function::getStaticMetaInfo ();
    BOOST_CHECK_EQUAL (meta.getName (), "function");
    BOOST_CHECK_EQUAL (meta.getParent (), & function_prototype::getStaticMetaInfo ());
    BOOST_CHECK_EQUAL (meta.getChildCount (), 0u);
}

BOOST_AUTO_TEST_SUITE_END ();
