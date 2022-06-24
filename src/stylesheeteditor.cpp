#include "stylesheeteditor.h"

StyleSheetEditor::StyleSheetEditor(QWidget* parent) : QTextEdit(parent)
{
    connect(document(), &QTextDocument::blockCountChanged, this, [this] {
        updateLineNumbersAreaWidth();
        mLineNumberAreaWidget->update();
    });
    return;
}

StyleSheetEditor::~StyleSheetEditor() { }

void StyleSheetEditor::updateLineNumbersAreaWidth()
{
    mLineNumbersAreaWidth
        = fontMetrics()
              .boundingRect(QString::number(qMax(1, document()->blockCount())))
              .width()
        + 16;
    setViewportMargins(mLineNumbersAreaWidth, 0, 0, 0);
    return;
}
