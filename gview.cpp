#include "gview.h"

#include <QtMath>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QWidget>


Shape::Shape(QString name) : m_name(name){}
QString Shape::name() const
{
    return m_name;
}
Shape::~Shape(){}

GraphicsShapeItem::GraphicsShapeItem(QString name, QPen pen, QBrush brush, QGraphicsItem *parent) :
    QAbstractGraphicsShapeItem(parent), Shape(name), m_hover(false)
{
    setPen(pen);
    setBrush(brush);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::MouseButton::LeftButton | Qt::MouseButton::RightButton);
}
GraphicsShapeItem::~GraphicsShapeItem()
{}
bool GraphicsShapeItem::hover() const
{
    return m_hover;
}
void GraphicsShapeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(contains(event->pos()))
    {
        m_hover = true;
        emit hoverEnter(this);
    }
    else
    {
        event->ignore();
    }

}
void GraphicsShapeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    update();
    emit hoverLeave(this);
}
void GraphicsShapeItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(contains(event->pos()))
    {
        m_hover = true;
        emit hoverEnter(this);
    }
    else
    {
        if(m_hover) {
            m_hover = false;
            emit hoverLeave(this);
        } else
            event->ignore();
    }
}
void GraphicsShapeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_hover)
        moveBy(event->pos().rx() - event->lastPos().rx(), event->pos().ry() - event->lastPos().ry());
}
void GraphicsShapeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_hover)
    {
        setZValue(100);
    }
}
void GraphicsShapeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_hover)
    {
        setZValue(0);
    }
}

GraphicsPolygonItem::GraphicsPolygonItem(QString name, QPen pen, QBrush brush, QGraphicsItem *parent) : GraphicsShapeItem(name, pen, brush, parent), m_polygon()
{}
GraphicsPolygonItem::~GraphicsPolygonItem(){}
QPolygonF &GraphicsPolygonItem::polygon()
{
    return m_polygon;
}
void GraphicsPolygonItem::setPolygon(QPolygonF &polygon)
{
    m_polygon = polygon;
}
bool GraphicsPolygonItem::contains(const QPointF &point) const
{
    return m_polygon.containsPoint(point, Qt::OddEvenFill);
}
QRectF GraphicsPolygonItem::boundingRect() const
{
    QPointF left_top(0,0), bottom_right(0, 0);
    for(QPointF point : m_polygon)
    {
        if(point.rx() < left_top.rx())
            left_top.setX(point.rx());
        if(point.ry() < left_top.ry())
            left_top.setY(point.ry());

        if(point.rx() > bottom_right.rx())
            bottom_right.setX(point.rx());
        if(point.ry() > bottom_right.ry())
            bottom_right.setY(point.ry());
    }
    return QRectF(left_top, bottom_right);
}
void GraphicsPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPolygon(m_polygon);
}

qreal Triangle::area() const
{
    qreal cos = (qPow(m_a, 2) + qPow(m_b, 2) - qPow(m_c, 2)) / (2*m_a*m_b);
    qreal gama = qAcos(cos);
    return m_a*m_b*qSin(gama) / 2;
}
qreal Triangle::length_of_hypotenuse() const
{
    return m_c;
}
Triangle::Triangle(qreal a, qreal b, qreal c, QPen pen, QBrush brush, QGraphicsItem *parent) :
    GraphicsPolygonItem("Triangle", pen, brush, parent), m_a(a), m_b(b), m_c(c)
{
    if(isValid())
    {
        QPointF A(this->x(), this->y());
        QPointF B(A.x(), A.y()+m_a);
        qreal cos = (qPow(m_a, 2) + qPow(m_b, 2) - qPow(m_c, 2)) / (2*m_a*m_b);
        qreal gama = qAcos(cos);
        QPointF C(this->x() + m_b * qCos(M_PI_2 - gama), this->y() + m_b * qSin(M_PI_2 - gama));
        polygon() << A << B << C;
   }
}
Triangle::Triangle(qreal length_of_sides[], QPen pen, QBrush brush, QGraphicsItem* parent) :
    Triangle(length_of_sides[0], length_of_sides[1], length_of_sides[2], pen, brush, parent)
{}
Triangle::Triangle(const Triangle &other) : Triangle(other.m_a, other.m_b, other.m_c, other.pen(), other.brush())
{
}
Triangle::~Triangle(){}
bool Triangle::isValid() const
{
    qreal sides[] = {m_a, m_b, m_c};
    std::sort(sides, sides + 3);
    return qAbs((qPow(sides[0], 2) + qPow(sides[1], 2) - qPow(sides[2], 2)) / (2*sides[0]*sides[1])) <= 1;
}
GraphicsShapeItem *Triangle::copy() const
{
    Triangle *tr = new Triangle(*this);
    return tr;
}

qreal Square::area() const
{
    return width() * height();
}
qreal Square::side() const
{
    return width();
}
bool Square::isValid() const
{
    return true;
}
QRectF Square::boundingRect() const
{
    return *this;
}
void Square::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(*this);
}
GraphicsShapeItem *Square::copy() const
{
    Square *tmp = new Square(*this);
    return tmp;
}
Square::Square(qreal a, QPen pen, QBrush brush, QGraphicsItem* parent) :
    GraphicsShapeItem("Square", pen, brush, parent), QRectF(0, 0, a, a)
{}
Square::Square(const Square &other) : Square(other.width(), other.pen(), other.brush())
{
}
Square::~Square()
{}

qreal Trapezoid::area() const
{
    return length_of_height()*length_of_midline();
}
qreal Trapezoid::length_of_midline() const
{
   return (m_AB + m_CD)/2;
}
qreal Trapezoid::length_of_height() const
{
    qreal height = qSqrt((-m_AB + m_CD + m_AD + m_BC)*(m_AB - m_CD + m_AD + m_BC)*(m_AB - m_CD + m_AD - m_BC)*(m_AB - m_CD - m_AD + m_BC));
    height /= 2*qAbs(m_CD - m_AB);
    return height;
}
bool Trapezoid::isValid() const
{
    bool non_zero = m_AB && m_BC && m_CD && m_AD && m_CD != m_AB;
    qreal height= length_of_height();
    return (qAbs(height / m_AD) <= 1 && qAbs(height / m_BC) <= 1 && non_zero);
}
GraphicsShapeItem *Trapezoid::copy() const
{
    return new Trapezoid(*this);
}
Trapezoid::Trapezoid(qreal AB, qreal CD, qreal AD, qreal BC, QPen pen, QBrush brush, QGraphicsItem* parent) :
    GraphicsPolygonItem("Trapezoid", pen, brush, parent), m_AB(AB), m_CD(CD), m_AD(AD), m_BC(BC)
{
    if(isValid()) {
        QPointF D(0, 0);

        // The height of the trapezoid
        qreal height = length_of_height();

        // The angle between the base AB and the left lateral side AD
        qreal alpha = qAsin(height / m_AD);

        QPointF A(D.x() - m_AD * qCos(alpha), D.y() - height);
        QPointF B(A.x() + m_AB, A.y());
        QPointF C(D.x() + m_CD, D.y());
        polygon() << A << B << C << D << A;
    }
}
Trapezoid::Trapezoid(const Trapezoid &other) :
    Trapezoid(other.m_AB, other.m_CD, other.m_AD, other.m_BC, other.pen(), other.brush())
{}
Trapezoid::~Trapezoid(){}


qreal Circle::area() const
{
    return M_2_PI * width() * width();
}
qreal Circle::radius() const
{
    return width()/2;
}
bool Circle::isValid() const
{
    return true;
}
QRectF Circle::boundingRect() const
{
    return *this;
}
void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    painter->drawEllipse(*(QRectF*)this);
}
bool Circle::contains(const QPointF &point) const
{
    QPointF center(QRectF::x() + width()/2, QRectF::y() + height()/2);
    return qPow(point.x() - center.rx(), 2) + qPow(point.y() - center.ry(), 2) <= qPow(width()/2, 2);
}
GraphicsShapeItem *Circle::copy() const
{
    return new Circle(*this);
}
Circle::Circle(qreal radius, QPen pen, QBrush brush, QGraphicsItem* parent) :
    GraphicsShapeItem("Circle", pen, brush, parent), QRectF(-radius, -radius, 2*radius, 2*radius){}
Circle::Circle(const Circle &other) :
    Circle(other.width()/2, other.pen(), other.brush())
{
}
Circle::~Circle(){}
