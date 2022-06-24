#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class WidgetsPreview;
class StyleSheetEditor;

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

private:
    Ui::MainWindow* ui;

    StyleSheetEditor* mStyleEditor; /*!< A \ref StyleSheetEditor providing user
                                     * the ability to edit Qt style sheets.
                                     */

    WidgetsPreview* mPreview; /*!< A \ref WidgetsPreview for real-time preview
                               * of the code in style editor.
                               */
};
#endif // MAINWINDOW_H
