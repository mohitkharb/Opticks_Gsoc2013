#ifndef MAINWINDOWF_H
#define MAINWINDOWF_H


#include <QDialog>
#include <vector>
#include<queue>

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
    int ***data1;//data1 :img1.ppm, data2 : img2.ppm
    int ***data2;//data1 :img1.ppm, data2 : img2.ppm
    int ***dataf; // fusion image
    int ***datasf;
    int ***readPPMImage(int imgsize[],char *filename, int band);


    QString ms_dir;
QString data_dir;
	QString temp_dir;
	QString out_dir;

     QString segName;
     QString img_name;
      int **merge_map;
      std::vector< std::vector< int > >object_info1;
     int **labels1;//nseg_ts2_1
      int NO1;
     std::vector< std::vector< int > > prepareObjectInfo(int rows, int cols, int **labels_1, int **labels_2, int **&dmerge, int &NO);
     void fillRegionObjectInfo(int **dt1,int **dt2,int **&dmerge,int val1,int val2,int label,int bi,int bj,std::vector< int > &meta_obj, int rows, int cols);
     void fillRegionNewLabels(int **olinfo,int **nlinfo,int ol,int nl,int sx,int sy,int **mymap, int ROWS, int COLS);
     int **prepareNewLabels(char *seg_result_label_file,int rows, int cols);
     void Fusion_GLSI(int **segmap, int ***&fimage, int ***msimage, int ***panimage, int onum, int xmin, int xmax, int ymin, int ymax,int bandnum);
     void writePPMImage(int ***op,int img1_size[],std::string fname);
     void PrepareStatistics(int **segmap, int ***msimage, int ***panimage,int onum,int xmin,int xmax,int ymin,int ymax,int bandnum,float &msmean,float &msstd,float &panmean,float &panstd,int &panmin,int &panmax, int &msmin, int &msmax);
     //void PrepareStatistics(int **&segmap, int ***&msimage, int ***&panimage,int &onum,int &xmin,int &xmax,int &ymin,int &ymax,int &bandnum,float &msmean,float &msstd,float &panmean,float &panstd,int &panmin,int &panmax);
     void GetHistogram (std::vector<int> data, std::vector<int> &val, std::vector<int> &cnt);
     void GetStats(int **segmap, std::vector<int> data,int onum,float &mean,float &std1, int &min, int &max,int xmin, int xmax, int ymin, int ymax, int nop);
     void FillBlackPixels(int ***&fimage,int rows,int cols,int **&segmap,int b);
     

private slots:
     void on_Browse_4_clicked();

     void on_Browse_2_clicked();

     void on_pushButton_dir_clicked();

     void on_pushButton_ms_clicked();

     void on_pushButton_clicked();


private:
    Ui::MainWindow_fusion *ui;
    void Show_process(std::string model);
};

#endif // MAINWINDOWF_H
