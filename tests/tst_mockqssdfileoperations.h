#ifndef TST_MOCKQSSDFILEOPERATIONS_H
#define TST_MOCKQSSDFILEOPERATIONS_H

#include "iqssdfileoperations.h"

#include <gmock/gmock.h>

class MockQssdFileOperations : public IQssdFileOperations
{
public:
    explicit MockQssdFileOperations(QObject* parent = nullptr)
        : IQssdFileOperations(parent) {};
    virtual ~MockQssdFileOperations() { }

    MOCK_METHOD(bool, newDocument, (QTextEdit * editor), (override));
    MOCK_METHOD(bool, openDocument,
        (QTextEdit * editor, IDocumentFile* docFile), (override));
    MOCK_METHOD(bool, saveDocument,
        (const QTextEdit* editor, IDocumentFile* outFile), (override));
    MOCK_METHOD(bool, writeToFile,
        (const QString& docContent, IDocumentFile* outFile), (override));
};

#endif // TST_MOCKQSSDFILEOPERATIONS_H
