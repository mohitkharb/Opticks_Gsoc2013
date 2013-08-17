#include "paintwindow.h"
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

paintwindow::paintwindow(QString img2, QWidget *parent) :
    QWidget(parent),
    imgname(img2)
{
	this->setWindowTitle("Image");
   update();
}

paintwindow::~paintwindow()
{
	delete(this);
}
void paintwindow::paintEvent(QPaintEvent *event)
{

    QPainter p(this);
    QImage qimage = QImage(imgname);

    p.drawImage(0,0,qimage);

 //   p.drawImage(0,0,qimage);
}
