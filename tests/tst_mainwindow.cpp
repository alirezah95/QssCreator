#include <QAction>
#include <QApplication>
#include <QTest>
#include <QToolBar>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mainwindow.h"

#include "tst_mockqssdeditor.h"
#include "tst_mockqssdfileoperations.h"

class TestMainWindow : public ::testing::Test
{
public:
    void SetUp()
    {
        editorMock = new MockQssdEditor;
        opersMock = new MockQssdFileOperations;

        mainWin = new MainWindow(editorMock, opersMock);
    }

    void TearDown()
    {
        delete mainWin;
        return;
    }

    MockQssdEditor* editorMock;
    MockQssdFileOperations* opersMock;

    MainWindow* mainWin;
};

TEST_F(TestMainWindow, testUndoRedo)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();

    auto undoAct = std::find_if(actions.begin(), actions.end(),
        [](QAction* curr) { return curr->objectName() == "actionUndo"; });

    auto redoAct = std::find_if(actions.begin(), actions.end(),
        [](QAction* curr) { return curr->objectName() == "actionRedo"; });

    ASSERT_NE(undoAct, actions.end());
    ASSERT_NE(redoAct, actions.end());

    editorMock->insertPlainText("Text");

    EXPECT_EQ((*undoAct)->isEnabled(), true);
    EXPECT_EQ((*redoAct)->isEnabled(), false);

    QTest::keyPress(editorMock, Qt::Key_Z, Qt::ControlModifier);

    EXPECT_EQ((*undoAct)->isEnabled(), false);
    EXPECT_EQ((*redoAct)->isEnabled(), true);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
