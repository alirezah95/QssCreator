#include "findreplacedialog.h"
#include "ui_findreplacedialog.h"

#include <QTextBlock>
#include <QTextEdit>

FindReplaceDialog::FindReplaceDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::FindReplaceDialog)
{
    ui->setupUi(this);

    mFindFormat.setBackground(QPalette().highlight());
    mFindFormat.setForeground(QPalette().highlightedText());

    auto hColor = QPalette().highlight().color();
    hColor = (hColor.lightnessF() > 0.5 ? hColor.darker() : hColor.lighter());
    mHighlightFormat.setBackground(QBrush(hColor));
    mHighlightFormat.setForeground(QPalette().highlightedText());

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
    mTextEdit = txtEdit;
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

void FindReplaceDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    resize(minimumSizeHint());
    return;
}

void FindReplaceDialog::findAllOccurences(const QString& text)
{
    if (!mTextEdit || text.isEmpty()) {
        return;
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

    QTextCursor current = mTextEdit->textCursor();
    if (current.isNull()) {
        return;
    }

    QTextCursor next = mTextEdit->document()->find(
        ui->findLEdit->text(), current, findFlags);

    if (!next.isNull()) {
        mTextEdit->setTextCursor(next);
    }
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

    QTextCursor current = mTextEdit->textCursor();
    if (current.isNull()) {
        return;
    }

    QTextCursor next = mTextEdit->document()->find(
        ui->findLEdit->text(), current, findFlags);

    if (!next.isNull()) {
        mTextEdit->setTextCursor(next);
    }
}

void FindReplaceDialog::onReplaceButtonPressed() { }

void FindReplaceDialog::onFindReplaceButtonPressed() { }
