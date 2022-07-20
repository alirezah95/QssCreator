#ifndef IQSSDEDITOR_H
#define IQSSDEDITOR_H

#include <QTextEdit>

#define FONT_MAX_POINT_SIZE 28
#define FONT_MIN_POINT_SIZE 11

class IQssdProcessor;

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
     * \brief Setter for the pointer of type \ref IQssdProcessor
     * \param proc
     */
    virtual void setProcessor(IQssdProcessor* proc) = 0;

    /*!
     * \brief Getter for internal \ref IQssdProcessor pointer
     * \return
     */
    virtual IQssdProcessor* getProcessor() const = 0;

    /*!
     * \brief Returns the stylesheet that is obtained after processing the
     * document.
     * \param update If true the \ref IQssdProcessor::processDocument() is
     * called and then the result is returned
     * \return A \a\b QString that can be set as the style sheet of a widget
     */
    virtual QString getQtStylesheet(bool update = false) = 0;

    /*!
     * \brief Returns the width of the area which the line numbers are drawn
     * onto it
     * \return An integer as the width of the area
     */
    virtual int getLineNumbersAreaWidth() const = 0;
};

#endif // IQSSDEDITOR_H
