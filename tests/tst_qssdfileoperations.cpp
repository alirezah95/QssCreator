#include <QApplication>
#include <QTextEdit>

#include "qssdfileoperations.h"
#include "tst_mockdocumentfile.h"

using namespace ::testing;
/*
 * Defining a test fixture
 */
class TextEdit : public testing::Test
{
public:
    virtual void SetUp() override
    {
        editor.insertPlainText("Testing QssdFileOperations");
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
    QssdFileOperations opers;

    opers.newDocument(&editor);

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

    QssdFileOperations dOper;
    dOper.openDocument(&editor, &docFile);

    EXPECT_EQ(editor.document()->toPlainText(), readAllRes);
};

TEST_F(TextEdit, testSaveDoucment)
{
    auto writeParam = QByteArray("Testing QssdFileOperations");
    MockDocumentFile docFile;
    EXPECT_CALL(
        docFile, open(QIODeviceBase::OpenMode(QIODeviceBase::WriteOnly)))
        .WillOnce(Return(true));
    EXPECT_CALL(docFile, write(writeParam)).WillOnce(Return(writeParam.size()));

    QssdFileOperations dOper;
    dOper.saveDocument(&editor, &docFile);
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
