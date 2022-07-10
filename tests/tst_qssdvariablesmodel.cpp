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

TEST_F(TestQssdVariablesModel, TestSetDataSignals)
{
    model->insertRows(0, 4, QModelIndex());

    QSignalSpy dtChangedSpy(model, &QssdVariablesModel::dataChanged);

    model->setData(model->index(0), "var_1", QssdVariablesModel::VariableName);
    model->setData(model->index(0), "3em", QssdVariablesModel::VariableValue);

    EXPECT_EQ(dtChangedSpy.count(), 2);

    auto fstEmission = dtChangedSpy[0];
    auto scndEmission = dtChangedSpy[1];

    EXPECT_EQ(fstEmission.at(0).toModelIndex().row(), 0);
    EXPECT_EQ(fstEmission.at(1).toModelIndex().row(), 0);
    EXPECT_TRUE(fstEmission.at(2).toList().at(0).toInt()
        == QssdVariablesModel::VariableName);

    EXPECT_EQ(scndEmission.at(0).toModelIndex().row(), 0);
    EXPECT_EQ(scndEmission.at(1).toModelIndex().row(), 0);
    EXPECT_TRUE(scndEmission.at(2).toList().at(0).toInt()
        == QssdVariablesModel::VariableValue);
}

TEST_F(TestQssdVariablesModel, TestData)
{
    model->insertRows(0, 4, QModelIndex());

    QSignalSpy dtChangedSpy(model, &QssdVariablesModel::dataChanged);

    model->setData(model->index(0), "var_1", QssdVariablesModel::VariableName);
    model->setData(model->index(0), "3em", QssdVariablesModel::VariableValue);

    EXPECT_STREQ(model->data(model->index(0), QssdVariablesModel::VariableName)
                     .toString()
                     .toStdString()
                     .c_str(),
        "var_1");
    EXPECT_STREQ(model->data(model->index(0), QssdVariablesModel::VariableValue)
                     .toString()
                     .toStdString()
                     .c_str(),
        "3em");
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
