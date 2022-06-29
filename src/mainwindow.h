#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class WidgetsPreview;
class StyleSheetEditor;
class DocumentOperations;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
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

private:
    Ui::MainWindow* ui;

    StyleSheetEditor* mStyleEditor; /*!< A \ref StyleSheetEditor providing user
                                     * the ability to edit Qt style sheets.
                                     */

    WidgetsPreview* mPreview; /*!< A \ref WidgetsPreview for real-time preview
                               * of the code in style editor.
                               */

    DocumentOperations* mDocOpers; /*!< A \ref DocumentOperations instance
                                    * responsible for handling file operations
                                    * related to the app text editor.
                                    */
};
#endif // MAINWINDOW_H
