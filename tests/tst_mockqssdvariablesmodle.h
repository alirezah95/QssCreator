#ifndef TST_MOCKQSSDVARIABLESMODLE_H
#define TST_MOCKQSSDVARIABLESMODLE_H

#include "iqssdvariablesmodel.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockListModel : public IQssdVariablesModel
{
public:
    MOCK_METHOD(int, rowCount, (const QModelIndex& parent), (const override));
    MOCK_METHOD(
        QVariant, data, (const QModelIndex& index, int role), (const override));

    MOCK_METHOD(bool, setData,
        (const QModelIndex& index, const QVariant& value, int role),
        (override));

    MOCK_METHOD(
        QString, getVarValue, (const QString& varName), (const override));
};

#endif
