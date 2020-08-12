#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include "gview.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    using shapes = enum {
        circle, triangle, square, trapezoid,
    };
    QString message_with_extra(Shape* s);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void draw(int newValue);
private:
    Ui::MainWindow *ui;
    QRandomGenerator m_generator;
};
#endif // MAINWINDOW_H
