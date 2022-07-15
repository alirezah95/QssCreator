#include "qssdprocessor.h"

#include "iqssdvariablesmodel.h"
#include <QTextEdit>

using Variable = QPair<QString, QString>;

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

void QssdProcessor::processDocument(QTextDocument* doc, bool updateModel)
{
    if (doc && mVarsModel) {
        QString content = doc->toPlainText();

        QVector<Variable> variables;

        auto defMatch = mVarDefineRegex.match(content);
        while (defMatch.hasMatch()) {
            content.replace(
                defMatch.capturedStart(), defMatch.capturedLength(), "");

            if (updateModel) {
                variables.emplace_back(
                    Variable(defMatch.captured(1), defMatch.captured(2)));
            }

            defMatch = mVarDefineRegex.match(content, defMatch.capturedStart());
        }
        if (updateModel && !variables.isEmpty()) {
            // set model's data
            //            mVarsModel->setVariables(variables);
        }

        // Replacing variable usage with actual value
        auto usageMatch = mVarUsageRegex.match(content);
        while (usageMatch.hasMatch()) {
            // Get the variable value from the vars model
            QString value
                = mVarsModel->getVariableValue(usageMatch.captured().sliced(1));
            content.replace(
                usageMatch.capturedStart(), usageMatch.capturedLength(), value);
            usageMatch = mVarUsageRegex.match(content);
        }
        return;
    }
    return;
}
