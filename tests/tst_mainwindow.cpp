#include <QApplication>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mainwindow.h"

TEST(TestMainWindow, testSignals) { }

int main(int argc, char* argv[])
{
    testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
