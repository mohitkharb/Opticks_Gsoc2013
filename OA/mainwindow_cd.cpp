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
        
		data1 = readPPMImage(imgsize,fileName1.toUtf8().data(),num_band);
       
        i1 = Image(imgsize[0],imgsize[1],3,data1);
        labels1 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions_1.txt",imgsize[0],imgsize[1]);
        QMessageBox::information(this,"msg","here1");

        merge_map = (int **)malloc(sizeof(int *)*imgsize[0]);
        for(int i=0; i<imgsize[0]; i++)
        merge_map[i] = (int *)malloc(sizeof(int)*imgsize[1]);
		
        for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                merge_map[i][j] = -1;

        object_info1 = prepareObjectInfo(imgsize[0],imgsize[1],labels1,labels1,merge_map,NO1);//using labels two times
        QMessageBox::information(this,"msg","here2");
        i1.createSegmentedImageInfo(merge_map);
        QMessageBox::information(this,"msg","here3");
        i1.createObjects(NO1,object_info1); // this function in turn calls the createAttribute function also
        QMessageBox::information(this,"msg","here4");


        //Handle the second image----------------------------------
		data2 = readPPMImage(imgsize,fileName2.toUtf8().data(),num_band);
        //data2 = readPPMImage(imgsize,ui->lineEdit_2->text().toUtf8().data(),num_band);
		QMessageBox::information(this,"msg","here5");
        i2 = Image(imgsize[0],imgsize[1],3,data2);
        labels2 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions_2.txt",imgsize[0],imgsize[1]);
		QMessageBox::information(this,"msg","here6");
		for(int i=0; i<imgsize[0]; i++)
            for(int j=0; j<imgsize[1]; j++)
                merge_map[i][j] = -1;

        object_info2 = prepareObjectInfo(imgsize[0],imgsize[1],labels2,labels2,merge_map,NO2);//using labels two times
		QMessageBox::information(this,"msg","here7");
        i2.createSegmentedImageInfo(merge_map);
        i2.createObjects(NO2,object_info2); // this function in turn calls the createAttribute function also
		QMessageBox::information(this,"msg","here8");
      
        //both images handled-----------------------------------
        //calling for change detection
		for(int i=0; i<imgsize[0]; i++)
        for(int j=0; j<imgsize[1]; j++)
            merge_map[i][j] = -1;
		QMessageBox::information(this,"msg","here9");
    object_info_intersect = prepareObjectInfo(imgsize[0],imgsize[1],labels1,labels2,merge_map,NO_I);
    int *meta_info1 = (int *)malloc(sizeof(int )*NO1);
	int *meta_info2 = (int *)malloc(sizeof(int )*NO2);
        for(int i=0; i<NO1; i++)
        meta_info1[i] = object_info1[i][1];
    for(int i=0; i<NO2; i++)
        meta_info2[i] = object_info2[i][1];

    pieceInfo = changed(imgsize[0],imgsize[1],labels1,labels2,meta_info1,meta_info2,NO1,NO2);
    QMessageBox::information(this,"msg","here10");
    classes = changeReporting(imgsize[0],imgsize[1],LOW,HIGH,pieceInfo,merge_map);
    QMessageBox::information(this,"msg","here11");
    final_codes = (int **)malloc(sizeof(int *)*imgsize[0]);
    for(int i=0; i<imgsize[0]; i++)
        final_codes[i] = (int *)malloc(sizeof(int)*imgsize[1]);

    for(int i=0; i<imgsize[0]; i++)
        for(int j=0; j<imgsize[1]; j++)
            final_codes[i][j] = classes[merge_map[i][j] - 2];
    QMessageBox::information(this,"msg","end");
    w= new selectCDType(imgsize[0],imgsize[1],final_codes,labels1,labels2,data1,data2,i1,i2,num_band);
    w->show();


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