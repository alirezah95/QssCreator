#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QTest>
#include <QToolBar>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "documentfile.h"
#include "mainwindow.h"

#include "tst_mockqssdeditor.h"
#include "tst_mockqssdfileoperations.h"
#include "tst_mockuserdialogs.h"

using namespace ::testing;

class TestMainWindow : public ::testing::Test
{
public:
    void SetUp()
    {
        editorMock = new MockQssdEditor;
        opersMock = new MockQssdFileOperations;
        userDlgsMock = new MockUserDialogs;

        mainWin = new MainWindow(editorMock, opersMock, userDlgsMock);
    }

    void TearDown()
    {
        delete mainWin;
        return;
    }

    MockQssdEditor* editorMock;
    MockQssdFileOperations* opersMock;
    MockUserDialogs* userDlgsMock;

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

TEST_F(TestMainWindow, TestUpdateWindowTitleDocModified)
{
    EXPECT_STREQ(mainWin->windowTitle().toStdString().c_str(),
        DOC_UNTITLED " - Qss Creator");

    editorMock->insertPlainText("Mock text");
    EXPECT_STREQ(mainWin->windowTitle().toStdString().c_str(),
        DOC_UNTITLED "* - Qss Creator");
}

TEST_F(TestMainWindow, TestNewDocumentWhenNoModified)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();
    auto newBtnAct = std::find_if(
        actions.constBegin(), actions.constEnd(), [](QAction* curr) -> bool {
            return curr->objectName() == "actionNewFile";
        });
    ASSERT_NE(newBtnAct, actions.end()) << "No \"new\" action";

    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _)).Times(0);
    EXPECT_CALL(*opersMock, newDocument(editorMock)).WillOnce(Return(true));

    (*newBtnAct)->trigger();
}

TEST_F(TestMainWindow, TestNewDocumentWhenModified)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();
    auto newBtnAct = std::find_if(
        actions.constBegin(), actions.constEnd(), [](QAction* curr) -> bool {
            return curr->objectName() == "actionNewFile";
        });
    ASSERT_NE(newBtnAct, actions.end()) << "No \"new\" action";

    editorMock->insertPlainText("TestNewDocumentWhenModified test case");

    // When user clicks Cancel
    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _))
        .Times(1)
        .WillOnce(Return(QMessageBox::Cancel));
    EXPECT_CALL(*opersMock, newDocument(editorMock)).Times(0);
    (*newBtnAct)->trigger();

    // When user clicks No
    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _))
        .Times(1)
        .WillOnce(Return(QMessageBox::No));
    EXPECT_CALL(*opersMock, newDocument(editorMock)).Times(1);
    EXPECT_CALL(*opersMock, saveDocument(_, _)).Times(0);
    (*newBtnAct)->trigger();

    // When user clicks Yes
    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _))
        .Times(1)
        .WillOnce(Return(QMessageBox::Yes));
    EXPECT_CALL(*userDlgsMock, getSaveFileName(_, _, _, _, _, _))
        .WillOnce(Return("/newdoc.qssd"));
    Expectation saveExp
        = EXPECT_CALL(*opersMock, saveDocument(_, _)).WillOnce(Return(true));
    EXPECT_CALL(*opersMock, newDocument(editorMock))
        .After(saveExp)
        .WillOnce(Return(true));
    (*newBtnAct)->trigger();
}

TEST_F(TestMainWindow, TestOpenDocument) { }

TEST_F(TestMainWindow, TestSaveDocument)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();
    auto saveBtnAct = std::find_if(
        actions.constBegin(), actions.constEnd(), [](QAction* curr) -> bool {
            return curr->objectName() == "actionSave";
        });
    ASSERT_NE(saveBtnAct, actions.end()) << "No \"save\" action";

    editorMock->insertPlainText("TestSaveAsDocument test case");

    Expectation gsfnEpx
        = EXPECT_CALL(*userDlgsMock, getSaveFileName(_, _, _, _, _, _))
              .WillOnce(Return("/newdoc.qssd"));

    EXPECT_CALL(*opersMock, saveDocument(editorMock, _))
        .After(gsfnEpx)
        .WillOnce(
            Invoke([](const QTextEdit* editor, IDocumentFile* docFile) -> bool {
                DocumentFile* file = dynamic_cast<DocumentFile*>(docFile);
                EXPECT_STREQ(file->file.fileName().toStdString().c_str(),
                    "/newdoc.qssd");
                return true;
            }));

    (*saveBtnAct)->trigger();
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
