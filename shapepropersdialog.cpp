#include "shapepropersdialog.h"
#include "ui_shapepropersdialog.h"

#include <QAction>
#include "shapepropmodel.h"

ShapePropersDialog::ShapePropersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShapePropersDialog)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene());
    ui->graphicsView->scene()->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    ui->tableView->setShowGrid(true);
}

ShapePropersDialog::~ShapePropersDialog()
{
    if(ui->graphicsView->scene()) delete ui->graphicsView->scene();
    delete ui;
}
void ShapePropersDialog::setGraphicsObject(GraphicsShapeItem *shape)
{
    ui->graphicsView->scene()->addItem(shape->copy());
    QList<QString> prop;
    QList<QList<QString>> table;

    prop.push_back("Name");
    prop.push_back(shape->name());
    table.push_back(prop);
    prop.clear();
    prop.push_back("Area");
    prop.push_back(QString("%1").arg(shape->area()));
    table.push_back(prop);
    prop.clear();
    prop.push_back("Color");
    prop.push_back(shape->brush().color().name());
    table.push_back(prop);

    ShapePropModel *model = new ShapePropModel(table);
    ui->tableView->setModel(model);
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:gray }");

    ui->tableView->update();
    ui->tableView->show();
}
