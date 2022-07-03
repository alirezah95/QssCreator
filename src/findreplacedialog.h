#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include <QTextCharFormat>

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

protected:
    /*!
     * \brief Reimplement \a\b QDialog::showEvent(QShowEvent*) to resize dialog
     * \param event
     */
    void showEvent(QShowEvent* event) override;

    /*!
     * \brief Reimplement \a\b QDialog::hideEvent(QShowEvent*) to clear find
     *
     * \param event
     */
    void hideEvent(QHideEvent* event) override;

private slots:
    /*!
     * \brief Slot to find all the occurences.
     * \param text The text to search for occurences
     */
    void findAllOccurences(const QString& text);

    /*!
     * \brief Finds the next occurence of the text in line edit and highlight it
     */
    void onFindNextButtonPressed();

    /*!
     * \brief Finds the previout occurence of the text in line edit and
     * highlight it
     */
    void onFindPrevButtonPressed();

    /*!
     * \brief Replace the text under the cursor with the text in replace line
     * edit
     */
    void onReplaceButtonPressed();

    /*!
     * \brief Replace the text under the cursor with the text in replace line
     * edit and calls the \ref onFindNextButtonPressed()
     */
    void onFindReplaceButtonPressed();

private:
    Ui::FindReplaceDialog* ui;

    QTextEdit* mTextEdit = nullptr;

    QTextCharFormat mFindFormat; /*!< The format that is used for highlighting
                                  * text occurences.
                                  */

    QTextCharFormat mHighlightFormat; /*!< The format that is used for text
                                       * occurence that the cursor is currently
                                       * on.
                                       */
};

#endif // FINDREPLACEDIALOG_H
