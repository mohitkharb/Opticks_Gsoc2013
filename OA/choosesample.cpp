#include "choosesample.h"
#include "ui_choosesample.h"
#include <QSignalMapper>
#include <QCheckBox>
#include <QObject>
#include<iostream>
#include"paint_box.h"
#include "mainwindow.h"
chooseSample::chooseSample(int ncls,int x,int y, int **labels1, QWidget *tempParent,QWidget *parent) :cls_no(ncls),X(x),Y(y),labels1_(labels1),tempParent(tempParent),
    QWidget(parent),
    ui(new Ui::chooseSample)
{
    for(int i=1; i<=ncls;i++){
        QString tmp;
        tmp="class";
        tmp.append(QString("%1").arg(i));
        classes << tmp;
    }
    ui->setupUi(this);
    ui->comboBox->addItems(classes);
    temp =0;
}

chooseSample::~chooseSample()
{
    delete tempParent;
    delete ui;
}

void chooseSample::on_pushButton_clicked()
{
    cls_no=temp;
    MainWindow *tmp  = (MainWindow*)tempParent;
    std::cout << "here converted" << std::endl;
    tmp->insertvalue(cls_no,X,Y,labels1_);
    this->close();
}

void chooseSample::on_comboBox_currentIndexChanged(int index)
{
    temp=index;

}
