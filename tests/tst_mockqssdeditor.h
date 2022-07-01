#include "iqssdeditor.h"

class MockQssdEditor : public IQssdEditor
{
    virtual int getLineNumbersAreaWidth() const override;
    virtual void setLineNumbersFont(QFont font) override;
    virtual QFont getLineNumbersFont() const override;
};
