#ifndef ATTRIBUTEBASED_CD_H
#define ATTRIBUTEBASED_CD_H

#include <QWidget>
#include "codes.h"
#include "pain2img.h"

namespace Ui {
class AttributeBased_cd;
}

class AttributeBased_cd : public QWidget
{
    Q_OBJECT
    
public:
    explicit AttributeBased_cd(int row,int col,int **final_codes,int **labels1,int **labels2,int ***data1,int ***data2,Image i1,Image i2,int num_band,QWidget *parent = 0);
    ~AttributeBased_cd();

    int rmaxArea_, rminArea_, rminMeanRed_, rmaxMeanRed_,rminMeanGreen_,rmaxMeanGreen_,rmaxMeanBlue_,rminMeanBlue_,rmaxPerimeter_,rminPerimeter_;
    float rmaxRoundness_,rminRoundness_,rmaxCompactness_,rminCompactness_,rmaxStdRed_,rminStdRed_,rmaxStdGreen_,rminStdGreen_,rmaxStdBlue_,rminStdBlue_;
    float rminContrast_,rmaxContrast_,rminCoarseness_,rmaxCoarseness_,rmaxDirection_,rminDirection_,rminRoughness_,rmaxRoughness_;
    int rmax_,rmin_;
    std::string attribute;
    int att,num_band;
    int flag1,flag2;
    int flag;
    std::vector<int> objs;
    std::vector<int> pixels;
    std::vector<int> pixelsnew;
    std::vector<int> objsnew;

    
    int maxMeanRed, maxMeanGreen, maxMeanBlue;
    int minMeanRed, minMeanGreen, minMeanBlue;
    float maxStdRed,maxStdGreen,maxStdBlue;
    float minStdRed,minStdGreen,minStdBlue;
    int maxArea,minArea,maxPerimeter,minPerimeter;
    float maxRoundness,minRoundness,maxCompactness,minCompactness;
    float maxRoughness,minRoughness,maxCoarseness,minCoarseness;
    float maxContrast,minContrast,maxDirection,minDirection;


    int row,col;
    int **final_codes;
    int ***data1,***data2;//data1 :img1.ppm, data2 : img2.ppm
    int **labels1;
    int **labels2;
    Image i1;
    Image i2;
private slots:
    void on_lineEdit_2_returnPressed();

    void on_lineEdit_returnPressed();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_3_returnPressed();

    void on_lineEdit_4_returnPressed();

    void on_lineEdit_5_returnPressed();

    void on_lineEdit_6_returnPressed();

    void on_radioButton_6_clicked();

    void on_lineEdit_7_returnPressed();

    void on_lineEdit_8_returnPressed();

    void on_radioButton_7_clicked();

    void on_radioButton_8_clicked();

    void on_lineEdit_9_returnPressed();

    void on_lineEdit_10_returnPressed();

    void on_lineEdit_11_returnPressed();

    void on_lineEdit_12_returnPressed();

    void on_radioButton_9_clicked();

    void on_radioButton_10_clicked();

    void on_radioButton_11_clicked();

    void on_lineEdit_13_returnPressed();

    void on_lineEdit_14_returnPressed();

    void on_lineEdit_15_returnPressed();

    void on_lineEdit_16_returnPressed();

    void on_lineEdit_17_returnPressed();

    void on_lineEdit_18_returnPressed();

    void on_lineEdit_19_returnPressed();

    void on_lineEdit_20_returnPressed();

    void on_radioButton_12_clicked();

    void on_radioButton_13_clicked();

    void on_radioButton_14_clicked();

    void on_radioButton_15_clicked();

    void on_radioButton_16_clicked();

    void on_lineEdit_21_returnPressed();

    void on_lineEdit_22_returnPressed();

    void on_lineEdit_23_returnPressed();

    void on_lineEdit_24_returnPressed();

    void on_lineEdit_25_returnPressed();

    void on_lineEdit_26_returnPressed();

    void on_lineEdit_27_returnPressed();

    void on_lineEdit_28_returnPressed();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    

   
private:
    Ui::AttributeBased_cd *ui;
};

#endif // ATTRIBUTEBASED_H
