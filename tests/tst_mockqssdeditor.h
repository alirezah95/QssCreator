#ifndef TST_MOCKQSSDEDITOR_H
#define TST_MOCKQSSDEDITOR_H

#include "iqssdeditor.h"

#include <gmock/gmock.h>

class MockQssdEditor : public IQssdEditor
{
public:
    explicit MockQssdEditor(QWidget* parent = nullptr) : IQssdEditor(parent) { }
    virtual ~MockQssdEditor() { }

    MOCK_METHOD(void, setProcessor, (IQssdProcessor*), (override));
    MOCK_METHOD(IQssdProcessor*, getProcessor, (), (const override));
    MOCK_METHOD(QString, getQtStylesheet, (bool update), (override));
    MOCK_METHOD(int, getLineNumbersAreaWidth, (), (const override));
};

#endif
