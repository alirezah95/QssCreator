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
        var.second = value.toString();
        emit dataChanged(index, index, QList<int>({ Roles::VariableValue }));
        return true;
    default:
        return false;
    }
}

bool QssdVariablesModel::insertRows(
    int row, int count, const QModelIndex& parent)
{
    if (parent.isValid() || row < 0 || row > mVariables.size() || count <= 0) {
        return false;
    }

    beginInsertRows(QModelIndex(), row, row + count);
    for (int i = 0; i < count; ++i) {
        mVariables.insert(row, Variable());
    }
    endInsertRows();

    return true;
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

    setData(index(var - mVariables.begin()), value, Roles::VariableValue);
    return true;
}

bool QssdVariablesModel::changeVariableName(
    const QString& oldName, const QString& newName)
{
    if (oldName.isEmpty() || newName.isEmpty()) {
        return false;
    }

    QVector<Variable>::iterator var = std::find_if(mVariables.begin(),
        mVariables.end(),
        [oldName](Variable& item) -> bool { return item.first == oldName; });
    if (var == mVariables.end()) {
        return false;
    }

    setData(index(var - mVariables.begin()), newName, Roles::VariableName);
    return true;
}

bool QssdVariablesModel::insertVariable(
    const QString& name, const QString& value)
{
    if (name.isEmpty() || value.isEmpty()) {
        return false;
    }

    insertRow(mVariables.size());
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

    removeRow(var - mVariables.begin());

    return true;
}
