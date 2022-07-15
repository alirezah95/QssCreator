#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class WidgetsPreview;
class IQssdEditor;
class IQssdFileOperations;
class IUserDialogs;
class IQssdProcessor;
class FindReplaceDialog;

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
     * \brief reset
     */
    void reset();
    /*!
     * \brief A private method to create all signal/slot connection and is used
     * to tidy ctor up
     */
    void setupConnections();

private:
    Ui::MainWindow* ui;

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
};
#endif // MAINWINDOW_H
