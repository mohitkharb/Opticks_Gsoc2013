#include "scarea.h"
#include <QScrollBar>
#include <QImage>

scarea::scarea( paint_box *pbox,/* nclasses *tp,*/ MainWindow *tp, QWidget *parent):
    QScrollArea(parent)
  ,tempParent(tp)
{
    setWidget(pbox);

    resize(800,600);

    scrollBarWidgets(Qt::AlignCenter);

}

