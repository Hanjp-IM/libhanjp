#include <gtest/gtest.h>
#include "hanjpautomata.h"

TEST(GObjectConstructDisposeTest, AutomataDefaultConstructDispose) {
	// Given HanjpAutomataDefault object
	HanjpAutomataDefault *amDefault = NULL;
	const char* strExpectObjType = "HanjpAutomataDefault";
	const char* strActualObjType = NULL;

	// When constructed
	amDefault = hanjp_amdefault_new();

	// Then, object pointer must not be null
	ASSERT_NE(amDefault, nullptr)
		<< "HanjpAutomataDefault construct failed";
	
	// Then, object type name must be same as the class name
	strActualObjType = G_OBJECT_TYPE_NAME(amDefault);
	ASSERT_STREQ(strActualObjType, strExpectObjType);

	// When destructed
	g_object_unref(amDefault);

	// Then, type check must be false
	ASSERT_FALSE(G_TYPE_CHECK_INSTANCE(amDefault))
		<< "HanjpAutomataDefault destruct failed";
}

