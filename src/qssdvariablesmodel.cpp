#include "qssdvariablesmodel.h"

template <class Variable>
QssdVariablesModel<Variable>::QssdVariablesModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

template <class Variable>
QssdVariablesModel<Variable>::~QssdVariablesModel()
{
}

template <class Variable>
int QssdVariablesModel<Variable>::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return mVariables.size();
}

template <class Variable>
QVariant QssdVariablesModel<Variable>::data(
    const QModelIndex& index, int role) const
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

template <class Variable>
bool QssdVariablesModel<Variable>::setData(
    const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex())) {
        return false;
    }

    QMap<QString, QString>::iterator itm = mVariables.begin();
    for (int i = 0; i < index.row(); ++i) {
        itm++;
    }
    if (role == Roles::VariableValue) {
        if ((*itm) == value.toString()) {
            return true;
        }
        (*itm) = value.toString();
        return true;
    }

    return false;
}
