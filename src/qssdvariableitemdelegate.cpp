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
        QRectF adjustedRect(option.rect.topLeft() + QPoint(4, 0),
            option.rect.size() - QSize(8, 0));
        painter->drawText(
            adjustedRect, Qt::AlignLeft | Qt::AlignVCenter, vName);

        // Draw var value
        QRectF vValueRect(adjustedRect.topLeft() + QPoint(nameWidth, 0),
            QSizeF(adjustedRect.width() - nameWidth, adjustedRect.height()));
        if (QColor::isValidColor(vValue)) {
            painter->save();

            // Draw a rectangle with the color specified by the var value
            QRectF colorRect(vValueRect.topLeft() + QPoint(1, 2),
                QSize(option.fontMetrics.boundingRect(vValue).width() + 4,
                    vValueRect.height() - 4));
            // Draw a rect border with text color
            QColor bgColor(vValue);
            painter->drawRect(colorRect);
            painter->fillRect(colorRect.adjusted(1, 1, 0, 0), QBrush(bgColor));

            // Draw the value text inside the colorRect
            QColor valueTxtColor;
            valueTxtColor.setRgb(
                bgColor.lightnessF() < 0.5 ? qRgb(1, 1, 1) : qRgb(0, 0, 0));
            painter->setPen(QPen(valueTxtColor));
            painter->drawText(colorRect, Qt::AlignVCenter, vValue);

            painter->restore();
        } else {
            painter->drawText(
                vValueRect, Qt::AlignLeft | Qt::AlignVCenter, vValue);
        }

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
        + QSize(12, 12);
}
