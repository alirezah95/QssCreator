#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "documentfile.h"
#include "findreplacedialog.h"
#include "iqssdfileoperations.h"
#include "qssdeditor.h"
#include "widgetspreview.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>
#include <QStandardPaths>

/* To create connections between toolbar actions and this class */
#define ACT_CONNECT_THIS(act, slot)                                            \
    connect(act, &QAction::triggered, this, &MainWindow::slot)

/* To create connections between toolbar actions and editor object */
#define ACT_CONNECT_EDITOR(act, slot)                                          \
    connect(act, &QAction::triggered, mStyleEditor, &IQssdEditor::slot)

MainWindow::MainWindow(
    IQssdEditor* editor, IQssdFileOperations* docOper, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mStyleEditor(editor),
      mPreview(new WidgetsPreview), mDocOpers(docOper), mFindReplaceDlg(nullptr)
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

    reset();

    setupConnections();

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
            DOC_FILTER);
        if (openFileName.isEmpty()) {
            return;
        }
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
                    DOC_FILTER);
            if (saveFileName.isEmpty()) {
                return;
            }
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
        auto saveAsFileName = QFileDialog::getSaveFileName(this,
            tr("Save File As"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            DOC_FILTER);
        if (saveAsFileName.isEmpty()) {
            return;
        }
        DocumentFile docFile(saveAsFileName);

        if (!mDocOpers->saveDocument(mStyleEditor, &docFile)) {
            qDebug() << "Error in saving file: " << docFile.fileName();
        }
    }
    return;
}

void MainWindow::exportDocument() { }

void MainWindow::reset()
{
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionSave->setEnabled(false);

    return;
}

void MainWindow::setupConnections()
{
    /* Set up actions connections */
    ACT_CONNECT_THIS(ui->actionNewFile, newDocument);
    ACT_CONNECT_THIS(ui->actionOpenFile, openDocument);
    ACT_CONNECT_THIS(ui->actionSave, save);
    ACT_CONNECT_THIS(ui->actionSaveAs, saveAs);

    ACT_CONNECT_EDITOR(ui->actionUndo, undo);
    ACT_CONNECT_EDITOR(ui->actionRedo, redo);
    ACT_CONNECT_EDITOR(ui->actionCopy, copy);
    ACT_CONNECT_EDITOR(ui->actionCut, cut);
    ACT_CONNECT_EDITOR(ui->actionPaste, paste);

    connect(mStyleEditor, &IQssdEditor::undoAvailable, ui->actionUndo,
        &QAction::setEnabled);
    connect(mStyleEditor, &IQssdEditor::redoAvailable, ui->actionRedo,
        &QAction::setEnabled);

    connect(mStyleEditor, &IQssdEditor::copyAvailable, this, [this](bool yes) {
        ui->actionCopy->setEnabled(yes);
        ui->actionCut->setEnabled(yes);
    });

    // Connection for increase/decrease font actions
    connect(ui->actionIncreaseFont, &QAction::triggered, this, [this] {
        auto font = mStyleEditor->font();
        font.setPointSize(qMin(FONT_MAX_POINT_SIZE, font.pointSize() + 1));
        mStyleEditor->setFont(font);
    });
    connect(ui->actionDecreaseFont, &QAction::triggered, this, [this] {
        auto font = mStyleEditor->font();
        font.setPointSize(qMax(FONT_MIN_POINT_SIZE, font.pointSize() - 1));
        mStyleEditor->setFont(font);
    });

    // Connection for find and find/replace action
    connect(ui->actionFind, &QAction::triggered, this, [this] {
        if (!mFindReplaceDlg) {
            mFindReplaceDlg = new FindReplaceDialog(this);
            mFindReplaceDlg->setModal(false);
            mFindReplaceDlg->setWindowModality(Qt::NonModal);
        }
        mFindReplaceDlg->setTextEdit(mStyleEditor);
        mFindReplaceDlg->setReplaceEnabled(false);
        mFindReplaceDlg->show();
    });
    connect(ui->actionFindReplace, &QAction::triggered, this, [this] {
        if (!mFindReplaceDlg) {
            mFindReplaceDlg = new FindReplaceDialog(this);
            mFindReplaceDlg->setModal(false);
            mFindReplaceDlg->setWindowModality(Qt::NonModal);
        }
        mFindReplaceDlg->setTextEdit(mStyleEditor);
        mFindReplaceDlg->setReplaceEnabled(true);
        mFindReplaceDlg->show();
    });

    return;
}
