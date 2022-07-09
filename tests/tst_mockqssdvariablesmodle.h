#ifndef TST_MOCKQSSDVARIABLESMODLE_H
#define TST_MOCKQSSDVARIABLESMODLE_H

#include "iqssdvariablesmodel.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockVariablesModel : public IQssdVariablesModel
{
public:
    MOCK_METHOD(
        QString, getVariableValue, (const QString& varName), (const override));

    MOCK_METHOD(bool, setVariableValue,
        (const QString& varName, const QVariant& value), (override));

    MOCK_METHOD(bool, changeVariableName,
        (const QString& oldName, const QString& newName), (override));

    MOCK_METHOD(bool, insertVariable,
        (const QString& name, const QString& value), (override));

    MOCK_METHOD(bool, removeVariable, (const QString& varName), (override));
};

#endif
