#include <QApplication>
#include <QCheckBox>
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

TEST_F(TestFindReplaceDialog, TestFindAllOccurencesSettingCurrentOccurIndex)
{
    editor->insertPlainText("cross-platform for creating gui for Linux,etc");

    auto edtCurs = editor->textCursor(); // is at the end of document
    edtCurs.movePosition(QTextCursor::Start);
    edtCurs.movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor, 4);
    editor->setTextCursor(edtCurs);
    // Now the cursor is at the start of "creating" word in document

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    findLEdit->setText("for");

    EXPECT_EQ(frDialog->getCurrentOccurenceIndex(), 2);
}

TEST_F(TestFindReplaceDialog, TestFindAllOccurencesNoFlags)
{
    editor->insertPlainText("cross-platform for creating gui for Linux, etc");

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    findLEdit->setText("for");

    auto extraSelects = editor->extraSelections();

    EXPECT_EQ(extraSelects.size(), 3);
    EXPECT_EQ(frDialog->getCurrentOccurenceIndex(), 0);
}

TEST_F(TestFindReplaceDialog, TestFindAllOccurencesWholeWord)
{
    editor->insertPlainText("cross-platform for creating gui for Linux, etc");

    auto wholeWordChBox = frDialog->findChild<QCheckBox*>("wholeWordChBox");
    ASSERT_NE(wholeWordChBox, nullptr) << "No whole word checkbox";
    wholeWordChBox->setChecked(true);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    findLEdit->setText("for");

    auto extraSelects = editor->extraSelections();

    EXPECT_EQ(extraSelects.size(), 2);
}

TEST_F(TestFindReplaceDialog, TestFindAllOccurencesMatchCase)
{
    editor->insertPlainText("cross-platform for creating gui for Linux, etc");

    auto matchCase = frDialog->findChild<QCheckBox*>("matchCaseChBox");
    ASSERT_NE(matchCase, nullptr) << "No match case checkbox";
    matchCase->setChecked(true);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    findLEdit->setText("For");

    auto extraSelects = editor->extraSelections();

    EXPECT_EQ(extraSelects.size(), 0);
}

TEST_F(TestFindReplaceDialog, TestFindNext)
{
    editor->insertPlainText(
        "Qt is cross-platform software for graphical user interfaces.");
    editor->moveCursor(QTextCursor::Start);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    auto findNxtBtn = frDialog->findChild<QPushButton*>("findNxtBtn");

    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    ASSERT_NE(findNxtBtn, nullptr) << "No find next button";

    findLEdit->setText("for");

    findNxtBtn->click();

    auto findCursor = editor->textCursor();
    EXPECT_STREQ(findCursor.selectedText().toStdString().c_str(), "for");

    // Perform another find operation
    findNxtBtn->click();
    findCursor = editor->textCursor();
    findCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    EXPECT_STREQ(findCursor.selectedText().toStdString().c_str(), "form");
}

TEST_F(TestFindReplaceDialog, TestFindNextWholeMatchCase)
{
    editor->insertPlainText(
        "Qt is cross-platform software for graphical user interfaces.");
    editor->moveCursor(QTextCursor::Start);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    auto findNxtBtn = frDialog->findChild<QPushButton*>("findNxtBtn");

    auto matchCase = frDialog->findChild<QCheckBox*>("matchCaseChBox");
    auto wholeWordChBox = frDialog->findChild<QCheckBox*>("wholeWordChBox");

    ASSERT_NE(wholeWordChBox, nullptr) << "No whole word checkbox";
    ASSERT_NE(matchCase, nullptr) << "No match case checkbox";
    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    ASSERT_NE(findNxtBtn, nullptr) << "No find next button";

    matchCase->setChecked(true);
    wholeWordChBox->setChecked(true);

    frDialog->setTextEdit(nullptr); // To prevent findAllOccurences() running
    findLEdit->setText("graph");
    frDialog->setTextEdit(editor);

    findNxtBtn->click();

    auto findCursor = editor->textCursor();
    EXPECT_EQ(findCursor.atStart(), true);
}

TEST_F(TestFindReplaceDialog, TestFindPrev)
{
    editor->insertPlainText(
        "Qt is cross-platform software for graphical user interfaces.");
    editor->moveCursor(QTextCursor::End);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    auto findPrevBtn = frDialog->findChild<QPushButton*>("findPrevBtn");

    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    ASSERT_NE(findPrevBtn, nullptr) << "No find previous button";

    findLEdit->setText("graph");

    findPrevBtn->click();

    auto findCursor = editor->textCursor();
    findCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    EXPECT_STREQ(findCursor.selectedText().toStdString().c_str(), "graphical");
}

TEST_F(TestFindReplaceDialog, TestFindPrevWholeMatchCase)
{
    editor->insertPlainText(
        "Qt is cross-platform software for graphical user interfaces.");
    editor->moveCursor(QTextCursor::End);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    auto findPrevBtn = frDialog->findChild<QPushButton*>("findPrevBtn");

    auto matchCase = frDialog->findChild<QCheckBox*>("matchCaseChBox");
    auto wholeWordChBox = frDialog->findChild<QCheckBox*>("wholeWordChBox");

    ASSERT_NE(wholeWordChBox, nullptr) << "No whole word checkbox";
    ASSERT_NE(matchCase, nullptr) << "No match case checkbox";
    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    ASSERT_NE(findPrevBtn, nullptr) << "No find previous button";

    matchCase->setChecked(true);
    wholeWordChBox->setChecked(true);

    frDialog->setTextEdit(nullptr); // To prevent findAllOccurences() running
    findLEdit->setText("graph");
    frDialog->setTextEdit(editor);

    findPrevBtn->click();

    auto findCursor = editor->textCursor();
    EXPECT_EQ(findCursor.atEnd(), true);
}

TEST_F(TestFindReplaceDialog, FindNextWithSelection)
{
    editor->insertPlainText(
        "Qt is cross-platform software for graphical user interfaces.");
    // Selecting all the text in the document
    editor->moveCursor(QTextCursor::Start, QTextCursor::KeepAnchor);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    auto findNxtBtn = frDialog->findChild<QPushButton*>("findNxtBtn");

    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    ASSERT_NE(findNxtBtn, nullptr) << "No find next button";

    frDialog->setTextEdit(nullptr); // To prevent findAllOccurences() running
    findLEdit->setText("for");
    frDialog->setTextEdit(editor);

    findNxtBtn->click();
    EXPECT_TRUE(editor->textCursor().hasSelection());
}

TEST_F(TestFindReplaceDialog, TestSetFindText)
{
    editor->insertPlainText(
        "Qt is cross-platform software for graphical user interfaces.");
    editor->moveCursor(QTextCursor::Start);
    editor->moveCursor(QTextCursor::NextWord, QTextCursor::KeepAnchor);

    EXPECT_STREQ(
        editor->textCursor().selectedText().toStdString().c_str(), "Qt ");

    frDialog->setFindText("Qt");

    EXPECT_TRUE(editor->textCursor().hasSelection());
}

TEST_F(TestFindReplaceDialog, TestTextOccurencesEdited)
{
    editor->insertPlainText("cross-platform for creating gui for Linux, etc");

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");

    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    findLEdit->setText("for");

    EXPECT_EQ(editor->extraSelections().size(), 3);

    // Move editor cursor at start
    editor->moveCursor(QTextCursor::Start);
    // Insert a "for" at start
    editor->insertPlainText("for");
    EXPECT_EQ(editor->extraSelections().size(), 4);

    // delete all the text
    editor->setPlainText("for Linux, etc");
    EXPECT_EQ(editor->extraSelections().size(), 1);
}

TEST_F(TestFindReplaceDialog, TestReplace)
{
    editor->insertPlainText("cross-platform for creating gui for Linux, etc");
    editor->moveCursor(QTextCursor::Start);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    auto replaceLEdit = frDialog->findChild<QLineEdit*>("replaceLEdit");

    auto findNxtBtn = frDialog->findChild<QPushButton*>("findNxtBtn");
    auto replaceBtn = frDialog->findChild<QPushButton*>("replaceBtn");

    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    ASSERT_NE(replaceLEdit, nullptr) << "No replace line edit";
    ASSERT_NE(findNxtBtn, nullptr) << "No find next button";
    ASSERT_NE(replaceBtn, nullptr) << "No replace button";

    findLEdit->setText("for");

    replaceLEdit->setText("***");
    replaceBtn->click();

    EXPECT_STREQ(editor->document()->toPlainText().toStdString().c_str(),
        "cross-plat***m for creating gui for Linux, etc");
}

TEST_F(TestFindReplaceDialog, TestReplaceFind)
{
    editor->insertPlainText("cross-platform for creating gui for Linux, etc");
    editor->moveCursor(QTextCursor::Start);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    auto replaceLEdit = frDialog->findChild<QLineEdit*>("replaceLEdit");

    auto findReplaceBtn = frDialog->findChild<QPushButton*>("findReplaceBtn");

    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    ASSERT_NE(replaceLEdit, nullptr) << "No replace line edit";
    ASSERT_NE(findReplaceBtn, nullptr) << "No findReplace button";

    findLEdit->setText("for");
    replaceLEdit->setText("***");

    findReplaceBtn->click();
    EXPECT_STREQ(editor->document()->toPlainText().toStdString().c_str(),
        "cross-plat***m for creating gui for Linux, etc");

    findReplaceBtn->click();
    EXPECT_STREQ(editor->document()->toPlainText().toStdString().c_str(),
        "cross-plat***m *** creating gui for Linux, etc");
}

TEST_F(TestFindReplaceDialog, TestReplaceAll)
{
    editor->insertPlainText("cross-platform for creating gui for Linux, etc");
    editor->moveCursor(QTextCursor::Start);

    auto findLEdit = frDialog->findChild<QLineEdit*>("findLEdit");
    auto replaceLEdit = frDialog->findChild<QLineEdit*>("replaceLEdit");

    auto replaceAllBtn = frDialog->findChild<QPushButton*>("replaceAllBtn");

    ASSERT_NE(findLEdit, nullptr) << "No find line edit";
    ASSERT_NE(replaceLEdit, nullptr) << "No replace line edit";
    ASSERT_NE(replaceAllBtn, nullptr) << "No replaceAll button";

    findLEdit->setText("for");
    replaceLEdit->setText("***");

    replaceAllBtn->click();
    EXPECT_STREQ(editor->document()->toPlainText().toStdString().c_str(),
        "cross-plat***m *** creating gui *** Linux, etc");
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
