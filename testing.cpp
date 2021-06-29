#include "automata.h"
#include "hanjp.h"
#include <gtest/gtest.h>
#include <cstring>
#include <gmodule.h>

using namespace std;
using namespace Hanjp;

class AutomataTest : public ::testing::Test {
protected:
    void SetUp() override {
        am.flush();
    }

    AutomataDefault am;
};

class InputContextTest : public ::testing::Test {
protected:
    void SetUp() override {
        init();
        context.reset();
    }

    void TearDown() override {
        fini();
    }

    InputContext context;
};

TEST_F(InputContextTest, Keyboard) {
    u32string testing;
    const char *stream = "qwertyuiop[]\\asdfghjkl;\'zxcvbnm,./QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
    const char *compare = "ㅂㅈㄷㄱ쇼ㅕㅑㅐㅔ[]\\ㅁㄴㅇㄹ호ㅓㅏㅣ;\'ㅋㅌㅊ퓨ㅜㅡ,./ㅃㅉㄸㄲ쑈ㅕㅑㅒㅖ{}|ㅁㄴㅇㄹ호ㅓㅏㅣ:\"ㅋㅌㅊ퓨ㅜㅡ<>?";

    for(int i = 0; stream[i]; i++) {
        context.process(stream[i]);
        testing += context.flush();
    }

    EXPECT_EQ(testing.length(), 34*2) << "fail with size: " << testing.length();
    EXPECT_STREQ(compare, g_ucs4_to_utf8((const gunichar*)testing.data(), testing.length(), NULL, NULL, NULL)) << "Failure on Keyboard check";
}