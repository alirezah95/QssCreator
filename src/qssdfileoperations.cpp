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

    auto newFile = new QTextDocument(editor);
    editor->setDocument(newFile);
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

    auto newFile = new QTextDocument(editor);
    newFile->setPlainText(docFile->readAll());
    editor->setDocument(newFile);
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

    return true;
}

bool QssdFileOperations::exportDocument(
    const QTextEdit* editor, IDocumentFile* outFile)
{
    return true;
}
