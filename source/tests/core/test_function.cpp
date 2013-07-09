#include <omni/core/function.hpp>
#include <omni/core/module.hpp>
#include <omni/core/context.hpp>

void test_function ()
{
	omni::core::context context;
	omni::core::module module (context, "test");
	omni::core::type & classDemo (module.addClass ("demo"));
	omni::core::function & funcDemonstrate(classDemo.addFunction ("demonstrate"));
	omni::core::function & funcMain (classDemo.addFunction ("main"));
}
