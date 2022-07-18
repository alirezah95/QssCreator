#ifndef QSSDVARIABLESMODEL_H
#define QSSDVARIABLESMODEL_H

#include "iqssdvariablesmodel.h"

#include <QMap>

using Variable = QPair<QString, QString>;

/*!
 * \brief The QssdVariablesModel class which is the concrete class
 * responsible for holding the variables of a document and notifying the
 * views (or other classes) about the changes in the model data.
 */
class QssdVariablesModel : public IQssdVariablesModel
{
    Q_OBJECT

public:
    explicit QssdVariablesModel(QObject* parent = nullptr);
    virtual ~QssdVariablesModel();

    /*!
     * \brief rowCount
     * \param parent
     * \return
     */
    virtual int rowCount(const QModelIndex& parent) const override;

    /*!
     * \brief data
     * \param index
     * \param role
     * \return
     */
    virtual QVariant data(const QModelIndex& index, int role) const override;

    /*!
     * \brief setVariables
     * \param vars
     */
    virtual void setVariables(const QVector<Variable>& vars) override;

    /*!
     * \brief getVariableValue
     * \param varName
     * \return
     */
    virtual QString getVariableValue(const QString& varName) const override;

    /*!
     * \brief getVariableValue
     * \param index
     * \return
     */
    virtual QString getVariableValue(const QModelIndex& index) const override;

    /*!
     * \brief setVariableValue
     * \param varName
     * \param value
     * \return
     */
    virtual bool setVariableValue(
        const QString& varName, const QString& value) override;

    /*!
     * \brief setVariableValue
     * \param index
     * \param value
     * \return
     */
    virtual bool setVariableValue(
        const QModelIndex& index, const QString& value) override;

    /*!
     * \brief setVariableName
     * \param oldName
     * \param newName
     * \return
     */
    virtual bool setVariableName(
        const QString& oldName, const QString& newName) override;

    /*!
     * \brief setVariableName
     * \param index
     * \param newName
     * \return
     */
    virtual bool setVariableName(
        const QModelIndex& index, const QString& newName) override;

    /*!
     * \brief insertVariable
     * \param name
     * \param value
     * \return
     */
    virtual bool insertVariable(
        int row, const QString& name, const QString& value) override;

    /*!
     * \brief removeVariable
     * \param varName
     * \return
     */
    virtual bool removeVariable(const QString& varName) override;

    /*!
     * \brief removeVariable
     * \param index
     * \return
     */
    virtual bool removeVariable(const QModelIndex& index) override;

    /*!
     * \brief size
     * \return
     */
    virtual size_t size() const override { return rowCount(QModelIndex()); };

private:
    /*!
     * \brief Check if variable with the given \a varName exists
     * \param varName
     * \return True if it exists and false otherwise
     */
    bool contains(const QString& varName) const;

private:
    QVector<Variable> mVariables; /*!< A list to store the variables */
};

#endif // QSSDVARIABLESMODEL_H
