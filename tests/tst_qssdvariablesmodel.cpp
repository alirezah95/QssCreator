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
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
