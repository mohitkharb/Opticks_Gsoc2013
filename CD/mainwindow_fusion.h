#ifndef MAINWINDOWF_H
#define MAINWINDOWF_H


#include <QDialog>
#include <vector>
#include<queue>
#include"codes.h"
#include "classifyattributes.h"
#include"nclasses.h"


class paint_box;
class ClassifyAttributes;

namespace Ui {
	class MainWindow_fusion;
}

class MainWindow_fusion : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow_fusion(QWidget *parent = 0);
    ~MainWindow_fusion();
    std::string model;
    QString fileName;
    QString fileName1;
    int imgsize[2];//imgsize[0] = rows(529) [1]=cols(940)
    unsigned char ***data1;//data1 :img1.ppm, data2 : img2.ppm
    unsigned char ***data2;//data1 :img1.ppm, data2 : img2.ppm
    unsigned char ***dataf; // fusion image
    unsigned char ***datasf;
    int glsiclicked;
    bool postprocessing;
    int numberOfClasses;

	 paint_box *fusedimg;
    paint_box *classifiedimg;
    ClassifyAttributes *cAtt;


    QString ms_dir;
QString data_dir;
	QString temp_dir;
	QString out_dir;

     QString segName;
     QString img_name;
      int **merge_map;
      char ***blackPixelFlag;
    std::vector< std::vector< int > >object_info1;
    std::vector< std::vector< int > >object_info_fusion;
    int **labels1;
    int **labelsFusion;
    int NO1;


	struct Point{
        int x;
        int y;
        int clusterId;
    };
    struct Cluster{
        double r,g,b;
    };

    int NOF;
    std::map<int,std::vector<int> >class_m_1;
    Image i1;
    Image iFused;
    void insertvalue(int n,int x, int y, int **labels, int img);
    void classification();
    float CalcDeterminant( float **mat, int order);
    void GetMinor(float **src, float **&dest, int row, int col, int order);
    void MatrixInversion(float **A, int order, float **&Y);
    float **getCovarianceMartix(int numberOfSamples, float **dataArray, float **meanVector, int classNumber, int ndims);
    float calculateMahaDist(float *sample, float **class_mean, float **full_cov_mat, int class_number, int ndims);


	 void Fusion_Brovey(int **segmap, unsigned char ***&fimage, unsigned char ***msimage, unsigned char ***panimage, int onum, int xmin, int xmax, int ymin, int ymax,int bandnum);
    void Fusion_GLSI(int **segmap, unsigned char ***&fimage, unsigned char ***msimage, unsigned char ***panimage, int onum, int xmin, int xmax, int ymin, int ymax,int bandnum);
    void writePPMImage(unsigned char ***op,int img1_size[],std::string fname);
    void PrepareStatistics(int **segmap, unsigned char ***msimage, unsigned char ***panimage,int onum,int xmin,int xmax,int ymin,int ymax,int bandnum,float &msmean,float &msstd,float &panmean,float &panstd,int &panmin,int &panmax, int &msmin, int &msmax);
    void GetHistogram (std::vector<unsigned char> data, std::vector<unsigned char> &val, std::vector<int> &cnt);
    void GetStats(int **segmap, std::vector<unsigned char> data,int onum,float &mean,float &std1, int &min, int &max,int xmin, int xmax, int ymin, int ymax, int nop);
    void FillBlackPixels(unsigned char ***&fimage,int rows,int cols,int **&segmap,int b);
    void Smoothing_GLSI(unsigned char ***&fimage,int **segmap,int b,unsigned char ***&sfimage,int rows,int cols);
     

private slots:
     void on_Browse_4_clicked();

     void on_Browse_2_clicked();

     void on_pushButton_dir_clicked();

     void on_pushButton_ms_clicked();

     void on_pushButton_clicked();
	 void on_radioButton_clicked();
	 void on_radioButton_2_clicked();
	 void on_checkBox_clicked();
	 void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_radioButton_clicked(bool checked);

    void on_pushButton_6_clicked();


private:
    Ui::MainWindow_fusion *ui;
    void Show_process(std::string model);
};

#endif // MAINWINDOWF_H
