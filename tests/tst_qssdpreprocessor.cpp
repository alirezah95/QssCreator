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
        preProc = new QssdPreprocessor(modelMock);
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

    //    EXPECT_CALL(*modelMock, rowCount(_)).WillRepeatedly(Return(1));
    EXPECT_CALL(
        *modelMock, insertVariable(QString("Variable"), QString("12px")))
        .Times(1);
    EXPECT_CALL(
        *modelMock, insertVariable(QString("var_1"), QString("#ff22ee")))
        .Times(1);
    EXPECT_CALL(*modelMock, insertVariable(QString("VAR_2"), QString("12px")))
        .Times(1);

    editor->document()->setModified(false);
    preProc->processDocumentVariables(editor);
}

TEST_F(TestQssdPreprocessor, TestProcessedDocumentContent)
{
    EXPECT_CALL(*modelMock, getVariableValue(QString("Variable")))
        .WillRepeatedly(Return("12px"));
    EXPECT_CALL(*modelMock, getVariableValue(QString("var_1")))
        .WillRepeatedly(Return("#ff22ee"));
    EXPECT_CALL(*modelMock, getVariableValue(QString("VAR_2")))
        .WillRepeatedly(Return("12px"));

    EXPECT_STREQ(
        preProc->getProcessedDocumentContent(editor).toStdString().c_str(),
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
