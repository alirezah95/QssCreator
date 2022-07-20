#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "documentfile.h"
#include "findreplacedialog.h"
#include "iqssdfileoperations.h"
#include "iqssdprocessor.h"
#include "iqssdvariablesmodel.h"
#include "iuserdialogs.h"
#include "qssdeditor.h"
#include "qssdvariableitemdelegate.h"
#include "widgetspreview.h"

#include <QCheckBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QStandardPaths>
#include <QWidgetAction>

/* To create connections between toolbar actions and this class */
#define ACT_CONNECT_THIS(act, slot)                                            \
    connect(act, &QAction::triggered, this, &MainWindow::slot)

/* To create connections between toolbar actions and editor object */
#define ACT_CONNECT_EDITOR(act, slot)                                          \
    connect(act, &QAction::triggered, mStyleEditor, &IQssdEditor::slot)

MainWindow::MainWindow(IQssdEditor* editor, IQssdFileOperations* docOper,
    IUserDialogs* userDlgs, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mStyleEditor(editor),
      mPreview(new WidgetsPreview), mDocOpers(docOper), mUserDlgs(userDlgs),
      mFindReplaceDlg(nullptr)
{
    ui->setupUi(this);

    mStyleEditor->setParent(this);
    mDocOpers->setParent(this);
    mUserDlgs->setParent(this);

    mSplitter = new QSplitter;
    mSplitter->setChildrenCollapsible(false);
    mSplitter->addWidget(mStyleEditor);
    mSplitter->addWidget(mPreview);
    mSplitter->setStretchFactor(mSplitter->indexOf(mStyleEditor), 1);
    mSplitter->setStretchFactor(mSplitter->indexOf(mPreview), 0);

    ui->centralwidget->layout()->addWidget(mSplitter);

    setMinimumSize(QSize(ui->variablesListVw->maximumSize().width()
            + mStyleEditor->minimumSizeHint().width()
            + mPreview->minimumSizeHint().width(),
        mStyleEditor->minimumSizeHint().height()));

    // Set variables view model to the model of the processor of the editor then
    // set the variables model delegate
    ui->variablesListVw->setModel(
        mStyleEditor->getProcessor()->getVariablesModel());
    ui->variablesListVw->setItemDelegate(new QssdVariableItemDelegate(this));

    setUpToolbar();

    updateWindowTitle();

    reset();

    setupConnections();

    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* ev)
{
    if (maybeSave()) {
        ev->accept();
    } else {
        ev->ignore();
    }
}

void MainWindow::newDocument()
{
    if (mDocOpers) {
        if (mStyleEditor->document()->isModified()) {
            if (!maybeSave()) {
                return;
            }
        }
        /* If document is not modified, newDocument() will be called without
         * condition.
         */
        if (mDocOpers->newDocument(mStyleEditor)) {
            updateWindowTitle();
        }
    }
    return;
}

void MainWindow::openDocument()
{
    if (mDocOpers && mUserDlgs) {
        if (mStyleEditor->document()->isModified()) {
            if (!maybeSave()) {
                return;
            }
        }
        auto openFileName = mUserDlgs->getOpenFileName(this, tr("Open File"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            DOC_FILTER);
        if (openFileName.isEmpty()) {
            return;
        }
        DocumentFile docFile(openFileName);

        if (!mDocOpers->openDocument(mStyleEditor, &docFile)) {
            qDebug() << "Error in opening file: " << docFile.fileName();
            return;
        }
        // Set the stylesheet on the preview widget
        mPreview->setStyleSheet(mStyleEditor->getQtStylesheet(true));

        updateWindowTitle();
    }
    return;
}

void MainWindow::save()
{
    if (mDocOpers) {
        if (saveDocument()) {
            // Set the stylesheet on the preview widget
            mPreview->setStyleSheet(mStyleEditor->getQtStylesheet(true));
            // Export document if auto export is enabled
            if (mAutoExportCheckBox->isChecked()) {
                exportDocument();
            }
        }
    }
    return;
}

void MainWindow::saveAs()
{
    if (mDocOpers) {
        auto saveAsFileName = mUserDlgs->getSaveFileName(this,
            tr("Save File As"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            DOC_FILTER);
        if (saveAsFileName.isEmpty()) {
            return;
        }

        DocumentFile docFile(saveAsFileName);
        if (!mDocOpers->saveDocument(mStyleEditor, &docFile)) {
            qDebug() << "Error in saving file: " << docFile.fileName();
            return;
        }
        if (saveAsFileName != mStyleEditor->documentTitle()) {
            mStyleEditor->setDocumentTitle(saveAsFileName);
            updateWindowTitle();
        }

        // Set the stylesheet on the preview widget
        mPreview->setStyleSheet(mStyleEditor->getQtStylesheet(true));
    }
    return;
}

void MainWindow::exportDocument()
{
    if (mStyleEditor->documentTitle() == DOC_UNTITLED) {
        // First save the document. Update var model is needed => call save()
        save();
    }
    const QString& exportContent
        = mStyleEditor->getQtStylesheet(mStyleEditor->document()->isModified());

    const QString& docTitle = mStyleEditor->documentTitle();

    DocumentFile exportDocFile(mIsAutoExportFilePathValid
            ? mAutoExportLEdit->text()
            : docTitle.sliced(0, docTitle.size() - 1));
    if (!mDocOpers->writeToFile(exportContent, &exportDocFile)) {
        qDebug() << "Export failed";
    }
}

void MainWindow::updateWindowTitle()
{
    if (mStyleEditor->documentTitle().isEmpty()) {
        mStyleEditor->setDocumentTitle(DOC_UNTITLED);
    }

    auto docTitle = mStyleEditor->documentTitle();
    int slashIndx = docTitle.lastIndexOf("/");
    if (slashIndx > -1 && slashIndx < docTitle.size() - 1) {
        docTitle = docTitle.sliced(slashIndx + 1);
    }
    if (docTitle.isEmpty()) {
        docTitle = DOC_UNTITLED;
        mStyleEditor->setDocumentTitle(docTitle);
    }

    setWindowTitle(docTitle
        + (mStyleEditor->document()->isModified() ? "* - " : " - ")
        + "Qss Creator");
}

void MainWindow::setUpToolbar()
{
    mAutoExportCheckBox = new QCheckBox;
    mAutoExportCheckBox->setObjectName("aExpChBox");
    mAutoExportCheckBox->setText(tr("&Auto Export"));
    mAutoExportCheckAct = new QWidgetAction(this);
    mAutoExportCheckAct->setObjectName("aExpChBoxAct");
    mAutoExportCheckAct->setDefaultWidget(mAutoExportCheckBox);

    mAutoExportLEdit = new QLineEdit;
    mAutoExportLEdit->setObjectName("aExpLEdit");
    mAutoExportLEdit->setMinimumWidth(200);
    mAutoExportLEdit->setMaximumWidth(450);
    QFont expLEditFont(font().family());
    expLEditFont.setPixelSize(15);
    mAutoExportLEdit->setFont(expLEditFont);
    mAutoExportLEditAct = new QWidgetAction(this);
    mAutoExportLEditAct->setObjectName("aExpLEditAct");
    mAutoExportLEditAct->setDefaultWidget(mAutoExportLEdit);

    mAutoExportBrowseBtn = new QPushButton;
    mAutoExportBrowseBtn->setObjectName("aExpBrowseBtn");
    mAutoExportBrowseBtn->setText(tr("&Select"));
    mAutoExportBrowseBtnAct = new QWidgetAction(this);
    mAutoExportBrowseBtnAct->setObjectName("aExpBrowseBtnAct");
    mAutoExportBrowseBtnAct->setDefaultWidget(mAutoExportBrowseBtn);

    mAutoExportLEdit->setFixedHeight(mAutoExportBrowseBtn->sizeHint().height());

    ui->mainToolBar->addAction(mAutoExportCheckAct);
    ui->mainToolBar->addAction(mAutoExportLEditAct);
    ui->mainToolBar->addAction(mAutoExportBrowseBtnAct);

    return;
}

bool MainWindow::maybeSave()
{
    /* If the document is modified and user presses No button on save
     * dialog, new document should be created. On the other hand if the
     * user press cancel the whole operation is canceled.
     *
     * If the use press Yes button there is two situations, one if the
     * user choose a file name (path) that the file can be saved at, in
     * this case the new document is created either. And if user cancel
     * choosing save file path (for example by pressing escape) and so
     * the file is not saved, new document should not be created and it
     * should be like a Cancel button press.
     */
    if (mStyleEditor->document()->isModified()) {
        if (auto btn = mUserDlgs->question(this, tr("Save Changes"),
                tr("Document is not saved, do you want to save changes?"),
                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                QMessageBox::Yes);
            btn == QMessageBox::Yes) {
            return saveDocument();
        } else if (btn == QMessageBox::No) {
            return true;
        } else /*(btn == QMessageBox::Cancel)*/ {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveDocument()
{
    QString saveFileName;
    if (mStyleEditor->documentTitle() == DOC_UNTITLED) {
        saveFileName = mUserDlgs->getSaveFileName(this, tr("Save File"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            DOC_FILTER);
    } else {
        saveFileName = mStyleEditor->documentTitle();
    }
    if (saveFileName.isEmpty()) {
        return false;
    }

    DocumentFile docFile(saveFileName);
    if (!mDocOpers->saveDocument(mStyleEditor, &docFile)) {
        qDebug() << "Error in saving file: " << docFile.fileName();
        return false;
    }
    if (saveFileName != mStyleEditor->documentTitle()) {
        mStyleEditor->setDocumentTitle(saveFileName);
        updateWindowTitle();
    }
    return true;
}

void MainWindow::reset()
{
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionSave->setEnabled(false);

    mAutoExportLEditAct->setEnabled(false);
    mAutoExportBrowseBtnAct->setEnabled(false);

    return;
}

void MainWindow::setupConnections()
{
    /* Set up actions connections */
    ACT_CONNECT_THIS(ui->actionNewFile, newDocument);
    ACT_CONNECT_THIS(ui->actionOpenFile, openDocument);
    ACT_CONNECT_THIS(ui->actionSave, save);
    ACT_CONNECT_THIS(ui->actionSaveAs, saveAs);
    ACT_CONNECT_THIS(ui->actionExport, exportDocument);

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

    connect(mStyleEditor->document(), &QTextDocument::modificationChanged, this,
        [this](bool modified) {
            updateWindowTitle();
            ui->actionSave->setEnabled(modified);
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
        if (!mFindReplaceDlg->isVisible()) {
            mFindReplaceDlg->setTextEdit(mStyleEditor);
            mFindReplaceDlg->setReplaceEnabled(false);
            mFindReplaceDlg->show();
        }

        if (auto selTxt = mStyleEditor->textCursor().selectedText();
            !selTxt.isEmpty()) {
            mFindReplaceDlg->setFindText(selTxt);
        }
    });
    connect(ui->actionFindReplace, &QAction::triggered, this, [this] {
        if (!mFindReplaceDlg) {
            mFindReplaceDlg = new FindReplaceDialog(this);
            mFindReplaceDlg->setModal(false);
            mFindReplaceDlg->setWindowModality(Qt::NonModal);
        }
        if (!mFindReplaceDlg->isVisible()) {
            mFindReplaceDlg->setTextEdit(mStyleEditor);
            mFindReplaceDlg->setReplaceEnabled(true);
            mFindReplaceDlg->show();
        }

        if (auto selTxt = mStyleEditor->textCursor().selectedText();
            !selTxt.isEmpty()) {
            mFindReplaceDlg->setFindText(selTxt);
        }
    });

    // Connection for auto export actions
    connect(
        mAutoExportCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
            mAutoExportLEditAct->setEnabled(checked);
            mAutoExportBrowseBtnAct->setEnabled(checked);
        });
    connect(mAutoExportLEdit, &QLineEdit::editingFinished, this, [this] {
        // Make line edit red if file path is not valid
        QFile file(mAutoExportLEdit->text());
        if (!file.open(QFile::ReadWrite)) {
            // Make line edit red
            mIsAutoExportFilePathValid = false;
            mAutoExportLEdit->setStyleSheet("QLineEdit {"
                                            "border: 1px solid red;"
                                            "}");
        } else {
            mIsAutoExportFilePathValid = true;
            mAutoExportLEdit->setStyleSheet("");
        }
    });
    connect(mAutoExportBrowseBtn, &QPushButton::clicked, this, [this] {
        if (mUserDlgs) {
            auto auExpFilePath
                = mUserDlgs->getSaveFileName(this, tr("Save File"),
                    QStandardPaths::writableLocation(
                        QStandardPaths::DocumentsLocation),
                    DOC_FILTER);
            if (!auExpFilePath.isEmpty()) {
                mAutoExportLEdit->setText(auExpFilePath);
                emit mAutoExportLEdit->editingFinished();
            }
        }
    });

    return;
}

void MainWindow::readSettings() { }
