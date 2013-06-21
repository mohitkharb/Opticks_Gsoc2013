#ifndef MAINWINDOW_CD_H
#define MAINWINDOW_CD_H

#include <QDialog>
//#include "codes.h"
#include<vector>
#include "selectcdtype.h"
//#include "attributebased.h"
//#include "paint_box.h"

namespace Ui {
    class MainWindow_cd;
}

class MainWindow_cd : public QDialog
{
    Q_OBJECT

public:
	 QString fileName1;
    QString fileName2;
    std::string model;
    std::string tmp;
    QString fileName;
    QString img1_name;
	QString img2_name;
    QString segName;
    QString ms_dir;
	QString data_dir;
	QString temp_dir;
	QString out_dir;

    int imgsize[2];
    int NO_I;
    float LOW,HIGH;
    int **merge_map,*classes,**final_codes;
    std::vector< std::vector< float > >pieceInfo;
    std::vector< std::vector< int > >object_info_intersect;
    //individual variables
    int ***data1,***data2;//data1 :img1.ppm, data2 : img2.ppm
    int NO1,NO2;
    int **labels1;
    int **labels2;
    std::vector< std::vector< int > >object_info1;
    std::vector< std::vector< int > >object_info2;
    Image i1;
    Image i2;
    int num_band;
	selectCDType *w;
 //   nclasses *ncls;
//    std::map<int,std::vector<int> >class_m_1;
 //   std::map<int,std::vector<int> >class_m_2;
 //   void insertvalue(int n,int x, int y, int **labels, int img);
    void ChangeDetection(int flag);

//	void classificationImage();
//	float CalcDeterminant( float **mat, int order);
//    void GetMinor(float **src, float **&dest, int row, int col, int order);
//    void MatrixInversion(float **A, int order, float **&Y);
//    float **getCovarianceMartix(int numberOfSamples, float **dataArray, float **meanVector, int classNumber, int ndims);
//    float calculateMahaDist(float *sample, float **class_mean, float **full_cov_mat, int class_number, int ndims);
	

    explicit MainWindow_cd(QWidget *parent = 0);
    ~MainWindow_cd();
//	void Ras2Vect();
//	void insertvalue(int n,int x, int y, int **labels);
//	std::map<int,std::vector<int> >class_m;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
	void on_pushButton_4_clicked();
	void on_pushButton_5_clicked();
	void on_pushButton_clicked();

   
private:
    Ui::MainWindow_cd *ui;
	void Show_process(std::string model);

};

#endif // MAINWINDOW_H
