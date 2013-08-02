#ifndef CHOOSESAMPLE_H
#define CHOOSESAMPLE_H

#include <QWidget>

namespace Ui {
class chooseSample;
}

class chooseSample : public QWidget
{
    Q_OBJECT
    
public:
    explicit chooseSample(int ncls,int x, int y, int **labels1,int img, QWidget *tempParent,QWidget *parent = 0);
    ~chooseSample();
    int **labels1_;
    int ncls_;
    int Img;
    int cls_no;
    int X;
    int Y;
    int temp;
    QWidget *tempParent;

    QStringList classes;
    
private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::chooseSample *ui;
};

#endif // CHOOSESAMPLE_H
