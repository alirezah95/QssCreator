#ifndef WIDGETSPREVIEW_H
#define WIDGETSPREVIEW_H

#include <QWidget>

namespace Ui {
class WidgetsPreview;
}

class WidgetsPreview : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetsPreview(QWidget* parent = nullptr);
    ~WidgetsPreview();

private:
    Ui::WidgetsPreview* ui;
};

#endif // WIDGETSPREVIEW_H
