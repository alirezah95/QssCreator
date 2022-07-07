#include "qssdpreprocessor.h"

#include "iqssdvariablesmodel.h"
#include <QTextEdit>

QssdPreprocessor::QssdPreprocessor(QObject* parent)
    : QObject { parent }, mEditor(nullptr)
{
    mVarDefineRegex = QRegularExpression(R"(\$\w*[\s\n]*=[\s\n]*#?\w*;)");
    mVarUsageRegex = QRegularExpression(R"(\$\w*)");
    return;
}

QssdPreprocessor::QssdPreprocessor(
    QTextEdit* editor, IQssdVariablesModel* varsModel, QObject* parent)
    : QssdPreprocessor(parent)
{
    setQssdEditor(editor);
    setVariablesModel(varsModel);
    return;
}

QssdPreprocessor::~QssdPreprocessor() { }

void QssdPreprocessor::setQssdEditor(QTextEdit* editor)
{
    mEditor = editor;
    if (mEditor) {
        connect(mEditor->document(), &QTextDocument::modificationChanged, this,
            &QssdPreprocessor::preProcessDocument);
    }
    return;
}

void QssdPreprocessor::setVariablesModel(IQssdVariablesModel* varsModel)
{
    mVarsModel = varsModel;
    if (mVarsModel) { }
    return;
}

QString QssdPreprocessor::getProcessedDocumentContent()
{
    if (mEditor && mVarsModel) {
        QString content = mEditor->document()->toPlainText();

        // Deleting variables definitions
        auto definitionMatch = mVarDefineRegex.match(content);
        while (definitionMatch.hasMatch()) {
            content.replace(definitionMatch.capturedStart(),
                definitionMatch.capturedLength(), "");
            definitionMatch = mVarDefineRegex.match(content);
        }

        // Replacing variable usage with actual value
        auto usageMatch = mVarUsageRegex.match(content);
        while (usageMatch.hasMatch()) {
            // Get the variable value from the vars model
            QString value
                = mVarsModel->getVarValue(usageMatch.captured().sliced(1));
            content.replace(
                usageMatch.capturedStart(), usageMatch.capturedLength(), value);
            usageMatch = mVarUsageRegex.match(content);
        }

        return content;
    }
    return QString();
}

void QssdPreprocessor::preProcessDocument(bool docIsModified)
{
    if (!docIsModified) {
        if (mEditor && mVarsModel) {
            const QString& content = mEditor->document()->toPlainText();
            auto matchIter = mVarDefineRegex.globalMatch(content);
            while (matchIter.hasNext()) {
                auto match = matchIter.next();
                mVarsModel->setData(mVarsModel->index(mVarsModel->rowCount()),
                    match.captured());
            }
        }
    }
    return;
}
