#ifndef IQSSDEDITOR_H
#define IQSSDEDITOR_H

#include <QTextEdit>

/*!
 * \brief The IQssdEditor class which is an interface for the Qt style sheet
 * document editors responsible for handling a Qssd document editing
 */
class IQssdEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit IQssdEditor(QWidget* parent = nullptr);
    virtual ~IQssdEditor();

    /*!
     * \brief Returns the width of the area which the line numbers are drawn
     * onto it
     * \return An integer as the width of the area
     */
    virtual int getLineNumbersAreaWidth() const = 0;

    /*!
     * \brief Sets the font of the line numbers
     * \param font A \a\b QFont
     */
    virtual void setLineNumbersFont(QFont font) = 0;

    /*!
     * \brief Returns the font of the line numbers
     * \return A \a\b QFont
     */
    virtual QFont getLineNumbersFont() const = 0;
};

#endif // IQSSDEDITOR_H
