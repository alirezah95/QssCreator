#ifndef TST_MOCKQSSDPREPROCESSOR_H
#define TST_MOCKQSSDPREPROCESSOR_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "iqssdprocessor.h"

using namespace ::testing;

class MockQssdPreprocessor : public IQssdPreprocessor
{
public:
    MOCK_METHOD(
        void, setVariablesModel, (IQssdVariablesModel * varsModel), (override));
    MOCK_METHOD(
        QString, getProcessedDocumentContent, (QTextEdit * editor), (override));
    MOCK_METHOD(
        void, processDocumentVariables, (QTextEdit * editor), (override));
};

#endif
