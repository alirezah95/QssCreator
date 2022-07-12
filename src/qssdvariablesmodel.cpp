#include "qssdvariablesmodel.h"

QssdVariablesModel::QssdVariablesModel(QObject* parent)
    : IQssdVariablesModel(parent)
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

// IQssdVariablesModel methods definition
QString QssdVariablesModel::getVariableValue(const QString& varName) const
{
    if (mVariables.size() == 0) {
        return QString();
    }

    QVector<Variable>::const_iterator var = std::find_if(mVariables.begin(),
        mVariables.end(), [varName](const Variable& item) -> bool {
            return item.first == varName;
        });
    if (var == mVariables.end()) {
        return QString();
    }

    return var->second;
}

QString QssdVariablesModel::getVariableValue(const QModelIndex& index) const
{
    return data(index, Roles::VariableValue).toString();
}

bool QssdVariablesModel::setVariableValue(
    const QString& varName, const QString& value)
{
    if (mVariables.size() == 0 || value.isEmpty()) {
        return false;
    }

    QVector<Variable>::iterator var = std::find_if(mVariables.begin(),
        mVariables.end(),
        [varName](Variable& item) -> bool { return item.first == varName; });
    if (var == mVariables.end()) {
        return false;
    }

    (*var).second = value;

    auto indx = index(var - mVariables.begin());
    emit dataChanged(indx, indx, { Roles::VariableValue });
    return true;
}

bool QssdVariablesModel::setVariableValue(
    const QModelIndex& index, const QString& value)
{
    if (!index.isValid() || index.row() > mVariables.size()
        || value.isEmpty()) {
        return false;
    }

    Variable& var = mVariables[index.row()];
    var.second = value;
    emit dataChanged(index, index, { Roles::VariableValue });
    return true;
}

bool QssdVariablesModel::setVariableName(
    const QString& oldName, const QString& newName)
{
    if (oldName.isEmpty() || newName.isEmpty() || contains(newName)) {
        return false;
    }

    QVector<Variable>::iterator var = std::find_if(mVariables.begin(),
        mVariables.end(),
        [oldName](Variable& item) -> bool { return item.first == oldName; });
    if (var == mVariables.end()) {
        return false;
    }

    (*var).first = newName;

    auto indx = index(var - mVariables.begin());
    emit dataChanged(indx, indx, { Roles::VariableName });
    return true;
}

bool QssdVariablesModel::setVariableName(
    const QModelIndex& index, const QString& newName)
{
    if (!index.isValid() || index.row() > mVariables.size() || newName.isEmpty()
        || contains(newName)) {
        return false;
    }

    Variable& var = mVariables[index.row()];
    var.first = newName;

    emit dataChanged(index, index, { Roles::VariableName });
    return true;
}

bool QssdVariablesModel::insertVariable(
    int row, const QString& name, const QString& value)
{
    if (name.isEmpty() || value.isEmpty() || contains(name)) {
        return false;
    }

    beginInsertRows(QModelIndex(), mVariables.size(), mVariables.size());
    mVariables.emplace_back(Variable(name, value));
    endInsertRows();
    return true;
}

bool QssdVariablesModel::removeVariable(const QString& varName)
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

    int indx = var - mVariables.begin();

    beginRemoveRows(QModelIndex(), indx, indx);
    mVariables.remove(indx);
    endRemoveRows();

    return true;
}

bool QssdVariablesModel::removeVariable(const QModelIndex& index)
{
    if (!index.isValid() || index.row() >= mVariables.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    mVariables.remove(index.row());
    endRemoveRows();
    return true;
}

bool QssdVariablesModel::contains(const QString& varName) const
{
    QVector<Variable>::const_iterator var = std::find_if(mVariables.begin(),
        mVariables.end(), [varName](const Variable& item) -> bool {
            return item.first == varName;
        });
    return (var != mVariables.end());
}
