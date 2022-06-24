#include "stylesheeteditor.h"

StyleSheetEditor::StyleSheetEditor(QWidget* parent)
    : QTextEdit(parent), mLineNumbersAreaWidget(new QWidget(this))
{
    connect(document(), &QTextDocument::blockCountChanged, this, [this] {
        updateLineNumbersAreaWidth();
        mLineNumbersAreaWidget->update();
    });
    return;
}

StyleSheetEditor::~StyleSheetEditor() { }

void StyleSheetEditor::resizeEvent(QResizeEvent* event)
{
    const QRect& cr = contentsRect();
    mLineNumbersAreaWidget->setGeometry(
        cr.left(), cr.right(), viewportMargins().left(), cr.height());
}

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
