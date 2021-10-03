#include <gtest/gtest.h>
#include "hanjpautomata.h"
#include "hanjpkeyboard.h"
#include "hanjpinputcontext.h"

class InputContextTest : public ::testing::Test {
protected:
	void SetUp() override {
        context = hanjp_ic_new();
    }

    void TearDown() override {
        g_object_unref(context);
    }

    HanjpInputContext *context;
};

TEST_F(InputContextTest, IC_Hajimemashite) {
    const char *s = "gkwlapaktlxp.";
    GArray *committed = hanjp_ic_ref_commit_string(context);

    while(*s) {
        hanjp_ic_process(context, (gint)(*s));
        s++;
    }

    s = g_ucs4_to_utf8((const gunichar*)committed->data, committed->len, NULL, NULL, NULL);

    EXPECT_STREQ(s, "はじめまして.");
    
    g_array_unref(committed);
}

