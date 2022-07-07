#include <QApplication>
#include <QTextEdit>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "qssdpreprocessor.h"
#include "tst_mockabstractlistmodel.h"

using namespace ::testing;

class TestQssdPreprocessor : public Test
{
public:
    void SetUp()
    {
        editor = new QTextEdit("$Variable= 12px;"
                               "$var_1=#ff22ee;$VAR_2");
        editor->append(" = 12px;"
                       "*{"
                       "    background-color: $var_1;"
                       "}"
                       "*:disabled {"
                       "    color: $VAR_2;"
                       "}"
                       "QFrame {"
                       "    color: $var3;"
                       "    background-color: $VAR_2;"
                       "}");

        modelMock = new MockListModel;
        preProc = new QssdPreprocessor(editor, modelMock);
    }

    void TearDown()
    {
        delete preProc;
        delete editor;
        delete modelMock;
        return;
    }

    QssdPreprocessor* preProc;
    QTextEdit* editor;
    MockListModel* modelMock;
};

TEST_F(TestQssdPreprocessor, TestDifinitionRegex)
{
    editor->document()->setModified(true);

    EXPECT_CALL(*modelMock, rowCount(_)).WillRepeatedly(Return(1));
    EXPECT_CALL(*modelMock, setData(_, QVariant("$Variable= 12px;"), _))
        .Times(1);
    EXPECT_CALL(*modelMock, setData(_, QVariant("$var_1=#ff22ee;"), _))
        .Times(1);
    EXPECT_CALL(*modelMock, setData(_, QVariant("$VAR_2\n = 12px;"), _))
        .Times(1);

    editor->document()->setModified(false);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
