#ifndef QSSDVARIABLESMODEL_H
#define QSSDVARIABLESMODEL_H

#include <QAbstractListModel>

class QssdVariablesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QssdVariablesModel(QObject* parent = nullptr);

private:
};

#endif // QSSDVARIABLESMODEL_H
