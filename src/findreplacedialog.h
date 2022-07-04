#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include <QTextCharFormat>
#include <QTextDocument>

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

    /*!
     * \brief Set the find line edit text which in turn finds all the occurences
     * of the texe
     * \param text
     */
    void setFindText(const QString& text);

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
    /*!
     * \brief Performs the find and move the \ref mTextEdit current cursor to
     * the match position (if any)
     * \param flags The find flags
     * \see \a\b QTextDocument::FindFlag
     */
    void findTextAndSetCursor(
        const QTextCursor& from, QFlags<QTextDocument::FindFlag> flags);

    /*!
     * \brief Resets all selections and extra selections (as a result of find
     * procedures) on the mTextEdit
     */
    void resetTextEdit();

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
