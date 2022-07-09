#include "qssdvariablesmodel.h"

QssdVariablesModel::QssdVariablesModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

QssdVariablesModel::~QssdVariablesModel() { }

// QAbstractListModel methods definition
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

    const auto& var = mVariables[index.row()];

    switch (role) {
    case Roles::VariableName:
        return var.first;
    case Roles::VariableValue:
        return var.second;
    default:
        return QVariant();
    }
}

bool QssdVariablesModel::setData(
    const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= rowCount(QModelIndex())) {
        return false;
    }

    auto& var = mVariables[index.row()];
    switch (role) {
    case Roles::VariableName:
        var.first = value.toString();
        emit dataChanged(index, index, QList<int>({ Roles::VariableName }));
        return true;
    case Roles::VariableValue:
        var.first = value.toString();
        emit dataChanged(index, index, QList<int>({ Roles::VariableValue }));
        return true;
    default:
        return false;
    }
}

// IQssdVariablesModel methods definition
QString QssdVariablesModel::variableValue(const QString& varName) const
{
    if (mVariables.size() == 0) {
        return QString();
    }

    QVector<Variable>::const_iterator var = std::find_if(mVariables.begin(),
        mVariables.end(),
        [varName](Variable& item) -> bool { return item.first == varName; });
    if (var == mVariables.end()) {
        return QString();
    }

    return var->second;
}

bool QssdVariablesModel::setVariableValue(
    const QString& varName, const QVariant& value)
{
    if (mVariables.size() == 0) {
        return false;
    }

    QVector<Variable>::iterator var = std::find_if(mVariables.begin(),
        mVariables.end(),
        [varName](Variable& item) -> bool { return item.first == varName; });
    if (var == mVariables.end()) {
        return false;
    }

    (*var).second = value.toString();
    return true;
}

bool QssdVariablesModel::insertVariable(
    const QString& name, const QString& value)
{
    if (name.isEmpty() || value.isEmpty()) {
        return false;
    }

    mVariables.emplace_back(Variable(name, value));
    return true;
}

bool QssdVariablesModel::removeVariable(const QString& varName)
{
    if (mVariables.size() == 0) {
        return false;
    }

    return mVariables.removeIf([varName](Variable& item) -> bool {
        return item.first == varName;
    }) > 0;
}
