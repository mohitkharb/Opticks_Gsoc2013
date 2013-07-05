#include <QPainter>
#include <QPaintDevice>
#include <QDebug>
#include <QPixmap>
#include <iostream>
#include <QFile>
#include <fstream>
#include "pain2img.h"

pain2img::pain2img(std::vector<int> ts2,int ***data,int row,int col,int band,char *str, QWidget *parent) :
    pix_(ts2),row(row),col(col),data(data),band(band),
    QWidget(parent)
{
     resize(col,row);
     this->setWindowTitle(str);
}
pain2img::~pain2img(){
    free(data);
    pix_.clear();
}

void pain2img :: paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(band==3){
    QImage *image = new QImage(col, row, QImage::Format_RGB32);
     QRgb value;


     int index=0;
     for(std::vector<int>::iterator it = pix_.begin();it!=pix_.end()-1;++it)
     {
        int x1 = *it % col;
        int y1 = *it /col;
        int red,green,blue;


               red = data[y1][x1][0];
               green = data[y1][x1][1];
               blue = data[y1][x1][2];



               value = qRgb(red, green, blue);
               image->setPixel(x1, y1, value);

     }
     painter.drawImage(0,0,*image);
    }
    else{
        QImage *image = new QImage(col,row, QImage::Format_Indexed8);
        for(int i=0;i<256;i++) image->setColor(i,qRgb(i,i,i));



         for(std::vector<int>::iterator it = pix_.begin();it!=pix_.end()-1;++it)
         {
                int x1 = *it % col;
                int y1 = *it /col;
                    int red;


                            red = data[y1][x1][0];
                            image->setPixel(x1,y1,red);



         }
         painter.drawImage(0,0,*image);

    }

}
