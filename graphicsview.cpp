#include "graphicsview.h"
#include <QDebug>
#include <QMouseEvent>
#include <QSharedPointer>

#include "gview.h"
#include "shapepropersdialog.h"

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
}
void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    GraphicsShapeItem *hovered_item = nullptr;
    for(QGraphicsItem *item : this->scene()->items())
    {
        GraphicsShapeItem *tmp = (GraphicsShapeItem *)item;
        if(tmp->hover()) {
            hovered_item = tmp;
            break;
        }
    }
    if(hovered_item){
       ShapePropersDialog *dialog = new ShapePropersDialog(this);
       //GraphicsShapeItem tmp = new GraphicsShapeItem(*hovered_item);
       dialog->setGraphicsObject(hovered_item);
       dialog->show();
    }
}
