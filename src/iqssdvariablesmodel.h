#ifndef IQSSDVARIABLESMODEL_H
#define IQSSDVARIABLESMODEL_H

#include <QAbstractListModel>

/*!
 * \brief The IQssdVariablesModel class which is the abstract base class for
 * a model of the variables inside a \a\b QTextEdit
 * \details This class provides the interface required for a model of variables.
 * There are some invariants required for the data (variables) integrity and
 * validity and this interface will make sure that at least the methods
 * signature will not allow breaking them. Subclasses of this interface should
 * not break these invariants too.
 * One of this invariants is that for inserting a variable a valid \a name (non
 * empty) and a valid \a value (non empty) is required. So for this to always be
 * true the \a\b insertRows() of \a\b QAbstractListModel class must be overriden
 * by this interface and be made final so subclasses don't override it, and also
 * \a\b removeRows().
 */
class IQssdVariablesModel : public QAbstractListModel
{
public:
    explicit IQssdVariablesModel(QObject* parent = nullptr);
    virtual ~IQssdVariablesModel();

    /*!
     * \brief \a\b QAbstractListModel::rowCount()
     * \param parent
     * \return
     */
    virtual int rowCount(const QModelIndex& parent) const override = 0;

    /*!
     * \brief \a\b QAbstractListModel::data()
     * \param index
     * \param role
     * \return
     */
    virtual QVariant data(
        const QModelIndex& index, int role) const override = 0;

    // not allowing insertRows overriding
    /*!
     * \brief Overrides \a\b QAbstactListModel::insertRows() and make it final
     * to make sure that this method won't do any insertion
     * \param row
     * \param count
     * \param index
     * \return
     */
    virtual bool insertRows(
        int row, int count, const QModelIndex& index) override final;

    /*!
     * \brief Finds the variable with \a varName if any, and returns its value.
     * If there is no variable with the given name an empty string is returned.
     * Calls \ref data() internally with proper value
     * \param varName The name of the variable
     * \return A \a\b QString as the variable value
     */
    virtual QString getVariableValue(const QString& varName) const = 0;

    /*!
     * \brief This is an overloaded method to get the value of a variable at
     * index \a index. Calls \ref data() internally with proper value
     * \param index
     * \return The value of the variable as a \a\b QString
     */
    virtual QString getVariableValue(const QModelIndex& index) const = 0;

    /*!
     * \brief Find the variable with the given \a varName, if any, and sets its
     * value to \a value
     * \param varName Name of the variable
     * \param value New value of the variable
     * \param role
     * \return
     */
    virtual bool setVariableValue(const QString& varName, const QString& value)
        = 0;

    /*!
     * \brief This is an overloaded method to set the value of variable at index
     * \a index
     * \param index
     * \param value
     * \return
     */
    virtual bool setVariableValue(
        const QModelIndex& index, const QString& value)
        = 0;

    /*!
     * \brief Change the name of the variable to the \a newName
     * \param oldName Current name of the variable
     * \param newName New name of the variable
     * \return
     */
    virtual bool setVariableName(const QString& oldName, const QString& newName)
        = 0;

    /*!
     * \brief This is an overloaded method to set (change) the name of a
     * variable using its indexx
     * \param oldName
     * \param newName
     * \return
     */
    virtual bool setVariableName(
        const QModelIndex& index, const QString& newName)
        = 0;

    /*!
     * \brief Inserts a variable with the given \a name and \a value to the
     * internal data
     * \param row The new variable will be inserted at this index
     * \param name The name of the new variable
     * \param value The value of the new variable
     * \return True if data inserted successfuly and false otherwise
     */
    virtual bool insertVariable(
        int row, const QString& name, const QString& value)
        = 0;

    /*!
     * \brief Overrides \a\b QAbstractListModel::removeRows() and make it pure
     * virtual to make sure subclasses define it
     * \param row
     * \param count
     * \param parent
     * \return
     */
    virtual bool removeRows(
        int row, int count, const QModelIndex& parent) override = 0;

    /*!
     * \brief Removes a variable from internal data
     * \param name
     * \return True if data was removed successfully and false otherwise
     */
    virtual bool removeVariable(const QString& varName) = 0;

    /*!
     * \brief This is an overloaded method to remove a variable by its index
     * \param index
     * \return
     */
    virtual bool removeVariable(const QModelIndex& index) = 0;

    /*!
     * \brief Returns the size of the model's internal data
     * \return
     */
    virtual size_t size() const = 0;
};

#endif // IQSSDVARIABLESMODEL_H
