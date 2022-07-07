#include "qssdpreprocessor.h"

#include <QAbstractListModel>
#include <QTextEdit>

QssdPreprocessor::QssdPreprocessor(QObject* parent)
    : QObject { parent }, mEditor(nullptr)
{
    mVarDefineRegex = QRegularExpression(R"(\$\w*[\s\n]*=[\s\n]*#?\w*;)");
    mVarUsageRegex = QRegularExpression(R"()");
    return;
}

QssdPreprocessor::QssdPreprocessor(
    QTextEdit* editor, QAbstractListModel* varsModel, QObject* parent)
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

void QssdPreprocessor::setVariablesModel(QAbstractListModel* varsModel)
{
    mVarsModel = varsModel;
    if (mVarsModel) { }
    return;
}

QString QssdPreprocessor::getProcessedDocumentContent()
{
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
