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

TEST_F(TestQssdVariablesModel, TestInsertVariable)
{
    QSignalSpy insertSigSpy(model, &QssdVariablesModel::rowsInserted);

    model->insertVariable(model->size(), "var2", "margin");

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

TEST_F(TestQssdVariablesModel, TestSetDataSignals)
{
    //    model->insertRows(0, 1, QModelIndex());

    //    QSignalSpy dtChangedSpy(model, &QssdVariablesModel::dataChanged);

    //    model->setData(model->index(0), "var_1",
    //    QssdVariablesModel::VariableName); model->setData(model->index(0),
    //    "3em", QssdVariablesModel::VariableValue);

    //    EXPECT_EQ(dtChangedSpy.count(), 2);

    //    auto fstEmission = dtChangedSpy[0];
    //    auto scndEmission = dtChangedSpy[1];

    //    EXPECT_EQ(fstEmission.at(0).toModelIndex().row(), 0);
    //    EXPECT_EQ(fstEmission.at(1).toModelIndex().row(), 0);
    //    EXPECT_TRUE(fstEmission.at(2).toList().at(0).toInt()
    //        == QssdVariablesModel::VariableName);

    //    EXPECT_EQ(scndEmission.at(0).toModelIndex().row(), 0);
    //    EXPECT_EQ(scndEmission.at(1).toModelIndex().row(), 0);
    //    EXPECT_TRUE(scndEmission.at(2).toList().at(0).toInt()
    //        == QssdVariablesModel::VariableValue);
}

TEST_F(TestQssdVariablesModel, TestData)
{
    model->insertVariable(model->size(), "var_1", "3em");

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
    model->insertVariable(model->size(), "var_1", "3em");

    EXPECT_STREQ(model->getVariableValue("var_1").toStdString().c_str(), "3em");
    EXPECT_STREQ(
        model->getVariableValue(model->index(0)).toStdString().c_str(), "3em");
}

TEST_F(TestQssdVariablesModel, TestSetVariableValue)
{
    model->insertVariable(model->size(), "var_1", "3em");
    model->setVariableValue("var_1", "#ff00ff");

    EXPECT_STREQ(
        model->getVariableValue("var_1").toStdString().c_str(), "#ff00ff");
    model->setVariableValue(model->index(0), "#aabbcc");
    EXPECT_STREQ(
        model->getVariableValue("var_1").toStdString().c_str(), "#aabbcc");
}

TEST_F(TestQssdVariablesModel, TestSetVariableName)
{
    model->insertVariable(model->size(), "var_1", "3em");

    model->setVariableName("var_1", "fstVariable");
    EXPECT_STREQ(model->data(model->index(0), QssdVariablesModel::VariableName)
                     .toString()
                     .toStdString()
                     .c_str(),
        "fstVariable");

    model->setVariableName(model->index(0), "var1st");
    EXPECT_STREQ(model->data(model->index(0), QssdVariablesModel::VariableName)
                     .toString()
                     .toStdString()
                     .c_str(),
        "var1st");
}

TEST_F(TestQssdVariablesModel, TestSetVariableNameToExistingName)
{
    QSignalSpy chDataSigSpy(model, &QssdVariablesModel::dataChanged);
    model->insertVariable(model->size(), "var_1", "3em");
    model->insertVariable(model->size(), "color_var", "#bbccdd");

    model->setVariableName("var_1", "color_var");
    EXPECT_EQ(chDataSigSpy.count(), 0);
    EXPECT_STREQ(model->data(model->index(0), QssdVariablesModel::VariableName)
                     .toString()
                     .toStdString()
                     .c_str(),
        "var_1");
}

TEST_F(TestQssdVariablesModel, TestRemoveVariable)
{
    model->insertVariable(model->size(), "var_1", "3em");
    model->insertVariable(model->size(), "var_2", "#aabbcc");

    QSignalSpy removeSigSpy(model, &QssdVariablesModel::rowsRemoved);

    model->removeVariable(model->index(1));

    ASSERT_EQ(removeSigSpy.count(), 1);
    const QList<QVariant>& fstEmission = removeSigSpy.takeFirst();
    EXPECT_EQ(fstEmission.at(1).toInt(), 1);
    EXPECT_EQ(fstEmission.at(1).toInt(), 1);

    model->removeVariable("var_1");

    ASSERT_EQ(removeSigSpy.count(), 1);
    const QList<QVariant>& secEmission = removeSigSpy.takeFirst();
    EXPECT_EQ(fstEmission.at(1).toInt(), 1);
    EXPECT_EQ(fstEmission.at(1).toInt(), 1);
}

TEST_F(TestQssdVariablesModel, TestNotExistingVariable)
{
    model->insertVariable(model->size(), "var_1", "3em");

    QSignalSpy removeSigSpy(model, &QssdVariablesModel::rowsRemoved);
    model->removeVariable("wrongVar");

    EXPECT_EQ(model->rowCount(QModelIndex()), 1);
    ASSERT_EQ(removeSigSpy.count(), 0);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
