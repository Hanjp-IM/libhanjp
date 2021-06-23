#include "automata.h"
#include "hanjp.h"
#include <gtest/gtest.h>

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

    InputContext context;
};

TEST_F(AutomataTest, Basic) {
    EXPECT_EQ(7 * 6, 42);
}

TEST_F(InputContextTest, Basic) {
    EXPECT_EQ(2 * 3, 6);
}