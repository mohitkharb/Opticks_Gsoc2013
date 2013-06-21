#include "selectcdtype.h"
#include "ui_selectcdtype.h"

selectCDType::selectCDType(int row,int col,int **final_codes,int **labels1,int **labels2,int ***data1,int ***data2,Image i1,Image i2,int num_band, QWidget *parent) :
    row(row),col(col),final_codes(final_codes),labels1(labels1),labels2(labels2),data1(data1),data2(data2),i1(i1),i2(i2),num_band(num_band),
    QWidget(parent),
    ui(new Ui::selectCDType)
{
    ui->setupUi(this);
    this->setWindowTitle("Change Detection Modes");
}

selectCDType::~selectCDType()
{
    delete ui;
}

void selectCDType::on_pushButton_clicked()
{
    ClassBased *w = new ClassBased(row,col,final_codes,labels1,labels2,data1,data2,num_band);
    w->show();
}

void selectCDType::on_pushButton_2_clicked()
{
 //   AttributeBased *a= new AttributeBased(row,col,final_codes,labels1,labels2,data1,data2,i1,i2,num_band,Flag);
 //   a->show();
}
