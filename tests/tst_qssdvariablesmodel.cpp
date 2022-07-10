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
    model->insertRows(0, 1, QModelIndex());

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
    model->insertRows(0, 1, QModelIndex());

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

TEST_F(TestQssdVariablesModel, TestDataInvalid)
{
    EXPECT_FALSE(model->data(model->index(0), QssdVariablesModel::VariableName)
                     .isValid());
    EXPECT_FALSE(model->data(model->index(0), QssdVariablesModel::VariableValue)
                     .isValid());
}

TEST_F(TestQssdVariablesModel, TestGetVariableValue)
{
    model->insertRows(0, 1, QModelIndex());
    model->setData(model->index(0), "var_1", QssdVariablesModel::VariableName);
    model->setData(model->index(0), "3em", QssdVariablesModel::VariableValue);

    EXPECT_STREQ(model->getVariableValue("var_1").toStdString().c_str(), "3em");
}

TEST_F(TestQssdVariablesModel, TestSetVariableValue)
{
    model->insertRows(0, 1, QModelIndex());
    model->setData(model->index(0), "var_1", QssdVariablesModel::VariableName);
    model->setVariableValue("var_1", "#ff00ff");

    EXPECT_STREQ(
        model->getVariableValue("var_1").toStdString().c_str(), "#ff00ff");
}

TEST_F(TestQssdVariablesModel, TestChangeVariableName)
{
    model->insertRows(0, 1, QModelIndex());
    model->setData(model->index(0), "var_1", QssdVariablesModel::VariableName);
    model->setData(model->index(0), "3em", QssdVariablesModel::VariableValue);

    model->changeVariableName("var_1", "fstVariable");

    EXPECT_STREQ(model->data(model->index(0), QssdVariablesModel::VariableName)
                     .toString()
                     .toStdString()
                     .c_str(),
        "fstVariable");
}

TEST_F(TestQssdVariablesModel, TestInsertVariable)
{
    QSignalSpy insertSigSpy(model, &QssdVariablesModel::rowsInserted);
    model->insertVariable("var2", "margin");

    EXPECT_EQ(insertSigSpy.count(), 1);
    auto fstEmission = insertSigSpy.takeFirst();
    EXPECT_EQ(fstEmission.at(1).toInt(), 0);
    EXPECT_EQ(fstEmission.at(2).toInt(), 0);

    EXPECT_STREQ(model->data(model->index(0), QssdVariablesModel::VariableName)
                     .toString()
                     .toStdString()
                     .c_str(),
        "var2");
    EXPECT_STREQ(model->data(model->index(0), QssdVariablesModel::VariableValue)
                     .toString()
                     .toStdString()
                     .c_str(),
        "margin");
}

TEST_F(TestQssdVariablesModel, TestRemoveVariable)
{
    model->insertRows(0, 2, QModelIndex());
    model->setData(model->index(1), "var_1", QssdVariablesModel::VariableName);
    model->setData(model->index(1), "3em", QssdVariablesModel::VariableValue);

    QSignalSpy removeSigSpy(model, &QssdVariablesModel::rowsRemoved);
    model->removeVariable("var_1");

    ASSERT_EQ(removeSigSpy.count(), 1);
    const QList<QVariant>& fstEmission = removeSigSpy.takeFirst();
    EXPECT_EQ(fstEmission.at(1).toInt(), 1);
    EXPECT_EQ(fstEmission.at(1).toInt(), 1);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
