#ifndef CLASSBASED_H
#define CLASSBASED_H

#include <QWidget>
#include <iostream>
#include "paintbox.h"
#include "paintbox2.h"
namespace Ui {
class ClassBased;
}

class ClassBased : public QWidget
{
    Q_OBJECT
    
public:
    explicit ClassBased(int row,int col,int **final_codes,int **labels1,int **labels2,int ***data1,int ***data2,int num_band,QWidget *parent = 0);
    ~ClassBased();
    std::vector<int> ts2;
    std::vector<int> ts1;
    int flag2,flag1,num_band;

    int row,col;
    int **final_codes;
    int ***data1,***data2;//data1 :img1.ppm, data2 : img2.ppm
    int **labels1;
    int **labels2;

    paintbox *pbox;
    void createImage(std::string fname);
    void writePPMImage(int ***op,int img1_size[],std::string fname);
private slots:
    bool isSelected(int px);

    void on_checkBox_clicked();
    void on_pushButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_5_clicked();

    void on_checkBox_6_clicked();

    void on_checkBox_8_clicked();

    void on_checkBox_7_clicked();

    void on_checkBox_9_clicked();

private:
    Ui::ClassBased *ui;
    int a1,a2,a3,a4,a5,a6,a7,a8,a9;
};

#endif // CLASSBASED_H
