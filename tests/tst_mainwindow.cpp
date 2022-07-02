#include <QAction>
#include <QApplication>
#include <QClipboard>
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

    (*undoAct)->trigger();
    EXPECT_STREQ(
        editorMock->document()->toPlainText().toStdString().c_str(), "");
    EXPECT_EQ((*undoAct)->isEnabled(), false);
    EXPECT_EQ((*redoAct)->isEnabled(), true);

    (*redoAct)->trigger();
    EXPECT_STREQ(
        editorMock->document()->toPlainText().toStdString().c_str(), "Text");
    EXPECT_EQ((*undoAct)->isEnabled(), true);
    EXPECT_EQ((*redoAct)->isEnabled(), false);
}

TEST_F(TestMainWindow, TestCopyPaste)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();

    auto copyAct = std::find_if(actions.begin(), actions.end(),
        [](QAction* curr) { return curr->objectName() == "actionCopy"; });

    auto pasteAct = std::find_if(actions.begin(), actions.end(),
        [](QAction* curr) { return curr->objectName() == "actionPaste"; });

    ASSERT_NE(copyAct, actions.end()) << "No \"copy\" action.";
    ASSERT_NE(pasteAct, actions.end()) << "No \"paste\" action.";

    editorMock->insertPlainText("Test copy/paste");
    editorMock->selectAll();

    // Test copy action functionality
    EXPECT_EQ((*copyAct)->isEnabled(), true);
    (*copyAct)->trigger();
    EXPECT_EQ(qApp->clipboard()->text(), "Test copy/paste");

    QTest::keyPress(editorMock, Qt::Key_End, Qt::NoModifier);

    // Test paste action functionality
    (*pasteAct)->trigger();
    EXPECT_STREQ(editorMock->document()->toPlainText().toStdString().c_str(),
        "Test copy/pasteTest copy/paste");
}

TEST_F(TestMainWindow, TestCutPaste)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();

    auto cutAct = std::find_if(actions.begin(), actions.end(),
        [](QAction* curr) { return curr->objectName() == "actionCut"; });

    auto pasteAct = std::find_if(actions.begin(), actions.end(),
        [](QAction* curr) { return curr->objectName() == "actionPaste"; });

    ASSERT_NE(cutAct, actions.end()) << "No \"cut\" action.";
    ASSERT_NE(pasteAct, actions.end()) << "No \"paste\" action.";

    editorMock->insertPlainText("Test cut/paste");
    editorMock->selectAll();

    // Test copy action functionality
    EXPECT_EQ((*cutAct)->isEnabled(), true);
    (*cutAct)->trigger();
    EXPECT_EQ(qApp->clipboard()->text(), "Test cut/paste");

    QTest::keyPress(editorMock, Qt::Key_End, Qt::NoModifier);

    // Test paste action functionality
    (*pasteAct)->trigger();
    EXPECT_STREQ(editorMock->document()->toPlainText().toStdString().c_str(),
        "Test cut/paste");
}

TEST_F(TestMainWindow, TestIncreaseDecreaseFont)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();

    auto incFontAct
        = std::find_if(actions.begin(), actions.end(), [](QAction* curr) {
              return curr->objectName() == "actionIncreaseFont";
          });

    auto decFontAct
        = std::find_if(actions.begin(), actions.end(), [](QAction* curr) {
              return curr->objectName() == "actionDecreaseFont";
          });

    ASSERT_NE(incFontAct, actions.end()) << "No \"increase font\" action.";
    ASSERT_NE(decFontAct, actions.end()) << "No \"decrease font\" action.";

    qreal currFontPS = editorMock->font().pointSize();
    (*incFontAct)->trigger();
    EXPECT_EQ(editorMock->font().pointSize(), ++currFontPS);

    (*decFontAct)->trigger();
    EXPECT_EQ(editorMock->font().pointSize(), --currFontPS);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
