#ifndef PAINTBOX_H
#define PAINTBOX_H

#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include <iostream>
class paintbox : public QWidget
{
    Q_OBJECT
public:
    explicit paintbox(std::string fname,int row,int col,QWidget *parent = 0);
    QImage  *qimg;
    std::string fname_;
    int X,Y;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent
                           *event);

};

#endif // PAINTBOX_H
