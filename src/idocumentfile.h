#ifndef IDOCUMENTFILE_H
#define IDOCUMENTFILE_H

#include <QFile>
#include <QObject>

#define DOC_POSTFIX  ".qssd"
#define DOC_UNTITLED "untitled.qssd"
#define DOC_FILTER   "Qssd files (*.qssd)"

/*!
 * \brief The IDocumentFile class is an interface and an adapter for \a\b QFile
 * to be used in \ref DocumentOperations class and also to ease unit testing.
 */
class IDocumentFile : public QObject
{
    Q_OBJECT
public:
    IDocumentFile(QObject* parent = nullptr);
    virtual ~IDocumentFile();

    virtual bool exists() const = 0;
    virtual bool open(QIODevice::OpenMode mode) = 0;

    virtual QString fileName() const = 0;
    virtual void setFileName(const QString& fileName) = 0;

    virtual QByteArray readAll() = 0;

    virtual qint64 write(const QByteArray& byteArray) = 0;
    virtual qint64 write(const char* data, qint64 maxSize) = 0;
};

#endif // IDOCUMENTFILE_H
