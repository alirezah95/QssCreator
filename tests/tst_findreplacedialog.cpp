#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QTextCursor>
#include <QTextEdit>

#include <gtest/gtest.h>

#include "findreplacedialog.h"

class TestFindReplaceDialog : public ::testing::Test
{
public:
    void SetUp()
    {
        editor = new QTextEdit;
        frDialog = new FindReplaceDialog;

        frDialog->setTextEdit(editor);
    }

    void TearDown()
    {
        delete editor;
        delete frDialog;
    }

    QTextEdit* editor;
    FindReplaceDialog* frDialog;
};

TEST_F(TestFindReplaceDialog, TestFindAllOccurences)
{
    editor->insertPlainText("cross-platform for creating gui for Linux, etc");

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    findLEdit->setText("for");

    auto extraSelects = editor->extraSelections();

    EXPECT_EQ(extraSelects.size(), 3);

    for (auto& selection : extraSelects) {
        selection.cursor.insertText("***");
    }
    EXPECT_STREQ(editor->document()->toPlainText().toStdString().c_str(),
        "cross-plat***m *** creating gui *** Linux, etc");
}

TEST_F(TestFindReplaceDialog, TestFindNext)
{
    editor->insertPlainText(
        "Qt is cross-platform software for graphical user interfaces.");
    editor->moveCursor(QTextCursor::Start);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    auto findNxtBtn = frDialog->findChild<QPushButton*>("findNxtBtn");

    findLEdit->setText("graph");
    findNxtBtn->click();

    auto findCursor = editor->textCursor();
    findCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    EXPECT_STREQ(findCursor.selectedText().toStdString().c_str(), "graphical");
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
