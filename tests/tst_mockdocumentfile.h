#include "idocumentfile.h"

#include <gmock/gmock.h>

/*
 * Defining a mock object for IDocumentFie
 */
class MockDocumentFile : public IDocumentFile
{
public:
    MockDocumentFile() : IDocumentFile(nullptr) { }
    virtual ~MockDocumentFile() {};

    MOCK_METHOD(bool, exists, (), (const));
    MOCK_METHOD(bool, open, (QIODeviceBase::OpenMode mode));
    MOCK_METHOD(QString, fileName, (), (const));
    MOCK_METHOD(void, setFileName, (const QString& fileName));
    MOCK_METHOD(QByteArray, readAll, ());
    MOCK_METHOD(qint64, write, (const QByteArray& byteArray));
};
