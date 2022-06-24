#include <QCoreApplication>
#include <QSignalSpy>
#include <QTest>

#include "stylesheeteditor.h"

class TestStyleSheetEditor : public QObject
{
    Q_OBJECT

public:
    TestStyleSheetEditor();
    ~TestStyleSheetEditor();

private slots:
    void testBlockCount();
    void testBlockCountChangedSignal();
    void testGetLineNumberAreaWidth();
};

TestStyleSheetEditor::TestStyleSheetEditor() { }

TestStyleSheetEditor::~TestStyleSheetEditor() { }

void TestStyleSheetEditor::testBlockCount()
{
    auto editor = QSharedPointer<StyleSheetEditor>(new StyleSheetEditor());

    for (int i = 0; i < 12; ++i) {
        QTest::keyPress(editor.get(), Qt::Key_Enter);
    }
    QCOMPARE(editor->document()->blockCount(), 13);
}

void TestStyleSheetEditor::testBlockCountChangedSignal()
{
    auto editor = QSharedPointer<StyleSheetEditor>(new StyleSheetEditor());
    QSignalSpy bcChanged(editor->document(), &QTextDocument::blockCountChanged);

    int keyPressTimes = 3;
    for (int i = 0; i < keyPressTimes; ++i) {
        QTest::keyPress(editor.get(), Qt::Key_Enter);
    }

    QCOMPARE(bcChanged.count(), keyPressTimes);
}

void TestStyleSheetEditor::testGetLineNumberAreaWidth()
{
    auto editor = QSharedPointer<StyleSheetEditor>(new StyleSheetEditor());

    for (int i = 0; i < 12; ++i) {
        QTest::keyPress(editor.get(), Qt::Key_Enter);
    }
    QCOMPARE(editor->getLineNumbersAreaWidth(),
        editor->fontMetrics().boundingRect("13").width() + 16);
}

QTEST_MAIN(TestStyleSheetEditor);

#include "tst_stylesheeteditor.moc"
