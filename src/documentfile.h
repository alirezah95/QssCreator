#ifndef DOCUMENTFILE_H
#define DOCUMENTFILE_H

#include "idocumentfile.h"

class DocumentFile : public IDocumentFile
{
public:
    QFile file;

public:
    explicit DocumentFile(QObject* parent = nullptr);
    explicit DocumentFile(const QString& name);
    DocumentFile(const QString& name, QObject* parent = nullptr);
    DocumentFile();

    virtual bool exists() const override;
    virtual bool open(QIODeviceBase::OpenMode mode) override;
    virtual QString fileName() const override;

    virtual QByteArray readAll() override;
    virtual QByteArray read(qint64 maxSize) override;
    virtual qint64 read(char* data, qint64 maxSize) override;

    virtual qint64 write(const char* data, qint64 maxSize) override;
    virtual qint64 write(const char* data) override;
    virtual qint64 write(const QByteArray& byteArray) override;
};

inline DocumentFile::DocumentFile(QObject* parent) : file(parent) { }

inline DocumentFile::DocumentFile(const QString& name) : file(name) { }

inline DocumentFile::DocumentFile(const QString& name, QObject* parent)
    : file(name, parent)
{
}

inline DocumentFile::DocumentFile() { }

inline bool DocumentFile::exists() const
{
    return file.exists();
}

inline bool DocumentFile::open(QIODeviceBase::OpenMode mode)
{
    return file.open(mode);
}

inline QString DocumentFile::fileName() const
{
    return file.fileName();
}

inline QByteArray DocumentFile::readAll()
{
    return file.readAll();
}

inline QByteArray DocumentFile::read(qint64 maxSize)
{
    return file.read(maxSize);
}

inline qint64 DocumentFile::read(char* data, qint64 maxSize)
{
    return file.read(data, maxSize);
}

inline qint64 DocumentFile::write(const char* data, qint64 maxSize)
{
    return file.write(data, maxSize);
}

inline qint64 DocumentFile::write(const char* data)
{
    return file.write(data);
}

inline qint64 DocumentFile::write(const QByteArray& byteArray)
{
    return file.write(byteArray);
}

#endif // DOCUMENTFILE_H
