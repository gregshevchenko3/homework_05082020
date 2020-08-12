#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget* parent = nullptr);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // GRAPHICSVIEW_H
