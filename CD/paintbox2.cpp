#include "paintbox2.h"
#include <QPainter>
#include <QPaintDevice>
#include <QDebug>
#include <QPixmap>
#include <iostream>
#include <QFile>
#include <fstream>

Paintbox2::Paintbox2(int v,std::vector<int> ts2,int *** data,int row,int col,char *str,int num_band,QWidget *parent) :
    V_(v),ts2_(ts2),filedata(data),row(row),col(col),num_band(num_band),
    QWidget(parent)
{
    resize(col,row);
    this->setWindowTitle(str);
}
Paintbox2::~Paintbox2()
{
    delete(this);
}

void Paintbox2::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    if(num_band==3){
        QImage *image = new QImage(col,row, QImage::Format_RGB32);
        QRgb value;




        int index = 0;



        int p = ts2_[V_];


        for(std::vector<int>::iterator it = ts2_.begin();it!=ts2_.end()-1;++it)
        {
            int x1 = index % col;
            int y1 = index /col;
            int tsv = *it;
            if(tsv == p)
            {

                int red,green,blue;


                red = filedata[y1][x1][0];
                green = filedata[y1][x1][1];
                blue = filedata[y1][x1][2];

                value = qRgb(red, green, blue);
                image->setPixel(x1, y1, value);


            }

            index++;
        }


        painter.drawImage(0,0,*image);
    }
    else{
        QImage *image = new QImage(col,row, QImage::Format_Indexed8);
        for(int i=0;i<256;i++) image->setColor(i,qRgb(i,i,i));

        int index = 0;
        int p = ts2_[V_];
        for(std::vector<int>::iterator it = ts2_.begin();it!=ts2_.end()-1;++it)
        {
            int x1 = index % col;
            int y1 = index /col;
            int tsv = *it;
            if(tsv == p)
            {

                int red;


                red = filedata[y1][x1][0];

                image->setPixel(x1,y1,red);


            }

            index++;
        }
        painter.drawImage(0,0,*image);
    }

}
