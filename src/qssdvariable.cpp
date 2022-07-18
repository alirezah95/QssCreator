#include "qssdvariable.h"

QssdVariable::QssdVariable() { }

QssdVariable::QssdVariable(const QString& name, const QString& value)
    : mName(name), mValue(value)
{
}

void QssdVariable::setName(const QString& name)
{
    mName = name;
    return;
}

void QssdVariable::setValue(const QString& value)
{
    mValue = value;
    return;
}
