#ifndef PAINTBOX2_H
#define PAINTBOX2_H
#include <QWidget>
#include "paintbox.h"
class Paintbox2: public QWidget
{
    Q_OBJECT
public:
    explicit Paintbox2(int v,std::vector<int> ts2,int ***filedata,int row,int col,char *str,int num_band,QWidget *parent = 0);
    ~Paintbox2();
    std::vector<int> ts2_;
    int V_;
    int *** filedata;
    int row,col,num_band;


signals:

public slots:
protected:
    void paintEvent(QPaintEvent *);

};

#endif // PAINTBOX2_H
