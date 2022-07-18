#include "iqssdvariablesmodel.h"

IQssdVariablesModel::IQssdVariablesModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

IQssdVariablesModel::~IQssdVariablesModel() { }

bool IQssdVariablesModel::insertRows(
    int row, int count, const QModelIndex& index)
{
    // does nothing
    return false;
}

bool IQssdVariablesModel::removeRows(
    int row, int count, const QModelIndex& parent)
{
    // does nothing
    return false;
}
