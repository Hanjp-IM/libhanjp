#include <gtest/gtest.h>
#include "hanjpinputcontext.h"

class InputContextTest : public ::testing::Test {
protected:
	void SetUp() override {
		// Create context object for each tests
        context = hanjp_ic_new();
    }

    void TearDown() override {
		// Destruct context object when each tests were done
        g_object_unref(context);
    }

    HanjpInputContext *context;
};

TEST_F(InputContextTest, IC_Hajimemashite) {
    const char *strGivenKeyInput = "gkwlapaktlxp.";
	const char *strExpectJapanese = "はじめまして.";
	const char *strActualJapanese = NULL;

	// Given Hanjp Input context
    GArray *committed = hanjp_ic_ref_commit_string(context);

	// When given key input were processed
    while(*strGivenKeyInput) {
        hanjp_ic_process(context, (gint)(*strGivenKeyInput));
        strGivenKeyInput++;
    }
    strActualJapanese = g_ucs4_to_utf8((const gunichar*)committed->data, committed->len, NULL, NULL, NULL);

	// Then, converted japanese must be same as expected
    EXPECT_STREQ(strActualJapanese, strExpectJapanese);
    
	// Destruct temporal variable in test
    g_array_unref(committed);
}

