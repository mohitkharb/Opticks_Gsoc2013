#include "mainwindow_cd.h"
#include "ui_mainwindow_cd.h"
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
//#include<gdal\gdal.h>



MainWindow_cd::MainWindow_cd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow_cd)
{
    ui->setupUi(this);
	this->setWindowTitle("Change Detection");
}

MainWindow_cd::~MainWindow_cd()
{
    delete ui;
}

void MainWindow_cd::Show_process(std::string mod)
{

    FILE *procOut = _popen(mod.c_str(), "r");

    if(procOut == NULL)
        exit(1);

    char output[200];
    while(fgets(output, sizeof(output), procOut) != NULL) {
        ui->textBrowser->append(output);
    }
    fclose(procOut);

}

void MainWindow_cd::on_radioButton_clicked()
{
    image_type = 2;
    num_band = 3;
}

void MainWindow_cd::on_radioButton_2_clicked()
{
    image_type = 1;
    num_band = 3;
}

void MainWindow_cd::on_pushButton_2_clicked()
{
	 ui->textBrowser->setText("calculating mean shift.");
        if(!data_dir.isEmpty() && !fileName2.isEmpty() && !fileName1.isEmpty())
        {

			
        QList <QString> list = fileName1.split(QRegExp("/"));

        QString img1_fullname = list.last();  
		QList <QString> list1 = img1_fullname.split(QRegExp("\\."));
		 img1_name = list1.first();




        //QString img = ui->lineEdit->text();
        QString sbw = ui->lineEdit->text();
        QString rbw = ui->lineEdit_2->text();
        QString mra = ui->lineEdit_3->text();
      

        //std::cout << img_fullname.to

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
		//		QString tmp1= "Load('";
		//		tmp1.append(tempimg1);
		//		tmp1.append("',IMAGE);\n");
        
                out<<"Load('"<< fileName1 <<"',IMAGE);\n";
            }
            else if(i==23){
                out<<"Save('C:\\Windows\\Temp\\seg_"<< img1_name<<".ppm', PPM, SEGM_IMAGE);\n";

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
        
        mod1.append("\\meanshift.exe ");

        mod1.append(ms_dir);
        mod1.append("\\try");
        model=mod1.toUtf8().data();

        Show_process(model);


		model="move C:\\Windows\\Temp\\map_label_pruning_spurious_regions.txt C:\\Windows\\Temp\\map_label_pruning_spurious_regions_1.txt";

        Show_process(model);



		QList <QString> list2 = fileName2.split(QRegExp("/"));
		QString img2_fullname = list2.last();  
		QList <QString> list3 = img2_fullname.split(QRegExp("\\."));
		 img2_name = list3.first();


        //QString img = ui->lineEdit->text();
        sbw = ui->lineEdit_4->text();
        rbw = ui->lineEdit_5->text();
        mra = ui->lineEdit_6->text();
      

        //std::cout << img_fullname.to
		
        QFile expFile1(file_exp.toUtf8().data());
        expFile1.open(QIODevice::ReadWrite);

        QFile tryFile1(file_try.toUtf8().data());
        tryFile1.open(QIODevice::WriteOnly);

        QTextStream in1(&expFile1);
        QTextStream out1(&tryFile1);
        i=1;
        line = in1.readLine();
        while(!in1.atEnd()){
            if(i==6){
                out1<<"SpatialBandwidth = "<< sbw <<";\n";
            }
            else if(i==7){
                out1<<"RangeBandwidth = "<< rbw <<";\n";
            }
            else if(i==8){
                out1<<"MinimumRegionArea ="<< mra <<";\n";
            }
            else if(i==15){
		//		QString tmp1= "Load('";
		//		tmp1.append(tempimg1);
		//		tmp1.append("',IMAGE);\n");
        
                out1<<"Load('"<< fileName2 <<"',IMAGE);\n";
            }
            else if(i==23){
                out1<<"Save('C:\\Windows\\Temp\\seg_"<< img2_name<<".ppm', PPM, SEGM_IMAGE);\n";

            }
            else{
                out1<<line<<"\n";
            }
            line =in1.readLine();
            i++;
        }
        tryFile1.close();
        expFile1.close();
		

        QString mod2 = ms_dir;
        
        mod2.append("\\meanshift.exe ");

        mod2.append(ms_dir);
        mod2.append("\\try");
        std::string model1 =mod2.toUtf8().data();
		QMessageBox::information(this,"msg",mod2);
		
        Show_process(model1);
		
		model="move C:\\Windows\\Temp\\map_label_pruning_spurious_regions.txt C:\\Windows\\Temp\\map_label_pruning_spurious_regions_2.txt";
        Show_process(model);
		//QMessageBox::information(this,"msg","here3");

		
        }
        else ui->textBrowser->setText("Select directory and file names.");
}

void MainWindow_cd::on_pushButton_3_clicked()
{
	  ui->textBrowser->setText("Change Detection Started.");

    if(!fileName2.isEmpty() && !fileName1.isEmpty() && !ms_dir.isEmpty())
    {
        
        LOW = atof(ui->lineEdit_7->text().toUtf8().data());
        HIGH = atof(ui->lineEdit_8->text().toUtf8().data());
        std::cout <<LOW<<" "<<HIGH<<std::endl;
        //Handle the first image------------------------
        data1 = readPPMImage(imgsize,"C:/Windows/Temp/testing.ppm",num_band);
        std::cout<<"here1"<<std::endl;
        i1 = Image(imgsize[0],imgsize[1],3,data1);

        //reading the labels from meanshift result
        merge_map = (int **)malloc(sizeof(int *)*imgsize[0]);
        for(int i=0; i<imgsize[0]; i++)
            merge_map[i] = (int *)malloc(sizeof(int)*imgsize[1]);

        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                merge_map[i][j] = -1;

        labels1 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions_1.txt",imgsize[0],imgsize[1]);
        FILE *lab = fopen("C:/Windows/Temp/map_label_pruning_spurious_regions_1.txt","w");
        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                fprintf(lab,"%d ",labels1[i][j]);
        fclose(lab);

        //calculating the object's mean color values which will help in stitching the tiles
        object_info1 = prepareObjectInfo(imgsize[0],imgsize[1],labels1,labels1,merge_map,NO1);//using labels two times
        i1.createSegmentedImageInfo(merge_map);
        i1.createObjects(NO1,object_info1,0);

        std::cout<<"after meanshift NO1: "<<NO1<<std::endl;

        for(int i=0; i<imgsize[0]; i++)
            free(labels1[i]);
        free(labels1);

        std::cout<<"after freeing labels - 1"<<std::endl;
        labels1 = stitchChunks("C:/Windows/Temp/map_label_pruning_spurious_regions_1.txt",imgsize[0],imgsize[1],i1);
        std::cout<<"after stitching chunka"<<std::endl;

        lab = fopen("C:/Windows/Temp/map_label_pruning_spurious_regions_1.txt","w");
        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                fprintf(lab,"%d ",labels1[i][j]);
        fclose(lab);


        for(int i=0; i<imgsize[0]; i++)
            free(labels1[i]);
        free(labels1);
        std::cout<<"after freeing labels - 2"<<std::endl;

        free(i1.objectArray);

        labels1 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions_1.txt",imgsize[0],imgsize[1]);

        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                merge_map[i][j] = -1;


        object_info1 = prepareObjectInfo(imgsize[0],imgsize[1],labels1,labels1,merge_map,NO1);//using labels two times
        std::cout<<"after stitching NO1: "<<NO1<<std::endl;
        i1.createSegmentedImageInfo(merge_map);

        for(int i=0; i<imgsize[0]; i++)
        {
            free(merge_map[i]);
        }
        free(merge_map);

        i1.createObjects(NO1,object_info1,1); // this function in turn calls the createAttribute function also


        ///////////////////SECOND IMAGE///////////////////////////////////////
        data2 = readPPMImage(imgsize,"C:/Windows/Temp/testing2.ppm",num_band);
        std::cout<<"here1"<<std::endl;
        i2 = Image(imgsize[0],imgsize[1],3,data2);

        //reading the labels from meanshift result
        merge_map = (int **)malloc(sizeof(int *)*imgsize[0]);
        for(int i=0; i<imgsize[0]; i++)
            merge_map[i] = (int *)malloc(sizeof(int)*imgsize[1]);

        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                merge_map[i][j] = -1;

        labels2 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions_2.txt",imgsize[0],imgsize[1]);
        lab = fopen("C:/Windows/Temp/map_label_pruning_spurious_regions_2.txt","w");
        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                fprintf(lab,"%d ",labels2[i][j]);
        fclose(lab);

        //calculating the object's mean color values which will help in stitching the tiles
        object_info2 = prepareObjectInfo(imgsize[0],imgsize[1],labels2,labels2,merge_map,NO2);//using labels two times
        i2.createSegmentedImageInfo(merge_map);
        i2.createObjects(NO2,object_info2,0);

        std::cout<<"after meanshift NO2: "<<NO2<<std::endl;

        for(int i=0; i<imgsize[0]; i++)
            free(labels2[i]);
        free(labels2);

        std::cout<<"after freeing labels - 1"<<std::endl;
        labels2 = stitchChunks("C:/Windows/Temp/map_label_pruning_spurious_regions_2.txt",imgsize[0],imgsize[1],i2);
        std::cout<<"after stitching chunka"<<std::endl;

        lab = fopen("C:/Windows/Temp/map_label_pruning_spurious_regions_2.txt","w");
        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                fprintf(lab,"%d ",labels2[i][j]);
        fclose(lab);


        for(int i=0; i<imgsize[0]; i++)
            free(labels2[i]);
        free(labels2);
        std::cout<<"after freeing labels - 2"<<std::endl;

        free(i2.objectArray);

        labels2 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions_2.txt",imgsize[0],imgsize[1]);

        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                merge_map[i][j] = -1;


        object_info2 = prepareObjectInfo(imgsize[0],imgsize[1],labels2,labels2,merge_map,NO2);//using labels two times
        std::cout<<"after stitching NO2: "<<NO2<<std::endl;
        i2.createSegmentedImageInfo(merge_map);

        i2.createObjects(NO2,object_info2,1); // this function in turn calls the createAttribute function also

        //both images handled-----------------------------------

        //Chance Detection phase
        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                merge_map[i][j] = -1;

        object_info_intersect = prepareObjectInfo(imgsize[0],imgsize[1],labels1,labels2,merge_map,NO_I);
        std::cout<<"here7"<<std::endl;
		int *meta_info1, *meta_info2;
		meta_info1 = (int *) malloc(sizeof(int)*NO1);
		meta_info2 = (int *) malloc(sizeof(int)*NO2);
        for(int i=0; i<NO1; i++)
            meta_info1[i] = object_info1[i][1];
        for(int i=0; i<NO2; i++)
            meta_info2[i] = object_info2[i][1];

        pieceInfo = changed(imgsize[0],imgsize[1],labels1,labels2,meta_info1,meta_info2,NO1,NO2);
        std::cout<<"here8"<<std::endl;
        classes = changeReporting(imgsize[0],imgsize[1],LOW,HIGH,pieceInfo,merge_map);
        std::cout<<"here9"<<std::endl;

        final_codes = (int **)malloc(sizeof(int *)*imgsize[0]);
        //code.txt
        for(int i=0; i<imgsize[0]; i++)
            final_codes[i] = (int *)malloc(sizeof(int)*imgsize[1]);

        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                final_codes[i][j] = classes[merge_map[i][j] - 2];
        for(int i=0; i<imgsize[0]; i++)
        {
            free(merge_map[i]);
        }
        free(merge_map);
        std::cout<<"end"<<std::endl;
    }
    else ui->textBrowser->setText("select files.");
}

void MainWindow_cd::on_pushButton_clicked()
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
void MainWindow_cd::on_pushButton_4_clicked()
{
	 QFileDialog::Options options;
    QString selectedFilter;
    fileName1 = QFileDialog::getOpenFileName(this,
                                             tr("QFileDialog::getOpenFileName()"),
                                             ui->lineEdit_9->text(),
                                             tr("All Files (*);;Text Files (*.txt)"),
                                             &selectedFilter,
                                             options);


    if (!fileName1.isEmpty())
        ui->lineEdit_9->setText(fileName1);
}
void MainWindow_cd::on_pushButton_5_clicked()
{
	 QFileDialog::Options options;
    QString selectedFilter;
    fileName2 = QFileDialog::getOpenFileName(this,
                                             tr("QFileDialog::getOpenFileName()"),
                                             ui->lineEdit_10->text(),
                                             tr("All Files (*);;Text Files (*.txt)"),
                                             &selectedFilter,
                                             options);

    if (!fileName2.isEmpty())
        ui->lineEdit_10->setText(fileName2);
}