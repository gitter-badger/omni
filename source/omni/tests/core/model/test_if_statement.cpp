#include <omni/core/model/if_statement.hpp>
#include <omni/core/context.hpp>
#include <omni/core/model/module.hpp>
#include <omni/core/model/builtin_literal_expression.hpp>
#include <omni/core/model/return_statement.hpp>
#include <omni/core/model/block.hpp>
#include <omni/core/model/function.hpp>
#include <omni/core/model/meta_info.hpp>

#include <omni/tests/test_file_manager.hpp>
#include <omni/tests/test_utils.hpp>

#include <boost/test/auto_unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE (ifStatementTests)

BOOST_AUTO_TEST_CASE (ctor)
{
    using namespace omni::core;
    context c;

    auto condition = std::make_shared <model::builtin_literal_expression <bool>> (c, true);
    auto trueBlock = std::make_shared <model::block> ();
    auto elseBlock = std::make_shared <model::block> ();

    model::if_statement ifStatement (condition, trueBlock, elseBlock);
    BOOST_CHECK (ifStatement.getCondition () == condition);
    BOOST_CHECK (ifStatement.getTrueBlock () == trueBlock);
    BOOST_CHECK (ifStatement.getElseBlock () == elseBlock);

    model::if_statement ifStatement2 (condition, trueBlock, nullptr);
    BOOST_CHECK (ifStatement2.getCondition () == condition);
    BOOST_CHECK (ifStatement2.getTrueBlock () == trueBlock);
    BOOST_CHECK (ifStatement2.getElseBlock () == nullptr);
}

BOOST_AUTO_TEST_CASE (metaInfo)
{
    using namespace omni::core::model;

    meta_info & meta = if_statement::getStaticMetaInfo ();
    BOOST_CHECK_EQUAL (meta.getName (), "if_statement");
    BOOST_CHECK_EQUAL (meta.getParent (), & statement::getStaticMetaInfo ());
    BOOST_CHECK_EQUAL (meta.getChildCount (), 0u);
}

BOOST_AUTO_TEST_CASE (mixedTests)
{
    using namespace omni::core;
    context c;
    model::module m (c, "test");

    auto runDemo = [& c, & m] (bool cond) -> int 
    {
        auto condition = std::make_shared <model::builtin_literal_expression <bool>> (c, cond);
        auto trueBlock = std::make_shared <model::block> ();
        trueBlock->appendStatement (std::make_shared <model::return_statement> (std::make_shared <model::builtin_literal_expression <int>> (c, 42)));
        auto elseBlock = std::make_shared <model::block> ();
        elseBlock->appendStatement (std::make_shared <model::return_statement> (std::make_shared <model::builtin_literal_expression <int>> (c, 128)));

        auto ifStatement = std::make_shared <model::if_statement> (condition, trueBlock, elseBlock);

        auto body = std::make_shared <model::block> ();
        body->appendStatement (ifStatement);

        std::stringstream str;
        str << "test" << cond;
        std::shared_ptr <model::function> func = m.createFunction (str.str (), c.sharedBasicType (model::type_class::t_signedInt), body);

        omni::tests::test_file_manager testFileManager;
        return omni::tests::runFunction <int> (func, testFileManager, "ifStatementMixedTests");
    };

    int result = runDemo (true);
    BOOST_CHECK_EQUAL (result, 42);
    BOOST_CHECK_NE (result, 128);
    
    result = runDemo (false);
    BOOST_CHECK_EQUAL (result, 128);
    BOOST_CHECK_NE (result, 42);
}

BOOST_AUTO_TEST_SUITE_END ()
