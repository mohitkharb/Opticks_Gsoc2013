#include "mainwindow_cr.h"
#include "ui_mainwindow_cr.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui>
#include <QProcess>
#include <QDir>
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <string>
#include<sstream>
#include <QDebug>
#include<QList>
#include<list>
#include<math.h>
//#include "cloud_detect_removal.h"

int ***base, ***aux, ***cloud, ***op, ***res;
std::vector< std::vector< int > > auxOrderedQueueBand1;
std::vector< std::vector< int > > auxOrderedQueueBand2;
std::vector< std::vector< int > > auxOrderedQueueBand3;
int img1_size[2],img2_size[2];

using namespace std;

int ***readPPMImage(int imgsize[],char *filename,int bands)
{
    int row, col;
    FILE *file;
    char  buffer[1024];
    int   nrows, ncols, ngray, red, grn, blu ;

    /* open the image file for reading binary */
    if ( (file = fopen( filename, "rb") ) == NULL )
    {
        printf("readPPMImage(): file %s could not be opened\n", filename );
        exit( EXIT_FAILURE );
    }

    /* read signature on first line */
    if ( fscanf( file, "%s", buffer ) != 1 )
    {
        printf("readPPMImage(): error in image header, no signature\n" );
        exit( EXIT_FAILURE );
    }

    if ( strncmp( buffer, "P6", 2 ) != 0 )
    {
        printf("readPPMimage(): file %s is not a PPM file\n", filename );
        printf("Signature: %s\n", buffer );
        exit( EXIT_FAILURE );
    }

    /* skip over comment lines */
    int moreComments = 1, ch;
    while ( moreComments )
    {
        /* skip over possible white space */
        while ( (ch=fgetc(file)) && isspace(ch) );
        if ( ch=='#' )
        {
            /* comments are required to end with line-feed */
            fgets( buffer, 1024, file );
        }
        else
        {
            moreComments = 0;
            ungetc( ch, file );
        }
    }

    /* get ncols, nrows, ngray and eat the required single white space */
    int count = fscanf(file,"%d %d %d", &ncols, &nrows, &ngray );
    imgsize[0] = nrows;
    imgsize[1] = ncols;
    if ( count != 3 )
    {
        printf("readPPMimage(): error in image header\n" );
        exit( EXIT_FAILURE );
    }

    fgetc(file);

    if ( ngray != 255 )
    {
        printf("readPPMimage(): file %s is %d, not 8 bits per color\n",
               filename, ngray );
        exit( EXIT_FAILURE );
    }

    int ***data = (int ***)malloc(sizeof(int **)*nrows);

    for(int i=0; i<nrows; i++)
    {
        data[i] = (int **)malloc(sizeof(int)*ncols);
        for(int j=0; j<ncols; j++)
            data[i][j] = (int *)malloc(sizeof(int)*3);
    }

    /* read the pixel data: 3 bytes per disk pixel */
    /* (not nec. same size as sizeof(pixel) */
    if(bands>1){
        for ( row=0; row<nrows; row++ )
            for ( col=0; col<ncols; col++ )
            {
                red = (int)fgetc( file );
                grn = (int)fgetc( file );
                blu = (int)fgetc( file );
                data[row][col][0] = red;
                data[row][col][1] = grn;
                data[row][col][2] = blu;
            }
    }
    else{
        for ( row=0; row<nrows; row++ )
            for ( col=0; col<ncols; col++ )
            {
                red = (int)fgetc( file );
                data[row][col][0] = red;
                data[row][col][1] = 0;
                data[row][col][2] = 0;
            }
    }


    fclose( file );
    return(data);
}


void dilate(int ***op, int ***&final_op, int img1_size[],int size_se)
{

    int i,j,k,l,m,n,flag;

    int nrows = img1_size[0];
    int ncols = img1_size[1];

    int cent_se;
    if(size_se%2 == 1)
        cent_se = (size_se-1)/2;
    else
        cent_se = (size_se)/2;

    for(i=0;i<nrows;i++)
    {
        for(j=0;j<ncols;j++)
        {
            flag = 0;


            k=i-cent_se;

            if(k<0)
                k=0;

            if(size_se%2 == 1)
            {
                m = i+cent_se;
                n = j+cent_se;
            }
            else
            {
                m = i+cent_se-1;
                n = j+cent_se-1;
            }
            if(m>=nrows)
                m=nrows-1;
            if(n>=ncols)
                n=ncols-1;

            for(;k<=m;k++)
            {
                l=j-cent_se;
                if(l<0)
                    l=0;
                for(;l<=n;l++)
                {
                    if(op[k][l][0] > 0)
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag == 1)
                    break;
            }

            if(flag == 1)
            {
                final_op[i][j][0] = 255;
                final_op[i][j][1] = 255;
                final_op[i][j][2] = 255;
            }
            else
            {
                final_op[i][j][0] = 0;
                final_op[i][j][1] = 0;
                final_op[i][j][2] = 0;
            }
        }
    }
}

int ***cloud_detection(int img1_size[],int ***base, int n)
{
    int i,j;


    int nrows = img1_size[0];
    int ncols = img1_size[1];

    int ***op;
    int ***final_op;

    op = (int ***)malloc(sizeof(int **)*nrows);

    for(i=0;i<nrows;i++)
    {
        op[i] = (int **)malloc(sizeof(int *)*ncols);
        for(j=0;j<ncols;j++)
            op[i][j] = (int *)malloc(sizeof(int)*3);
    }

    final_op = (int ***)malloc(sizeof(int **)*nrows);

    for(i=0;i<nrows;i++)
    {
        final_op[i] = (int **)malloc(sizeof(int *)*ncols);
        for(j=0;j<ncols;j++)
            final_op[i][j] = (int *)malloc(sizeof(int)*3);
    }

    for(i=0;i<nrows;i++)
        for(j=0;j<ncols;j++)
        {
            // Thresholds for cloud pixels
            //            if(base[i][j][2] >= 0 && base[i][j][1] > 180 && base[i][j][0] > 230)
            if(base[i][j][2] >= 0 && base[i][j][1] > 180 && base[i][j][0] > 150)
            {
                op[i][j][0] = 255;
                op[i][j][1] = 255;
                op[i][j][2] = 255;
            }
            else
            {
                op[i][j][0] = 0;
                op[i][j][1] = 0;
                op[i][j][2] = 0;
            }
        }


    dilate(op,final_op, img1_size,n);

    FILE *fp = fopen("cloud.ppm", "wb"); /* b - binary mode */
    fprintf(fp, "P6\n%d %d\n255\n", img1_size[1], img1_size[0]);
    for (j = 0; j < img1_size[0]; ++j)
    {
        for (i = 0; i < img1_size[1]; ++i)
        {
            static unsigned char color[3];
            color[0] = final_op[j][i][0];  /* red */
            color[1] = final_op[j][i][1];  /* green */
            color[2] = final_op[j][i][2];  /* blue */
            fwrite(color, 1, 3, fp);
        }
    }
    fclose(fp);
    return final_op;


}

// Returns distance between two pixels of the same image
float distance(int ***img1, int r1, int c1, int r2, int c2)
{
    // returns distance if two pixels are different
    if(r1 != r2 || c1 != c2)
        return sqrt(powf(img1[r1][c1][0] - img1[r2][c2][0],2) + powf(img1[r1][c1][1] - img1[r2][c2][1],2) + powf(img1[r1][c1][2] - img1[r2][c2][2],2));
    // returns 100000 if pixels indices are the same
    else
        return 100000;
}



int ***csf(int ***base, int ***aux, int ***cloud, int img1_size[], int img2_size[], std::vector< std::vector <int> > auxOrderedQueueBand1 , std::vector< std::vector <int> > auxOrderedQueueBand2, std::vector< std::vector <int> > auxOrderedQueueBand3)
//void csf(int startRow, int  endRow)

{
    //    extern int ***aux,***base,***cloud,***op;
    //    extern int img1_size[2],img2_size[2];
    //    extern std::vector< std::vector< int > > auxOrderedQueueBand1;
    //    extern std::vector< std::vector< int > > auxOrderedQueueBand2;
    //    extern std::vector< std::vector< int > > auxOrderedQueueBand3;
    //        int ***op = (int ***)malloc(sizeof(int **)*img1_size[0]);
    int i,j,k,l;

    op = (int ***)malloc(sizeof(int **)*img1_size[0]);


    for(i=0;i<img1_size[0];i++)
    {
        op[i] = (int **)malloc(sizeof(int *)*img1_size[1]);
        for(j=0;j<img1_size[1];j++)
            op[i][j] = (int *)malloc(sizeof(int)*3);
    }


    int row,col;
    float min_dist,dist;

    int nrows = img1_size[0];
    int ncols = img1_size[1];

    int auxRows = img2_size[0];
    int auxCols = img2_size[1];

    FILE *fi = fopen("closest_pixels.txt","w");

    for(i=0;i<nrows;i++)
    {
        for(j=0;j<ncols;j++)
        {
            std::cout<<"Pixel :"<<i<<" "<<j<<std::endl;
            // Checks if the pixel is cloud pixel
            if(cloud[i][j][0] > 0 || cloud[i][j][1] > 0 || cloud[i][j][2] > 0)
            {
                row = i;
                col = j;
                min_dist = 10000000;

                int redColor = aux[i][j][0];
                int flagIndex = 1;
                int flagBreak = 0;

                while(1)
                {
                    if(flagIndex == 6)
                        break;
                    if(flagIndex == 1)
                        k = redColor;
                    else if(flagIndex % 2 == 0)
                        k = redColor - flagIndex/2;
                    else
                        k = redColor + flagIndex/2;
                    flagIndex++;

                    if(k>=0 && k<256)
                    {
                        for(l=0; l< (int)auxOrderedQueueBand1[k].size(); l++)
                        {
                            int r = auxOrderedQueueBand1[k][l]/ncols;
                            int c = auxOrderedQueueBand1[k][l]%ncols;
                            if(cloud[r][c][0] == 0 && cloud[r][c][1] == 0 && cloud[r][c][2] == 0)
                            {
                                dist = distance(aux,i,j,r,c);
                                if(min_dist > dist)
                                {
                                    min_dist = dist;
                                    row = r;
                                    col = c;
                                    flagBreak = 1;
                                }
                            }

                        }

                    }
//                    if(flagBreak == 1)
//                        break;
                }

                std::cout<<"After Red While"<<std::endl;
                int greenColor = aux[i][j][1];
                flagIndex = 1;
                flagBreak = 0;
                while(1)
                {
                    if(flagIndex == 6)
                        break;
                    if(flagIndex == 1)
                        k = greenColor;
                    else if(flagIndex % 2 == 0)
                        k = greenColor - flagIndex/2;
                    else
                        k = greenColor + flagIndex/2;
                    flagIndex++;

                    if(k>=0 && k<256)
                    {
                        for(l=0; l<(int)auxOrderedQueueBand2[k].size(); l++)
                        {
                            int r = auxOrderedQueueBand2[k][l]/ncols;
                            int c = auxOrderedQueueBand2[k][l]%ncols;
                            if(cloud[r][c][0] == 0 && cloud[r][c][1] == 0 && cloud[r][c][2] == 0)
                            {
                                dist = distance(aux,i,j,r,c);
                                if(min_dist > dist)
                                {
                                    min_dist = dist;
                                    row = r;
                                    col = c;

                                }
                            }

                        }


                    }
//                    if(flagBreak == 1)
//                        break;
                }

                std::cout<<"After green While"<<std::endl;
                int blueColor = aux[i][j][2];
                flagIndex = 1;
                flagBreak = 0;
                while(1)
                {
                    if(flagIndex == 6)
                        break;
                    if(flagIndex == 1)
                        k = blueColor;
                    else if(flagIndex % 2 == 0)
                        k = blueColor - flagIndex/2;
                    else
                        k = blueColor + flagIndex/2;
                    flagIndex++;

                    if(k>=0 && k<256)
                    {
                        for(l=0; l<(int)auxOrderedQueueBand3[k].size(); l++)
                        {
                            int r = auxOrderedQueueBand3[k][l]/ncols;
                            int c = auxOrderedQueueBand3[k][l]%ncols;
                            if(cloud[r][c][0] == 0 && cloud[r][c][1] == 0 && cloud[r][c][2] == 0)
                            {
                                dist = distance(aux,i,j,r,c);
                                if(min_dist > dist)
                                {
                                    min_dist = dist;
                                    row = r;
                                    col = c;
                                    flagBreak = 1;
                                }
                            }

                        }

                    }
//                    if(flagBreak == 1)
//                        break;
                }

                std::cout<<"After blue While"<<std::endl;
                fprintf(fi,"%d %d %d %d\n",i+1,j+1,row+1,col+1);
                // Assigns a value of a pixel in base image corresponding to minimum distance.
                op[i][j][0] = base[row][col][0];
                op[i][j][1] = base[row][col][1];
                op[i][j][2] = base[row][col][2];

            }
            // If not cloud pixel, assigns base image value of that pixel.
            else
            {
                op[i][j][0] = base[i][j][0];
                op[i][j][1] = base[i][j][1];
                op[i][j][2] = base[i][j][2];
            }
        }
    }

    fclose(fi);

        return op;
}

int ***maxfilter(int ***op, int ***cloud, int img1_size[])
{
    int rows = img1_size[0];
    int cols = img1_size[1];

    int ***res;

    res = (int ***)malloc(sizeof(int **)*rows);

    for(int i=0;i<rows;i++)
    {
        res[i] = (int **)malloc(sizeof(int *)*cols);
        for(int j=0;j<cols;j++)
            res[i][j] = (int *)malloc(sizeof(int)*3);
    }

    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            // Checks if the pixel is cloud pixel
            if(cloud[i][j][0] > 0 || cloud[i][j][1] > 0 || cloud[i][j][2] > 0)
            {
                int left, right, top, bottom, ft_size, ft_hsize;

                ft_size = 5;

                ft_hsize = (ft_size/2);

                left = i-ft_hsize;
                if(left < 0)
                    left = 0;
                top = j-ft_hsize;
                if(top < 0)
                    top = 0;


                right = i+ft_hsize;
                if(right >= img1_size[0])
                    right = img1_size[0] - 1;
                bottom = j+ft_hsize;
                if(bottom >= img1_size[1])
                    bottom = img1_size[1] - 1;

                int red_occ[256] = {0};
                int green_occ[256] = {0};
                int blue_occ[256] = {0};
                for(int k=left; k<=right; k++)
                {
                    for(int l=top;l<=bottom;l++)
                    {
                        red_occ[op[k][l][0]] +=1;
                        green_occ[op[k][l][1]] +=1;
                        blue_occ[op[k][l][2]] +=1;
                    }
                }

                int red_val=0, green_val=0, blue_val=0;

                int max_occ_r, max_occ_g, max_occ_b;
                max_occ_r = max_occ_g = max_occ_b = -1;

                for(int k=0;k<256;k++)
                {
                    if(red_occ[k] > max_occ_r)
                    {
                        max_occ_r = red_occ[k];
                        red_val = k;
                    }
                    if(green_occ[k] > max_occ_g)
                    {
                        max_occ_g = green_occ[k];
                        green_val = k;
                    }
                    if(blue_occ[k] > max_occ_b)
                    {
                        max_occ_b = blue_occ[k];
                        blue_val = k;
                    }

                }

                res[i][j][0] = red_val;
                res[i][j][1] = green_val;
                res[i][j][2] = blue_val;
            }
            else
            {
                res[i][j][0] = op[i][j][0];
                res[i][j][1] = op[i][j][1];
                res[i][j][2] = op[i][j][2];
            }
        }
    }

    return res;
}


void writePPMImage(int ***op,int img1_size[])
{

    int i, j;
    FILE *fp = fopen("result.ppm", "wb"); /* b - binary mode */
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

std::vector< std::vector <int> > createOrderedQueue(int ***aux, int img_size[], int bandnum)
{
    std::vector< std::vector <int> > result(256);
    for(int i=0;i < img_size[0];i++)
    {
        for(int j=0;j<img_size[1];j++)
        {
            int lin = i*img_size[1] + j;
            result[aux[i][j][bandnum]].push_back(lin);
        }
    }
    return result;
}



MainWindow_cr::MainWindow_cr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow_cr)
{
    ui->setupUi(this);
    this->setWindowTitle("Missing Data Filling");
}

MainWindow_cr::~MainWindow_cr()
{
    delete ui;
}

void MainWindow_cr::on_pushButton_clicked()
{
    QFileDialog::Options options;
    QString selectedFilter;
    fileName1 = QFileDialog::getOpenFileName(this,
                                             tr("select refrence image"),
                                             ui->label->text(),
                                             tr("All Files (*);;Text Files (*.txt)"),
                                             &selectedFilter,
                                             options);

    if (!fileName1.isEmpty())
        ui->label->setText(fileName1);

  



}

void MainWindow_cr::on_pushButton_3_clicked()
{
    QFileDialog::Options options;
    QString selectedFilter;
    fileName2 = QFileDialog::getOpenFileName(this,
                                             tr("Select Auxilary Image"),
                                             ui->label_2->text(),
                                             tr("All Files (*);;Text Files (*.txt)"),
                                             &selectedFilter,
                                             options);

    if (!fileName2.isEmpty())
        ui->label_2->setText(fileName2);
    







}


void MainWindow_cr::on_pushButton_5_clicked()
{
    int n;
    //    extern int ***base, ***aux, ***cloud, ***op, ***res;
    //    std::vector< std::vector< int > > auxOrderedQueueBand1;
    //     std::vector< std::vector< int > > auxOrderedQueueBand2;
    //     std::vector< std::vector< int > > auxOrderedQueueBand3;
    //     int img1_size[2],img2_size[2];

	base = readPPMImage(img1_size,fileName1.toUtf8().data(),3);

	aux = readPPMImage(img2_size,fileName2.toUtf8().data(),3);

    auxOrderedQueueBand1 = createOrderedQueue(aux, img2_size, 0);
    auxOrderedQueueBand2 = createOrderedQueue(aux, img2_size, 1);
    auxOrderedQueueBand3 = createOrderedQueue(aux, img2_size, 2);

    n = atoi(ui->lineEdit->text().toStdString().c_str());

    cloud = cloud_detection(img1_size,base,n);
    std::cout << "after cloud_detection"<< std::endl;
    //op = csf(base,aux,cloud,img1_size,img2_size);

//    op = (int ***)malloc(sizeof(int **)*img1_size[0]);
//    for(i=0;i<img1_size[0];i++)
//    {
//        op[i] = (int **)malloc(sizeof(int *)*img1_size[1]);
//        for(j=0;j<img1_size[1];j++)
//            op[i][j] = (int *)malloc(sizeof(int)*3);
//    }


//    int numRowsPerThread = 10;
//    int numThreads = img1_size[0] / numRowsPerThread + 1;
//    QFuture<void> *result = new QFuture<void> [numThreads];
//    i=0, j=numRowsPerThread - 1;
//    if(j>img1_size[0] - 1)
//        j = img1_size[0] - 1;
//    int threadId = 0;
//    while(i < img1_size[0])
//    {
//        result[threadId++] = QtConcurrent::run(csf,i,j);
//        i=j+1;
//        j=i + numRowsPerThread - 1;
//        if(j>=img1_size[0])
//            j=img1_size[0] - 1;
//    }
//    for( i = 0; i<numThreads; i++)
//        result[i].waitForFinished();


    op = csf(base,aux,cloud,img1_size,img2_size, auxOrderedQueueBand1, auxOrderedQueueBand2,auxOrderedQueueBand3);
    std::cout << "after csf"<< std::endl;
    res = maxfilter(op,cloud,img1_size);
    std::cout << "after max filter"<< std::endl;

    writePPMImage(res,img1_size);

    ui->pushButton_6->setEnabled(true);
}

void MainWindow_cr::on_pushButton_2_clicked()
{

	paintwindow *pw = new paintwindow(fileName1.toUtf8().data());
    ui->mdiArea->addSubWindow(pw);
    pw->setWindowFlags(Qt::FramelessWindowHint);


    pw->showMaximized();

}

void MainWindow_cr::on_pushButton_4_clicked()
{

	paintwindow *pw2 = new paintwindow(fileName2.toUtf8().data());
    ui->mdiArea->addSubWindow(pw2);
    pw2->setWindowFlags(Qt::FramelessWindowHint);
    pw2->showMaximized();

}

void MainWindow_cr::on_pushButton_6_clicked()
{
    paintwindow *pw2 = new paintwindow("result.ppm");
    ui->mdiArea->addSubWindow(pw2);
    pw2->setWindowFlags(Qt::FramelessWindowHint);


    pw2->showMaximized();
}
