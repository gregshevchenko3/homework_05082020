#ifndef GVIEW_H
#define GVIEW_H

#include <QAbstractGraphicsShapeItem>
#include <QPen>
#include <QBrush>


class Shape : public QObject{
    Q_OBJECT
    QString m_name;
public:
    virtual qreal area() const  = 0;
    virtual bool isValid() const = 0;
    QString name() const;

    Shape(QString name);
    Shape(const Shape& other);
    virtual ~Shape();
signals:
    void hoverEnter(Shape* shape);
    void hoverLeave(Shape* shape);
};

class GraphicsShapeItem : public QAbstractGraphicsShapeItem, public Shape
{
    bool m_hover;
    qreal m_savedZ;
public:
    GraphicsShapeItem(QString name, QPen pen = QPen(), QBrush brush = QBrush(), QGraphicsItem* parent = nullptr);
    GraphicsShapeItem(const GraphicsShapeItem &other);
    virtual ~GraphicsShapeItem();

    virtual GraphicsShapeItem* copy() const = 0;
    bool hover() const;
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

class GraphicsPolygonItem : public GraphicsShapeItem {
    QPolygonF m_polygon;
public:
    GraphicsPolygonItem(QString name, QPen pen = QPen(), QBrush brush = QBrush(), QGraphicsItem* parent = nullptr);
    GraphicsPolygonItem(const GraphicsPolygonItem& other);
    virtual ~GraphicsPolygonItem();

    QPolygonF& polygon();
    void setPolygon(QPolygonF& polygon);
    bool contains(const QPointF &point) const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};

class Triangle : public GraphicsPolygonItem
{
    qreal m_a, m_b, m_c;
public:
    qreal area() const;
    qreal length_of_hypotenuse() const;
    bool isValid() const;

    GraphicsShapeItem* copy() const;

    Triangle(qreal a = 0, qreal b = 0, qreal c = 0, QPen pen = QPen(), QBrush brush = QBrush(), QGraphicsItem* parent = nullptr);
    Triangle(qreal length_of_sides[3], QPen pen = QPen(), QBrush brush = QBrush(), QGraphicsItem* parent = nullptr);
    Triangle(const Triangle& other);
    ~Triangle();
};

class Square : public GraphicsShapeItem, QRectF
{
public:
    qreal area() const;
    qreal side() const;
    bool isValid() const;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    GraphicsShapeItem* copy() const;

    Square(qreal a, QPen pen = QPen(), QBrush brush = QBrush(), QGraphicsItem *parent = nullptr);
    Square(const Square& other);
    ~Square();
};

class Trapezoid : public GraphicsPolygonItem
{
    qreal m_AB, m_CD, m_AD, m_BC;
public:
    qreal area() const;
    qreal length_of_midline() const;
    qreal length_of_height() const;
    bool isValid() const;

    GraphicsShapeItem* copy() const;

    Trapezoid(qreal AB, qreal CD, qreal AD, qreal BC, QPen pen = QPen(), QBrush brush = QBrush(), QGraphicsItem *parent = nullptr);
    Trapezoid(const Trapezoid& other);
    ~Trapezoid();
};

class Circle : public GraphicsShapeItem, QRectF
{
public:
    qreal area() const;
    qreal radius() const;
    bool isValid() const;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    bool contains(const QPointF &point) const;

    GraphicsShapeItem* copy() const;

    Circle(qreal radius, QPen pen = QPen(), QBrush brush = QBrush(), QGraphicsItem* parent = nullptr);
    Circle(const Circle& other);
    ~Circle();
};

#endif // GVIEW_H
