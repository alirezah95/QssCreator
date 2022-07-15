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

void QssdProcessor::processDocument(QTextDocument* doc, bool updateModel)
{
    if (doc && mVarsModel) {
        const QString& content = doc->document()->toPlainText();
        auto matchIter = mVarDefineRegex.globalMatch(content);
        while (matchIter.hasNext()) {
            auto match = matchIter.next();
            mVarsModel->insertVariable(
                mVarsModel->size(), match.captured(1), match.captured(2));
        }
    }
    return;
}
