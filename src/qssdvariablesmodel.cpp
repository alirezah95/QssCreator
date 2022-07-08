#include "qssdvariablesmodel.h"

QssdVariablesModel::QssdVariablesModel(QObject* parent)
    : IQssdVariablesModel(parent)
{
}

QssdVariablesModel::~QssdVariablesModel() { }

int QssdVariablesModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return mVariables.size();
}

QVariant QssdVariablesModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex())) {
        return QVariant();
    }

    QMap<QString, QString>::const_key_value_iterator itm
        = mVariables.constKeyValueBegin();
    for (int i = 0; i < index.row(); ++i) {
        itm++;
    }
    switch (role) {
    case Roles::VariableName:
        return (*itm).first;
    case Roles::VariableValue:
        return (*itm).second;
    default:
        return QVariant();
    }
}
