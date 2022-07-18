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
    DocumentFile(const QString& name, QObject* parent);
    virtual ~DocumentFile();

    virtual bool exists() const override;
    virtual bool open(QIODeviceBase::OpenMode mode) override;

    virtual QString fileName() const override;
    virtual void setFileName(const QString& fileName) override;

    virtual QByteArray readAll() override;

    virtual qint64 write(const QByteArray& byteArray) override;
    virtual qint64 write(const char* data, qint64 maxSize) override;
};

inline DocumentFile::DocumentFile(QObject* parent)
    : IDocumentFile(parent), file(parent)
{
}

inline DocumentFile::DocumentFile(const QString& name) : file(name) { }

inline DocumentFile::DocumentFile(const QString& name, QObject* parent)
    : IDocumentFile(parent), file(name, parent)
{
}

inline DocumentFile::~DocumentFile() { }

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

inline void DocumentFile::setFileName(const QString& fileName)
{
    return file.setFileName(fileName);
}

inline QByteArray DocumentFile::readAll()
{
    return file.readAll();
}

inline qint64 DocumentFile::write(const QByteArray& byteArray)
{
    return file.write(byteArray);
}

inline qint64 DocumentFile::write(const char* data, qint64 maxSize)
{
    return file.write(data, maxSize);
}

#endif // DOCUMENTFILE_H
