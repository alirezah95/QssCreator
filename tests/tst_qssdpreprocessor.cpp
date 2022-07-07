#include <QApplication>
#include <QTextEdit>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "qssdpreprocessor.h"
#include "tst_mockqssdvariablesmodle.h"

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
                       "    color: $Variable;"
                       "    background-color: $VAR_2;"
                       "}");

        modelMock = new MockVariablesModel;
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
    MockVariablesModel* modelMock;
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

TEST_F(TestQssdPreprocessor, TestProcessedDocumentContent)
{
    EXPECT_CALL(*modelMock, getVarValue(QString("Variable")))
        .WillRepeatedly(Return("12px"));
    EXPECT_CALL(*modelMock, getVarValue(QString("var_1")))
        .WillRepeatedly(Return("#ff22ee"));
    EXPECT_CALL(*modelMock, getVarValue(QString("VAR_2")))
        .WillRepeatedly(Return("12px"));

    EXPECT_STREQ(preProc->getProcessedDocumentContent().toStdString().c_str(),
        ""
        ""
        ""
        "*{"
        "    background-color: #ff22ee;"
        "}"
        "*:disabled {"
        "    color: 12px;"
        "}"
        "QFrame {"
        "    color: 12px;"
        "    background-color: 12px;"
        "}");
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}