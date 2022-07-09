#ifndef IQSSDVARIABLESMODEL_H
#define IQSSDVARIABLESMODEL_H

#include <QtCore>
/*!
 * \brief The IQssdVariablesModel class which is the abstract base class for
 * a model of the variables inside a \a\b QTextEdit, it makes sure that the
 * subclasses implement methods and functionality required for a class to store
 * and retrieve a variable
 */
class IQssdVariablesModel
{
public:
    explicit IQssdVariablesModel();
    virtual ~IQssdVariablesModel();

    /*!
     * \brief Finds the variable with \a varName if any, and returns its value.
     * If there is no variable with the given name an empty string is returned
     * \param varName The name of the variable
     * \return A \a\b QString as the variable value
     */
    virtual QString variableValue(const QString& varName) const = 0;

    /*!
     * \brief Find the variable with the given \a varName, if any, and sets its
     * value to \a value
     * \param varName Name of the variable
     * \param value New value of the variable
     * \param role
     * \return
     */
    virtual bool setVariableValue(const QString& varName, const QVariant& value)
        = 0;

    /*!
     * \brief Inserts a variable with the given \a name and \a value to the
     * internal data
     * \param name
     * \param value
     * \return True if data inserted successfuly and false otherwise
     */
    virtual bool insertVariable(const QString& name, const QString& value) = 0;

    /*!
     * \brief Removes a variable from internal data
     * \param name
     * \return True if data was removed successfully and false otherwise
     */
    virtual bool removeVariable(const QString& varName) = 0;
};

#endif // IQSSDVARIABLESMODEL_H
