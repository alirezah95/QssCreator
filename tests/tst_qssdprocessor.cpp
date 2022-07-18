#include <QApplication>
#include <QTextEdit>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "qssdprocessor.h"
#include "tst_mockqssdvariablesmodle.h"

using namespace ::testing;

class TestQssdProcessor : public Test
{
public:
    void SetUp()
    {
        editor = new QTextEdit();
        editor->document()->setPlainText("$var1 = #ff3320;\n"
                                         "$var2 = red;\n"
                                         "$var3 = 13px;\n"
                                         "$var2 = 22;\n"
                                         "$ = 2em;\n"
                                         "$5thVar = 4em;\n");

        modelMock = new MockVariablesModel;
        proc = new QssdProcessor(modelMock);
    }

    void TearDown()
    {
        delete proc;
        delete editor;
        delete modelMock;
        return;
    }

    QssdProcessor* proc;
    QTextEdit* editor;
    MockVariablesModel* modelMock;
};

TEST_F(TestQssdProcessor, TestProcessedDocument)
{
    EXPECT_CALL(*modelMock, setVariables(_))
        .WillOnce(Invoke([](const QVector<QPair<QString, QString>>& vars) {
            EXPECT_EQ(vars.size(), 4);

            EXPECT_STREQ(vars[0].first.toStdString().c_str(), "var1");
            EXPECT_STREQ(vars[0].second.toStdString().c_str(), "#ff3320");

            EXPECT_STREQ(vars[1].first.toStdString().c_str(), "var2");
            EXPECT_STREQ(vars[1].second.toStdString().c_str(), "red");

            EXPECT_STREQ(vars[2].first.toStdString().c_str(), "var3");
            EXPECT_STREQ(vars[2].second.toStdString().c_str(), "13px");

            EXPECT_STREQ(vars[3].first.toStdString().c_str(), "var2");
            EXPECT_STREQ(vars[3].second.toStdString().c_str(), "22");
        }));
    auto content = proc->processDocument(editor->document(), true);

    EXPECT_STREQ(content.toStdString().c_str(),
        "\n"
        "\n"
        "\n"
        "\n"
        "$ = 2em;\n"
        "$5thVar = 4em;\n");
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
