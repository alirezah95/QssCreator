#ifndef QSSDVARIABLE_H
#define QSSDVARIABLE_H

#include <QColor>
#include <QString>

/*!
 * \brief The QssdVariable class representing a variable defined for the Qssd
 * editor.
 * \details This variable has a name and a value both of type \a\b QString. The
 * value can either be a simple string (like a number or any string) or an a
 * color which starts with \a # sign.
 */
class QssdVariable
{
public:
    /*!
     * \brief Default ctor. Name and value are empty and invalid
     */
    QssdVariable();

    /*!
     * \brief Construct a \ref QssVariable with the given name and string value.
     * The \a value can be \a\b QColor name (starting with #)
     * \param name Name of the variable. Must be unique
     * \param value Value of the variable. If starts with \a #, it is
     * interpreted as a color
     */
    QssdVariable(const QString& name, const QString& value);

    /*!
     * \brief Variable's name setter.
     * \param name The new name of the variable
     */
    void setName(const QString& name);

    /*!
     * \brief Variable's name getter.
     */
    auto getName() const { return mName; }

    /*!
     * \brief Variable's value setter
     * \param value The new value for the variable
     */
    void setValue(const QString& value);

    /*!
     * \brief Variable's value getter
     */
    auto getValue() const { return mValue; }

    /*!
     * \brief Checks if variable's is valid
     * \return The true if variable has non-empty (non-null) name and value
     */
    bool isValid() const { return !mValue.isEmpty() && !mName.isEmpty(); }

    /*!
     * \brief Checks if the value of this variable is a color
     * \return \a\b QColor::isValidColor() on the variable value
     */
    bool isColor() const { return QColor::isValidColor(mValue); }

private:
    QString mName = ""; /*!< Holds the name of the variable. */

    QString mValue = ""; /*!< Holds the value of the variable. */
};

#endif // QSSVARIABLE_H
