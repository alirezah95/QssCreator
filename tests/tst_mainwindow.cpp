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
#include "tst_mockqssdprocessor.h"
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
        processorMock = new MockQssdProcessor;

        mainWin = new MainWindow(
            editorMock, opersMock, userDlgsMock, processorMock);
    }

    void TearDown()
    {
        delete mainWin;
        return;
    }

    MockQssdEditor* editorMock;
    MockQssdFileOperations* opersMock;
    MockUserDialogs* userDlgsMock;
    MockQssdProcessor* processorMock;

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
    EXPECT_CALL(*opersMock, newDocument(editorMock))
        .WillOnce(Invoke([](const QTextEdit* editor) {
            editor->document()->setModified(false);
            return true;
        }));

    (*newBtnAct)->trigger();
}

TEST_F(TestMainWindow, TestNewDocumentWhenModifiedAndNoFileIsSelected)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();
    auto newBtnAct = std::find_if(
        actions.constBegin(), actions.constEnd(), [](QAction* curr) -> bool {
            return curr->objectName() == "actionNewFile";
        });
    ASSERT_NE(newBtnAct, actions.end()) << "No \"new\" action";

    // User choose yes to save modified doc
    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _))
        .WillOnce(Return(QMessageBox::Yes));
    // User doesn't choose nay save file path, for example by pressing escape
    EXPECT_CALL(*userDlgsMock, getSaveFileName(_, _, _, _, _, _))
        .WillOnce(Return(""));
    // So newDocument() should not be called
    EXPECT_CALL(*opersMock, newDocument(editorMock)).Times(0);

    editorMock->insertPlainText(
        "TestNewDocumentWhenModifiedAndNoFileIsSelected test case");
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
    EXPECT_CALL(*opersMock, saveDocument(_, _)).Times(0);
    EXPECT_CALL(*opersMock, newDocument(editorMock))
        .WillOnce(Invoke([](const QTextEdit* editor) {
            editor->document()->setModified(false);
            return true;
        }));
    (*newBtnAct)->trigger();

    // When user clicks Yes
    // Reset doc to modified so tests are valid
    editorMock->document()->setModified(true);
    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _))
        .Times(1)
        .WillOnce(Return(QMessageBox::Yes));
    EXPECT_CALL(*userDlgsMock, getSaveFileName(_, _, _, _, _, _))
        .WillOnce(Return("/newdoc.qssd"));
    Expectation saveExp
        = EXPECT_CALL(*opersMock, saveDocument(_, _))
              .WillOnce(
                  Invoke([](const QTextEdit* editor, IDocumentFile* outFile) {
                      // Following line is needed otherwise successive tests
                      // won't go right
                      editor->document()->setModified(false);
                      return true;
                  }));
    EXPECT_CALL(*opersMock, newDocument(editorMock))
        .After(saveExp)
        .WillOnce(Invoke([](const QTextEdit* editor) {
            editor->document()->setModified(false);
            return true;
        }));
    (*newBtnAct)->trigger();
}

TEST_F(TestMainWindow, TestOpenDocumentWhenNoModified)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();
    auto openBtnAct = std::find_if(
        actions.constBegin(), actions.constEnd(), [](QAction* curr) -> bool {
            return curr->objectName() == "actionOpenFile";
        });
    ASSERT_NE(openBtnAct, actions.end()) << "No \"open file\" action";

    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _)).Times(0);
    Expectation openFlNmExp
        = EXPECT_CALL(*userDlgsMock, getOpenFileName(_, _, _, _, _, _))
              .WillOnce(Return("/openfile.qssd"));
    EXPECT_CALL(*opersMock, openDocument(editorMock, _))
        .WillOnce(Invoke([](const QTextEdit* editor, IDocumentFile* docFile) {
            EXPECT_STREQ(
                docFile->fileName().toStdString().c_str(), "/openfile.qssd");
            editor->document()->setModified(false);
            return true;
        }));
    EXPECT_CALL(*processorMock, processDocument(editorMock->document(), true))
        .Times(1);

    (*openBtnAct)->trigger();
}

TEST_F(TestMainWindow, TestOpenDocumentWhenModified)
{
    const auto& actions
        = mainWin->findChild<QToolBar*>("mainToolBar")->actions();
    auto openBtnAct = std::find_if(
        actions.constBegin(), actions.constEnd(), [](QAction* curr) -> bool {
            return curr->objectName() == "actionOpenFile";
        });
    ASSERT_NE(openBtnAct, actions.end()) << "No \"open file\" action";

    editorMock->insertPlainText("TestOpenDocumentWhenModified test case");

    // When user clicks Cancel
    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _))
        .Times(1)
        .WillOnce(Return(QMessageBox::Cancel));
    EXPECT_CALL(*userDlgsMock, getOpenFileName(_, _, _, _, _, _)).Times(0);
    EXPECT_CALL(*opersMock, openDocument(editorMock, _)).Times(0);
    (*openBtnAct)->trigger();

    // When user clicks No
    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _))
        .Times(1)
        .WillOnce(Return(QMessageBox::No));
    EXPECT_CALL(*userDlgsMock, getOpenFileName(_, _, _, _, _, _))
        .WillOnce(Return("/openfile.qssd"));
    EXPECT_CALL(*opersMock, saveDocument(_, _)).Times(0);
    EXPECT_CALL(*opersMock, openDocument(editorMock, _))
        .WillOnce(Invoke([](const QTextEdit* editor, IDocumentFile* openFile) {
            EXPECT_STREQ(
                openFile->fileName().toStdString().c_str(), "/openfile.qssd");
            editor->document()->setModified(false);
            return true;
        }));
    EXPECT_CALL(*processorMock, processDocument(editorMock->document(), true))
        .Times(1);
    (*openBtnAct)->trigger();

    // When user clicks Yes
    // Must set document modified since call to
    // openDocument()/newDocument()/saveDocument() all makes the doc not
    // modified. And for this test it is needed that the doc be modified
    editorMock->document()->setModified(true);
    EXPECT_CALL(*userDlgsMock, question(_, _, _, _, _))
        .Times(1)
        .WillOnce(Return(QMessageBox::Yes));
    Expectation gsfnExp
        = EXPECT_CALL(*userDlgsMock, getSaveFileName(_, _, _, _, _, _))
              .WillOnce(Return("/newdoc.qssd"));
    EXPECT_CALL(*opersMock, saveDocument(_, _))
        .After(gsfnExp)
        .WillOnce(Invoke([](const QTextEdit* editor, IDocumentFile* outFile) {
            // Following line is needed otherwise successive tests
            // won't go right
            editor->document()->setModified(false);
            return true;
        }));
    Expectation gofnExp
        = EXPECT_CALL(*userDlgsMock, getOpenFileName(_, _, _, _, _, _))
              .WillOnce(Return("/anotheropenfile.qssd"));
    EXPECT_CALL(*opersMock, openDocument(editorMock, _))
        .After(gofnExp)
        .WillOnce(Invoke([](const QTextEdit* editor, IDocumentFile* openFile) {
            EXPECT_STREQ(openFile->fileName().toStdString().c_str(),
                "/anotheropenfile.qssd");
            editor->document()->setModified(false);
            return true;
        }));
    EXPECT_CALL(*processorMock, processDocument(editorMock->document(), true))
        .Times(2);
    (*openBtnAct)->trigger();
}

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
                editor->document()->setModified(false);
                return true;
            }));
    EXPECT_CALL(*processorMock, processDocument(editorMock->document(), true))
        .Times(1);

    (*saveBtnAct)->trigger();
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
