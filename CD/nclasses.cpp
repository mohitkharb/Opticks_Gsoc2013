#include "nclasses.h"
#include "ui_nclasses.h"
#include "mainwindow_fusion.h"
#include "scarea.h"
nclasses::nclasses(std::string fname1, int row,int col,int **labels1, MainWindow_fusion *tempParent,QWidget *parent) :
    fname1_(fname1),row_(row),col_(col),labels1_(labels1),tempParent(tempParent),
    QWidget(parent),
    ui(new Ui::nclasses)
{
    ui->setupUi(this);
    this->setWindowTitle("Classification");
}

nclasses::~nclasses()
{
    delete ui;
}

void nclasses::on_pushButton_clicked()
{
    int i= atoi(ui->lineEdit->text().toUtf8().data());
     pbox = new paint_box(fname1_,row_,col_,i,labels1_,1,tempParent);
     scarea *area = new scarea(pbox,tempParent);


     area->show();
//     pbox->update();
//     pbox->show();

    ui->pushButton_3->setEnabled(true);
    ui->pushButton->setDisabled(true);
}

void nclasses::on_pushButton_3_clicked()
{
    /*MainWindow *tmp  = (MainWindow*)tempParent;
    tmp->classificationImage1();
    tmp->classificationImage2();
    tmp->ChangeDetection(1);
    this->close();*/
    MainWindow_fusion *tmp = (MainWindow_fusion*)tempParent;
    tmp->classification();
    this->close();
}

void nclasses::on_pushButton_2_clicked()
{
    /*MainWindow *tmp  = (MainWindow*)tempParent;
    tmp->ChangeDetection(0);
    this->close();*/
}
