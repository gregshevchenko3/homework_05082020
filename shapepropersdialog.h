#ifndef SHAPEPROPERSDIALOG_H
#define SHAPEPROPERSDIALOG_H

#include <QDialog>
#include "gview.h"

namespace Ui {
class ShapePropersDialog;
}

class ShapePropersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShapePropersDialog(QWidget *parent = nullptr);
    ~ShapePropersDialog();
    void setGraphicsObject(GraphicsShapeItem *shape);

private:
    Ui::ShapePropersDialog *ui;
};

#endif // SHAPEPROPERSDIALOG_H
