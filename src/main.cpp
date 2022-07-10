#include "mainwindow.h"
#include "qssdeditor.h"
#include "qssdfileoperations.h"
#include "qssdprocessor.h"
#include "userdialogs.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    auto editor = new QssdEditor;
    auto docOper = new QssdFileOperations;
    auto userDlgs = new UserDialogs;
    auto processor = new QssdProcessor;

    MainWindow w(editor, docOper, userDlgs, processor);
    w.show();

    return a.exec();
}
