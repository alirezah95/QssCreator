#include "widgetspreview.h"
#include "ui_widgetspreview.h"

WidgetsPreview::WidgetsPreview(QWidget* parent)
    : QWidget { parent }, ui(new Ui::WidgetsPreview)
{
    ui->setupUi(this);
    return;
}

WidgetsPreview::~WidgetsPreview()
{
    delete ui;
}
