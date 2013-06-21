#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include<gdal/gdal_alg.h>
#include<gdal/cpl_conv.h>
#include <gdal/gdal_priv.h>
#include<gdal/cpl_string.h>
#include<gdal/ogrsf_frmts.h>
#include<gdal/cpl_port.h>


MainWindow::MainWindow(int rows, int cols, std::vector<double> dataR, std::vector<double> dataG, std::vector<double> dataB, int bandnum, QWidget *parent) :
	Rows(rows), Cols(cols), dataR(dataR), dataG(dataG), dataB(dataB),num_band(bandnum),
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Show_process(std::string mod)
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


void MainWindow::on_pushButton_2_clicked()
{
	 ui->textBrowser->setText("calculating mean shift.");
        if(!data_dir.isEmpty())
        {

			img_name = "OpticksImage";
            std::cout << Rows << " -- " << Cols << std::endl;
			tempimg1 = temp_dir + "\\" + img_name +".ppm";
		//	QMessageBox::information(this,"value",tempimg1);
			FILE *fp21 = fopen(tempimg1.toUtf8().data(), "wb"); /* b - binary mode */
		//	FILE *fp21 = fopen("C:\\Windows\\Temp\\img.ppm", "wb"); /* b - binary mode */
            fprintf(fp21, "P6\n%d %d\n255\n", Cols, Rows);
            
            
            for(int i = 0;i < Rows; i++){
                for(int k = 0;k < Cols; k++){

                    static unsigned char color[3];
                    color[0]	= dataB[i*Cols + k ];
                    color[1]    = dataG[i*Cols + k ];
                    color[2]	= dataR[i*Cols + k ];
                  //  std::cout << int (color[0]) << " " << int (color[1]) << " " << int (color[2]) << std::endl;
                    fwrite(color, 1, 3, fp21);

                }
            }


            fclose(fp21);
            




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
        
                out<<"Load('"<< tempimg1 <<"',IMAGE);\n";
            }
            else if(i==23){
                out<<"Save('C:\\Windows\\Temp\\seg_"<< img_name<<".ppm', PPM, SEGM_IMAGE);\n";

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

        }
        else ui->textBrowser->setText("Select directory and file names.");
}

void MainWindow::on_pushButton_3_clicked()
{
	       if(!ms_dir.isEmpty())
       {
		   data1 = readPPMImage(imgsize,tempimg1.toUtf8().data(),num_band);
    std::cout<<"here1"<<std::endl;
               i1 = Image(Rows, Cols, 3, data1);
        
			   labels1 = prepareNewLabels("C:/Windows/Temp/map_label_pruning_spurious_regions.txt",Rows,Cols);
   std::cout<<"here2"<<std::endl;

               merge_map = (int **)malloc(sizeof(int *)*Rows);
               for(int i=0; i < Rows; i++)
                       merge_map[i] = (int *)malloc(sizeof(int)*Cols);

               for(int i=0; i<Rows; i++)
                       for(int j=0; j<Cols; j++)
                               merge_map[i][j] = -1;


               
               object_info1 = prepareObjectInfo(Rows,Cols,labels1,labels1,merge_map,NO1);//using labels two times
               std::cout<<"here3"<<std::endl;
               i1.createSegmentedImageInfo(merge_map);
               std::cout<<"here4"<<std::endl;
               i1.createObjects(NO1,object_info1); // this function in turn calls the createAttribute function also
               std::cout<<"here5"<<std::endl;
               ui->textBrowser->setText("Feature Extraction Done.");

			   


         }
       else ui->textBrowser->setText("Select correct Mean Shift directory or Image name missing.");
}

void MainWindow::on_pushButton_clicked()
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
void MainWindow::on_pushButton_4_clicked()
{
	int i= atoi(ui->lineEdit_4->text().toUtf8().data());
	//std::string fname ="C:/Windows/Temp/testing.ppm";
	std::string fname = tempimg1.toUtf8().data();

	pbox = new paint_box(fname,Rows,Cols,i,labels1,this);
	pbox->show();
}
void MainWindow::on_pushButton_5_clicked()
{
	classificationImage();
	AttributeBased *a= new AttributeBased(Rows,Cols,labels1,data1,i1,num_band,1);
	a->show();
}
void MainWindow:: insertvalue(int n, int x, int y,int **labels)
{

    int val = labels[y][x];
 


    std::vector<int> temp;
    temp = class_m[n];
    
    if(temp.size()!=0)
    {
        temp.push_back(val);
        class_m[n] = temp;
        
 
    }
    else
    {
        std::vector<int> newtemp;
        newtemp.push_back(val);
        class_m[n]= newtemp;
        

    }



}
void MainWindow::GetMinor(float **src, float **&dest, int row, int col, int order)
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



float MainWindow::CalcDeterminant( float **mat, int order)
{
    // order must be >= 0
    // stop the recursion when matrix is a single element
    if( order == 1 )
        return mat[0][0];

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

    return det;
}


void MainWindow::MatrixInversion(float **A, int order, float **&Y)
{
    // get the determinant of a
    float det = 1.0/CalcDeterminant(A,order);

    // memory allocation
    float *temp = (float *)malloc(sizeof(float)*(order-1)*(order-1));
    float **minor = (float **)malloc(sizeof(float *)*(order-1));
    for(int i=0;i<order-1;i++)
        minor[i] = (float *)malloc(sizeof(float)*(order-1));

    for(int j=0;j<order;j++)
    {
        for(int i=0;i<order;i++)
        {
            // get the co-factor (matrix) of A(j,i)
            GetMinor(A,minor,j,i,order);
            Y[i][j] = det*CalcDeterminant(minor,order-1);
            if( (i+j)%2 == 1)
                Y[i][j] = -Y[i][j];
        }
    }

}

float **MainWindow::getCovarianceMartix(int numberOfSamples, float **dataArray, float **meanVector, int classNumber, int ndims)
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

float MainWindow::calculateMahaDist(float *sample, float **class_mean, float **full_cov_mat, int class_number, int ndims)
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

void MainWindow::classificationImage(){
    int current_class=0, i = 0, k, ndims = 7; //ndims is the number of dimensions of the data samples
    int class_num = int (class_m.size());

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
        
        i = 0 ;
        std::vector <int> tempvect = class_m[current_class];

        //create data matrix here
        float **data_samples = (float **)malloc(sizeof(float *)*(tempvect.size()));
        for(int p=0; p< (int) tempvect.size(); p++)
            data_samples[p] = (float *)malloc(sizeof(float)*ndims);

        for(int m=0; m<ndims; m++)
            for(int n=0; n<ndims; n++ )
                inverted_class_cov_mat[m][n] = 0.0;
        
        while(i < (int) tempvect.size() )
        {
            k = tempvect[i];

            value_r = i1.objectArray[k].fVector.meanRed;
            value_g = i1.objectArray[k].fVector.meanGreen;
            value_b = i1.objectArray[k].fVector.meanBlue;
            texture_coarseness = i1.objectArray[k].fVector.coarseness;
            texture_direction = i1.objectArray[k].fVector.direction;
            texture_contrast = i1.objectArray[k].fVector.contrast;
            texture_roughness = i1.objectArray[k].fVector.roughness;

            data_samples[i][0] = value_r;
            data_samples[i][1] = value_g;
            data_samples[i][2] = value_b;
            data_samples[i][3] = texture_coarseness;
            data_samples[i][4] = texture_direction;
            data_samples[i][5] = texture_contrast;
            data_samples[i][6] = texture_roughness;

            class_mean[current_class][0] += value_r;
            class_mean[current_class][1] += value_g;
            class_mean[current_class][2] += value_b;
            class_mean[current_class][3] += texture_coarseness;
            class_mean[current_class][4] += texture_direction;
            class_mean[current_class][5] += texture_contrast;
            class_mean[current_class][6] += texture_roughness;

            i++;
        }
        class_mean[current_class][0] /= tempvect.size();
        class_mean[current_class][1] /= tempvect.size();
        class_mean[current_class][2] /= tempvect.size();
        class_mean[current_class][3] /= tempvect.size();
        class_mean[current_class][4] /= tempvect.size();
        class_mean[current_class][5] /= tempvect.size();
        class_mean[current_class][6] /= tempvect.size();

        /*std::cout << class_mean[current_class][0] << " " <<
                     class_mean[current_class][1] << " " <<
                     class_mean[current_class][2] << " " <<
                     class_mean[current_class][3] << " " <<
                     class_mean[current_class][4] << " " <<
                     class_mean[current_class][5] << " " <<
                     class_mean[current_class][6] << std::endl;
					 */
        
        // call covariance matrix function here
        class_cov_mat = getCovarianceMartix(tempvect.size(), data_samples, class_mean, current_class, ndims);
        

        // call Matrix inversion function here
        
        MatrixInversion(class_cov_mat, ndims, inverted_class_cov_mat);

        for(int m=0; m<ndims; m++)
            for(int n=0; n<ndims; n++)
                full_cov_mat[current_class*ndims + m][n] = inverted_class_cov_mat[m][n];


        current_class++;
    }
    
    for(int i=0; i<class_num*ndims; i++){
        for(int j=0; j<ndims; j++)
            std::cout << full_cov_mat[i][j] << " ";
    
    }


    float min_dev=10000000;
    int j = 0;
    float *sample= (float *)malloc(sizeof(float)*ndims);
    float mDist=0.0;

    //  FILE *new_class;
    // new_class = fopen("C:\\Windows\\Temp\\new_class_1.txt","w");

    while( j < NO1)
    {
        value_r = i1.objectArray[j].fVector.meanRed;
        value_g = i1.objectArray[j].fVector.meanGreen;
        value_b = i1.objectArray[j].fVector.meanBlue;
        texture_coarseness = i1.objectArray[j].fVector.coarseness;
        texture_direction = i1.objectArray[j].fVector.direction;
        texture_contrast = i1.objectArray[j].fVector.contrast;
        texture_roughness = i1.objectArray[j].fVector.roughness;

        sample[0] = value_r;
        sample[1] = value_g;
        sample[2] = value_b;
        sample[3] = texture_coarseness;
        sample[4] = texture_direction;
        sample[5] = texture_contrast;
        sample[6] = texture_roughness;

        min_dev = 10000000.0;
        mDist = 0.0;
        int class_flag=0;
        for(i=0; i<class_num; i++)
        {
            //call Mahalanobis Distance function for each object here
            mDist = calculateMahaDist(sample,class_mean,full_cov_mat,i,ndims);

            if(mDist < min_dev)
            {
                //my_new_class = i;
                i1.objectArray[j].fVector.classNumber = i+1; // i+1 : so that the class number starts from 1 instead of 0
                class_flag = 1;
                min_dev = mDist;
            }
        }
        if(class_flag == 0){
            //std::cout << sample[0] << " " <<  sample[1] << " " <<  sample[2] << " " <<  sample[3] << " " <<  sample[4] << " " <<  sample[5] << " " <<  sample[6] << " " << std::endl;
            for(int cs=0; cs<class_num; cs++){
                float dis = calculateMahaDist(sample,class_mean,full_cov_mat,cs,ndims);
    
            }
    
        }

        //    fprintf(new_class,"%d\n",i1.objectArray[j].fVector.classNumber);
        j++;

    }
    class_m.clear();
    //  fclose(new_class);

}
void MainWindow::Ras2Vect(){

	
	    GDALAllRegister();//To load drivers for all known raster formats

    //GDALDataset *RasImage;//This holds original data in GeoTiff format..
    //RasImage=(GDALDataset*)GDALOpen(argv[1],GA_Update);//input geotiff file

    int ROWS=imgsize[0];
    int COLS=imgsize[1];
    int onum;

    GDALDataset *TempImage;//for creating a 32bit datatype image ..

    GDALDriver *poDriver1;

    poDriver1 = GetGDALDriverManager()->GetDriverByName("GTiff");
	QString seg_tiff = temp_dir+"\\SegImage.tiff";
	TempImage = poDriver1->Create( seg_tiff.toUtf8().data(), COLS, ROWS, 1, GDT_Int32,NULL);//this creates an image of 1 band with data type uint32
//	TempImage = poDriver1->Create( "C:\\Windows\\Temp\\SegImage.tiff", COLS, ROWS, 1, GDT_Int32,NULL);//this creates an image of 1 band with data type uint32
/*	if(!TempImage){
		cout << "xxx\n";
		return;	
	}
	*/
    int *segimage;//this will hold segmentation result...
    segimage=(int *)malloc(sizeof(int)*ROWS*COLS);


    int i;

    for(i=0;i<ROWS*COLS;i++)
    {
        int y=ROWS-1-(i/COLS);
        int x=(i%COLS);

        segimage[i]=labels1[y][x];

    }

    GDALRasterBand *SegBand;//this will hold the segmenatation result..
    SegBand=TempImage->GetRasterBand(1);
    //SegBand->RasterIO(GF_Write,0,0,COLS,ROWS,(void *)segimage,COLS,ROWS,GDT_Int32,0,0);//Writing the BSQ formatted data to SegBand...
    SegBand->RasterIO(GF_Write,0,0,COLS,ROWS,(void *)segimage,COLS,ROWS,GDT_Int32,0,0);//Writing the BSQ formatted data to SegBand...
    //Now we are ready with an image band holding our segmentation result...
    //Now we have to all gdal function that polygonize a band..

	
    OGRSFDriver *poDriver;//handler for vector data

    OGRRegisterAll();

    poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");//can change output vector format...
    //Driver_gml = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("GML");//can change output vector format...
    OGRDataSource *poDS;

    
   // std::cout << img_name.toUtf8().data() << std::endl;
//	QString temp="C:\\Windows\\Temp\\";
//	temp.append(img_name);

	
	QString shp = QInputDialog::getText(this,"Input","Enter the name of file you want to save");
	QString vecFile = out_dir + "\\" +shp;
	//QMessageBox::information(this,"xx",vecFile);
	
   // std::cout << "xyz.shp" <<std::endl;
	poDS = poDriver->CreateDataSource( vecFile.toStdString().c_str(), NULL );//Open a vector file in default format...
  //  if( access( temp.toUtf8().data(), F_OK ) != -1 ) {
  //      deletecheck = "del ";
  //      deletecheck.append(temp);
  //      Show_process(deletecheck.toUtf8().data());

  //  }
  //  std::cout<< "here1 " <<std::endl;
	
	//poDS = poDriver->CreateDataSource( "C:\\tmp\\aa.shp", NULL );//Open a vector file in default format...
    std::cout<< "here2 " <<std::endl;
    if( poDS == NULL )
    {
		QMessageBox::information(this,"Error","Creation of Output failed\n");
        //printf( "Creation of output file failed.\n" );
        exit( 1 );
    }
	
    std::cout<< "here3 " <<std::endl;
    //source_gml = Driver_gml->CreateDataSource( "mygmlfile.gml",NULL );//Open a vector file in default format...

    OGRLayer *poLayer;
	poLayer = poDS->CreateLayer( shp.toStdString().c_str(), NULL, wkbPolygon, NULL );//main_layer is our only layer for the vector result..
    //layer_gml = source_gml->CreateLayer( "main_layer", NULL, wkbPolygon, NULL );//main_layer is our only layer for the vector result..
    if( poLayer == NULL )
    {
		QMessageBox::information(this,"Error","Layer creation failed.\n");
        //printf( "Layer creation failed.\n" );
        exit( 1 );
    }
    //Now we need to add fields that each feature can have...



std::cout<< "here4 " <<std::endl;
    OGRFieldDefn oField1( "ObjectNum", OFTInteger);
    oField1.SetWidth(32);
    poLayer->CreateField(&oField1);
    //layer_gml->CreateField(&oField1);

    OGRFieldDefn oField2( "Mean_Red", OFTInteger);
    oField2.SetWidth(32);
    poLayer->CreateField(&oField2,FALSE);
    //layer_gml->CreateField(&oField2);

    OGRFieldDefn oField3( "Mean_Green", OFTInteger);
    oField3.SetWidth(32);
    poLayer->CreateField(&oField3);
    //layer_gml->CreateField(&oField3);

    OGRFieldDefn oField4( "Mean_Blue", OFTInteger);
    oField4.SetWidth(32);
    oField4.SetPrecision(8);
    poLayer->CreateField(&oField4);
    //layer_gml->CreateField(&oField4);

    OGRFieldDefn oField5( "Std_Red", OFTReal);
    oField5.SetWidth(32);
    oField5.SetPrecision(8);
    poLayer->CreateField(&oField5);
    //layer_gml->CreateField(&oField5);

    OGRFieldDefn oField6( "Std_Green", OFTReal);
    oField6.SetWidth(32);
    oField6.SetPrecision(8);
    poLayer->CreateField(&oField6);
    //layer_gml->CreateField(&oField6);

    OGRFieldDefn oField7( "Std_Blue", OFTReal);
    oField7.SetWidth(32);
    oField7.SetPrecision(8);
    poLayer->CreateField(&oField7);
    //layer_gml->CreateField(&oField7);
    //poLayer is a pointer to a layer in which the next function is going to write the data..

    OGRFieldDefn oField8( "Area", OFTInteger);
    oField8.SetWidth(32);
    oField8.SetPrecision(8);
    poLayer->CreateField(&oField8);
    //layer_gml->CreateField(&oField8);

    OGRFieldDefn oField9( "Perimeter", OFTInteger);
    oField9.SetWidth(32);
    oField9.SetPrecision(8);
    poLayer->CreateField(&oField9);
    //layer_gml->CreateField(&oField10);

    OGRFieldDefn oField10( "XCentroid", OFTInteger);
    oField10.SetWidth(32);
    oField10.SetPrecision(8);
    poLayer->CreateField(&oField10);
    //layer_gml->CreateField(&oField10);

    OGRFieldDefn oField11( "YCentroid", OFTInteger);
    oField11.SetWidth(32);
    oField11.SetPrecision(8);
    poLayer->CreateField(&oField11);
    //layer_gml->CreateField(&oField10);

    OGRFieldDefn oField12( "Roundness", OFTReal);
    oField12.SetWidth(32);
    oField12.SetPrecision(8);
    poLayer->CreateField(&oField12);
    //layer_gml->CreateField(&oField10);

    OGRFieldDefn oField13( "Compactness", OFTReal);
    oField13.SetWidth(32);
    oField13.SetPrecision(8);
    poLayer->CreateField(&oField13);
    //layer_gml->CreateField(&oField10);

    OGRFieldDefn oField14( "Fcoarseness", OFTReal);
    oField14.SetWidth(32);
    oField14.SetPrecision(8);
    poLayer->CreateField(&oField14);
    //layer_gml->CreateField(&oField10);
    OGRFieldDefn oField15( "Fdirection", OFTReal);
    oField15.SetWidth(32);
    oField15.SetPrecision(8);
    poLayer->CreateField(&oField15);
    //layer_gml->CreateField(&oField10);

    OGRFieldDefn oField16( "classNumber", OFTInteger);
    oField16.SetWidth(32);
    oField16.SetPrecision(8);
    poLayer->CreateField(&oField16);

    OGRFieldDefn oField17( "Fcontrast", OFTReal);
    oField15.SetWidth(32);
    oField15.SetPrecision(8);
    poLayer->CreateField(&oField17);


    OGRFieldDefn oField18( "Froughness", OFTReal);
    oField15.SetWidth(32);
    oField15.SetPrecision(8);
    poLayer->CreateField(&oField18);
    //layer_gml->CreateField(&oField10);


    int rc=GDALPolygonize(SegBand,NULL,(OGRLayerH)poLayer,0,NULL,NULL,NULL);//This will write segment number in the first field i.e. ID...
    std::cout<< "here5 " <<std::endl;

    int nof=poLayer->GetFeatureCount();


    std::cout<< "here6 " <<std::endl;
    double **Attribute;
    Attribute=(double **)malloc(NO1*sizeof(double *));
    std::cout << NO1 << std::endl;

    for(i=0;i<NO1;i++)
    {
        Attribute[i]=(double *)malloc(sizeof(double)*17);//number of attributes is 14.
    }
    //int k;
    //std::cout<<nof <<std::endl;
    std::cout<< "here7 " <<std::endl;
   for(i=0;i<nof;i++)
    {
        OGRFeature *feature;
        feature = poLayer->GetFeature(i);
        onum=feature->GetFieldAsInteger(0);

		feature->SetField(1,i1.objectArray[onum].fVector.meanRed);//Mean_Red
        feature->SetField(2,i1.objectArray[onum].fVector.meanGreen);//Mean_Green
        feature->SetField(3,i1.objectArray[onum].fVector.meanBlue);//Mean_Blue
        feature->SetField(4,i1.objectArray[onum].fVector.stdRed);//Std_Red
        feature->SetField(5,i1.objectArray[onum].fVector.stdGreen);//Std_Green
        feature->SetField(6,i1.objectArray[onum].fVector.stdBlue);//Std_Blue
        feature->SetField(7,i1.objectArray[onum].fVector.area);//Area
        feature->SetField(8,i1.objectArray[onum].fVector.perimeter);//Perimeter
        feature->SetField(9,i1.objectArray[onum].fVector.xCentroid);//XCentroid
        feature->SetField(10,i1.objectArray[onum].fVector.yCentroid);//YCentroid
        feature->SetField(11,i1.objectArray[onum].fVector.roundness);//Roundness
        feature->SetField(12,i1.objectArray[onum].fVector.compactness);//Compactness
        feature->SetField(13,i1.objectArray[onum].fVector.coarseness);//FCoarseness
        feature->SetField(14,i1.objectArray[onum].fVector.direction);//Fdirection
        feature->SetField(15,i1.objectArray[onum].fVector.classNumber);//classNumber
        feature->SetField(16,i1.objectArray[onum].fVector.contrast);
        feature->SetField(17,i1.objectArray[onum].fVector.roughness);


		poLayer->SetFeature(feature);
		//poLayer->GetFIDColumn()
		OGRFeature::DestroyFeature(feature);
    }
   std::cout<< "here 8 " << std::endl;
   poLayer->SyncToDisk();
   // cout<<"attribute added\n";
   std::cout<< "here 9 " << std::endl;
   poDS->SyncToDisk();

	std::cout<< "here 10 " << std::endl;




    //	fclose(fseg);
    OGRDataSource::DestroyDataSource(poDS);//Removes the pointer...
    //OGRDataSource::DestroyDataSource(source_gml);//Removes the pointer...
	
    
}
void MainWindow::on_pushButton_6_clicked()
{
	Ras2Vect();
	ui->textBrowser->setText("shape file saved");
}