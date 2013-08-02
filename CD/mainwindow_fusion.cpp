#include "mainwindow_fusion.h"
#include "ui_mainwindow_fusion.h"
#include <QSignalMapper>
#include <QCheckBox>
#include <QObject>
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
#include <algorithm>
#include"paint_box.h"
#include "scarea.h"


MainWindow_fusion::MainWindow_fusion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow_fusion)
{

	postprocessing = 0;
    ui->setupUi(this);
	this->setWindowTitle("Image Fusion");
}

MainWindow_fusion::~MainWindow_fusion()
{
    delete ui;
}


void MainWindow_fusion::on_Browse_4_clicked()
{
    QFileDialog::Options options;
    QString selectedFilter;
    fileName = QFileDialog::getOpenFileName(this,
                                tr("QFileDialog::getOpenFileName()"),
                                ui->lineEdit_3->text(),
                                tr("All Files (*);;Text Files (*.txt)"),
                                &selectedFilter,
                                options);
    //System.Out.println(fileName);
    //ui->textBrowser->append(fileName);
//   printf("%s",fileName1);

    if (!fileName.isEmpty())
        ui->lineEdit_3->setText(fileName);
    data1 = readPPMImage(imgsize,ui->lineEdit_3->text().toUtf8().data(),3);
    //std::cout << data1[10][10][0] << std::endl;
    //std::cout << data1[10][10][1] << std::endl;
    //std::cout << data1[10][10][2] << std::endl;
    std::cout << ui->lineEdit_3->text().toUtf8().data() << std::endl;
}

void MainWindow_fusion::on_Browse_2_clicked()
{
    QFileDialog::Options options;
    QString selectedFilter;
    fileName1 = QFileDialog::getOpenFileName(this,
                                tr("QFileDialog::getOpenFileName()"),
                                ui->lineEdit2->text(),
                                tr("All Files (*);;Text Files (*.txt)"),
                                &selectedFilter,
                                options);
    //System.Out.println(fileName);
    ui->textBrowser->append(fileName1);
   // printf("%s",fileName);

    if (!fileName1.isEmpty())
        ui->lineEdit2->setText(fileName1);
    data2 = readPPMImage(imgsize,ui->lineEdit2->text().toUtf8().data(),3);
   // std::cout << data2[10][10][0] << std::endl;
   // std::cout << data2[10][10][1] << std::endl;
   // std::cout << data2[10][10][2] << std::endl;
    std::cout << ui->lineEdit2->text().toUtf8().data() << std::endl;
}

void MainWindow_fusion::on_pushButton_dir_clicked()
{
QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

    data_dir = QFileDialog::getExistingDirectory(this,
                                    tr("QFileDialog::getExistingDirectory()"),
                                    ui->label_2->text(),
                                    options);
        if (!data_dir.isEmpty())
            ui->label_2->setText(data_dir);

		ms_dir=data_dir;
		ms_dir.append("\\mymeanshift\\release");
		temp_dir=data_dir;
		temp_dir.append("\\Temp");
		out_dir=data_dir;
		out_dir.append("\\output");

}
void MainWindow_fusion::Show_process(std::string model)
{

    FILE *procOut = _popen(model.c_str(), "r");

    if(procOut == NULL)
        exit(1);

    char output[200];
    while(fgets(output, sizeof(output), procOut) != NULL) {
        ui->textBrowser->append(output);
    }
    fclose(procOut);


}
void MainWindow_fusion::on_pushButton_ms_clicked()
{
	ui->textBrowser->setText("calculating mean shift.");
    if(!fileName1.isEmpty() && !ms_dir.isEmpty())
    {



    QString img = ui->lineEdit2->text();
    QString sbw = ui->lineEdit_s1->text();
    QString rbw = ui->lineEdit_r1->text();
    QString mra = ui->lineEdit_m1->text();
    QList <QString> list = img.split(QRegExp("/"));
    img_name = list.last();

    segName= ms_dir;
    segName.append("\\seg_images\\seg_");
    segName.append(img_name);



    QString file_exp = ms_dir;
    file_exp.append("\\exp");
    QFile expFile(file_exp.toUtf8().data());
    expFile.open(QIODevice::ReadWrite);

    QString file_try=ms_dir;
    file_try.append("\\try");

    QFile tryFile(file_try.toUtf8().data());
    tryFile.open(QIODevice::WriteOnly);

    QTextStream in(&expFile);
    QTextStream out(&tryFile);
    int i=1;
    QString line = in.readLine();
    while(!in.atEnd()){
        if(i==6){
            out<<"SpatialBandwidth = "<< sbw <<";\n";
        }
        else if(i==7){
            out<<"RangeBandwidth = "<< rbw <<";\n";
        }
        else if(i==8){
            out<<"MinimumRegionArea ="<< mra <<";\n";
        }
        else if(i==15){
            out<<"Load('"<< img<<"',IMAGE);\n";
        }
        else if(i==23){
            out<<"Save('C:\\Windows\\Temp\\seg_"<< img_name<<"', PPM, SEGM_IMAGE);\n";
        }
        else{
            out<<line<<"\n";
        }
        line =in.readLine();
        i++;
    }
    tryFile.close();
    expFile.close();

    QString mod1 = ms_dir;
    //mod1.append("/Meanshift ");
    mod1.append("\\meanshift.exe ");

    mod1.append(ms_dir);
    mod1.append("\\try");
    model=mod1.toUtf8().data();

//    std::cout << model<<std::endl;
    //model = "/home/lsi/Ankush/DEAL_project/Meanshift /home/lsi/Ankush/DEAL_project/try";
    Show_process(model);
    labels1 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions.txt",imgsize[0],imgsize[1]);
    //labels1 = prepareNewLabels("/tmp/map_label_pruning_spurious_regions.txt",imgsize[0],imgsize[1]);
    merge_map = (int **)malloc(sizeof(int *)*imgsize[0]);
        for(int i=0; i<imgsize[0]; i++)
            merge_map[i] = (int *)malloc(sizeof(int)*imgsize[1]);

        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                merge_map[i][j] = -1;

        labels1 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions.txt",imgsize[0],imgsize[1]);
        FILE *lab = fopen("C:/Windows/Temp/map_label_pruning_spurious_regions.txt","w");
        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                fprintf(lab,"%d ",labels1[i][j]);
        fclose(lab);

        object_info1 = prepareObjectInfo(imgsize[0],imgsize[1],labels1,labels1,merge_map,NO1);//using labels two times
        i1.createSegmentedImageInfo(merge_map);
        i1.createFusedObjects(NO1,object_info1,0, data1, data1);

        std::cout<<"after meanshift NO1: "<<NO1<<std::endl;

        for(int i=0; i<imgsize[0]; i++)
            free(labels1[i]);
        free(labels1);

        labels1 = stitchChunks("C:/Windows/Temp/map_label_pruning_spurious_regions.txt",imgsize[0],imgsize[1],i1);
        std::cout<<"after stitching chunks"<<std::endl;

        lab = fopen("C:/Windows/Temp/map_label_pruning_spurious_regions.txt","w");
        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                fprintf(lab,"%d ",labels1[i][j]);
        fclose(lab);


        for(int i=0; i<imgsize[0]; i++)
            free(labels1[i]);
        free(labels1);
         std::cout<<"after freeing labels - 2"<<std::endl;


        free(i1.objectArray);

        labels1 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions.txt",imgsize[0],imgsize[1]);

//        std::cout <<"after preparing new labels for pan image" << imgsize[0] << " img " << imgsize[1] << std::endl;

        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                merge_map[i][j] = -1;
        object_info1 = prepareObjectInfo(imgsize[0],imgsize[1],labels1,labels1,merge_map,NO1);//using labels two times

        std::cout<<"after stitching chunks NO1: "<<NO1<<std::endl;
//        std::cout << "calculated object info for pan image"<<std::endl;
        i1.createSegmentedImageInfo(merge_map);

        for(i=0; i<imgsize[0]; i++)
        {
            free(merge_map[i]);
        }
        free(merge_map);

        i1.NO = NO1;

    }
    else ui->textBrowser->setText("Select directory and file names.");
}
void MainWindow_fusion::on_radioButton_clicked()
{
    glsiclicked = 1;
}
void MainWindow_fusion::on_radioButton_2_clicked()
{
    glsiclicked = 0;
}

void MainWindow_fusion::on_checkBox_clicked()
{
    postprocessing = !postprocessing;
}

void MainWindow_fusion::Fusion_Brovey(int **segmap, unsigned char ***&fimage, unsigned char ***msimage, unsigned char ***panimage, int onum, int xmin, int xmax, int ymin, int ymax,int bandnum)
{
    int i,j;

    for (i=xmin;i<=xmax;i++)
    {
        for (j=ymin;j<=ymax;j++)
        {
            if (segmap[i][j]==onum) {
                if(msimage[i][j][0] == 0 && msimage[i][j][1] == 0 && msimage[i][j][2] == 0)
                    fimage[i][j][bandnum] = 0;
                else
                    fimage[i][j][bandnum] = (unsigned char)floor(((msimage[i][j][bandnum]*1.0)/(msimage[i][j][0]+msimage[i][j][1]+msimage[i][j][2]))*panimage[i][j][0]);

            }
        }
    }

}


void MainWindow_fusion::GetHistogram (std::vector<unsigned char> data, std::vector<unsigned char> &val, std::vector<int> &cnt)
{
            int it;
    it = 0;
    int curval,curcnt = 0;
    curval = data[it];
    int size = data.size();

    it = 0;
    while (it < size)
    {
        if(data[it] == curval)
            curcnt++;
        else
        {
            val.push_back(curval);
            cnt.push_back(curcnt);
            curval = data[it];
            curcnt = 1;
        }
        it++;
    }
    cnt.push_back(curcnt);
    val.push_back(curval);

}
void MainWindow_fusion::GetStats(int **segmap, std::vector<unsigned char> data,int onum,float &mean,float &stnd, int &min, int &max,int xmin, int xmax, int ymin, int ymax, int nop)
{
//GetStats(segmap,pandata,onum,&panmean,&panstd,&panmin,&panmax,xmin,xmax,ymin,ymax);

        std::vector<unsigned char> val;
        std::vector<int> cnt;
        float Tprune = atof(ui->lineEditTh->text().toUtf8().data());
    GetHistogram(data,val,cnt);

    int size;
    size = val.size();

    int maxgap = -1;

    int it=0;
    int cit;
    int wit;

    if (size == 1)
    {
        min = val[0];
        max = val[0]+10;
        mean = val[0]+5;
        stnd = 0;
    }
    else {

        while(it < size - 1)
        {
            if (val[it+1] - val[it] > maxgap) {
                maxgap = (val[it+1] - val[it]);
                wit = it;
            }
            it++;
        }


        float count = 0;
        min = val[0];
        max = val[size-1];

        it = 0;
        cit = 0;
        while(it <= wit)
        {
            count = count + cnt[cit];

            it++;
            cit++;
        }
        count = count / nop ;


        if (count < 1 - Tprune)/*his means that we have to fix the minimum of the histogram*/
        {
            min = val[wit+1];
            max = val[size - 1];
        }

        if( count > Tprune)/*This means that we have to fix the maximum of the histogram*/
        {
            min = val[0];
            max = val[wit];
        }

        /*Now we have range from which we have to prepare the mean and standard deviation of the histogram */

        it = 0;
        cit = 0;
        float sum = 0;
        int curval,curcnt;
        int numberofpixels=0;

        while (it < size)
        {
            curval = val[it];
            curcnt = cnt[cit];
            sum = sum + curval*curcnt;
            numberofpixels += curcnt;
            it++;
            cit++;
        }
        mean = sum/nop;
        if(numberofpixels != nop)
            std::cout<< " error in freq calculation" << std::endl;


        it = 0;
        cit = 0;
        sum = 0;

        while (it < size)
        {
            curval = val[it];
            curcnt = cnt[cit];

            sum = sum + ((curval - mean)*(curval - mean))*curcnt;
            it++;
            cit++;
        }
        stnd = sum/nop;
        stnd = sqrt(stnd);
    }

}
void MainWindow_fusion::PrepareStatistics(int **segmap, unsigned char ***msimage, unsigned char ***panimage,int onum,int xmin,int xmax,int ymin,int ymax,int bandnum,float &msmean,float &msstd,float &panmean,float &panstd,int &panmin,int &panmax, int &msmin, int &msmax)
//void MainWindow::PrepareStatistics(int **&segmap, int ***&msimage, int ***&panimage,int &onum,int &xmin,int &xmax,int &ymin,int &ymax,int &bandnum,float &msmean,float &msstd,float &panmean,float &panstd,int &panmin,int &panmax)
{
//PrepareStatistics(segmap,msimage,panimage,onum,xmin,xmax,ymin,ymax,bandnum,&msmean,&msstd,&panmean,&panstd,&panmin,&panmax);
//PrepareStatistics(segmap,msimage,panimage,onum,xmin,xmax,ymin,ymax,bandnum,msmean,msstd,panmean,panstd,panmin,panmax);
        //Tprune = 0.9;

        int i,j;
    std::vector<unsigned char> pandata;
    std::vector<unsigned char> msdata;

    int index = 0;

    for (i=xmin;i<=xmax;i++)
    {
        for(j=ymin;j<=ymax;j++)
        {
            if (segmap[i][j] == onum){
//                char *p = new char[2];
//                char *m = new char[2];

                pandata.push_back(panimage[i][j][0]);
                msdata.push_back(msimage[i][j][bandnum]);
                index++;
            }

        }
    }

    sort(pandata.begin(),pandata.end());
    sort(msdata.begin(),msdata.end());


    GetStats(segmap,pandata,onum,panmean,panstd,panmin,panmax,xmin,xmax,ymin,ymax,index);
    GetStats(segmap,msdata, onum,msmean,msstd,msmin,msmax,xmin,xmax,ymin,ymax,index);

    pandata.clear();
    msdata.clear();


}

void MainWindow_fusion::Fusion_GLSI(int **segmap, unsigned char ***&fimage, unsigned char ***msimage, unsigned char ***panimage, int onum, int xmin, int xmax, int ymin, int ymax,int bandnum)
{

        float msmean,msstd,panmean,panstd,curpan,dist;
    int panmin,panmax,msmin,msmax;

    int i,j;

    /*This calculates mean and stddev after cleaning is applied to histograms*/
    PrepareStatistics(segmap,msimage,panimage,onum,xmin,xmax,ymin,ymax,bandnum,msmean,msstd,panmean,panstd,panmin,panmax,msmin,msmax);

    if(msmean > 255)
        msmean = 255;
    if(panmean > 255)
        panmean = 255;

    for(i=xmin;i<=xmax;i++)
    {
        for(j=ymin;j<=ymax;j++)
        {

            if (segmap[i][j] == onum)
            {
                if (panimage[i][j][0] >= panmin && panimage[i][j][0] <= panmax)
                {
                    curpan = panimage[i][j][0];

                    if (curpan > panmean)
                    {
                        if(curpan - panmean == 0)
                            dist = 0;
                        else if(panmax - panmean == 0)
                            dist = curpan - panmean;
                        else
                            dist = (curpan - panmean)/(panmax - panmean);

                        int final_value = msmean + dist*(msmax - msmean);
                        if(final_value > 255)
                            fimage[i][j][bandnum] = 255;
                        else
                            fimage[i][j][bandnum] = final_value;
                    }
                    else
                    {
                        if(curpan - panmean == 0)
                            dist = 0;
                        else if(panmin - panmean == 0)
                            dist = panmean - curpan;
                        else
                            dist = (panmean - curpan)/(panmean - panmin);

                        int final_value = msmean - dist*(msmean - msmin);
                        if( final_value > 255)
                            fimage[i][j][bandnum] = 255;
                        else
                            fimage[i][j][bandnum] = final_value;
                     }

                }
                else
                {
                    //fimage[i][j][bandnum] = -1;
                    blackPixelFlag[i][j][bandnum] = 1;
                }

            }
        }
    }
   // std::cout << "coming out of FGLSI" << std::endl;
}

void MainWindow_fusion::FillBlackPixels(unsigned char ***&fimage,int rows,int cols,int **&segmap,int b)
{
    /*This function fills the black pixels created by Fusion_GLSI.
      The neighbourhood of the black pixel is searched and the object
      with maximum area replaces the black pixel with its value.*/

    int bcnt = 0;/*holds number of black pixels in the image*/
    int i,j,k,l;
    int curarea,winclass,maxarea,winval,onum;

    for (i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {
            if ( blackPixelFlag[i][j][b] == 1)
                bcnt++;
        }
    }

    while(bcnt>0)
    {
        for (i=0;i<rows;i++)
        {
            for(j=0;j<cols;j++)
            {
                if (blackPixelFlag[i][j][b] == 1)
                {
                    maxarea = -1;
                    winval = 0;
                    winclass = -1;
                    for (k=i-1;k<=i+1;k++)
                    {
                        for (l=j-1;l<=j+1;l++)
                        {
                            if ((k>=0 && k<rows && l>=0 && l<cols) && (k!=i || l!=j) && fimage[k][l][b]!=-1)
                            {
                                onum = segmap[k][l];

                                curarea = object_info1[onum][1];

                                if (curarea > maxarea)
                                {
                                    maxarea = curarea;
                                    winval = fimage[k][l][b];
                                    winclass = onum;
                                }
                            }
                        }
                    }

                    if (maxarea>0)/*this means it has been set by some pixel*/
                    {
                        fimage[i][j][b] = winval;
                       // segmap[i][j] = winclass;
                        bcnt--;
                    }
                }

            }
        }
    }


}

void MainWindow_fusion::Smoothing_GLSI(unsigned char ***&fimage,int **segmap,int b,unsigned char ***&sfimage,int rows,int cols)
{
    /*This code smoothes the edges of objects fused by GLSI algorithm.*/
    int minimumArea = atoi(ui->lineEditArea->text().toUtf8().data());

    int i,j,k,l;
    int start,flag_do,brovey,glsi,sum,cnt,curclass;

    for (i=0;i<rows;i++)
    {
        for (j=0;j<cols;j++)
        {
            start = 0;
            flag_do = 0;
            glsi = 1;
            brovey = 1;
            sum = 0;
            cnt = 0;
            for (k=i-1;k<=i+1;k++)
            {
                for (l=j-1;l<=j+1;l++)
                {
                    if (k>=0 && k<rows && l>=0 && l<cols)
                    {
                        sum = sum + fimage[k][l][b];
                        cnt++;
                        if (start == 0)
                        {
                            curclass = segmap[k][l];//initialising ideal class value for whole window.
                            start = 1;


                            if (object_info1[curclass][1]>=minimumArea)/*this means that this pixel was fused
                                                                        using GLSI algorithm*/
                            {
                                brovey = 0;
                            }
                            else
                                glsi = 0;


                            continue;
                        }

                        if (segmap[k][l]!=curclass)
                        {
                            flag_do = 1;/*this means we have more than one object in this window*/
                        }

                        if (object_info1[segmap[k][l]][1]>=minimumArea)/*this means that this pixel was fused
                                                                    using GLSI algorithm*/
                        {
                            brovey = 0;
                        }
                        else
                            glsi = 0;

                    }
                }
            }

            sum = sum - (int)fimage[i][j][b];


            if (flag_do == 0)/*this means we have only object in the window..so no need of smoothing*/
            {
                sfimage[i][j][b] = fimage[i][j][b];
                continue;
            }

            if (brovey == 1)/*this means that we have more than one object but all are in brovey so no need of smoothing */
            {
                sfimage[i][j][b] = fimage[i][j][b];
                continue;
            }

            /*if we are at this stage that would mean that we have to smoothing*/

            sfimage[i][j][b] = (unsigned char)floor((sum*1.0)/cnt);

        }
    }
}

void MainWindow_fusion::writePPMImage(unsigned char ***op,int img1_size[],std::string fname)
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
void MainWindow_fusion::on_pushButton_clicked()
{
	std::cout << glsiclicked << std::endl;
    std::cout << postprocessing << std::endl;


    int minimumArea = atoi(ui->lineEditArea->text().toUtf8().data());
    float threshold = atof(ui->lineEditTh->text().toUtf8().data());
    std::cout << minimumArea << std::endl;
    std::cout << threshold << std::endl;
    std::string fname = "C:/Windows/Temp/fusion.ppm";
    int glsi_count = 0;
    int not_glsi_count = 0;


	if(glsiclicked == 1){
        std::cout << "GLSI CLICKED" << std::endl;

        dataf = (unsigned char ***)malloc(sizeof(unsigned char **)*imgsize[0]);

        for(int i=0; i<imgsize[0]; i++)
        {
            dataf[i] = (unsigned char **)malloc(sizeof(unsigned char *)*imgsize[1]);
            for(int j=0; j<imgsize[1]; j++)
                dataf[i][j] = (unsigned char *)malloc(sizeof(unsigned char)*3);
        }

        std::cout<<"after data dataf and sf init"<<std::endl;
        std::vector< Point> pointCollection;
        std::vector< Cluster> clusters;

        blackPixelFlag = (char ***)malloc(sizeof(char **)*imgsize[0]);
        for(int i=0; i<imgsize[0]; i++)
        {
            blackPixelFlag[i] = (char **)malloc(sizeof(char *)*imgsize[1]);
            for(int j=0;j<imgsize[1];j++)
                blackPixelFlag[i][j] = (char *)malloc(sizeof(char)*3);
        }

        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
            {
                blackPixelFlag[i][j][0] = 0;
                blackPixelFlag[i][j][1] = 0;
                blackPixelFlag[i][j][2] = 0;
            }

        for(int b = 0; b < 3 ; b++)
        {
            for(int i=0;i<NO1;i++)
            {
                //std::cout << "fusion call for object : " << i << " size: " << object_info1[i][1]<< std::endl;

                if (object_info1[i][1] < minimumArea) /* Keeping the code generic so that we can change the attribute.*/
                {

                    //Fusion_Brovey(labels1,dataf,data1,data2,i,object_info1[i][2],object_info1[i][3],object_info1[i][4],object_info1[i][5],b);
                    if(b==0)
                    {
                        not_glsi_count++;
                        for(int j=object_info1[i][2]; j<=object_info1[i][3]; j++)
                        {
                            for(int k=object_info1[i][4]; k<=object_info1[i][5]; k++)
                            {
                                if(labels1[j][k] == i)
                                {
                                    struct Point temp;
                                    temp.x = j;
                                    temp.y = k;
                                    temp.clusterId = -1;
                                    pointCollection.push_back(temp);
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(b==0)
                        glsi_count++;
                    Fusion_GLSI(labels1,dataf,data1,data2,i,object_info1[i][2],object_info1[i][3],object_info1[i][4],object_info1[i][5],b);
                }
            }
        }


        //Do K-means clustering here for the objects which are lesser in area than the minimum area parameter
        if(pointCollection.size() > 0)
        {
            int numberOfClusters = 5;
            for(int i=0; i<5; i++)
            {
                struct Cluster temp;
                int index = (pointCollection.size()/5)*i;
                temp.r = data1[(pointCollection[index]).x][(pointCollection[index]).y][0];
                temp.g = data1[(pointCollection[index]).x][(pointCollection[index]).y][1];
                temp.b = data1[(pointCollection[index]).x][(pointCollection[index]).y][2];

                clusters.push_back(temp);

            }
            int movement = 0;
            while(1)
            {
                movement = 0;
                for(int i=0; i<pointCollection.size(); i++)
                {
                    int pred = data1[(pointCollection[i]).x][(pointCollection[i]).y][0];
                    int pgreen = data1[(pointCollection[i]).x][(pointCollection[i]).y][1];
                    int pblue = data1[(pointCollection[i]).x][(pointCollection[i]).y][2];

                    double min_dist = 100000;
                    int min_dist_cluster = -1;
                    for(int j=0; j<5; j++)
                    {
                        double cred = clusters[j].r;
                        double cgreen = clusters[j].g;
                        double cblue = clusters[j].b;

                        double distance = sqrt((pred-cred)*(pred-cred) + (pgreen-cgreen)*(pgreen-cgreen) + (pblue-cblue)*(pblue-cblue));
                        if(distance < min_dist)
                        {
                            min_dist = distance;
                            min_dist_cluster = j;
                        }
                    }
                    if(pointCollection[i].clusterId != min_dist_cluster)
                    {
                        pointCollection[i].clusterId = min_dist_cluster;
                        movement++;
                    }

                }
                if(movement ==0)
                    break;
                else
                {
                    for(int j=0; j<5; j++)
                    {
                        double sumr=0,sumg=0,sumb=0,noPoints=0;
                        for(int i=0; i<pointCollection.size(); i++)
                        {
                            if(pointCollection[i].clusterId == j)
                            {
                                sumr += data1[(pointCollection[i]).x][(pointCollection[i]).y][0];
                                sumg += data1[(pointCollection[i]).x][(pointCollection[i]).y][1];
                                sumb += data1[(pointCollection[i]).x][(pointCollection[i]).y][2];

                                noPoints++;

                            }
                        }
                        clusters[j].r = sumr/noPoints;
                        clusters[j].g = sumg/noPoints;
                        clusters[j].b = sumb/noPoints;
                    }
                }
            }

            FILE *cluster_info;
            cluster_info = fopen("C:/Windows/Temp/cluster_info.txt","w");
            for(int i=0; i<pointCollection.size(); i++)
            {
                fprintf(cluster_info,"%d %d %d\n",pointCollection[i].x,pointCollection[i].y,pointCollection[i].clusterId);
            }
            fclose(cluster_info);

            int **clusteredLabels;
            clusteredLabels = (int **)malloc(sizeof(int *)*imgsize[0]);
            for(int i=0; i< imgsize[0]; i++)
                clusteredLabels[i] = (int *)malloc(sizeof(int )*imgsize[1]);

            for(int i=0; i<imgsize[0]; i++)
                for(int j=0; j<imgsize[1]; j++)
                    clusteredLabels[i][j] = -1;

            for(int i=0; i<pointCollection.size(); i++)
            {
                int x = pointCollection[i].x;
                int y = pointCollection[i].y;
                clusteredLabels[x][y] = pointCollection[i].clusterId;
            }

            for(int i=0; i<5; i++)
            {
                int xmin = imgsize[0] + 1;
                int xmax = -1;
                int ymin = imgsize[1] +1;
                int ymax = -1;
                for(int j=0; j<pointCollection.size(); j++)
                {
                    if(pointCollection[j].clusterId == i)
                    {
                        if(xmin > pointCollection[j].x)
                            xmin = pointCollection[j].x;
                        if(xmax < pointCollection[j].x)
                            xmax = pointCollection[j].x;

                        if(ymin > pointCollection[j].y)
                            ymin = pointCollection[j].y;
                        if(ymax < pointCollection[j].y)
                            ymax = pointCollection[j].y;
                    }

                }
                Fusion_GLSI(clusteredLabels,dataf,data1,data2,i,xmin,xmax,ymin,ymax,0);
                Fusion_GLSI(clusteredLabels,dataf,data1,data2,i,xmin,xmax,ymin,ymax,1);
                Fusion_GLSI(clusteredLabels,dataf,data1,data2,i,xmin,xmax,ymin,ymax,2);


            }
            for(int i=0; i<imgsize[0]; i++)
                free(clusteredLabels[i]);
            free(clusteredLabels);
        }

        FillBlackPixels(dataf,imgsize[0],imgsize[1],labels1,0);
        FillBlackPixels(dataf,imgsize[0],imgsize[1],labels1,1);
        FillBlackPixels(dataf,imgsize[0],imgsize[1],labels1,2);
        if (postprocessing == 1)
        {
//            for(int i=0; i<imgsize[0]; i++)
//            {
//                for(int j=0; j<imgsize[1]; j++)
//                {
//                    free(data1[i][j]);
//                    free(data2[i][j]);
//                }
//                free(data1[i]);
//                free(data2[i]);
//            }
//            free(data1);
//            free(data2);

            datasf = (unsigned char ***)malloc(sizeof(unsigned char **)*imgsize[0]);

            for(int i=0; i<imgsize[0]; i++)
            {
                datasf[i] = (unsigned char **)malloc(sizeof(unsigned char *)*imgsize[1]);

                for(int j=0; j<imgsize[1]; j++)
                    datasf[i][j] = (unsigned char *)malloc(sizeof(unsigned char)*3);
            }
            Smoothing_GLSI(dataf,labels1,0,datasf,imgsize[0],imgsize[1]);
            Smoothing_GLSI(dataf,labels1,1,datasf,imgsize[0],imgsize[1]);
            Smoothing_GLSI(dataf,labels1,2,datasf,imgsize[0],imgsize[1]);

            for(int i=0; i<imgsize[0]; i++)
            {
                for(int j=0; j<imgsize[1]; j++)
                {
                    free(dataf[i][j]);
                }
                free(dataf[i]);
            }
            free(dataf);

        }

        for(int i=0; i<imgsize[0]; i++)
        {
           // blackPixelFlag[i] = (char **)malloc(sizeof(char)*imgsize[1]);
            for(int j=0;j<imgsize[1];j++)
                free(blackPixelFlag[i][j]);
            free(blackPixelFlag[i]);
        }
        free(blackPixelFlag);


        std::cout << "GLSI Done for " <<glsi_count << " objects leaving " << not_glsi_count<< " objects"<< std::endl;
        if(postprocessing == 1)
        {

            writePPMImage(datasf,imgsize,fname);

        }
        else
        {

            writePPMImage(dataf,imgsize,fname);

        }


        ui->textBrowser->setText("Fusion Done. File written by GLSI in Temp dir.");
        std::cout << "file written by glsi" << std::endl;
    }
    else{
        dataf = (unsigned char ***)malloc(sizeof(unsigned char **)*imgsize[0]);
        for(int i=0; i<imgsize[0]; i++)
        {
            dataf[i] = (unsigned char **)malloc(sizeof(unsigned char *)*imgsize[1]);
            for(int j=0; j<imgsize[1]; j++)
                dataf[i][j] = (unsigned char *)malloc(sizeof(unsigned char)*3);
        }
        for(int b = 0; b < 3 ; b++){
            for(int i=0;i<NO1;i++)
            {

                //std::cout << i << std::endl;
                Fusion_Brovey(labels1,dataf,data1,data2,i,object_info1[i][2],object_info1[i][3],object_info1[i][4],object_info1[i][5],b);

            }
        }

        writePPMImage(dataf,imgsize,fname);

        ui->textBrowser->setText("Fusion Done. File written by Brovey in Temp dir.");
        std::cout << "file written by brovey" << std::endl;
    }

    if(glsiclicked == 1)
    {
        if(postprocessing == 1)
            iFused = Image(imgsize[0],imgsize[1],3,datasf);
        else
            iFused = Image(imgsize[0],imgsize[1],3,dataf);
    }
    else
        iFused = Image(imgsize[0],imgsize[1],3,dataf);

}

void MainWindow_fusion::GetMinor(float **src, float **&dest, int row, int col, int order)
{
    // indicate which col and row is being copied to dest
    int colCount=0,rowCount=0;

    for(int i = 0; i < order; i++ )
    {
        if( i != row )
        {
            colCount = 0;
            for(int j = 0; j < order; j++ )
            {
                // when j is not the element
                if( j != col )
                {
                    dest[rowCount][colCount] = src[i][j];
                    colCount++;
                }
            }
            rowCount++;
        }
    }

}

float MainWindow_fusion::CalcDeterminant( float **mat, int order)
{
    // order must be >= 0
   // std::cout<<"entered det func with order: "<< order <<std::endl;
    // stop the recursion when matrix is a single element
    if( order == 1 )
    {
        //std::cout << "exited det func with order: "<< order <<std::endl;
        return mat[0][0];
    }

    // the determinant value
    float det = 0;

    // allocate the cofactor matrix
    float **minor = (float **)malloc(sizeof(float *)*(order-1));
    for(int i=0;i<order-1;i++)
        minor[i] = (float *)malloc(sizeof(float)*(order-1));

    for(int i = 0; i < order; i++ )
    {
        // get minor of element (0,i)
        GetMinor( mat, minor, 0, i , order);
        // the recusion is here!

        det += (i%2==1?-1.0:1.0) * mat[0][i] * CalcDeterminant(minor,order-1);
        //det += pow( -1.0, i ) * mat[0][i] * CalcDeterminant( minor,order-1 );
    }

    //std::cout << "exited det func with order: "<< order <<std::endl;
    return det;
}


void MainWindow_fusion::MatrixInversion(float **A, int order, float **&Y)
{
    // get the determinant of a
    //std::cout<<"before calculating det"<<std::endl;
    float det = 1.0/CalcDeterminant(A,order);
   // std::cout<<"after calculating det"<<std::endl;

    // memory allocation
    float *temp = (float *)malloc(sizeof(float)*(order-1)*(order-1));
    float **minor = (float **)malloc(sizeof(float *)*(order-1));
    for(int i=0;i<order-1;i++)
        minor[i] = (float *)malloc(sizeof(float)*(order-1));

    std::cout<<"inside inverse matrix function"<<std::endl;
    for(int j=0;j<order;j++)
    {
        for(int i=0;i<order;i++)
        {
            //std::cout<<"i j (before minor): "<<i<<j<<std::endl;
            std::cout<<std::endl;
            // get the co-factor (matrix) of A(j,i)
            GetMinor(A,minor,j,i,order);
            //std::cout<<"i j (after minor): "<<i<<j<<std::endl;
            Y[i][j] = det*CalcDeterminant(minor,order-1);
           // std::cout<<"i j (after det): "<<i<<j<<std::endl;
            if( (i+j)%2 == 1)
                Y[i][j] = -Y[i][j];
        }
    }
    std::cout<<"after the last loop"<<std::endl;

}

float **MainWindow_fusion::getCovarianceMartix(int numberOfSamples, float **dataArray, float **meanVector, int classNumber, int ndims)
{
    int i, j, k;

    //malloc space for cov-matrix here
    float **cov = (float **)malloc(sizeof(float *)*ndims);
    for(i=0; i<ndims; i++)
        cov[i] = (float *)malloc(sizeof(float)*ndims);

    for(i=0; i<ndims; i++)
        for(j=0; j<ndims; j++)
            cov[i][j] = 0.0;

    for(int i=0; i<numberOfSamples; i++){
        for(int j=0; j<ndims; j++)
            std::cout << dataArray[i][j] << " ";
        std::cout << "\n";
    }

    for(i=0; i<numberOfSamples; i++)
        for(j=0; j<ndims; j++)
            dataArray[i][j] -= meanVector[classNumber][j];

    std::cout << "mean vector" << std::endl;
    for(int i=0; i<ndims; i++)
        std::cout << meanVector[classNumber][i] << " ";
    std::cout << "\n";

    std::cout << "data matrix"<< std::endl;
    for(int i=0; i<numberOfSamples; i++){
        for(int j=0; j<ndims; j++)
            std::cout << dataArray[i][j] << " ";
        std::cout << "\n";
    }


    for (i=0; i< ndims; i++) {
        for (j = i; j < ndims; j++) {
            for (k=0; k < numberOfSamples; k++) {
                float val = dataArray[k][i]*dataArray[k][j];
                cov[i][j] += val;
            }
            cov[i][j] = cov[i][j]/(1.0*numberOfSamples);
            cov[j][i] = cov[i][j];
        }
    }
    std::cout << "cov mat in function" << std::endl;
    for(int i=0; i<ndims; i++){
        for(int j=0; j<ndims; j++)
            std::cout << cov[i][j] << " ";
        std::cout << "\n";
    }

    return(cov);
}

float MainWindow_fusion::calculateMahaDist(float *sample, float **class_mean, float **full_cov_mat, int class_number, int ndims)
{
    float term = 0.0;

    float *mat1 = (float *)malloc(sizeof(float)*ndims);
    for(int i=0; i<ndims; i++)
        mat1[i] = 0.0;

    for(int i=0; i<ndims; i++)
        for(int j=0; j<ndims; j++)
            mat1[i] += (sample[j] - class_mean[class_number][j]) * (full_cov_mat[class_number*ndims + j][i]);

    for(int i=0; i<ndims; i++)
        term += mat1[i] * (sample[i] - class_mean[class_number][i]);


    return(term);

}

void MainWindow_fusion::classification(){

    int ndims=0;
    for(int i=0;i<20;i++)
    {
        if(cAtt->classify[i] == 1)
            ndims++;
    }
    float area,roundness,perimeter,compactness,std_r,std_g,std_b,std_rf,std_gf,std_bf,mean_r,mean_g,mean_b,mean_rf,mean_gf,mean_bf;

    int current_class=0, i = 0, k;// ndims = 7; //ndims is the number of dimensions of the data samples
    int class_num = int (class_m_1.size());
    numberOfClasses = class_num;


    float value_r, value_g, value_b, texture_coarseness, texture_direction, texture_contrast, texture_roughness;
    float **class_mean;

    class_mean = (float **)malloc(sizeof(float *)*class_num);
    for(int i=0; i<class_num; i++)
        class_mean[i] = (float *)malloc(sizeof(float)*ndims);
    for(int i=0; i<class_num; i++)
        for(int j=0; j<ndims; j++)
            class_mean[i][j] = 0.0;

    float **full_cov_mat = (float **)malloc(sizeof(float *)*(class_num*ndims));
    for(int i=0; i<class_num*ndims; i++)
        full_cov_mat[i] = (float *)malloc(sizeof(float)*ndims);

    float **class_cov_mat;
    float **inverted_class_cov_mat = (float **)malloc(sizeof(float *)*ndims);
    for(int i=0; i<ndims; i++)
        inverted_class_cov_mat[i] = (float *)malloc(sizeof(float)*ndims);


    std::cout << "before the loop: calculating class means" << std::endl;
    while(current_class < class_num)
    {
        std::cout << "classno: " << current_class << std::endl;
        i = 0 ;
        std::vector <int> tempvect = class_m_1[current_class];

        //create data matrix here
        float **data_samples = (float **)malloc(sizeof(float *)*(tempvect.size()));
        for(int p=0; p<tempvect.size(); p++)
            data_samples[p] = (float *)malloc(sizeof(float)*ndims);

        for(int m=0; m<ndims; m++)
            for(int n=0; n<ndims; n++ )
                inverted_class_cov_mat[m][n] = 0.0;
        std::cout << "selected class: " << current_class << std::endl;
        while(i < tempvect.size() )
        {
            k = tempvect[i];
            int j=0;

            if(cAtt->classify[0] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.area;
            if(cAtt->classify[1] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.perimeter;
            if(cAtt->classify[2] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.roundness;
            if(cAtt->classify[3] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.compactness;
            if(cAtt->classify[4] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.contrast;
            if(cAtt->classify[5] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.roughness;
            if(cAtt->classify[6] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.coarseness;
            if(cAtt->classify[7] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.direction;


            mean_r =0;mean_g=0;mean_b=0;
            std_r =0; std_g=0;std_b=0;

            //calculating spectral features from MSS image with the help of (x,y) coordinates of PAN image
            int x1,x2,y1,y2;
            x1 = i1.objectArray[k].xmin;
            x2 = i1.objectArray[k].xmax;
            y1 = i1.objectArray[k].ymin;
            y2 = i1.objectArray[k].ymax;
           // std::cout << "bb values: " << x1 << " "<<x2 << " "<< y1 << " " <<y2 << std::endl;
//            for(int ii=x1; ii<=x2; ii++)
//            {
//                for(int jj=y1; jj<=y2; jj++)
//                {
//                    if(i1.objectArray[k].mask[ii-x1][jj-y1])
//                    {
//                       // std::cout << data1[ii][jj][0] <<" "<< data1[ii][jj][1] << " " << data1[ii][jj][2] << std::endl;
//                        if(cAtt->classify[8] == 1 || cAtt->classify[11] == 1)
//                            mean_r += data1[ii][jj][0];
//                        if(cAtt->classify[9] == 1 || cAtt->classify[12] == 1)
//                            mean_g += data1[ii][jj][1];
//                        if(cAtt->classify[10] == 1 || cAtt->classify[13] == 1)
//                            mean_b += data1[ii][jj][2];
//                        if(cAtt->classify[14] == 1 || cAtt->classify[17] == 1)
//                            mean_r += iFused.data[ii][jj][0];
//                        if(cAtt->classify[15] == 1 || cAtt->classify[18] == 1)
//                            mean_g += iFused.data[ii][jj][1];
//                        if(cAtt->classify[16] == 1 || cAtt->classify[19] == 1)
//                            mean_b += iFused.data[ii][jj][2];
//                    }
//                }
//            }
//            if(cAtt->classify[8] == 1 || cAtt->classify[11] == 1)
//                mean_r /= i1.objectArray[k].NOP;
//            if(cAtt->classify[9] == 1 || cAtt->classify[12] == 1)
//                mean_g /= i1.objectArray[k].NOP;
//            if(cAtt->classify[10] == 1 || cAtt->classify[13] == 1)
//                mean_b /= i1.objectArray[k].NOP;
//            if(cAtt->classify[14] == 1 || cAtt->classify[17] == 1)
//                mean_r /= i1.objectArray[k].NOP;
//            if(cAtt->classify[15] == 1 || cAtt->classify[18] == 1)
//                mean_g /= i1.objectArray[k].NOP;
//            if(cAtt->classify[16] == 1 || cAtt->classify[19] == 1)
//                mean_b /= i1.objectArray[k].NOP;


//            for(int ii=x1; ii<=x2; ii++)
//            {
//                for(int jj=y1; jj<=y2; jj++)
//                {
//                    if(i1.objectArray[k].mask[ii-x1][jj-y1])
//                    {
//                        if(cAtt->classify[11] == 1)
//                            std_r = std_r + ((data1[ii][jj][0] - mean_r) * (data1[ii][jj][0] - mean_r));
//                        if(cAtt->classify[12] == 1)
//                            std_g = std_g + ((data1[ii][jj][1] - mean_g) * (data1[ii][jj][1] - mean_g));
//                        if(cAtt->classify[13] == 1)
//                            std_b = std_b + ((data1[ii][jj][2] - mean_b) * (data1[ii][jj][2] - mean_b));
//                        if(cAtt->classify[17] == 1)
//                            std_r = std_r + ((iFused.data[ii][jj][0] - mean_r) * (iFused.data[ii][jj][0] - mean_r));
//                        if(cAtt->classify[18] == 1)
//                            std_g = std_g + ((iFused.data[ii][jj][1] - mean_g) * (iFused.data[ii][jj][1] - mean_g));
//                        if(cAtt->classify[19] == 1)
//                            std_b = std_b + ((iFused.data[ii][jj][2] - mean_b) * (iFused.data[ii][jj][2] - mean_b));
//                    }
//                }
//            }

//            if(cAtt->classify[11] == 1)
//                std_r = sqrt(std_r / i1.objectArray[k].NOP);
//            if(cAtt->classify[12] == 1)
//                std_g = sqrt(std_g / i1.objectArray[k].NOP);
//            if(cAtt->classify[13] == 1)
//                std_b = sqrt(std_b / i1.objectArray[k].NOP);
//            if(cAtt->classify[17] == 1)
//                std_r = sqrt(std_r / i1.objectArray[k].NOP);
//            if(cAtt->classify[18] == 1)
//                std_g = sqrt(std_g / i1.objectArray[k].NOP);
//            if(cAtt->classify[19] == 1)
//                std_b = sqrt(std_b / i1.objectArray[k].NOP);

            if(cAtt->classify[8] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.meanMSS[0];
            if(cAtt->classify[9] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.meanMSS[1];
            if(cAtt->classify[10] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.meanMSS[2];
            if(cAtt->classify[11] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.stdMSS[0];
            if(cAtt->classify[12] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.stdMSS[1];
            if(cAtt->classify[13] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.stdMSS[2];

            if(cAtt->classify[14] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.meanFused[0];
            if(cAtt->classify[15] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.meanFused[1];
            if(cAtt->classify[16] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.meanFused[2];
            if(cAtt->classify[17] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.stdFused[0];
            if(cAtt->classify[18] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.stdFused[1];
            if(cAtt->classify[19] == 1)
                data_samples[i][j++] = i1.objectArray[k].fVector.stdFused[2];



            for(int l=0;l<j;l++)
            {
                class_mean[current_class][l] += data_samples[i][l];
            }

            i++;
        }
        for(int l=0;l<ndims;l++)
        {
            class_mean[current_class][l] /= tempvect.size();
        }


        std::cout << "before call to cov mat function"<< std::endl;
        // call covariance matrix function here
        class_cov_mat = getCovarianceMartix(tempvect.size(), data_samples, class_mean, current_class, ndims);
        std::cout << "after call to cov mat function"<< std::endl;

        // call Matrix inversion function here
        std::cout << "before call to inverse matrix function"<< std::endl;
        MatrixInversion(class_cov_mat, ndims, inverted_class_cov_mat);
        std::cout << "after call to inverse matrix function"<< std::endl;

        for(int m=0; m<ndims; m++)
            for(int n=0; n<ndims; n++)
                full_cov_mat[current_class*ndims + m][n] = inverted_class_cov_mat[m][n];


        current_class++;
    }
    std::cout << "full covariance matrix"<< std::endl;
    for(int i=0; i<class_num*ndims; i++){
        for(int j=0; j<ndims; j++)
            std::cout << full_cov_mat[i][j] << " ";
        std::cout << "\n";
    }


    float min_dev=10000000;
    int j = 0;
    float *sample= (float *)malloc(sizeof(float)*ndims);
    float mDist=0.0;

    int *dimensions = (int *)malloc(sizeof(int)*ndims);
    int jj=0;
    for(int i=0;i<20;i++)
    {
        if(cAtt->classify[i]==1)
            dimensions[jj++] = i;
    }

    //  FILE *new_class;
    // new_class = fopen("C:\\Windows\\Temp\\new_class_1.txt","w");

    while( j < NO1 )
    {
        int k=0;
            sample[k++] = i1.objectArray[j].fVector.area;

            sample[k++] = i1.objectArray[j].fVector.perimeter;

            sample[k++] = i1.objectArray[j].fVector.roundness;

            sample[k++] = i1.objectArray[j].fVector.compactness;
            sample[k++] = i1.objectArray[j].fVector.contrast;
            sample[k++] = i1.objectArray[j].fVector.roughness;
            sample[k++] = i1.objectArray[j].fVector.coarseness;
            sample[k++] = i1.objectArray[j].fVector.direction;

        mean_r =0;mean_g=0;mean_b=0;
        mean_rf =0;mean_gf=0;mean_bf=0;
        std_r =0; std_g=0;std_b=0;
        std_rf =0; std_gf=0;std_bf=0;

        //calculating spectral features from MSS image with the help of (x,y) coordinates of PAN image
        int x1,x2,y1,y2;
        x1 = i1.objectArray[j].xmin;
        x2 = i1.objectArray[j].xmax;
        y1 = i1.objectArray[j].ymin;
        y2 = i1.objectArray[j].ymax;
       // std::cout << "bb values: " << x1 << " "<<x2 << " "<< y1 << " " <<y2 << std::endl;
//        for(int ii=x1; ii<=x2; ii++)
//        {
//            for(int jj=y1; jj<=y2; jj++)
//            {
//                if(i1.objectArray[j].mask[ii-x1][jj-y1])
//                {
//                   // std::cout << data1[ii][jj][0] <<" "<< data1[ii][jj][1] << " " << data1[ii][jj][2] << std::endl;

//                                            mean_r += data1[ii][jj][0];
//                                            mean_g += data1[ii][jj][1];
//                                            mean_b += data1[ii][jj][2];
//                                            mean_rf += iFused.data[ii][jj][0];
//                                            mean_gf += iFused.data[ii][jj][1];
//                                            mean_bf += iFused.data[ii][jj][2];
//                }
//            }
//        }

//            mean_r /= i1.objectArray[j].NOP;
//            mean_g /= i1.objectArray[j].NOP;
//            mean_b /= i1.objectArray[j].NOP;
//            mean_rf /= i1.objectArray[j].NOP;
//            mean_gf /= i1.objectArray[j].NOP;
//            mean_bf /= i1.objectArray[j].NOP;

//        for(int ii=x1; ii<=x2; ii++)
//        {
//            for(int jj=y1; jj<=y2; jj++)
//            {
//                if(i1.objectArray[j].mask[ii-x1][jj-y1])
//                {
//                        std_r = std_r + ((data1[ii][jj][0] - mean_r) * (data1[ii][jj][0] - mean_r));
//                        std_g = std_g + ((data1[ii][jj][1] - mean_g) * (data1[ii][jj][1] - mean_g));
//                        std_b = std_b + ((data1[ii][jj][2] - mean_b) * (data1[ii][jj][2] - mean_b));
//                        std_rf = std_rf + ((iFused.data[ii][jj][0] - mean_rf) * (iFused.data[ii][jj][0] - mean_rf));
//                        std_gf = std_gf + ((iFused.data[ii][jj][1] - mean_gf) * (iFused.data[ii][jj][1] - mean_gf));
//                        std_bf = std_bf + ((iFused.data[ii][jj][2] - mean_bf) * (iFused.data[ii][jj][2] - mean_b));
//                }
//            }
//        }

//        //here
//            std_r = sqrt(std_r / i1.objectArray[j].NOP);
//            std_g = sqrt(std_g / i1.objectArray[j].NOP);
//            std_b = sqrt(std_b / i1.objectArray[j].NOP);
//            std_rf = sqrt(std_rf / i1.objectArray[j].NOP);
//            std_gf = sqrt(std_gf / i1.objectArray[j].NOP);
//            std_bf = sqrt(std_bf / i1.objectArray[j].NOP);

            sample[k++] = i1.objectArray[j].fVector.meanMSS[0];
            sample[k++] = i1.objectArray[j].fVector.meanMSS[1];
            sample[k++] = i1.objectArray[j].fVector.meanMSS[2];
            sample[k++] = i1.objectArray[j].fVector.stdMSS[0];
            sample[k++] = i1.objectArray[j].fVector.stdMSS[1];
            sample[k++] = i1.objectArray[j].fVector.stdMSS[2];

            sample[k++] = i1.objectArray[j].fVector.meanFused[0];
            sample[k++] = i1.objectArray[j].fVector.meanFused[1];
            sample[k++] = i1.objectArray[j].fVector.meanFused[2];
            sample[k++] = i1.objectArray[j].fVector.stdFused[0];
            sample[k++] = i1.objectArray[j].fVector.stdFused[1];
            sample[k++] = i1.objectArray[j].fVector.stdFused[2];

            float *final_sample = (float *)malloc(sizeof(float )*ndims);
            int ii=0;
            for(i=0; i<20; i++)
            {
                if(cAtt->classify[i]==1)
                {
                    final_sample[ii++] = sample[i];
                //final_sample[i] = sample[dimensions[i]];
                }
            }
        min_dev = 10000000.0;
        mDist = 0.0;
        int class_flag=0;
        for(i=0; i<class_num; i++)
        {
            //call Mahalanobis Distance function for each object here
            mDist = calculateMahaDist(final_sample,class_mean,full_cov_mat,i,ndims);

            if(mDist < min_dev)
            {
                //my_new_class = i;
                i1.objectArray[j].fVector.classNumber = i+1; // i+1 : so that the class number starts from 1 instead of 0
                class_flag = 1;
                min_dev = mDist;
            }
        }
        if(class_flag == 0)
        {
            //std::cout << sample[0] << " " <<  sample[1] << " " <<  sample[2] << " " <<  sample[3] << " " <<  sample[4] << " " <<  sample[5] << " " <<  sample[6] << " " << std::endl;
            for(int cs=0; cs<class_num; cs++){
                float dis = calculateMahaDist(final_sample,class_mean,full_cov_mat,cs,ndims);
                std::cout<< "mah dist: " << dis << std::endl;
            }
            std::cout << "object no. creating problem: " << j << std::endl;
        }

        //    fprintf(new_class,"%d\n",i1.objectArray[j].fVector.classNumber);
        j++;

    }
    class_m_1.clear();
    std::cout<<"exiting classification"<<std::endl;
    //  fclose(new_class);

}

void MainWindow_fusion:: insertvalue(int n, int x, int y,int **labels, int img)
{

    int val = labels[y][x];
    std::cout << "val" << std::endl;


    std::cout << val << std::endl;
    std::cout <<"class_no : "<< n << " Img No: "<< img<<std::endl;
    std::vector<int> temp;    
    if (img==1)
        temp = class_m_1[n];
    //else temp = class_m_2[n];
    //std::cout<<"huddle 1"<<std::endl;
    if(temp.size()!=0)
    {
        temp.push_back(val);
        if (img==1)
            class_m_1[n] = temp;
      //  else
        //    class_m_2[n] = temp;

        std::cout << "exisiting" << std::endl;
        std::cout<<"huddle 2"<<std::endl;
    }
    else
    {
        //std::cout<<"huddle 3 i"<<std::endl;
        std::vector<int> newtemp;
       //std::cout<<"huddle 3 ii"<<std::endl;
        newtemp.push_back(val);
       // std::cout<<"huddle 3 iii"<<std::endl;
        if (img==1)
            class_m_1[n]= newtemp;
        //std::cout<<"huddle 3 iv"<<std::endl;
        //else
          //  class_m_2[n]= newtemp;
        std::cout << "new" << std::endl;
        //std::cout<<"huddle 3"<<std::endl;
    }
   // std::cout << "pushed" << std::endl;


}
void MainWindow_fusion::on_pushButton_3_clicked()
{
    cAtt = new ClassifyAttributes(imgsize[0],imgsize[1],labels1,this);
    cAtt->update();
    cAtt->show();

}

void MainWindow_fusion::on_pushButton_2_clicked()
{
    fusedimg = new paint_box("C:/Windows/Temp/fusion.ppm","Fused Image",imgsize[0],imgsize[1]);
    scarea *area = new scarea(fusedimg,this);


    area->show();
//    fusedimg->update();
//    fusedimg->show();
}

void MainWindow_fusion::on_pushButton_4_clicked()
{
    int interval = 255/(numberOfClasses - 1);
    std::cout<<"interval:" << interval << std::endl;
    if(class_m_1.size() == 1)
        interval =0;

    unsigned char ***classifiedImage = (unsigned char ***)malloc(sizeof(unsigned char **)*imgsize[0]);
    for(int i=0; i<imgsize[0]; i++)
    {
        classifiedImage[i] = (unsigned char **)malloc(sizeof(unsigned char *)*imgsize[1]);
        for(int j=0; j<imgsize[1]; j++)
            classifiedImage[i][j] = (unsigned char *)malloc(sizeof(unsigned char)*3);

    }
    for(int i=0; i<imgsize[0]; i++)
    {
        for(int j=0; j<imgsize[1]; j++)
        {
            int O_ID = labels1[i][j];
            int C_ID = i1.objectArray[O_ID].fVector.classNumber;

            classifiedImage[i][j][0] = (unsigned char)(C_ID - 1)*interval;
            classifiedImage[i][j][1] = (unsigned char)(C_ID - 1)*interval;
            classifiedImage[i][j][2] = (unsigned char)(C_ID - 1)*interval;
        }
    }

    std::string fname = "C:/Windows/Temp/classified.ppm";
    writePPMImage(classifiedImage,imgsize,fname);
    classifiedimg = new paint_box(fname,"Classified Image",imgsize[0],imgsize[1]);
    scarea *area = new scarea(classifiedimg,this);
    area->show();
    //    classifiedimg->update();
//    classifiedimg->show();
}

void MainWindow_fusion::on_pushButton_6_clicked()
{
    i1.createFusedObjects(NO1,object_info1,1, data1, iFused.data);
    ui->textBrowser->setText("Attributes calculated. Click on Classification.");
}
