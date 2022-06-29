#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "documentoperations.h"
#include "stylesheeteditor.h"
#include "widgetspreview.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>

#define ACTION_CONNECT(object, slot)                                           \
    connect(object, &QAction::triggered, this, slot)

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      mStyleEditor(new StyleSheetEditor), mPreview(new WidgetsPreview),
      mDocOpers(new DocumentOperations(this))
{
    ui->setupUi(this);

    auto splitter = new QSplitter;
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(mStyleEditor);
    splitter->addWidget(mPreview);

    auto hbox = new QHBoxLayout;
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->addWidget(splitter);

    ui->centralwidget->setLayout(hbox);

    /* Set up actions connections */
    ACTION_CONNECT(ui->actionNewFile, &MainWindow::newDocument);
    ACTION_CONNECT(ui->actionOpenFile, &MainWindow::openDocument);
    ACTION_CONNECT(ui->actionSave, &MainWindow::save);
    ACTION_CONNECT(ui->actionSaveAs, &MainWindow::saveAs);

    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newDocument()
{
    if (mDocOpers) { }
    return;
}

void MainWindow::openDocument()
{
    if (mDocOpers) { }
    return;
}

void MainWindow::save()
{
    if (mDocOpers) { }
    return;
}

void MainWindow::saveAs()
{
    if (mDocOpers) { }
    return;
}
