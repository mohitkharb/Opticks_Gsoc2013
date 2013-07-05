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
    unsigned char ***data1,***data2;
    int NO1,NO2;
    int **labels1;
    int **labels2;
    std::vector< std::vector< int > >object_info1;
    std::vector< std::vector< int > >object_info2;
    Image i1;
    Image i2;
    int num_band;
	int image_type;
	selectCDType *w;

    void ChangeDetection(int flag);


	

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

	void on_radioButton_clicked();

    void on_radioButton_2_clicked();

   
private:
    Ui::MainWindow_cd *ui;
	void Show_process(std::string model);

};

#endif // MAINWINDOW_H
