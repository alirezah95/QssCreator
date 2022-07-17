#include "qssdvariableitemdelegate.h"

#include "qssdvariablesmodel.h"

#include <QLabel>
#include <QPainter>

QssdVariableItemDelegate::QssdVariableItemDelegate(QObject* parent)
    : QStyledItemDelegate { parent }
{
    lbl = new QLabel;
}

QssdVariableItemDelegate::~QssdVariableItemDelegate()
{
    delete lbl;
}

void QssdVariableItemDelegate::paint(QPainter* painter,
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        painter->save();
        painter->fillRect(option.rect, option.palette.base());

        QRectF leftHalf(option.rect.topLeft(),
            QSize(option.rect.width() / 2 - 1, option.rect.height()));
        painter->drawText(leftHalf, Qt::AlignLeft | Qt::AlignVCenter,
            index.data(IQssdVariablesModel::VariableName).toString());

        painter->drawLine(QPointF(leftHalf.right() + 1, leftHalf.top()),
            QPointF(leftHalf.right() + 1, leftHalf.bottom()));

        QRectF rightHalfRect(
            leftHalf.topRight() + QPoint(2, 0), leftHalf.size());
        painter->drawText(rightHalfRect, Qt::AlignCenter,
            index.data(IQssdVariablesModel::VariableValue).toString());

        auto bgOption(option);
        bgOption.palette.setColor(
            QPalette::Highlight, bgOption.palette.color(QPalette::Base));

        QPen rectPen;
        rectPen.setColor(option.palette.color(QPalette::Highlight));
        painter->setPen(rectPen);

        if (option.state & QStyle::State_Selected) {
            painter->drawRect(option.rect.adjusted(1, 1, -1, -1));
        }

        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

void QssdVariableItemDelegate::setModelData(
    QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
}
