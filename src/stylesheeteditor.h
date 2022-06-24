#ifndef STYLESHEETEDITOR_H
#define STYLESHEETEDITOR_H

#include <QWidget>

namespace Ui {
class StyleSheetEditor;
}

class StyleSheetEditor : public QWidget
{
    Q_OBJECT

public:
    explicit StyleSheetEditor(QWidget* parent = nullptr);
    ~StyleSheetEditor();

private:
    Ui::StyleSheetEditor* ui;
};

#endif // STYLESHEETEDITOR_H
