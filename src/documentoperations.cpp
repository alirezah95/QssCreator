#include "documentoperations.h"

#include "stylesheeteditor.h"

#include <QFile>

DocumentOperations::DocumentOperations(QObject* parent) : QObject { parent } { }

bool DocumentOperations::newFile(StyleSheetEditor* editor)
{
    if (!editor) {
        return false;
    }

    auto newFile = new QTextDocument(editor);
    editor->setDocument(newFile);
    editor->setDocumentTitle("Untitled.qssd");

    return true;
}

bool DocumentOperations::openFile(
    StyleSheetEditor* editor, const QString& filename)
{
    if (!editor) {
        return false;
    }

    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    auto newFile = new QTextDocument(editor);
    newFile->setPlainText(file.readAll());
    editor->setDocument(newFile);
    editor->setDocumentTitle(filename);

    return true;
}

bool DocumentOperations::save(
    const StyleSheetEditor* editor, const QString& filename)
{
    if (!editor) {
        return false;
    }

    QFile out(filename);
    if (!out.open(QFile::WriteOnly)) {
        return false;
    }

    if (out.write(editor->document()->toPlainText().toUtf8()) < 0) {
        return false;
    }

    return true;
}
