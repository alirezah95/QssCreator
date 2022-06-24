#include "stylesheeteditor.h"

StyleSheetEditor::StyleSheetEditor(QWidget* parent) : QTextEdit(parent) { }

StyleSheetEditor::~StyleSheetEditor() { }

int StyleSheetEditor::getLineNumberAreaWidth() const
{
    // Count the digits of the doucument's block count
    int lineCount = qMax(1, document()->blockCount());
    int digitCount = 0;
    while (lineCount > 0) {
        lineCount /= 10;
        digitCount++;
    }
    return (fontMetrics().boundingRect('0').width() * digitCount + 16);
}

void StyleSheetEditor::updateLineNumbersAreaWidth() { }
