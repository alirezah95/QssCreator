#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "documentfile.h"
#include "iqssdfileoperations.h"
#include "qssdeditor.h"
#include "widgetspreview.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>
#include <QStandardPaths>

#define ACTION_CONNECT(object, slot)                                           \
    connect(object, &QAction::triggered, this, slot)

MainWindow::MainWindow(
    IQssdEditor* editor, IQssdFileOperations* docOper, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mStyleEditor(editor),
      mPreview(new WidgetsPreview), mDocOpers(docOper)
{
    ui->setupUi(this);

    mStyleEditor->setParent(this);
    mDocOpers->setParent(this);

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
    if (mDocOpers) {
        mDocOpers->newDocument(mStyleEditor);
    }
    return;
}

void MainWindow::openDocument()
{
    if (mDocOpers) {
        auto openFileName = QFileDialog::getOpenFileName(this, tr("Open File"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            "Text files (*.qssd)");
        DocumentFile docFile(openFileName);

        if (!mDocOpers->openDocument(mStyleEditor, &docFile)) {
            qDebug() << "Error in opening file: " << docFile.fileName();
        }
    }
    return;
}

void MainWindow::save()
{
    if (mDocOpers) {
        if (mStyleEditor->documentTitle() == DOC_UNTITLED) {
            auto saveFileName
                = QFileDialog::getSaveFileName(this, tr("Save File"),
                    QStandardPaths::writableLocation(
                        QStandardPaths::DocumentsLocation),
                    "Text files (*.qssd)");
            DocumentFile docFile(saveFileName);

            if (!mDocOpers->saveDocument(mStyleEditor, &docFile)) {
                qDebug() << "Error in saving file: " << docFile.fileName();
            }
        }
    }
    return;
}

void MainWindow::saveAs()
{
    if (mDocOpers) {
        auto saveFileName = QFileDialog::getSaveFileName(this,
            tr("Save File As"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            "Text files (*.qssd)");
        DocumentFile docFile(saveFileName);

        if (!mDocOpers->saveDocument(mStyleEditor, &docFile)) {
            qDebug() << "Error in saving file: " << docFile.fileName();
        }
    }
    return;
}

void MainWindow::exportDocument() { }
