#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class WidgetsPreview;
class IQssdEditor;
class IQssdFileOperations;
class IUserDialogs;
class IQssdProcessor;
class IDocumentFile;
class FindReplaceDialog;
class QSplitter;
class QWidgetAction;
class QCheckBox;
class QPushButton;
class QLineEdit;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Construct an object with the given \a editor and \a docOper. The
     * MainWindow takes the ownership of the \a editor and \a docOper
     * \note Neither \a editor nor docOper should be nullptr.
     * \param editor A pointer to a subclass of \ref IQssdEditor which is used
     * as the application's document editor
     * \param docOper A pointer to a
     * \param parent
     */
    MainWindow(IQssdEditor* editor, IQssdFileOperations* docOper,
        IUserDialogs* userDlgs, QWidget* parent = nullptr);
    ~MainWindow();

    /*!
     * \brief Getter for validity of auto export file path
     * \return
     */
    bool isAutoExportFilePathValid() const
    {
        return mIsAutoExportFilePathValid;
    }

protected:
    /*!
     * \brief Overrides \a\b QWidget::closeEvent() to handle app closing
     * \param ev
     */
    virtual void closeEvent(QCloseEvent* ev);

private slots:
    /*!
     * \brief Calls \ref DocumentOperations::newDocument(QTextEdit*)
     * \details This slot is connected to toolbar's new file action
     */
    void newDocument();

    /*!
     * \brief Prompt user to save current document (if needed) and then calls
     * \ref DocumentOperations::openDocument(QTextEdit*, \ref IDocumentFile*)
     * \details This slot is connected to toolbar's open file action
     */
    void openDocument();

    /*!
     * \brief Saves the current document using \ref
     * DocumentOperations::saveDocument(QTextEdit*, \ref IDocumentFile*).
     * \details This slot is connected to toolbar's save file action
     */
    void save();

    /*!
     * \brief Prompts user for a the save path and then aves the document using
     * \ref DocumentOperations::saveDocument(const QTextEdit*, \ref
     * IDocumentFile*).
     * \details This slot is connected to toolbar's saveAs file action
     */
    void saveAs();

    /*!
     * \brief Exports the document using \ref
     * DocumentOperations::exportDocument(const QTextEdit*, \ref
     * IDocumentFile*).
     * \details This slot is connected to toolbar's export file action
     */
    void exportDocument();

    /*!
     * \brief Updates the title of the windows based on the documents title and
     * status
     */
    void updateWindowTitle();

private:
    /*!
     * \brief Set up the actions and items in the main toolbar
     */
    void setUpToolbar();

    /*!
     * \brief Prompt user for saving current document
     * \return \a True if user presses \a Save and saves the doc or presses \a
     * No button. Returns \a false if user presses \a Cancel button or presses
     * \a Save button but the save process goes wrong
     */
    bool maybeSave();

    /*!
     * \brief Saves the document using the \a docFile
     * \param docFile
     * \return True if saved successfully and false otherwise
     */
    bool saveDocument();

    /*!
     * \brief reset
     */
    void reset();

    /*!
     * \brief A private method to create all signal/slot connection and is used
     * to tidy ctor up
     */
    void setupConnections();

    /*!
     * \brief Reads the settings from \a\b QSettings and apply them
     */
    void readSettings();

    /*!
     * \brief Write the settings to \a\b QSettings
     */
    void writeSettings();

private:
    Ui::MainWindow* ui;

    QSplitter* mSplitter; /*!< A splitter for style editor and preview widget.*/

    IQssdEditor* mStyleEditor; /*!< A \ref StyleSheetEditor providing user
                                * the ability to edit Qt style sheets.
                                */

    WidgetsPreview* mPreview; /*!< A \ref WidgetsPreview for real-time preview
                               * of the code in style editor.
                               */

    IQssdFileOperations* mDocOpers; /*!< A \ref DocumentOperations instance
                                     * responsible for handling file operations
                                     * related to the app text editor.
                                     */

    IUserDialogs* mUserDlgs; /*!< A \ref IUserDialogs instance responsible for
                              * showing user dialogs for different kind of
                              * operations
                              */

    FindReplaceDialog* mFindReplaceDlg; /*!< A \ref FindReplaceDialog to perform
                                         * find and replace
                                         */

    bool mIsAutoExportFilePathValid = false; /*!< A boolean variable indicating
                                              * if the file path for auto export
                                              * is valid
                                              */

    QCheckBox* mAutoExportCheckBox; /*!< Checkbox for set/unset auto export*/
    QWidgetAction* mAutoExportCheckAct; /*!< A checkbox \a\b QWidgetAction to
                                         * enable/disable automatic export.
                                         */

    QLineEdit* mAutoExportLEdit; /*!< Line edit for setting auto export path.*/
    QWidgetAction* mAutoExportLEditAct; /*!< A line edit \a\b QWidgetAction to
                                         * set the file path of auto export.
                                         */

    QPushButton* mAutoExportBrowseBtn; /*!< Button for selecting export path*/
    QWidgetAction* mAutoExportBrowseBtnAct; /*!< A button to browse the auto
                                             * export file path.
                                             */
};
#endif // MAINWINDOW_H
