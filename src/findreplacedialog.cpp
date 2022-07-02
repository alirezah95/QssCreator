#include "findreplacedialog.h"
#include "ui_findreplacedialog.h"

#include <QTextEdit>

FindReplaceDialog::FindReplaceDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::FindReplaceDialog)
{
    ui->setupUi(this);

    connect(ui->findNxtBtn, &QPushButton::clicked, this,
        &FindReplaceDialog::onFindNextButtonPressed);
    connect(ui->findPrevBtn, &QPushButton::clicked, this,
        &FindReplaceDialog::onFindPrevButtonPressed);
    connect(ui->replaceBtn, &QPushButton::clicked, this,
        &FindReplaceDialog::onReplaceButtonPressed);
    connect(ui->findReplaceBtn, &QPushButton::clicked, this,
        &FindReplaceDialog::onFindReplaceButtonPressed);
}

FindReplaceDialog::FindReplaceDialog(QTextEdit* txtEdit, QWidget* parent)
    : QDialog(parent), mTextEdit(txtEdit)
{
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

void FindReplaceDialog::onFindNextButtonPressed() { }

void FindReplaceDialog::onFindPrevButtonPressed() { }

void FindReplaceDialog::onReplaceButtonPressed() { }

void FindReplaceDialog::onFindReplaceButtonPressed() { }
