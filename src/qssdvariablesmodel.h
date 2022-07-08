#ifndef QSSDVARIABLESMODEL_H
#define QSSDVARIABLESMODEL_H

#include "iqssdvariablesmodel.h"

#include <QMap>

/*!
 * \brief The QssdVariablesModel class which is the concrete class responsible
 * for holding the variables of a document and notifying the views (or other
 * classes) about the changes in the model data
 */
class QssdVariablesModel : public IQssdVariablesModel
{
    Q_OBJECT

public:
    explicit QssdVariablesModel(QObject* parent = nullptr);
    virtual ~QssdVariablesModel();

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
     * \brief Implements \ref IQssdVariablesModel::getVarValue()
     * \param varName
     * \return The value of the variable with the given \a varName. If no value
     * exists returns an empty \a\b QString.
     */
    virtual QString data(const QString& varName) const override;

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

    /*!
     * \brief insertData
     * \param name
     * \param value
     * \return
     */
    virtual bool insertData(const QString& name, const QString& value) override;

    /*!
     * \brief removeData
     * \param name
     * \return
     */
    virtual bool removeData(const QString& name) override;

private:
    QMap<QString, QString> mVariables; /*!< A map to store the variables and
                                        * their values.
                                        */
};

#endif // QSSDVARIABLESMODEL_H
