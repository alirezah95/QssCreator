#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "stylesheeteditor.h"
#include "widgetspreview.h"

#include <QHBoxLayout>
#include <QSplitter>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      mStyleEditor(new StyleSheetEditor), mPreview(new WidgetsPreview)
{
    ui->setupUi(this);

    auto splitter = new QSplitter;
    splitter->addWidget(mStyleEditor);
    splitter->addWidget(mPreview);

    auto hbox = new QHBoxLayout;
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->addWidget(splitter);

    ui->centralwidget->setLayout(hbox);
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}
