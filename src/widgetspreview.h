#ifndef WIDGETSPREVIEW_H
#define WIDGETSPREVIEW_H

#include <QWidget>

namespace Ui {
class WidgetsPreview;
}

/*!
 * \brief The WidgetsPreview class which is a collection of all the widgets
 * (almost) to be used as real-time preview for stylesheet
 */
class WidgetsPreview : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetsPreview(QWidget* parent = nullptr);
    ~WidgetsPreview();

    /*!
     * \brief Reimplements \a\b QWidget::minimumSizeHint() to return minimum
     * size required for preview widget
     * \return
     */
    virtual QSize minimumSizeHint() const override { return QSize(300, 400); }

private:
    Ui::WidgetsPreview* ui;
};

#endif // WIDGETSPREVIEW_H
