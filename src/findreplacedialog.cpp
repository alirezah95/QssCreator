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

    // Clear text edit selection if any
    if (auto cursor = mTextEdit->textCursor(); cursor.hasSelection()) {
        cursor.clearSelection();
        mTextEdit->setTextCursor(cursor);
    }

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

    auto editorDoc = mTextEdit->document();
    // Get the cursor to the begining of the document
    QTextCursor currCursor(editorDoc->begin());

    currCursor = editorDoc->find(text, currCursor, findFlags);
    while (!currCursor.isNull()) {
        QTextEdit::ExtraSelection selection = { currCursor, mFindFormat };
        extraSelcts.push_back(selection);

        currCursor = editorDoc->find(text, currCursor, findFlags);
    }
    mTextEdit->setExtraSelections(extraSelcts);

    mOccurenceCursor.clearSelection();
    return;
}

void FindReplaceDialog::onFindNextButtonPressed()
{
    if (!mTextEdit || ui->findLEdit->text().isEmpty()) {
        return;
    }

    QFlags<QTextDocument::FindFlag> findFlags;
    if (ui->matchCaseChBox->isChecked()) {
        findFlags |= QTextDocument::FindCaseSensitively;
    }
    if (ui->wholeWordChBox->isChecked()) {
        findFlags |= QTextDocument::FindWholeWords;
    }

    findTextAndSetCursor(
        mOccurenceCursor.isNull() ? mTextEdit->textCursor() : mOccurenceCursor,
        findFlags);
    return;
}

void FindReplaceDialog::onFindPrevButtonPressed()
{
    if (!mTextEdit || ui->findLEdit->text().isEmpty()) {
        return;
    }

    QFlags<QTextDocument::FindFlag> findFlags(QTextDocument::FindBackward);
    if (ui->matchCaseChBox->isChecked()) {
        findFlags |= QTextDocument::FindCaseSensitively;
    }
    if (ui->wholeWordChBox->isChecked()) {
        findFlags |= QTextDocument::FindWholeWords;
    }

    findTextAndSetCursor(
        mOccurenceCursor.isNull() ? mTextEdit->textCursor() : mOccurenceCursor,
        findFlags);
    return;
}

void FindReplaceDialog::onReplaceButtonPressed()
{
    if (mOccurenceCursor.isNull() || mOccurenceCursor != mTextEdit->textCursor()
        || !mTextEdit->textCursor().hasSelection()) {
        return;
    }
    mOccurenceCursor.insertText(ui->replaceLEdit->text());
    return;
}

void FindReplaceDialog::onFindReplaceButtonPressed() { }

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
