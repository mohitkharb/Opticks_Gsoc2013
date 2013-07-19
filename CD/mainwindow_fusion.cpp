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


MainWindow_fusion::MainWindow_fusion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow_fusion)
{


    ui->setupUi(this);
	this->setWindowTitle("Image Fusion");
}

MainWindow_fusion::~MainWindow_fusion()
{
    delete ui;
}
void MainWindow_fusion::fillRegionNewLabels(int **olinfo,int **nlinfo,int ol,int nl,int sx,int sy,int **mymap, int ROWS, int COLS)
{
        std::queue<int> myq;
        mymap[sx][sy]=1;
        myq.push(sx*COLS+sy);//pusing inital pixel into the queue
        nlinfo[sx][sy]=nl;
        int pos,pos_i,pos_j,i,j;

        while(myq.size()!=0)
        {
                pos=myq.front();
                myq.pop();
                pos_i=pos/COLS;
                pos_j=pos%COLS;

                /*//now pushing the untouched pixels in its neighbourhood into the queue..
                for(i=pos_i-1;i<=pos_i+1;i++)
                {
                    for(j=pos_j-1;j<=pos_j+1;j++)
                    {
                        if((i>=0 && i<ROWS) &&(j>=0 && j<COLS))
                        {
                            if (olinfo[i][j]==ol && mymap[i][j]==0)//this means this point should not pushed and this is our neighbour..
                            {

                                myq.push(i*COLS+j);
                                mymap[i][j]=1;
                                nlinfo[i][j]=nl;
                            }
                        }
                    }
                }
                */


                i=pos_i-1;j=pos_j;

                //		printf("%d.%d\n",pos_i,pos_j);
                                if(i>=0 && j < COLS && olinfo[i][j]==ol && mymap[i][j]==0)
                                {
                                //				printf("1\n");
                                                                myq.push(i*COLS+j);
                                                                mymap[i][j]=1;
                                                                nlinfo[i][j]=nl;
                                }

                                i=pos_i+1;j=pos_j;
                                if(i<ROWS && j < COLS && olinfo[i][j]==ol && mymap[i][j]==0)
                                {
                                //				printf("2\n");
                                                                myq.push(i*COLS+j);
                                                                mymap[i][j]=1;
                                                                nlinfo[i][j]=nl;
                                }
                                i=pos_i;j=pos_j-1;
                                if(j>=0 && i <ROWS && olinfo[i][j]==ol && mymap[i][j]==0)
                                {
                                //				printf("3\n");
                                                                myq.push(i*COLS+j);
                                                                mymap[i][j]=1;
                                                                nlinfo[i][j]=nl;
                                }
                                i=pos_i;j=pos_j+1;
                                if(j<COLS && i<ROWS && olinfo[i][j]==ol && mymap[i][j]==0)
                                {
                                //				printf("4\n");
                                                                myq.push(i*COLS+j);
                                                                mymap[i][j]=1;
                                                                nlinfo[i][j]=nl;
                                }




        }
        return;
}

int **MainWindow_fusion::prepareNewLabels(char *seg_result_label_file,int rows, int cols)
{

        FILE *fol = fopen(seg_result_label_file,"r");
        int **mymap,**olinfo,**nlinfo;
        int i,j;
        
        mymap=(int **)malloc(sizeof(int *)*rows);
        olinfo=(int **)malloc(sizeof(int *)*rows);
        nlinfo=(int **)malloc(sizeof(int *)*rows);

        for(i=0;i<rows;i++)
        {
                mymap[i]=(int *)malloc(sizeof(int)*cols);
                olinfo[i]=(int *)malloc(sizeof(int)*cols);
                nlinfo[i]=(int *)malloc(sizeof(int)*cols);
        }

        for(i=0;i<rows;i++)
        {
                for(j=0;j<cols;j++)
                {
                        fscanf(fol,"%d",&olinfo[i][j]);
                        mymap[i][j]=0;
                }
        }


        int nl=0,ol;

        for(i=0;i<rows;i++)
        {
                for(j=0;j<cols;j++)
                {
                        if(mymap[i][j]==0)//this means untouched so we can start a new label from here
                        {
                                ol=olinfo[i][j];
                                fillRegionNewLabels(olinfo,nlinfo,ol,nl,i,j,mymap,rows,cols);
                                nl++;
                        }
                }
        }
       // std::cout << nl << std::endl;
        return(nlinfo);
}
void MainWindow_fusion::fillRegionObjectInfo(int **dt1,int **dt2,int **&dmerge,int val1,int val2,int label,int bi,int bj,std::vector< int > &meta_obj, int rows, int cols)
{
        //dt1 is map of time1
        //dt2 is map of time2
        //dmerge is the merged map
        //val1 and val2 is the value pair of the new object
        // bi and bj are the indexes of the first encountered pixel of this object
        std::queue<int> myq;
        myq.push(bi*cols+bj);//pusing inital pixel into the queue
        int pos,pos_i,pos_j,i,j;
        int xmin,xmax,ymin,ymax,nop;//nop is number of pixels in the object;
        nop=0;
        xmin=bi;
        xmax=bi;
        ymin=bj;
        ymax=bj;

        while(myq.size()!=0)
        {
                pos=myq.front();
                myq.pop();
                pos_i=pos/cols;
                pos_j=pos%cols;

                if(dt1[pos_i][pos_j]==val1 && dt2[pos_i][pos_j]==val2)//this means that the current pixel is of our use..
                {
                        dmerge[pos_i][pos_j]=label;
                        //update meta info for this oject..
                        if(pos_i<xmin)
                                xmin=pos_i;
                        if(pos_i>xmax)
                                xmax=pos_i;

                        if(pos_j<ymin)
                                ymin=pos_j;
                        if(pos_j>ymax)
                                ymax=pos_j;
                        nop++;

                        //now pushing the untouched pixels in its neighbourhood into the queue..
                        for(i=pos_i-1;i<=pos_i+1;i++)
                        {
                                for(j=pos_j-1;j<=pos_j+1;j++)
                                {
                                        if((i>=0 && i<rows) &&(j>=0 && j<cols))
                                        {
                                                if (dmerge[i][j]>0 && dmerge[i][j]!=label)//this means this point should not pushed and this is our neighbour..
                                                {
                                                        //fprintf(rag_merge,"%d %ld\n",label,dmerge[i][j]);
                                                }
                                                else if (dmerge[i][j]<0 && dmerge[i][j]!=-1*label)//this point is to be added...
                                                {
                                                        myq.push(i*cols+j);
                                                        dmerge[i][j]=-1*label;
                                                }
                                        }
                                }
                        }
                }
        }
        meta_obj[0]=label-1;
        meta_obj[1]=nop;
        meta_obj[2]=xmin;
        meta_obj[3]=xmax;
        meta_obj[4]=ymin;
        meta_obj[5]=ymax;
//	cout<<"heads up:: returning from fillRegionObjInfo\n";
        return;
}

std::vector< std::vector< int > > MainWindow_fusion::prepareObjectInfo(int rows, int cols, int **labels_1, int **labels_2, int **&dmerge, int &NO)
{
        //Merging of images & finding Minimum Bounding Boxes of objects and storing this info - starts here---

        //labels_1 contains segmentation result of time 1
        //labels_2 contains segmentation result of time 2

        int count=0,i,j,label;

        int val1, val2,max=-1;

//	FILE *obj_info;
//	obj_info = fopen("objectinfo_dump.txt","w");
//	int **meta_obj;//this is going to be a NOx6 array storing xmin xmax ymin ymax size of the object
        std::vector< std::vector < int > >meta_obj;
        std::vector< int >v(6);
        //preparing merge map
        label=2;
        for(i=0;i<rows;i++)
        {
                for(j=0;j<cols;j++)
                {
                        if(dmerge[i][j]<0)//this means new object will start from this pixel
                        {
                                val1=labels_1[i][j];
                                val2=labels_2[i][j];
                                //cout<<"heads up:: before fillregionobjectinfo call\n";
                                //this function will fill the newly touched region
                                //cout<<i<<" "<<j<<endl;
                                fillRegionObjectInfo(labels_1,labels_2,dmerge,val1,val2,label,i,j,v,rows,cols);
                                meta_obj.push_back(v);
//				fprintf(obj_info,"%d %d %d %d %d %d\n",meta_obj[count][0],meta_obj[count][1],meta_obj[count][2],meta_obj[count][3],meta_obj[count][4],meta_obj[count][5]);
                                //cout<<"heads up:: after returning from fillregionobjectinfo call :: now label value: "<<label<<endl;
                                //cout<<"metaobj values: "<<meta_obj[count][0]<<" "<<meta_obj[count][1]<<endl;
                                count++;
                                label++;

                        }
                }
        }
        //printf("numboer of objects: %d\n",NO);
//	fclose(obj_info);
        NO = meta_obj.size();
//	NO = label -1;
        return meta_obj;

}

int ***MainWindow_fusion::readPPMImage(int imgsize[],char *filename, int band)
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
                if(band>1){
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
                                    //grn = (int)fgetc( file );
                                    //blu = (int)fgetc( file );
                                    data[row][col][0] = red;
                                    data[row][col][1] = 0;
                                    data[row][col][2] = 0;
                            }
                    }


                fclose( file );
                return(data);
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
     object_info1 = prepareObjectInfo(imgsize[0],imgsize[1],labels1,labels1,merge_map,NO1);//using labels two times

    }
    else ui->textBrowser->setText("Select directory and file names.");
}


void MainWindow_fusion::GetHistogram (std::vector<int> data, std::vector<int> &val, std::vector<int> &cnt)
{
        int it;
        it = 0;
        int curval,curcnt = 0;
        curval = data[it];
        int size = data.size();

        it = 0;
//        std::cout << "Printing data vector"<<std::endl;
//        while(it < size)
//        {
//            std::cout << data[it] << " ";
//            it++;
//        }


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

//        size = val.size();
//        it = 0;

//        while(it < size)
//        {
//            std::cout << val[it] << " ";
//            it++;
//        }
//        std::cout << std::endl;

//        it = 0;
//        while(it < size)
//        {
//            std::cout << cnt[it] << " ";
//            it++;
//        }
//        std::cout << std::endl;

}
void MainWindow_fusion::GetStats(int **segmap, std::vector<int> data,int onum,float &mean,float &std, int &min, int &max,int xmin, int xmax, int ymin, int ymax,int nop)
{
//GetStats(segmap,pandata,onum,&panmean,&panstd,&panmin,&panmax,xmin,xmax,ymin,ymax);

        std::vector<int> val;
        std::vector<int> cnt;
        float Tprune = atof(ui->lineEditTh->text().toUtf8().data());
        GetHistogram(data,val,cnt);

        int size;
        size = val.size();


        int maxgap = -1;
       // std::cout << Tprune << std::endl;

        int it=0;
        int cit=0;
        int wit=0;

        if (size == 1)
        {
            min = val[0];
            max = val[0]+10;
            mean = val[0]+5;
            std = 0;
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

//            int titer;

//            titer = val[0];
//            std::cout << "printing New val Data"<< std::endl;
//            while (titer < size)
//                    {
//                        std::cout << val[titer] << " ";
//                        titer++;
//                    }
//                    std::cout << std::endl;

//                    titer = 0;
//                    std::cout << "printing New cnt Data"<< std::endl;
//                    while (titer < size)
//                    {
//                        std::cout << cnt[titer] << " ";
//                        titer++;
//                    }
//                    std::cout << std::endl;

            //std::cout << min << " " << max << "to ";

            it = 0;
            cit = 0;
            while(it <= wit)
            {
                count = count + cnt[cit];

                it++;
                cit++;
            }
            count = count / nop ;

            //std::cout << count << "chekcing count" << std::endl;

            if (count < 1 - Tprune)/*his means that we have to fix the minimum of the histogram*/
            {
                //std::cout << "adjusting min"<<std::endl;
                min = val[wit+1];
                max = val[size - 1];
            }

            if( count > Tprune)/*This means that we have to fix the maximum of the histogram*/
            {
                //std::cout << "adjusting max"<<std::endl;
                min = val[0];
                max = val[wit];
            }
            //std::cout << min << " " << max << std::endl;

            /*Now we have range from which we have to prepare the mean and standard deviation of the histogram */

            it = 0;
            cit = 0;
            float sum = 0;
            int curval,curcnt;


            while (it < size)
            {
                curval = val[it];
                curcnt = cnt[cit];
                sum = sum + curval*curcnt;
                it++;
                cit++;
            }
            mean = sum/nop;

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
            std = sum/nop;
            std = sqrt(std);
        }
}
void MainWindow_fusion::PrepareStatistics(int **segmap, int ***msimage, int ***panimage,int onum,int xmin,int xmax,int ymin,int ymax,int bandnum,float &msmean,float &msstd,float &panmean,float &panstd,int &panmin,int &panmax, int &msmin, int &msmax)
//void MainWindow::PrepareStatistics(int **&segmap, int ***&msimage, int ***&panimage,int &onum,int &xmin,int &xmax,int &ymin,int &ymax,int &bandnum,float &msmean,float &msstd,float &panmean,float &panstd,int &panmin,int &panmax)
{
//PrepareStatistics(segmap,msimage,panimage,onum,xmin,xmax,ymin,ymax,bandnum,&msmean,&msstd,&panmean,&panstd,&panmin,&panmax);
//PrepareStatistics(segmap,msimage,panimage,onum,xmin,xmax,ymin,ymax,bandnum,msmean,msstd,panmean,panstd,panmin,panmax);
        //Tprune = 0.9;

        int i,j;

        std::vector<int> pandata;
        std::vector<int> msdata;

        int index = 0;

        for (i=xmin;i<=xmax;i++)
        {
                for(j=ymin;j<=ymax;j++)
                {
                        if (segmap[i][j] == onum) {
                            pandata.push_back(panimage[i][j][0]);
                            msdata.push_back(msimage[i][j][bandnum]);
                        }
                        index++;
                }
        }

        sort(pandata.begin(),pandata.end());
        sort(msdata.begin(),msdata.end());

//        std::vector<int>::iterator titer;

//        titer = pandata.begin();
//        std::cout << "printing Pan Data"<< std::endl;
//        while (titer < pandata.end())
//        {
//            std::cout << *titer << " ";
//            titer++;
//        }
//        std::cout << std::endl;

//        titer = msdata.begin();
//        std::cout << "printing MS Data"<< std::endl;
//        while (titer < msdata.end())
//        {
//            std::cout << *titer << " ";
//            titer++;
//        }
//        std::cout << std::endl;



        GetStats(segmap,pandata,onum,panmean,panstd,panmin,panmax,xmin,xmax,ymin,ymax,object_info1[onum][1]);
        GetStats(segmap,msdata, onum,msmean,msstd,msmin,msmax,xmin,xmax,ymin,ymax,object_info1[onum][1]);

}

void MainWindow_fusion::Fusion_GLSI(int **segmap, int ***&fimage, int ***msimage, int ***panimage, int onum, int xmin, int xmax, int ymin, int ymax,int bandnum)
{

        float msmean,msstd,panmean,panstd,curpan,dist;
        int panmin,panmax,msmin,msmax;

        int i,j;

        /*This calculates mean and stddev after cleaning is applied to histograms*/
        PrepareStatistics(segmap,msimage,panimage,onum,xmin,xmax,ymin,ymax,bandnum,msmean,msstd,panmean,panstd,panmin,panmax,msmin,msmax);

        //std::cout << msmean << " " << msstd <<" " << panmean << " " << panstd << " " << panmin << " " << panmax <<std::endl;

        for(i=xmin;i<=xmax;i++)
        {
                for(j=ymin;j<=ymax;j++)
                {

                        if (segmap[i][j] == onum)
                        {
                            //std::cout << panmin << " " << panmax << "-- checking" << std::endl;
                                if (panimage[i][j][0]>=panmin && panimage[i][j][0]<=panmax)
                                {
//                                    curpan = panimage[i][j][0];
//                                    dist = ((curpan - panmean)*1.0)/panstd;
//                                    //std::cout << msmean + dist*msstd << std::endl;

//                                    if (msmean + dist*msstd < 0)
//                                        fimage[i][j][bandnum] = 0;
//                                    else
//                                        fimage[i][j][bandnum] = floor(msmean + dist*msstd);
                                    curpan = panimage[i][j][0];

                                    if (curpan > panmean)
                                    {
                                        dist = (curpan - panmean)/(panmax - panmean);
                                        fimage[i][j][bandnum] = msmean + dist*(msmax - msmean);
                                    }
                                    else
                                    {
                                        dist = (panmean - curpan)/(panmean - panmin);
                                        fimage[i][j][bandnum] = msmean - dist*(msmean - msmin);
                                    }

//                                    if (fimage[i][j][bandnum] == 0)
//                                        std::cout << "Problem :"<<msmin << " "<< msmean << " "<< msmax << " "<< panmin << " "<< panmean << " "<< panmax << " "<<curpan<< " "<<onum<<" "<<std::endl;

//                                    if (fimage[i][j][bandnum] == 255)
//                                        std::cout << "Problem :"<<msmin << " "<< msmean << " "<< msmax << " "<< panmin << " "<< panmean << " "<< panmax << " "<<curpan<< " "<<onum<<" "<<std::endl;

                                }
                                else
                                {
                                        fimage[i][j][bandnum] = -1;
                                }

                        }
                }
        }
}

void MainWindow_fusion::FillBlackPixels(int ***&fimage,int rows,int cols,int **&segmap,int b)
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
            if (fimage[i][j][b] == -1)
                bcnt++;
        }
    }

    while(bcnt>0)
    {
        for (i=0;i<rows;i++)
        {
            for(j=0;j<cols;j++)
            {
                if (fimage[i][j][b] == -1)
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
                        segmap[i][j] = winclass;
                        bcnt--;
                    }
                }

            }
        }
    }


}


void MainWindow_fusion::writePPMImage(int ***op,int img1_size[],std::string fname)
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
//	std::cout << glsiclicked << std::endl;
//	std::cout << postprocessing << std::endl;


	int minimumArea = atoi(ui->lineEditArea->text().toUtf8().data());
	float threshold = atof(ui->lineEditTh->text().toUtf8().data());
	std::cout << minimumArea << std::endl;
	std::cout << threshold << std::endl;
	std::string fname = "C:/Windows/Temp/fusion.ppm";
	//std::string fname = "/tmp/fusion.ppm";
	int glsi_count = 0;




		dataf = (int ***)malloc(sizeof(int **)*imgsize[0]);

		for(int i=0; i<imgsize[0]; i++)
		{
			dataf[i] = (int **)malloc(sizeof(int)*imgsize[1]);
			for(int j=0; j<imgsize[1]; j++)
				dataf[i][j] = (int *)malloc(sizeof(int)*3);
		}
		//std::cout << "malloc" << std::endl;

		datasf = (int ***)malloc(sizeof(int **)*imgsize[0]);

		for(int i=0; i<imgsize[0]; i++)
		{
			datasf[i] = (int **)malloc(sizeof(int)*imgsize[1]);

			for(int j=0; j<imgsize[1]; j++)
				datasf[i][j] = (int *)malloc(sizeof(int)*3);
		}

		for(int b = 0; b < 3 ; b++)
		{
			for(int i=0;i<NO1;i++)
			{
				//std::cout << object_info1[i][1] << " " << i << std::endl;
				

					glsi_count++;
					//std::cout << "here glsi" << std::endl;
					Fusion_GLSI(labels1,dataf,data1,data2,i,object_info1[i][2],object_info1[i][3],object_info1[i][4],object_info1[i][5],b);
					//Fusion_GLSI(segmap,fimage,msimage,panimage,onum,xmin,xmax,ymin,ymax,bandnum);

			}

			/* Code for Removing Black Pixels..*/
			FillBlackPixels(dataf,imgsize[0],imgsize[1],labels1,b);


		}


		std::cout << "GLSI Done for " <<glsi_count << " objects" << std::endl;
		//std::string fname = "/tmp/fusion.ppm";


			writePPMImage(dataf,imgsize,fname);
			free(dataf);
			free(datasf);

		ui->textBrowser->setText("file written by GLSI in Temp dir.");
		std::cout << "file written by glsi" << std::endl;
	
	
}



