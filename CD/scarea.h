#ifndef SCAREA_H
#define SCAREA_H
#include "mainwindow_fusion.h"
#include "paint_box.h"
#include <QScrollArea>
#include "nclasses.h"

class paint_box;
class MainWindow_fusion;
class nclasses;
class scarea : public QScrollArea
{
    Q_OBJECT
public:
    explicit scarea( paint_box *pbox,/* nclasses *tp,*/ MainWindow_fusion *tp, QWidget *parent = 0);
    MainWindow_fusion *tempParent;

protected:


signals:

public slots:


};

#endif // SCAREA_H
