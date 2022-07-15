#include "mainwindow.h"
#include "qssdeditor.h"
#include "qssdfileoperations.h"
#include "qssdprocessor.h"
#include "qssdvariablesmodel.h"
#include "userdialogs.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    auto varsModel = new QssdVariablesModel(&a);
    auto processor = new QssdProcessor(varsModel);
    auto editor = new QssdEditor(processor);

    auto docOper = new QssdFileOperations;

    auto userDlgs = new UserDialogs;

    MainWindow w(editor, docOper, userDlgs);
    w.show();

    return a.exec();
}
