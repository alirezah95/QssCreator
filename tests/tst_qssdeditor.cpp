#include <QCoreApplication>
#include <QSignalSpy>
#include <QTest>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "qssdeditor.h"

#include "tst_mockqssdprocessor.h"
#include "tst_mockqssdvariablesmodle.h"

using namespace ::testing;

class TestQssdEditor : public Test
{
public:
    void SetUp()
    {
        procMock = new MockQssdProcessor;
        editor = new QssdEditor(procMock);
        return;
    }

    void TearDown()
    {
        delete editor;

        return;
    }

    MockQssdProcessor* procMock;
    QssdEditor* editor;
};

TEST_F(TestQssdEditor, TestBlockCount)
{
    for (int i = 0; i < 12; ++i) {
        QTest::keyPress(editor, Qt::Key_Enter);
    }
    EXPECT_EQ(editor->document()->blockCount(), 13);
}

TEST_F(TestQssdEditor, TestBlockCountChangedSignal)
{
    QSignalSpy bcChanged(editor->document(), &QTextDocument::blockCountChanged);

    int keyPressTimes = 3;
    for (int i = 0; i < keyPressTimes; ++i) {
        QTest::keyPress(editor, Qt::Key_Enter);
    }

    EXPECT_EQ(bcChanged.count(), keyPressTimes);
}

TEST_F(TestQssdEditor, TestGetLineNumberAreaWidth)
{
    for (int i = 0; i < 12; ++i) {
        QTest::keyPress(editor, Qt::Key_Enter);
    }
    EXPECT_EQ(editor->getLineNumbersAreaWidth(),
        QFontMetrics(editor->getLineNumbersFont()).boundingRect("0").width() * 2
            + 16);
}

TEST_F(TestQssdEditor, TestLineNumberAreaWidthJittering)
{
    // Getting block count to 21
    for (int i = 0; i < 20; ++i) {
        QTest::keyPress(editor, Qt::Key_Enter);
    }
    int lnNumsAreaWidth1 = editor->getLineNumbersAreaWidth();

    // Getting block count to 28
    for (int i = 0; i < 7; ++i) {
        QTest::keyPress(editor, Qt::Key_Enter);
    }
    int lnNumsAreaWidth2 = editor->getLineNumbersAreaWidth();

    EXPECT_TRUE(lnNumsAreaWidth1 == lnNumsAreaWidth2);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
