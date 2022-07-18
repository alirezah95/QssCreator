#include <QApplication>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "qssdvariable.h"

TEST(TestQssdVariable, TestDefaultCtor)
{
    QssdVariable var;

    EXPECT_FALSE(var.isValid());
    EXPECT_FALSE(var.isColor());
}

TEST(TestQssdVariable, TestNameValueCtorAsValue)
{
    QssdVariable var("Variable", "12px");

    EXPECT_TRUE(var.isValid());
    EXPECT_FALSE(var.isColor());
}

TEST(TestQssdVariable, TestNameValueCtorAsColor)
{
    QssdVariable var("Variable", "#ff0000");

    EXPECT_TRUE(var.isValid());
    EXPECT_TRUE(var.isColor());
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
