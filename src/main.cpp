#include "mainwindow.h"
#include "qssdeditor.h"
#include "qssdfileoperations.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    auto editor = new QssdEditor;
    auto docOper = new QssdFileOperations;

    MainWindow w(editor, docOper);
    w.show();

    return a.exec();
}
