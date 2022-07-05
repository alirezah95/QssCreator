#include "qssdfileoperations.h"

#include "idocumentfile.h"

#include <QFile>
#include <QTextEdit>

QssdFileOperations::QssdFileOperations(QObject* parent)
    : IQssdFileOperations { parent }
{
}

QssdFileOperations::~QssdFileOperations() { }

bool QssdFileOperations::newDocument(QTextEdit* editor)
{
    if (!editor) {
        return false;
    }

    editor->document()->clear();
    // Probably not needed since clear() does that
    editor->document()->setModified(false);
    editor->document()->clearUndoRedoStacks();
    editor->setDocumentTitle(DOC_UNTITLED);

    return true;
}

bool QssdFileOperations::openDocument(QTextEdit* editor, IDocumentFile* docFile)
{
    if (!editor || !docFile) {
        return false;
    }

    if (!docFile->open(QFile::ReadOnly)) {
        return false;
    }

    editor->document()->setPlainText(docFile->readAll());
    // Must set modified to false
    editor->document()->setModified(false);
    editor->setDocumentTitle(docFile->fileName());

    return true;
}

bool QssdFileOperations::saveDocument(
    const QTextEdit* editor, IDocumentFile* outFile)
{
    if (!editor || !outFile) {
        return false;
    }

    if (!outFile->open(QFile::WriteOnly)) {
        return false;
    }

    const auto& editorTextBtArr = editor->document()->toPlainText().toUtf8();
    if (outFile->write(editorTextBtArr) != editorTextBtArr.length()) {
        return false;
    }
    editor->document()->setModified(false);

    return true;
}

bool QssdFileOperations::exportDocument(
    const QTextEdit* editor, IDocumentFile* outFile)
{
    return true;
}
