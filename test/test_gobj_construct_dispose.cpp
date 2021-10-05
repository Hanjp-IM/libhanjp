#include <gtest/gtest.h>
#include "hanjpautomata.h"
#include "hanjpkeyboard.h"
#include "hanjpinputcontext.h"


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


TEST(GObjectConstructDisposeTest, KeyboardDefaultConstructDispose) {
	// Given HanjpKeyboardDefault object
	HanjpKeyboardDefault *kbDefault = NULL;
	const char* strExpectObjType = "HanjpKeyboardDefault";
	const char* strActualObjType = NULL;

	// When constructed
	kbDefault = hanjp_keyboarddefault_new();

	// Then, object pointer must not be null
	ASSERT_NE(kbDefault, nullptr)
		<< "HanjpKeyboardDefault construct failed";

	// Then, object type name must be same as the class name
	strActualObjType = G_OBJECT_TYPE_NAME(kbDefault);
	ASSERT_STREQ(strActualObjType, strExpectObjType);

	// When destructed
	g_object_unref(kbDefault);

	// Then, type check must be false
	ASSERT_FALSE(G_TYPE_CHECK_INSTANCE(kbDefault))
		<< "HanjpKeyboardDefault destruct failed";
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

