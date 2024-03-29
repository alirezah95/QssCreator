#include "findreplacedialog.h"
#include "ui_findreplacedialog.h"

#include <QApplication>
#include <QTextBlock>
#include <QTextEdit>

FindReplaceDialog::FindReplaceDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::FindReplaceDialog)
{
    ui->setupUi(this);

    auto hColor = QPalette().highlight().color();
    hColor = (hColor.lightnessF() > 0.5 ? hColor.darker() : hColor.lighter());
    mFindFormat.setBackground(QBrush(hColor));
    mFindFormat.setForeground(QPalette().highlightedText());

    connect(ui->findNxtBtn, &QPushButton::clicked, this,
        &FindReplaceDialog::onFindNextButtonPressed);
    connect(ui->findPrevBtn, &QPushButton::clicked, this,
        &FindReplaceDialog::onFindPrevButtonPressed);
    connect(ui->replaceBtn, &QPushButton::clicked, this,
        &FindReplaceDialog::onReplaceButtonPressed);
    connect(ui->findReplaceBtn, &QPushButton::clicked, this,
        &FindReplaceDialog::onFindReplaceButtonPressed);
    connect(ui->replaceAllBtn, &QPushButton::clicked, this,
        &FindReplaceDialog::onReplaceAllButtonPressed);

    connect(ui->findLEdit, &QLineEdit::textChanged, this,
        &FindReplaceDialog::findAllOccurences);
    connect(ui->wholeWordChBox, &QCheckBox::clicked, this, [this] {
        findAllOccurences(ui->findLEdit->text());
        return;
    });
    connect(ui->matchCaseChBox, &QCheckBox::clicked, this, [this] {
        findAllOccurences(ui->findLEdit->text());
        return;
    });
}

FindReplaceDialog::FindReplaceDialog(QTextEdit* txtEdit, QWidget* parent)
    : FindReplaceDialog(parent)
{
    mTextEdit = txtEdit;
}

FindReplaceDialog::~FindReplaceDialog()
{
    delete ui;
}

void FindReplaceDialog::setTextEdit(QTextEdit* txtEdit)
{
    if (mTextEdit) {
        // Disconnect all connections
        mTextEdit->disconnect(this);
    }
    mOccurenceCursor = QTextCursor();
    mTextEdit = txtEdit;

    if (mTextEdit) {
        mOccurenceCursor = mTextEdit->textCursor();
        mOccurenceCursor.movePosition(QTextCursor::Start);
        mOccurenceCursor.clearSelection();

        connect(mTextEdit->document(), &QTextDocument::contentsChange, this,
            [this](int from, int charsRemoved, int charsAdded) {
                if (charsRemoved || charsAdded) {
                    // It's not just a format change, need to find occurences
                    findAllOccurences(ui->findLEdit->text());
                }
            });
    }
    return;
}

void FindReplaceDialog::setReplaceEnabled(bool repEnabled)
{
    ui->replaceBtn->setVisible(repEnabled);
    ui->findReplaceBtn->setVisible(repEnabled);
    ui->replaceLbl->setVisible(repEnabled);
    ui->replaceLEdit->setVisible(repEnabled);

    return;
}

void FindReplaceDialog::setFindText(const QString& text)
{
    resetTextEdit();
    ui->findLEdit->setText(text);
    return;
}

void FindReplaceDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    resize(minimumSizeHint());

    if (auto par = parentWidget()) {
        move(QPoint(par->geometry().x() + par->width() - width(),
                 par->geometry().y())
            + QPoint(0, par->frameGeometry().height() - par->height()));
    }
    return;
}

void FindReplaceDialog::hideEvent(QHideEvent* event)
{
    QDialog::hideEvent(event);
    resetTextEdit();
}

void FindReplaceDialog::findAllOccurences(const QString& text)
{
    if (!mTextEdit || text.isEmpty()) {
        return;
    }

    // Clear text edit selection if any, and clear extra selections
    if (auto cursor = mTextEdit->textCursor(); cursor.hasSelection()) {
        cursor.clearSelection();
        mTextEdit->setTextCursor(cursor);
    }
    mTextEdit->setExtraSelections({});

    // Set occurence index to -1
    mCurrentOccurenceIndex = -1;

    // To hold the occurences as extra selections
    QList<QTextEdit::ExtraSelection> extraSelcts;

    // Construct the find flags
    QFlags<QTextDocument::FindFlag> findFlags;
    if (ui->matchCaseChBox->isChecked()) {
        findFlags |= QTextDocument::FindFlag::FindCaseSensitively;
    }
    if (ui->wholeWordChBox->isChecked()) {
        findFlags |= QTextDocument::FindFlag::FindWholeWords;
    }

    QTextCursor editorCurrentCursor = mTextEdit->textCursor();

    auto editorDoc = mTextEdit->document();
    // Get the cursor to the begining of the document
    QTextCursor currCursor(editorDoc->begin());

    currCursor = editorDoc->find(text, currCursor, findFlags);
    while (!currCursor.isNull()) {
        QTextEdit::ExtraSelection selection = { currCursor, mFindFormat };
        extraSelcts.push_back(selection);

        // If currCursor is after the actual current cursor of the text edit its
        // index should be stored as the current occurence index
        if (mCurrentOccurenceIndex == -1
            && currCursor.anchor() >= editorCurrentCursor.position()) {
            mCurrentOccurenceIndex = extraSelcts.size() - 1;
        }

        currCursor = editorDoc->find(text, currCursor, findFlags);
    }
    if (extraSelcts.size() > 0) {
        mTextEdit->setExtraSelections(extraSelcts);

        // If the actual cursor in the editor is at the end of document, then
        // the if statement in the while loop will never evaluate to true even
        // if there are occurences. So in that case (if there are occurences)
        // the first occurence index should be stored in mCurrentOccurenceIndex
        if (mCurrentOccurenceIndex == -1) {
            mCurrentOccurenceIndex = 0;
        }
        mTextEdit->setTextCursor(extraSelcts[mCurrentOccurenceIndex].cursor);
    }
    return;
}

void FindReplaceDialog::onFindNextButtonPressed()
{
    if (!mTextEdit || ui->findLEdit->text().isEmpty()) {
        return;
    }

    const auto& allOccurences = mTextEdit->extraSelections();
    if (allOccurences.size() == 0) {
        // No match is found by findAllOccurences()
        return;
    }

    // If there are occurences but mCurrOccurence is -1 (invalid), update it
    if (mCurrentOccurenceIndex == -1) {
        updateCurrentOccureneceIndex();
    } else {
        // Increase current occurence index
        mCurrentOccurenceIndex++;
        if (mCurrentOccurenceIndex >= allOccurences.size()) {
            mCurrentOccurenceIndex = 0;
        }
    }
    mTextEdit->setTextCursor(allOccurences[mCurrentOccurenceIndex].cursor);

    return;
}

void FindReplaceDialog::onFindPrevButtonPressed()
{
    if (!mTextEdit || ui->findLEdit->text().isEmpty()) {
        return;
    }

    const auto& allOccurences = mTextEdit->extraSelections();
    if (allOccurences.size() == 0) {
        // No match is found by findAllOccurences()
        return;
    }

    // If there are occurences but mCurrOccurence is -1 (invalid), update it
    if (mCurrentOccurenceIndex == -1) {
        updateCurrentOccureneceIndex();
    } else {
        // Increase current occurence index
        mCurrentOccurenceIndex--;
        if (mCurrentOccurenceIndex < 0) {
            mCurrentOccurenceIndex = allOccurences.size() - 1;
        }
    }
    mTextEdit->setTextCursor(allOccurences[mCurrentOccurenceIndex].cursor);

    return;
}

void FindReplaceDialog::onReplaceButtonPressed()
{
    if (mCurrentOccurenceIndex == -1) {
        return;
    }
    const auto& allOccurences = mTextEdit->extraSelections();
    if (allOccurences.size() == 0) {
        // No match is found by findAllOccurences()
        return;
    }

    QTextCursor currOccurCursor = allOccurences[mCurrentOccurenceIndex].cursor;
    if (currOccurCursor != mTextEdit->textCursor()
        || !mTextEdit->textCursor().hasSelection()) {
        return;
    }
    currOccurCursor.insertText(ui->replaceLEdit->text());

    // Invalidate current occurence for next replace and also resets editor's
    // cursor moving to next occurence which findAllOccurences() causes it
    mCurrentOccurenceIndex = -1;
    auto editCurs = mTextEdit->textCursor();
    editCurs.setPosition(currOccurCursor.position());
    mTextEdit->setTextCursor(editCurs);

    return;
}

void FindReplaceDialog::onFindReplaceButtonPressed()
{
    onReplaceButtonPressed();
    onFindNextButtonPressed();
}

void FindReplaceDialog::onReplaceAllButtonPressed()
{
    if (!mTextEdit) {
        return;
    }
    auto allOccurences = mTextEdit->extraSelections();
    if (allOccurences.size() == 0) {
        return;
    }

    const auto& replaceTxt = ui->replaceLEdit->text();
    for (int i = 0; i < allOccurences.size(); ++i) {
        QTextCursor& cursor = allOccurences[i].cursor;
        cursor.beginEditBlock();
        cursor.insertText(replaceTxt);
    }

    for (int i = 0; i < allOccurences.size(); ++i) {
        QTextCursor& cursor = allOccurences[i].cursor;
        cursor.endEditBlock();
    }
}

void FindReplaceDialog::findTextAndSetCursor(
    const QTextCursor& from, QFlags<QTextDocument::FindFlag> flags)
{
    // Clear selection in text edit
    if (auto cursor = mTextEdit->textCursor(); cursor.hasSelection()) {
        cursor.clearSelection();
        mTextEdit->setTextCursor(cursor);
    }

    QTextCursor found
        = mTextEdit->document()->find(ui->findLEdit->text(), from, flags);

    if (!found.isNull()) {
        // Save a copy of it
        mOccurenceCursor = found;
        mTextEdit->setTextCursor(found);
    }
    return;
}

void FindReplaceDialog::updateCurrentOccureneceIndex()
{
    mCurrentOccurenceIndex = -1;
    const auto& editorTextCursor = mTextEdit->textCursor();
    const auto& allOccurences = mTextEdit->extraSelections();
    for (int i = 0; i < allOccurences.size(); ++i) {
        if (allOccurences[i].cursor.anchor() >= editorTextCursor.position()) {
            mCurrentOccurenceIndex = i;
            return;
        }
    }
    if (mCurrentOccurenceIndex == -1 && allOccurences.size() > 0) {
        mCurrentOccurenceIndex = 0;
    }
}

void FindReplaceDialog::resetTextEdit()
{
    if (mTextEdit) {
        if (auto cursor = mTextEdit->textCursor(); cursor.hasSelection()) {
            cursor.clearSelection();
            mTextEdit->setTextCursor(cursor);
        }
        mTextEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
    }
    return;
}
