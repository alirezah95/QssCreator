#ifndef TST_MOCKQSSDVARIABLESMODLE_H
#define TST_MOCKQSSDVARIABLESMODLE_H

#include "iqssdvariablesmodel.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockVariablesModel : public IQssdVariablesModel
{
public:
    MOCK_METHOD(int, rowCount, (const QModelIndex& parent), (const override));

    MOCK_METHOD(
        QVariant, data, (const QModelIndex& index, int role), (const override));

    MOCK_METHOD(bool, removeRows,
        (int row, int count, const QModelIndex& parent), (override));

    MOCK_METHOD(
        QString, getVariableValue, (const QString& varName), (const override));

    MOCK_METHOD(QString, getVariableValue, (const QModelIndex& index),
        (const override));

    MOCK_METHOD(bool, setVariableValue,
        (const QString& varName, const QString& value), (override));

    MOCK_METHOD(bool, setVariableValue,
        (const QModelIndex& index, const QString& value), (override));

    MOCK_METHOD(bool, setVariableName,
        (const QString& oldName, const QString& newName), (override));

    MOCK_METHOD(bool, setVariableName,
        (const QModelIndex& index, const QString& newName), (override));

    MOCK_METHOD(bool, insertVariable,
        (const QString& name, const QString& value), (override));

    MOCK_METHOD(bool, removeVariable, (const QString& varName), (override));

    MOCK_METHOD(bool, removeVariable, (const QModelIndex& index), (override));

    MOCK_METHOD(size_t, size, (), (const override));
};

#endif
