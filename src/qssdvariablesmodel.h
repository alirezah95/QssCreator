#ifndef QSSDVARIABLESMODEL_H
#define QSSDVARIABLESMODEL_H

#include "iqssdvariablesmodel.h"

#include <QAbstractListModel>
#include <QMap>

/*!
 * \brief The QssdVariablesModel class which is the concrete class responsible
 * for holding the variables of a document and notifying the views (or other
 * classes) about the changes in the model data.
 * \details This class inherits both \ref IQssdVariablesModel and \a\b
 * QAbstractListModel to provide functionality of the two classes
 */
template <class Variable>
class QssdVariablesModel : public IQssdVariablesModel, public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        VariableName = Qt::UserRole + 1,
        VariableValue
    };
    explicit QssdVariablesModel(QObject* parent = nullptr);
    virtual ~QssdVariablesModel();

    // QAbstractListModel functionality
    /*!
     * \brief Reimplement \a\b QAbstractListModel::rowCount()
     * \param parent
     * \return
     */
    virtual int rowCount(const QModelIndex& parent) const override;

    /*!
     * \brief Reimplemnt \a\b QAbstractListModel::data()
     * \param index
     * \param role
     * \return
     */
    virtual QVariant data(const QModelIndex& index, int role) const override;

    /*!
     * \brief Reimplements \a\b QAbstractItemModel::setData() to set a variable
     * value only
     * \param index
     * \param value
     * \param role Only \ref Roles::VariableValue is accepted
     * \return
     */
    virtual bool setData(
        const QModelIndex& index, const QVariant& value, int role) override;

    // IQssdVariablesModel functionality
    /*!
     * \brief Implements \ref IQssdVariablesModel::variableValue()
     * \param varName
     * \return
     */
    virtual QString variableValue(const QString& varName) const override;

    /*!
     * \brief Implements \ref IQssdVariablesModel::setVariableValue()
     * \param varName
     * \param value
     * \return
     */
    virtual bool setVariableValue(
        const QString& varName, const QVariant& value) override;

    /*!
     * \brief Implements \ref IQssdVariablesModel::insertVariable()
     * \param name
     * \param value
     * \return
     */
    virtual bool insertVariable(
        const QString& name, const QString& value) override;

    /*!
     * \brief Implements \ref IQssdVariablesModel::removeVariable()
     * \param varName
     * \return
     */
    virtual bool removeVariable(const QString& varName) override;

private:
    QMap<QString, QString> mVariables; /*!< A map to store the variables and
                                        * their values.
                                        */
};

#endif // QSSDVARIABLESMODEL_H
