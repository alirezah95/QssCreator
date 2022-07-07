#include "qssdpreprocessor.h"

#include <QTextEdit>

QssdPreprocessor::QssdPreprocessor(QObject* parent)
    : QObject { parent }, mEditor(nullptr)
{
    mVarDefineRegex = QRegularExpression(R"(\$\w*[\s\n]*=[\s\n]*#?\w*;)");
    mVarUsageRegex = QRegularExpression(R"()");
    return;
}

QssdPreprocessor::QssdPreprocessor(QTextEdit* editor, QObject* parent)
    : QssdPreprocessor(parent)
{
    setQssdEditor(editor);
    return;
}

QssdPreprocessor::~QssdPreprocessor() { }

void QssdPreprocessor::setQssdEditor(QTextEdit* editor)
{
    if (mEditor) {
        connect(mEditor->document(), &QTextDocument::modificationChanged, this,
            &QssdPreprocessor::preProcessDocument);
    }
    return;
}

QString QssdPreprocessor::getProcessedDocumentContent()
{
    return QString();
}

void QssdPreprocessor::preProcessDocument(bool docIsModified) { }
