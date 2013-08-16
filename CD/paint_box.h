#ifndef paint_box_H
#define paint_box_H

#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include "choosesample.h"
#include <iostream>
#include <QMainWindow>
#include "mainwindow_fusion.h"

class MainWindow_fusion;
class paint_box : public QWidget
{
    Q_OBJECT
public:
    explicit paint_box(std::string fname,int row,int col,int cls,int **labels1,int img, MainWindow_fusion *tempParent,QWidget *parent = 0);
    explicit paint_box(std::string fname,std::string title, int row,int col,QWidget *parent=0);
    QImage  *qimg;
    void fillsample(int n);
    std::string fname_;
    int **labels1_;
    int imgNo;
    int ME;
    int X,Y,ncls,class_index;
    chooseSample *choosesample ;

    MainWindow_fusion *tempParent ;

//    bool fusedImageShow;


signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent
                           *event);

};

#endif // paint_box_H
