#include <QCoreApplication>
#include <QTest>

class TestDocumentOperations : public QObject
{
    Q_OBJECT

private slots:
    void testNewFile();
    void testOpenFile();
    void testSaveFile();
};

void TestDocumentOperations::testNewFile() { }

void TestDocumentOperations::testOpenFile() { }

void TestDocumentOperations::testSaveFile() { }

QTEST_MAIN(TestDocumentOperations);

#include "tst_documentoperations.moc"
