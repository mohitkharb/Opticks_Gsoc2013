#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
//#include "codes.h"
#include "attributebased.h"
#include "paint_box.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
	QString fileName1;
    std::string model;
    std::string tmp;
    QString fileName;
    QString img_name;
    QString segName;
    QString ms_dir;
	QString data_dir;
	QString temp_dir;
	QString out_dir;
	QString tempimg1;

    int imgsize[2];// 0: rows, 1: cols
    int NO_I;
    float LOW,HIGH;//
    int **merge_map,*classes,**final_codes;
    //individual variables
    int ***data1;//data1 :img1.ppm,
    int NO1;
    int **labels1;
	std::vector<double> dataR;
	std::vector<double> dataG;
	std::vector<double> dataB;
	int Rows;
	int Cols;
    std::vector< std::vector< int > >object_info1;

    Image i1;
	paint_box *pbox;

    int num_band;

	void classificationImage();
	float CalcDeterminant( float **mat, int order);
    void GetMinor(float **src, float **&dest, int row, int col, int order);
    void MatrixInversion(float **A, int order, float **&Y);
    float **getCovarianceMartix(int numberOfSamples, float **dataArray, float **meanVector, int classNumber, int ndims);
    float calculateMahaDist(float *sample, float **class_mean, float **full_cov_mat, int class_number, int ndims);
	

    explicit MainWindow(int rows, int cols, std::vector<double> dataR, std::vector<double> dataG, std::vector<double> dataB,int bandnum, QWidget *parent = 0);
    ~MainWindow();
	void Ras2Vect();
	void insertvalue(int n,int x, int y, int **labels);
	std::map<int,std::vector<int> >class_m;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
	void on_pushButton_4_clicked();
	void on_pushButton_5_clicked();
	void on_pushButton_6_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
	void Show_process(std::string model);

};

#endif // MAINWINDOW_H
