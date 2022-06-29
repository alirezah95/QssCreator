#ifndef IDOCUMENTFILE_H
#define IDOCUMENTFILE_H

#include <QFile>
#include <QObject>

/*!
 * \brief The IDocumentFile class is an interface and an adapter for \a\b QFile
 * to be used in \ref DocumentOperations class and also to ease unit testing.
 */
class IDocumentFile : public QObject
{
    Q_OBJECT
public:
    IDocumentFile() = default;

    virtual bool exists() const = 0;
    virtual bool open(QIODevice::OpenMode mode) = 0;
    virtual QString fileName() const = 0;

    virtual QByteArray readAll() = 0;

    virtual qint64 write(const QByteArray& byteArray) = 0;
};

#endif // IDOCUMENTFILE_H
