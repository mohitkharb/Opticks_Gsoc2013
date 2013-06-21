#ifndef SELECTCDTYPE_H
#define SELECTCDTYPE_H

#include <QWidget>
//#include "codes.h"
#include "attributebased_cd.h"
#include "classbased.h"

namespace Ui {
class selectCDType;
}

class selectCDType : public QWidget
{
    Q_OBJECT
    
public:
    explicit selectCDType(int row,int col,int **final_codes,int **labels1,int **labels2,int ***data1,int ***data2,Image i1,Image i2,int num_band,QWidget *parent = 0);
    ~selectCDType();
    int row,col;
    int num_band;
    int Flag;

    int **final_codes;
    int ***data1,***data2;//data1 :img1.ppm, data2 : img2.ppm
    int **labels1;
    int **labels2;
    Image i1;
    Image i2;
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::selectCDType *ui;
};

#endif // SELECTCDTYPE_H
