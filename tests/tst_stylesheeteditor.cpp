#include <QCoreApplication>
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

void TestStyleSheetEditor::testGetLineNumberAreaWidth()
{
    auto editor = QSharedPointer<StyleSheetEditor>(new StyleSheetEditor());

    for (int i = 0; i < 12; ++i) {
        QTest::keyPress(editor.get(), Qt::Key_Enter);
    }
    QCOMPARE(editor->getLineNumberAreaWidth(),
        editor->fontMetrics().boundingRect('0').width() * 2 + 16);
}

QTEST_MAIN(TestStyleSheetEditor);

#include "tst_stylesheeteditor.moc"
