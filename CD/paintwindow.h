#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QWidget>



class paintwindow : public QWidget
{
    Q_OBJECT

public:
    explicit paintwindow(QString img2, QWidget *parent = 0);
    ~paintwindow();

private:
    QString imgname;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // PAINTWINDOW_H
