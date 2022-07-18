#include "qssdeditor.h"

#include "iqssdprocessor.h"

#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>

QssdEditor::QssdEditor(IQssdProcessor* proc, QWidget* parent)
    : IQssdEditor(parent), mLineNumbersAreaWidget(new QWidget(this)),
      mProcessor(proc)
{
    mProcessor->setParent(this);

    QFont editorFont("Mono");
    editorFont.setStyleHint(QFont::Monospace);
    editorFont.setPointSize(13);
    setLineNumbersFont(editorFont);

    mLineNumbersAreaWidget->installEventFilter(this);

    updateLineNumbersAreaWidth();

    connect(document(), &QTextDocument::blockCountChanged, this, [this] {
        updateLineNumbersAreaWidth();
        mLineNumbersAreaWidget->update();
    });

    setLineWrapMode(QTextEdit::NoWrap);
    return;
}

QssdEditor::~QssdEditor()
{
    if (mProcessor->parent() == this) {
        delete mProcessor;
    }
    return;
}

void QssdEditor::setProcessor(IQssdProcessor* proc)
{
    if (proc != nullptr) {
        mProcessor = proc;
    }
    return;
}

QString QssdEditor::getQtStylesheet(bool update)
{

}

void QssdEditor::setLineNumbersFont(QFont font)
{
    mLineNumbersAreaWidget->setFont(font);
    return;
}

bool QssdEditor::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == mLineNumbersAreaWidget && event->type() == QEvent::Paint) {
        if (auto pEv = dynamic_cast<QPaintEvent*>(event)) {
            QPainter painter(mLineNumbersAreaWidget);
            painter.fillRect(pEv->rect(), QBrush(Qt::lightGray));

            // Get the QTextCursor for position (0, 0) in viewport which is the
            // position of the first visible block
            auto fstVisibleCursor = cursorForPosition(QPoint(0, 0));
            // Get the rectangle of the first visibl block
            auto fstVisibleCursorRect = cursorRect(fstVisibleCursor);
            int currBlTop = fstVisibleCursorRect.top();
            int currBlBottom = fstVisibleCursorRect.bottom();

            // Get current block and block number
            auto currBlock = fstVisibleCursor.block();
            int currBlockNumber = currBlock.blockNumber();

            // Go through all the valid and visible lines and draw a line
            // number for them
            while (currBlock.isValid() && currBlTop <= pEv->rect().bottom()) {
                if (currBlock.isVisible()
                    && currBlBottom >= pEv->rect().top()) {
                    QString lineNumStr = QString::number(currBlockNumber + 1);

                    painter.setPen(Qt::black);
                    painter.drawText(0, currBlTop, mLineNumbersAreaWidth,
                        fontMetrics().height(), Qt::AlignCenter, lineNumStr);
                }

                currBlTop = currBlBottom;
                currBlBottom = currBlTop + fstVisibleCursorRect.height();
                currBlock = currBlock.next();

                currBlockNumber++;
            }
        }
        return true;
    }
    return false;
}

void QssdEditor::resizeEvent(QResizeEvent* event)
{
    const QRect& cr = contentsRect();
    mLineNumbersAreaWidget->setGeometry(
        cr.left(), cr.top(), viewportMargins().left(), cr.height());
    return QTextEdit::resizeEvent(event);
}

void QssdEditor::scrollContentsBy(int dx, int dy)
{
    mLineNumbersAreaWidget->scroll(dx, dy);
    return QTextEdit::scrollContentsBy(dx, dy);
}

void QssdEditor::updateLineNumbersAreaWidth()
{
    auto lnNumbersStr = QString::number(qMax(1, document()->blockCount()));
    mLineNumbersAreaWidth
        = mLineNumbersAreaWidget->fontMetrics().boundingRect("0").width()
            * lnNumbersStr.length()
        + 16;
    setViewportMargins(mLineNumbersAreaWidth, 0, 0, 0);
    return;
}
