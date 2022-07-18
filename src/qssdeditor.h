#ifndef STYLESHEETEDITOR_H
#define STYLESHEETEDITOR_H

#include <QTextEdit>

#include "iqssdeditor.h"

class QWidget;

/*!
 * \brief The QssdEditor class which is the <em>Qt style sheet document</em>
 * editor
 */
class QssdEditor : public IQssdEditor
{
    Q_OBJECT

public:
    /*!
     * \brief Cosntructs a \ref QssdEditor instance using the \a proc processor.
     * Note that the QssEditor takes ownership of the processor (will delete it
     * in ctor)
     * \param proc A pointer to an instance of \ref IQssdProcessor
     * \param parent
     */
    explicit QssdEditor(IQssdProcessor* proc, QWidget* parent = nullptr);
    virtual ~QssdEditor();

    /*!
     * \brief setProcessor
     * \param proc
     */
    virtual void setProcessor(IQssdProcessor* proc) override;

    /*!
     * \brief Getter for the \ref IQssdProcessor instance
     * \return
     */
    virtual IQssdProcessor* getProcessor() const override { return mProcessor; }

    /*!
     * \brief getQtStylesheet
     * \param update If true the \ref IQssdProcessor::processDocument() is
     * called and then the result is returned otherwise the stylesheet stored in
     * \ref mStylesheet is returned
     * \return A \a\b QString to be used as the stylesheet for widgets
     */
    virtual QString getQtStylesheet(bool update = false) override;

    /*!
     * \brief Returns the width required for the line numbers area so that the
     * numbers are properly shown on it.
     * \return An integer as the width
     */
    virtual int getLineNumbersAreaWidth() const override
    {
        return mLineNumbersAreaWidth;
    };

    /*!
     * \brief Implements \ref IQssdEditor::setLineNumbersFont(QFont)
     * \param font
     */
    virtual void setLineNumbersFont(QFont font) override;

    /*!
     * \brief Implements \ref IQssdEditor::getLineNumbersFont()
     * \return
     */
    virtual QFont getLineNumbersFont() const override
    {
        return mLineNumbersAreaWidget->font();
    }

protected:
    /*!
     * \brief Reimplements \a\b QWidget::eventFilter(QObject*, QEvent*)
     * \param obj
     * \param event
     * \return
     */
    bool eventFilter(QObject* obj, QEvent* event) override;

    /*!
     * \brief Overrides \a\b QTextEdti::resizeEvent(QResizeEvent*).
     * \param event
     */
    void resizeEvent(QResizeEvent* event) override;

    /*!
     * \brief Reimplements \a\b QTextEdit::scrollContentsBy(int, int)
     * \param dx
     * \param dy
     */
    void scrollContentsBy(int dx, int dy) override;

private slots:
    /*!
     * \brief Sets a margin to the text edit to make enough room for line
     * numbers area widget.
     */
    void updateLineNumbersAreaWidth();

private:
    QWidget* mLineNumbersAreaWidget; /*!< A \a\b QWidget used for drawing line
                                      * numbers onto
                                      */
    int mLineNumbersAreaWidth = 0;

    IQssdProcessor* mProcessor; /*!< An \ref IQssdProcessor object responsible
                                 * for processing the content of the editor's
                                 * docuemnt
                                 */

    QString mStylesheet; /*!< The Qt stylesheet that is obtained after compiling
                          * the content of the document.
                          */
};

#endif // STYLESHEETEDITOR_H
