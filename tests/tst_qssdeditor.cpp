#include <QCoreApplication>
#include <QSignalSpy>
#include <QTest>

#include "qssdeditor.h"

class TestQssdEditor : public QObject
{
    Q_OBJECT

public:
    TestQssdEditor();
    ~TestQssdEditor();

private slots:
    void testBlockCount();
    void testBlockCountChangedSignal();
    void testGetLineNumberAreaWidth();
    void testLineNumberAreaWidthJittering();
};

TestQssdEditor::TestQssdEditor() { }

TestQssdEditor::~TestQssdEditor() { }

void TestQssdEditor::testBlockCount()
{
    auto editor = QSharedPointer<IQssdEditor>(new QssdEditor());

    for (int i = 0; i < 12; ++i) {
        QTest::keyPress(editor.get(), Qt::Key_Enter);
    }
    QCOMPARE(editor->document()->blockCount(), 13);
}

void TestQssdEditor::testBlockCountChangedSignal()
{
    auto editor = QSharedPointer<QssdEditor>(new QssdEditor());
    QSignalSpy bcChanged(editor->document(), &QTextDocument::blockCountChanged);

    int keyPressTimes = 3;
    for (int i = 0; i < keyPressTimes; ++i) {
        QTest::keyPress(editor.get(), Qt::Key_Enter);
    }

    QCOMPARE(bcChanged.count(), keyPressTimes);
}

void TestQssdEditor::testGetLineNumberAreaWidth()
{
    auto editor = QSharedPointer<QssdEditor>(new QssdEditor());

    for (int i = 0; i < 12; ++i) {
        QTest::keyPress(editor.get(), Qt::Key_Enter);
    }
    QCOMPARE(editor->getLineNumbersAreaWidth(),
        QFontMetrics(editor->getLineNumbersFont()).boundingRect("0").width() * 2
            + 16);
}

void TestQssdEditor::testLineNumberAreaWidthJittering()
{
    auto editor = QSharedPointer<QssdEditor>(new QssdEditor());

    // Getting block count to 21
    for (int i = 0; i < 20; ++i) {
        QTest::keyPress(editor.get(), Qt::Key_Enter);
    }
    int lnNumsAreaWidth1 = editor->getLineNumbersAreaWidth();

    // Getting block count to 28
    for (int i = 0; i < 7; ++i) {
        QTest::keyPress(editor.get(), Qt::Key_Enter);
    }
    int lnNumsAreaWidth2 = editor->getLineNumbersAreaWidth();

    QVERIFY(lnNumsAreaWidth1 == lnNumsAreaWidth2);
}

QTEST_MAIN(TestQssdEditor);

#include "tst_qssdeditor.moc"
