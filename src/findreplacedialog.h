#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class FindReplaceDialog;
}

class QTextEdit;

class FindReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindReplaceDialog(QWidget* parent = nullptr);
    FindReplaceDialog(QTextEdit* txtEdit, QWidget* parent);
    ~FindReplaceDialog();

    /*!
     * \brief Sets the text editor for find/replace operations
     * \param txtEdit
     */
    void setTextEdit(QTextEdit* txtEdit);

    /*!
     * \brief Set replace operations enabled or disabled
     * \param repEnabled
     */
    void setReplaceEnabled(bool repEnabled);

private:
    Ui::FindReplaceDialog* ui;

    QTextEdit* mTextEdit = nullptr;
};

#endif // FINDREPLACEDIALOG_H
