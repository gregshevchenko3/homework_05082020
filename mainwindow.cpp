#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QTime>
#include <QtMath>
#include <QAbstractGraphicsShapeItem>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_generator(QTime::currentTime().second())
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene);
    ui->graphicsView->scene()->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->horizontalSlider->setValue(10);
    ui->lineEdit->setValidator(new QIntValidator(0, 100));

    connect(ui->horizontalSlider, &QSlider::valueChanged, [=](int value){
       ui->lineEdit->setText(QString("%1").arg(value));
    });
    connect(ui->lineEdit, &QLineEdit::textChanged, [=](QString value)
    {
        bool ok;
        int v = value.toInt(&ok);
        if(v >= 100) v = 99;
        ui->horizontalSlider->setValue(v);

    });
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::draw);
}

MainWindow::~MainWindow()
{
    for(QGraphicsItem *item : ui->graphicsView->scene()->items())
    {
        delete item;
    }
    if(ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    delete ui;
}

void MainWindow::draw(int newValue)
{
    for(QGraphicsItem *item : ui->graphicsView->scene()->items())
    {
        delete item;
    }
    ui->graphicsView->scene()->clear();
    int bound = ui->graphicsView->height()/2;
    QAbstractGraphicsShapeItem *item;
    while(newValue-- > 0)
    {
        int shape = m_generator.bounded(shapes::trapezoid + 1);
        do {
            Shape* s = nullptr;
            switch(shape)
            {
                case shapes::triangle:
                    item = new Triangle(m_generator.bounded(bound), m_generator.bounded(bound), m_generator.bounded(bound));
                    s = (Triangle*)item;
                    break;
                case shapes::circle:
                    item = new Circle(m_generator.bounded(bound));
                    s = (Circle*)item;
                    break;
                case shapes::square:
                    item = new Square(m_generator.bounded(bound));
                    s = (Square*)item;
                    break;
                case shapes::trapezoid:
                    item = new Trapezoid(m_generator.bounded(bound), m_generator.bounded(bound), m_generator.bounded(bound), m_generator.bounded(bound));
                    s = (Trapezoid*)item;
                    break;
            }
            if(!s->isValid()) {
                delete item;
                item = nullptr;
            } else {
                connect(s, &Shape::hoverEnter, [=](Shape* s){
                   QString name = "Shape: %1, area = %2 squre units,";
                   name = name.arg(s->name()).arg(s->area());
                   name += this->message_with_extra(s);
                   ui->statusbar->showMessage(name);
                });
                connect(s, &Shape::hoverLeave, ui->statusbar, &QStatusBar::clearMessage);
            }

        } while(!item);
        Qt::GlobalColor color = Qt::GlobalColor(m_generator.bounded(Qt::black, Qt::darkYellow));
        item->setPen(QPen(Qt::black));
        item->setBrush(QBrush(color));
        item->setPos(m_generator.bounded(-bound, bound), m_generator.bounded(-bound, bound));
        ui->graphicsView->scene()->addItem(item);
    }
}

QString MainWindow::message_with_extra(Shape* s)
{
    QString name = s->name();
    QString extra = "";
    QAbstractGraphicsShapeItem *item = nullptr;
    if(name == "Triangle")
    {
        extra = " hypotenuse = %1, color = %2, ";
        Triangle *tr = (Triangle*)s;
        item = tr;
        extra = extra.arg(tr->length_of_hypotenuse());
    } else if(name == "Square")
    {
        extra = " side = %1, color = %2, ";
        Square *rect = (Square*)s;
        item = rect;
        extra = extra.arg(rect->side());
    } else if(name == "Circle")
    {
        extra = " radius = %1, color = %2, ";
        Circle *c = (Circle*)s;
        item = c;
        extra = extra.arg(c->radius());
    }
    else if(name == "Trapezoid")
    {
        extra = " midline = %1, height = %2, color = %3, ";
        Trapezoid *c = (Trapezoid*)s;
        item = c;
        extra = extra.arg(c->length_of_midline()).arg(c->length_of_height());
    }
    return extra.arg(item->brush().color().name());
}
