#ifndef paint_box_H
#define paint_box_H

#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include"choosesample.h"
#include <iostream>
class paint_box : public QWidget
{
    Q_OBJECT
public:
    explicit paint_box(std::string fname,int row,int col,int cls,int **labels1, QWidget *tempParent,QWidget *parent = 0);
    QImage  *qimg;
    void fillsample(int n);
    std::string fname_;
    int **labels1_;
    int imgNo;
    int X,Y,ncls,class_index;
    chooseSample *choosesample ;

    QWidget *tempParent ;


signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent
                           *event);

};

#endif // paint_box_H
