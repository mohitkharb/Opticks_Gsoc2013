#include "paint_box.h"
//#include "mainwindow.h"
#include <QPainter>
#include <QPaintDevice>
#include <QDebug>
#include <QPixmap>
#include <iostream>
#include <QFile>
paint_box::paint_box(std::string fname,int row,int col, int i,int **labels1, QWidget *tempParent ,QWidget *parent) :/*img_(img),*/fname_(fname),ncls(i),labels1_(labels1),tempParent
    (tempParent),
    QWidget(parent)
{
    resize(col,row);
    X=Y=0;
   
        this->setWindowTitle("Input Image");
   



}
void paint_box::paintEvent(QPaintEvent *event)
{


    QPainter painter(this);
    qimg = new QImage(fname_.c_str());
    painter.drawImage(0,0,*qimg);


}
void paint_box::fillsample(int n)
{
    std::cout << "abcd"<<std::endl;

}

void paint_box::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();


    X=x;
    Y=y;



    choosesample = new chooseSample(ncls,x,y,labels1_,tempParent);
    choosesample->update();
    choosesample->show();

    class_index= choosesample->cls_no;
}
