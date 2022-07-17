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

        auto vName
            = index.data(IQssdVariablesModel::VariableName).toString() + " := ";
        auto vValue = index.data(IQssdVariablesModel::VariableValue).toString();

        int nameWidth = option.fontMetrics.boundingRect(vName).width();

        // Draw var name + " := "
        QRectF adjustedRect(option.rect.topLeft() + QPoint(2, 0),
            option.rect.size() - QSize(2, 0));
        painter->drawText(
            adjustedRect, Qt::AlignLeft | Qt::AlignVCenter, vName);

        // Draw var value
        QRectF vValueRect(adjustedRect.topLeft() + QPoint(nameWidth, 0),
            QSizeF(adjustedRect.width() - nameWidth, adjustedRect.height()));
        painter->drawText(vValueRect, Qt::AlignLeft | Qt::AlignVCenter, vValue);

        // Draw the border it view item is selected
        if (option.state & QStyle::State_Selected) {
            QPen rectPen;
            rectPen.setColor(option.palette.color(QPalette::Highlight));
            painter->setPen(rectPen);

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

QSize QssdVariableItemDelegate::sizeHint(
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return option.fontMetrics
               .boundingRect(
                   index.data(IQssdVariablesModel::VariableName).toString()
                   + " := "
                   + index.data(IQssdVariablesModel::VariableValue).toString())
               .size()
        + QSize(6, 6);
}
