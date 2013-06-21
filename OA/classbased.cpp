#include "classbased.h"
#include "ui_classbased.h"
#include <QDebug>
#include <fstream>
#include <QFile>
#include <stdlib.h>
ClassBased::ClassBased(int row,int col,int **final_codes,int **labels1,int **labels2,int ***data1,int ***data2,int num_band,QWidget *parent) :
    row(row),col(col),final_codes(final_codes),labels1(labels1),labels2(labels2),data1(data1),data2(data2),num_band(num_band),
    QWidget(parent),
    ui(new Ui::ClassBased)
{
    ui->setupUi(this);
    this->setWindowTitle("Geometric Class Based Change Analysis");
    flag2 = 0;
    flag1=0;
    a1=a2=a3=a4=a5=a6=a7=a8=a9=0;
}

ClassBased::~ClassBased()
{
    delete ui;
}
bool ClassBased :: isSelected(int px){
    if(px==1&&a1%2==0)
        return true;
    else if(px==2 && a2%2==0)
        return true;
    else if(px==3&&a3%2==0)
        return true;
    else if(px==4&&a4%2==0)
        return true;
    else if(px==5&&a5%2==0)
        return true;
    else if(px==6&&a6%2==0)
        return true;
    else if(px==7&&a7%2==0)
        return true;
    else if(px==8&&a8%2==0)
        return true;
    else if(px==9&&a9%2==0)
        return true;
    else return false;
}

void ClassBased::on_checkBox_clicked()
{
    a1++;
}
void ClassBased::on_checkBox_2_clicked()
{
    a2++;
}

void ClassBased::on_checkBox_3_clicked()
{
    a3++;
}

void ClassBased::on_checkBox_4_clicked()
{
    a4++;

}

void ClassBased::on_checkBox_5_clicked()
{
    a5++;

}

void ClassBased::on_checkBox_6_clicked()
{
    a6++;

}

void ClassBased::on_checkBox_8_clicked()
{
    a8++;
}

void ClassBased::on_checkBox_7_clicked()
{
    a7++;

}

void ClassBased::on_checkBox_9_clicked()
{
    a9++;
}
void ClassBased::writePPMImage(int ***op,int img1_size[],std::string fname)
{

    int i, j;
    FILE *fp = fopen(fname.c_str(), "wb"); /* b - binary mode */
    fprintf(fp, "P6\n%d %d\n255\n", img1_size[1], img1_size[0]);
    for (j = 0; j < img1_size[0]; ++j)
    {
        for (i = 0; i < img1_size[1]; ++i)
        {
            static unsigned char color[3];
            color[0] = op[j][i][0];  /* red */
            color[1] = op[j][i][1];  /* green */
            color[2] = op[j][i][2];  /* blue */
            fwrite(color, 1, 3, fp);
        }
    }
    fclose(fp);
}
void ClassBased::createImage(std::string fname)
{

    int x=0,y=0;
    int ***op;

    op = (int ***)malloc(sizeof(int **)*row);
    int i,j;
    for(i=0;i<row;i++)
    {
        op[i] = (int **)malloc(sizeof(int *)*col);
        for(j=0;j<col;j++)
            op[i][j] = (int *)malloc(sizeof(int)*3);
    }
    for(y = 0 ; y < row ; y++ ){
        for(x = 0; x < col ; x++){
            int px = final_codes[y][x];
            if(px==1&&a1%2==0){
                op[y][x][0] = 255;
                op[y][x][1] = 255;
                op[y][x][2] = 0;

            }

            else if(px==2 && a2%2==0){
                op[y][x][0] = 0;
                op[y][x][1] = 255;
                op[y][x][2] = 0;

            }

            else if(px==3&&a3%2==0){
                op[y][x][0] = 0;
                op[y][x][1] = 255;
                op[y][x][2] = 255;

            }

            else if(px==4&&a4%2==0){
                op[y][x][0] = 127;
                op[y][x][1] = 127;
                op[y][x][2] = 255;

            }

            else if(px==5&&a5%2==0){
                op[y][x][0] = 255;
                op[y][x][1] = 0;
                op[y][x][2] = 255;

            }
            else if(px==6&&a6%2==0){
                op[y][x][0] = 255;
                op[y][x][1] = 0;
                op[y][x][2] = 0;

            }
            else if(px==7&&a7%2==0){
                op[y][x][0] = 0;
                op[y][x][1] = 0;
                op[y][x][2] = 0;

            }
            else if(px==8&&a8%2==0){
                op[y][x][0] = 255;
                op[y][x][1] = 127;
                op[y][x][2] = 127;

            }
            else if(px==9&&a9%2==0){
                op[y][x][0] = 255;
                op[y][x][1] = 255;
                op[y][x][2] = 255;

            }
            else{
                op[y][x][0] = 0;
                op[y][x][1] = 0;
                op[y][x][2] = 255;

            }
        }
    }
    int img_size[2];
    img_size[0] = row;
    img_size[1] = col;
    writePPMImage(op,img_size,fname);
}


void ClassBased::on_radioButton_2_clicked()
{
    int c = final_codes[pbox->Y][pbox->X];
    if(isSelected(c)==true){
        if(flag2 ==0){
            flag2 = 1 ;
            int x=0,y=0;
            for(y = 0 ; y < row ; y++ ){
                for(x = 0; x < col ; x++){
                    ts2.push_back(labels2[y][x]);
                }
            }


        }
        Paintbox2 *tp = new Paintbox2(pbox->X +pbox->Y*col,ts2,data2,row,col,"Time 2",num_band);
        tp->show();
        tp->update();
    }
}
void ClassBased::on_radioButton_clicked()
{   int c = final_codes[pbox->Y][pbox->X];
    if(isSelected(c)==true){
        if(flag1 == 0){
            flag1 = 1 ;

            int x=0,y=0;
            for(y = 0 ; y < row ; y++ ){
                for(x = 0; x < col ; x++){
                    ts1.push_back(labels1[y][x]);
                }
            }

        }

        Paintbox2 *tp = new Paintbox2(pbox->X +pbox->Y*col,ts1,data1,row,col,"Time 1",num_band);
        tp->show();
    }
}


void ClassBased::on_pushButton_clicked()
{
    std::string fname = "C:/Windows/Temp/test.ppm";

    createImage(fname);


    pbox = new paintbox(fname,row,col);
    pbox->update();
    pbox->show();
}
