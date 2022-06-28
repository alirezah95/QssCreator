#include <QCoreApplication>
#include <QTest>
#include <QTextEdit>

#include "documentoperations.h"
#include "idocumentfile.h"

class TestDocumentFile : public IDocumentFile
{
public:
    TestDocumentFile() { }

    virtual bool exists() const override { return true; };

    virtual bool open(QIODeviceBase::OpenMode mode) override { return true; };

    virtual QString fileName() const override { return ":temporary/test"; };

    virtual QByteArray readAll() override
    {
        return "This is a stub class for DocumentFile class";
    };

    virtual QByteArray read(qint64 maxSize) override
    {
        QByteArray ret("This is a stub class for DocumentFile class");
        if (ret.size() > maxSize) {
            ret.chop(maxSize);
        }
        return ret;
    };

    virtual qint64 read(char* data, qint64 maxSize) override
    {
        data = (char*)"This is a stub class for DocumentFile class";
        return sizeof("This is a stub class for DocumentFile class");
    }

    virtual qint64 write(const char* data, qint64 maxSize) override
    {
        return qMax(maxSize, 200);
    }

    virtual qint64 write(const char* data) override { return 200; }

    virtual qint64 write(const QByteArray& byteArray) override
    {
        return byteArray.size();
    }
};

class TestDocumentOperations : public QObject
{
    Q_OBJECT

private slots:
    void testNewDocument();
    void testOpenDocument();
    void testSaveDoucment();
};

void TestDocumentOperations::testNewDocument()
{
    QTextEdit editor;
    editor.insertPlainText("An arbitrary plain text to test newFile method");

    DocumentOperations doc;

    doc.newDocument(&editor);
    QCOMPARE(editor.document()->toPlainText(), "");
}

void TestDocumentOperations::testOpenDocument() { }

void TestDocumentOperations::testSaveDoucment() { }

QTEST_MAIN(TestDocumentOperations);

#include "tst_documentoperations.moc"
