#include "paint_box.h"
//#include "mainwindow.h"
#include <QPainter>
#include <QPaintDevice>
#include <QDebug>
#include <QPixmap>
#include <iostream>
#include <QFile>
#include <QLabel>
#include <QHBoxLayout>

paint_box::paint_box(std::string fname,int row,int col, int i,int **labels1,int img, MainWindow_fusion *tempParent ,QWidget *parent) :
    /*img_(img),*/fname_(fname),ncls(i),labels1_(labels1),imgNo(img),
    tempParent(tempParent),
    QWidget(parent)
{
    ME=1;
    resize(col,row);
    X=Y=0;

    this->setWindowTitle("PAN Image : Choose Samples");

    QImage img1 = QImage(fname.c_str());
    QLabel *myLabel = new QLabel("",this);
    myLabel->setPixmap(QPixmap::fromImage(img1));

    myLabel->resize(row,col);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(myLabel);
    //else
    //  this->setWindowTitle("Time 2 Image");

}
paint_box::paint_box(std::string fname,std::string title,int row, int col,QWidget *parent):fname_(fname),
    QWidget(parent)
{
    ME=0;
    resize(col,row);
    QImage img1 = QImage(fname.c_str());
    QLabel *myLabel = new QLabel("",this);
    myLabel->setPixmap(QPixmap::fromImage(img1));

    myLabel->resize(row,col);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(myLabel);
    //    this->setWindowTitle(title.c_str());
    //    this->showNormal();

}

void paint_box::paintEvent(QPaintEvent *event)
{

    //        QPainter painter(this);
    //        qimg = new QImage(fname_.c_str());
    //        painter.drawImage(0,0,*qimg);

}
void paint_box::fillsample(int n)
{
    std::cout << "abcd"<<std::endl;

}

void paint_box::mouseReleaseEvent(QMouseEvent *event)
{
    if(ME==1){
        int x = event->x();
        int y = event->y();


        X=x;
        Y=y;
        choosesample = new chooseSample(ncls,x,y,labels1_,imgNo,tempParent);
        choosesample->update();
        choosesample->show();

        class_index= choosesample->cls_no;
    }
}
