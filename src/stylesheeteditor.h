#ifndef STYLESHEETEDITOR_H
#define STYLESHEETEDITOR_H

#include <QTextEdit>

class QWidget;

namespace Ui {
class StyleSheetEditor;
}

class StyleSheetEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit StyleSheetEditor(QWidget* parent = nullptr);
    ~StyleSheetEditor();

    /*!
     * \brief Returns the width required for the line numbers area so that the
     * numbers are properly shown on it.
     * \return An integer as the width
     */
    inline int getLineNumbersAreaWidth() const
    {
        return mLineNumbersAreaWidth;
    };

private slots:
    /*!
     * \brief Sets a margin to the text edit to make enough room for line
     * numbers area widget.
     */
    void updateLineNumbersAreaWidth();

private:
    QWidget* mLineNumberAreaWidget; /*!< A \a\b QWidget used for drawing line
                                     * numbers onto
                                     */
    int mLineNumbersAreaWidth = 0;
};

#endif // STYLESHEETEDITOR_H
