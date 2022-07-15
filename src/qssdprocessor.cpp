#include "qssdprocessor.h"

#include "iqssdvariablesmodel.h"
#include <QTextEdit>

QssdProcessor::QssdProcessor(IQssdVariablesModel* varsModel, QObject* parent)
    : IQssdProcessor(parent)
{
    mVarDefineRegex
        = QRegularExpression(R"(\$(\w*)[\s\n]*=[\s\n]*(#?[\w-]*);)");
    mVarUsageRegex = QRegularExpression(R"(\$\w*)");
    setVariablesModel(varsModel);
    return;
}

QssdProcessor::~QssdProcessor() { }

void QssdProcessor::setVariablesModel(IQssdVariablesModel* varsModel)
{
    mVarsModel = varsModel;
    if (mVarsModel) { }
    return;
}

// QString QssdProcessor::getProcessedDocumentContent(QTextEdit* editor)
//{
//     if (editor && mVarsModel) {
//         QString content = editor->document()->toPlainText();

//        // Deleting variables definitions
//        auto definitionMatch = mVarDefineRegex.match(content);
//        while (definitionMatch.hasMatch()) {
//            content.replace(definitionMatch.capturedStart(),
//                definitionMatch.capturedLength(), "");
//            definitionMatch = mVarDefineRegex.match(content);
//        }

//        // Replacing variable usage with actual value
//        auto usageMatch = mVarUsageRegex.match(content);
//        while (usageMatch.hasMatch()) {
//            // Get the variable value from the vars model
//            QString value
//                =
//                mVarsModel->getVariableValue(usageMatch.captured().sliced(1));
//            content.replace(
//                usageMatch.capturedStart(), usageMatch.capturedLength(),
//                value);
//            usageMatch = mVarUsageRegex.match(content);
//        }

//        return content;
//    }
//    return QString();
//}

void QssdProcessor::processDocument(QTextEdit* editor)
{
    if (editor && mVarsModel) {
        const QString& content = editor->document()->toPlainText();
        auto matchIter = mVarDefineRegex.globalMatch(content);
        while (matchIter.hasNext()) {
            auto match = matchIter.next();
            mVarsModel->insertVariable(
                mVarsModel->size(), match.captured(1), match.captured(2));
        }
    }
    return;
}
