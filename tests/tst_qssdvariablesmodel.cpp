#include <QApplication>
#include <QSignalSpy>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "qssdvariablesmodel.h"

using namespace ::testing;

class TestQssdVariablesModel : public Test
{
public:
    void SetUp()
    {
        model = new QssdVariablesModel;
        return;
    }

    void TearDown()
    {
        delete model;
        return;
    }

    QssdVariablesModel* model;
};

TEST_F(TestQssdVariablesModel, TestInsertRows)
{
    QSignalSpy insertSigSpy(model, &QAbstractListModel::rowsInserted);
    model->insertRows(0, 4, QModelIndex());

    EXPECT_EQ(insertSigSpy.count(), 1);
    EXPECT_EQ(model->size(), 4);
}

TEST_F(TestQssdVariablesModel, TestSetData)
{
    model->insertRows(0, 4, QModelIndex());

    QSignalSpy dtChangedSpy(model, &QssdVariablesModel::dataChanged);

    model->setData(model->index(0), "var_1", QssdVariablesModel::VariableName);
    model->setData(model->index(0), "3em", QssdVariablesModel::VariableValue);

    model->setData(model->index(1), "var_2", QssdVariablesModel::VariableName);
    model->setData(model->index(1), "12px", QssdVariablesModel::VariableValue);

    model->setData(model->index(2), "fgClr", QssdVariablesModel::VariableName);
    model->setData(model->index(2), "blue", QssdVariablesModel::VariableValue);

    model->setData(
        model->index(3), "bgColor", QssdVariablesModel::VariableName);
    model->setData(
        model->index(3), "#ffff00", QssdVariablesModel::VariableValue);

    EXPECT_EQ(dtChangedSpy.count(), 8);
    EXPECT_STREQ(model->data(model->index(0), QssdVariablesModel::VariableValue)
                     .toString()
                     .toStdString()
                     .c_str(),
        "3em");
    EXPECT_STREQ(model->data(model->index(1), QssdVariablesModel::VariableValue)
                     .toString()
                     .toStdString()
                     .c_str(),
        "12px");
    EXPECT_STREQ(model->data(model->index(2), QssdVariablesModel::VariableValue)
                     .toString()
                     .toStdString()
                     .c_str(),
        "blue");
    EXPECT_STREQ(model->data(model->index(3), QssdVariablesModel::VariableValue)
                     .toString()
                     .toStdString()
                     .c_str(),
        "#ffff00");
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
