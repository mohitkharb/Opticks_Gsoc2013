#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<qdialog.h>
#include "paintwindow.h"
#include <vector>
#include <QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>
#include <QThread>

namespace Ui {
    class MainWindow_cr;
}

int ***readPPMImage(int imgsize[],char *filename, int bands);

void dilate(int ***op, int ***&final_op, int img1_size[], int n);
int ***cloud_detection(int img1_size[],int ***base, int n);

float distance(int ***img1, int r1, int c1, int r2, int c2);
int ***csf(int ***base, int ***aux, int ***cloud, int img1_size[], int img2_size[], std::vector< std::vector <int> > oq1 , std::vector< std::vector <int> > oq2, std::vector< std::vector <int> > oq3);
//void csf(int startRow, int endRow);

int ***maxfilter(int ***op, int ***cloud,int img1_size[]);

void writePPMImage(int ***op,int img1_size[]);

std::vector< std::vector <int> > createOrderedQueue(int ***aux, int img_size[],int bandnum);


class MainWindow_cr : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow_cr(QWidget *parent = 0);
    ~MainWindow_cr();
    QString fileName1;
    QString fileName2;


private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow_cr *ui;
};

#endif // MAINWINDOW_H
