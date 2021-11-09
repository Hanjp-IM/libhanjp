#include <gtest/gtest.h>
#include "hanjpautomata.h"
#include "hanjpkeyboard.h"
#include "hanjpinputcontext.h"


TEST(GObjectConstructDisposeTest, AutomataDefaultConstructDispose) {
	// Given HanjpAutomataDefault object
	HanjpAutomataBuiltin *amBuiltin = NULL;
	const char* strExpectObjType = "HanjpAutomataBuiltin";
	const char* strActualObjType = NULL;

	// When constructed
	amBuiltin = hanjp_am_builtin_new();

	// Then, object pointer must not be null
	ASSERT_NE(amBuiltin, nullptr)
		<< "HanjpAutomataBuiltin construct failed";
	
	// Then, object type name must be same as the class name
	strActualObjType = G_OBJECT_TYPE_NAME(amBuiltin);
	ASSERT_STREQ(strActualObjType, strExpectObjType);

	// When destructed
	g_object_unref(amBuiltin);

	// Then, type check must be false
	ASSERT_FALSE(G_TYPE_CHECK_INSTANCE(amBuiltin))
		<< "HanjpAutomataBuiltin destruct failed";
}


TEST(GObjectConstructDisposeTest, KeyboardDefaultConstructDispose) {
	// Given HanjpKeyboardDefault object
	HanjpKeyboardBuiltin *kbBuiltin = NULL;
	const char* strExpectObjType = "HanjpKeyboardBuiltin";
	const char* strActualObjType = NULL;

	// When constructed
	kbBuiltin = hanjp_kb_builtin_new();

	// Then, object pointer must not be null
	ASSERT_NE(kbBuiltin, nullptr)
		<< "HanjpKeyboardBuiltin construct failed";

	// Then, object type name must be same as the class name
	strActualObjType = G_OBJECT_TYPE_NAME(kbBuiltin);
	ASSERT_STREQ(strActualObjType, strExpectObjType);

	// When destructed
	g_object_unref(kbBuiltin);

	// Then, type check must be false
	ASSERT_FALSE(G_TYPE_CHECK_INSTANCE(kbBuiltin))
		<< "HanjpKeyboardBuiltin destruct failed";
}


TEST(GObjectConstructDisposeTest, InputContextConstructDispose) {
	// Given HanjpInputContext object
	HanjpInputContext *ic = NULL;
	const char* strExpectObjType = "HanjpInputContext";
	const char* strActualObjType = NULL;

	// When constructed
	ic = hanjp_ic_new();

	// Then, object pointer must not be null
	ASSERT_NE(ic, nullptr)
		<< "HanjpInputContext construct failed";

	// Then, object type name must be same as the class name
	strActualObjType = G_OBJECT_TYPE_NAME(ic);
	ASSERT_STREQ(strActualObjType, strExpectObjType);

	// When destructed
	g_object_unref(ic);

	// Then, type check must be false
	ASSERT_FALSE(G_TYPE_CHECK_INSTANCE(ic))
		<< "HanjpInputContext destruct failed";
}

