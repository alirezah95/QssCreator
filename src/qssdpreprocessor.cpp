#include "qssdpreprocessor.h"

#include "iqssdvariablesmodel.h"
#include <QTextEdit>

QssdPreprocessor::QssdPreprocessor(QObject* parent)
    : IQssdPreprocessor { parent }
{
    mVarDefineRegex = QRegularExpression(R"(\$\w*[\s\n]*=[\s\n]*#?[\w-]*;)");
    mVarUsageRegex = QRegularExpression(R"(\$\w*)");
    return;
}

QssdPreprocessor::QssdPreprocessor(
    IQssdVariablesModel* varsModel, QObject* parent)
    : QssdPreprocessor(parent)
{
    setVariablesModel(varsModel);
    return;
}

QssdPreprocessor::~QssdPreprocessor() { }

void QssdPreprocessor::setVariablesModel(IQssdVariablesModel* varsModel)
{
    mVarsModel = varsModel;
    if (mVarsModel) { }
    return;
}

QString QssdPreprocessor::getProcessedDocumentContent(QTextEdit* editor)
{
    if (editor && mVarsModel) {
        QString content = editor->document()->toPlainText();

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

void QssdPreprocessor::processDocumentVariables(QTextEdit* editor)
{
    if (editor && mVarsModel) {
        const QString& content = editor->document()->toPlainText();
        auto matchIter = mVarDefineRegex.globalMatch(content);
        while (matchIter.hasNext()) {
            auto match = matchIter.next();
            mVarsModel->setData(
                mVarsModel->index(mVarsModel->rowCount()), match.captured());
        }
    }
    return;
}
