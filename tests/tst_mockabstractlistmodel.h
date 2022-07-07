#include <QAbstractListModel>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockListModel : public QAbstractListModel
{
public:
    MOCK_METHOD(int, rowCount, (const QModelIndex& parent), (const override));
    MOCK_METHOD(
        QVariant, data, (const QModelIndex& index, int role), (const override));

    MOCK_METHOD(bool, setData,
        (const QModelIndex& index, const QVariant& value, int role),
        (override));
};
