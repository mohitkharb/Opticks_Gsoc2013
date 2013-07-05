#include "paintbox.h"
//#include "mainwindow.h"
#include <QPainter>
#include <QPaintDevice>
#include <QDebug>
#include <QPixmap>
#include <iostream>
#include <QFile>
paintbox::paintbox(std::string fname,int row,int col, QWidget *parent) :fname_(fname),
    QWidget(parent)
{
    resize(col,row);
    X=Y=0;
    this->setWindowTitle("Change Image");
}
void paintbox::paintEvent(QPaintEvent *event)
{


    QPainter painter(this);
    qimg = new QImage(fname_.c_str());
    painter.drawImage(0,0,*qimg);

}


void paintbox::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();


    X=x;
    Y=y;
    
    


}
