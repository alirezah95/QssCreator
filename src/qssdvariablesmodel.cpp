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

bool QssdVariablesModel::removeRows(
    int row, int count, const QModelIndex& parent)
{
    if (parent.isValid() || row < 0 || count <= 0
        || row + count > mVariables.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count);
    mVariables.remove(row, count);
    endRemoveRows();

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
    return true;
}

bool QssdVariablesModel::setVariableValue(
    const QModelIndex& index, const QString& value)
{
}

bool QssdVariablesModel::setVariableName(
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

bool QssdVariablesModel::setVariableName(
    const QModelIndex& index, const QString& newName)
{
}

bool QssdVariablesModel::insertVariable(
    const QString& name, const QString& value)
{
    if (name.isEmpty() || value.isEmpty()) {
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

    removeRow(var - mVariables.begin());

    return true;
}

bool QssdVariablesModel::removeVariable(const QModelIndex& index) { }
