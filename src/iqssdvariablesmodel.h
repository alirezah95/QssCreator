#ifndef IQSSDVARIABLESMODEL_H
#define IQSSDVARIABLESMODEL_H

#include <QAbstractListModel>

/*!
 * \brief The IQssdVariablesModel class which is the abstract base class for
 * a model of the variables inside a \a\b QTextEdit
 * \details Thic abstract class is needed to force the subcalsses to define
 * \ref getVarValue(const QString&) method
 */
class IQssdVariablesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit IQssdVariablesModel(QObject* parent = nullptr);
    virtual ~IQssdVariablesModel();

    /*!
     * \brief Finds the variable with \a varName if any, and returns its value.
     * If there is no variable with the given name an empty string is returned
     * \param varName The name of the variable
     * \return A \a\b QString as the variable value
     */
    virtual QString getVarValue(const QString& varName) const = 0;
};

#endif // IQSSDVARIABLESMODEL_H
