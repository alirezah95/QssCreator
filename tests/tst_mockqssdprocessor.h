#ifndef TST_MOCKQSSDPROCESSOR_H
#define TST_MOCKQSSDPROCESSOR_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "iqssdprocessor.h"

using namespace ::testing;

class MockQssdProcessor : public IQssdProcessor
{
public:
    MOCK_METHOD(
        void, setVariablesModel, (IQssdVariablesModel * varsModel), (override));

    MOCK_METHOD(QString, processDocument,
        (QTextDocument * doc, bool updateModel), (override));
};

#endif
