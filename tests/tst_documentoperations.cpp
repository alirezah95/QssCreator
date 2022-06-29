#include <QApplication>
#include <QTextEdit>

#include <gmock/gmock.h>

#include "documentoperations.h"
#include "idocumentfile.h"

using namespace ::testing;

/*
 * Defining a mock object for IDocumentFie
 */
class MockDocumentFile : public IDocumentFile
{
public:
    MockDocumentFile() : IDocumentFile(nullptr) { }
    virtual ~MockDocumentFile() {};

    MOCK_METHOD(bool, exists, (), (const));
    MOCK_METHOD(bool, open, (QIODeviceBase::OpenMode mode));
    MOCK_METHOD(QString, fileName, (), (const));
    MOCK_METHOD(void, setFileName, (const QString& fileName));
    MOCK_METHOD(QByteArray, readAll, ());
    MOCK_METHOD(qint64, write, (const QByteArray& byteArray));
};

/*
 * Defining a test fixture
 */
class TextEdit : public testing::Test
{
public:
    virtual void SetUp() override
    {
        editor.insertPlainText("Testing DocumentOperations");
        return;
    }

    virtual void TearDown() override
    {
        editor.insertPlainText("");
        return;
    }

    QTextEdit editor;
};

TEST_F(TextEdit, testNewDocument)
{
    DocumentOperations doc;

    doc.newDocument(&editor);

    EXPECT_EQ(editor.document()->toPlainText(), "");
}

TEST_F(TextEdit, testOpenDocument)
{
    char readAllRes[] = "Mocking IDocumentFile";
    MockDocumentFile docFile;
    EXPECT_CALL(docFile, open(QIODeviceBase::OpenMode(QIODeviceBase::ReadOnly)))
        .WillOnce(Return(true));
    EXPECT_CALL(docFile, readAll()).WillOnce(Return(readAllRes));
    EXPECT_CALL(docFile, fileName()).WillOnce(Return(":temporary/file.txt"));

    DocumentOperations dOper;
    dOper.openDocument(&editor, &docFile);

    EXPECT_EQ(editor.document()->toPlainText(), readAllRes);
};

TEST_F(TextEdit, testSaveDoucment)
{
    auto writeParam = QByteArray("Testing DocumentOperations");
    MockDocumentFile docFile;
    EXPECT_CALL(
        docFile, open(QIODeviceBase::OpenMode(QIODeviceBase::WriteOnly)))
        .WillOnce(Return(true));
    EXPECT_CALL(docFile, write(writeParam)).WillOnce(Return(writeParam.size()));

    DocumentOperations dOper;
    dOper.saveDocument(&editor, &docFile);
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
