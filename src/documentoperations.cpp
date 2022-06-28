#include "documentoperations.h"

#include "idocumentfile.h"

#include <QFile>
#include <QTextEdit>

DocumentOperations::DocumentOperations(QObject* parent) : QObject { parent } { }

bool DocumentOperations::newDocument(QTextEdit* editor)
{
    if (!editor) {
        return false;
    }

    auto newFile = new QTextDocument(editor);
    editor->setDocument(newFile);
    editor->setDocumentTitle("Untitled.qssd");

    return true;
}

bool DocumentOperations::openDocument(QTextEdit* editor, IDocumentFile* docFile)
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

bool DocumentOperations::saveDocument(const QTextEdit* editor, IDocumentFile* outFile)
{
    if (!editor || !outFile) {
        return false;
    }

    if (!outFile->open(QFile::WriteOnly)) {
        return false;
    }

    if (outFile->write(editor->document()->toPlainText().toUtf8()) < 0) {
        return false;
    }

    return true;
}
