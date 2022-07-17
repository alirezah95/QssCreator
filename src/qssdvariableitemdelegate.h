#ifndef QSSDVARIABLEITEMDELEGATE_H
#define QSSDVARIABLEITEMDELEGATE_H

#include <QStyledItemDelegate>

class QLabel;

class QssdVariableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit QssdVariableItemDelegate(QObject* parent = nullptr);
    ~QssdVariableItemDelegate();

    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

    virtual void setModelData(QWidget* editor, QAbstractItemModel* model,
        const QModelIndex& index) const override;

    virtual QSize sizeHint(const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

private:
    QLabel* lbl;
};

#endif // QSSDVARIABLEITEMDELEGATE_H
