#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<cmath>

#include<malloc.h>
#include<string.h>
#include"codes.h"
#include<algorithm>
#include<queue>
#include<map>

//#define LOW 0.20
//#define HIGH 0.70
using namespace std;
int Image::getImageRows()
{
	return this->ROWS;
}

int Image::getImageCols()
{
	return this->COLS;
}

int ***readPPMImage(int imgsize[],char *filename, int band)
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
			//while ( (ch=fgetc(file)) && isspace(ch) );
			while(1){
				ch = fgetc(file); 
				if(!isspace(ch))
					break;
			}

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

void fillRegionNewLabels(int **olinfo,int **nlinfo,int ol,int nl,int sx,int sy,int **mymap, int ROWS, int COLS)
{
	queue<int> myq;
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

		//now pushing the untouched pixels in its neighbourhood into the queue..
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

int **prepareNewLabels(char *seg_result_label_file,int rows, int cols)
{
	
	FILE *fol = fopen(seg_result_label_file,"r");
	int **mymap,**olinfo,**nlinfo;
	int i,j;//k,label;
	//int val1,val2;
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
	return(nlinfo);
}

void fillRegionObjectInfo(int **dt1,int **dt2,int **&dmerge,int val1,int val2,int label,int bi,int bj,vector< int > &meta_obj, int rows, int cols)
{
	//dt1 is map of time1
	//dt2 is map of time2
	//dmerge is the merged map
	//val1 and val2 is the value pair of the new object
	// bi and bj are the indexes of the first encountered pixel of this object
	queue<int> myq;
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

vector< vector< int > > prepareObjectInfo(int rows, int cols, int **labels_1, int **labels_2, int **&dmerge, int &NO)
{
	//Merging of images & finding Minimum Bounding Boxes of objects and storing this info - starts here---

	//labels_1 contains segmentation result of time 1
	//labels_2 contains segmentation result of time 2
	
	int count=0,i,j,label;//k,label;

	int val1, val2,max=-1;

//	FILE *obj_info;
//	obj_info = fopen("objectinfo_dump.txt","w");
//	int **meta_obj;//this is going to be a NOx6 array storing xmin xmax ymin ymax size of the object
	vector< vector < int > >meta_obj;
	vector< int >v(6);
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

void Image::createSegmentedImageInfo(int **merge_map)
{
	int row,col;
	row = this->getImageRows();
	col = this->getImageCols();

	for(int i=0; i<row; i++)
		for(int j=0; j<col; j++)
			this->segmentedImageInfo[i][j] = merge_map[i][j]-1;

	return;
}


int Image::getNOB()
{
	return this->NOB;

}

int *Image::getPixelData(int x, int y)
{
	return this->data[x][y];
}

void Object::setObjectInfo(Image *parentImage)
{
	int count=0;
	this->parentImage = parentImage;

	for(int i=this->xmin; i<=this->xmax; i++)
	{
		for(int j=this->ymin; j<=this->ymax; j++)
		{
		//	printf("%d %d\n",this->parentImage->segmentedImageInfo[i][j],this->id);
			if(this->parentImage->segmentedImageInfo[i][j] == this->id)
			{
				this->mask[i-this->xmin][j-this->ymin] = 1;
				count++;
			}
			else
				this->mask[i-this->xmin][j-this->ymin] = 0;
		}
	}
	//printf("nop: %d\n",count);
}

void Image::createObjects(int NO, vector< vector< int > > objectInfo)
{
//	this->NO = *max_element(*segmentedImageInfo,(*segmentedImageInfo)+(this->ROWS * this->COLS));
	this->NO = NO;
	this->objectArray = (Object *)malloc(sizeof(Object)*this->NO);
	//FILE *objectinfo;
	//objectinfo = fopen("objectinfo_dump_2.txt","w");

	for(int i=0; i<NO; i++)
	{
			this->objectArray[i] = Object(objectInfo[i][0],objectInfo[i][1],objectInfo[i][2],objectInfo[i][3],objectInfo[i][4],objectInfo[i][5]);
			this->objectArray[i].setObjectInfo(this);
	//		fprintf(objectinfo,"%d %d %d %d %d %d\n",objectInfo[i][0],objectInfo[i][1],objectInfo[i][2],objectInfo[i][3],objectInfo[i][4],objectInfo[i][5]);
            std::cout<<std::endl;
            //std::cout<<"calling attribute function for object no - "<< i <<std::endl;
			this->objectArray[i].createAttribute(i);
			//cout<<"after call::object finished - "<<i<<endl;
	}
//    std::cout<<std::endl;
	//	fclose(objectinfo);
	return;
}


void Object::createAttribute(int objNumber)
{
    //cout<<"just inside createatt\n";
    int cnt=0,i,j;
    float ssum_red=0.0,ssum_green=0.0,ssum_blue=0.0,ssum_pan=0.0;
    float val = 0.0;
    int centroid_x = 0,centroid_y = 0;
	int perimeter =0;
    //cout<<"checkpoint: before accessing image data\n";
    int label = this->id;

    int rows = this->parentImage->getImageRows();
    int cols = this->parentImage->getImageCols();
//	int centroid_x_int, centroid_y_int;
    //cout << "inside attribute func\n";
    int myi,myj,myflagg;
    int nob;
    nob = this->parentImage->getNOB();

    int **temp_serial_data, **temp_bb_data, *sumPixelData, *pixeldata;
    sumPixelData = (int *)malloc(sizeof(int)*nob);

    temp_serial_data = (int **)malloc(sizeof(int *)*nob);
    for(i=0;i<nob;i++)
        temp_serial_data[i] = (int *)malloc(sizeof(int)*this->NOP);

    for(i=0;i<nob;i++)
        sumPixelData[i] = 0;

    temp_bb_data = (int **)malloc(sizeof(int *)*(this->xmax - this->xmin + 1));
    for(i=0; i<=this->xmax - this->xmin; i++)
        temp_bb_data[i] = (int *)malloc((this->ymax - this->ymin+1)*sizeof(int));

    pixeldata = (int *)malloc(sizeof(int)*nob);

    //printf("BB: %d %d %d %d\n",this->xmin,this->xmax,this->ymin,this->ymax);
    for(i=this->xmin;i<=this->xmax;i++)
    {
        for(j=this->ymin;j<=this->ymax;j++)
        {
    //		printf("%d ",this->mask[i-this->xmin][j-this->ymin]);

            if((i==this->xmin || i ==this->xmax || j == this->ymin || j == this->ymax) && this->mask[i-this->xmin][j-this->ymin])
            {
                perimeter += 1;

            }
            else if(this->mask[i-this->xmin][j-this->ymin])//==label)
            {
                myflagg=0;
                for(myi=i-1;myi<=i+1;myi++)
                {
                    for(myj=j-1;myj<=j+1;myj++)
                    {

                        if(myi>=this->xmin && myi<=this->xmax && myj<=this->ymax && myj>=this->ymin)
                        {

                            if(!this->mask[myi-this->xmin][myj-this->ymin])
                            {
                                myflagg=1;
                                break;
                            }
                        }

                    }
                    if(myflagg==1)
                        break;
                }

                if(myflagg==1)
                {
                    perimeter += 1;
                }
            }

        //	printf("perimeter: %d\n",perimeter);

            if(this->mask[i-this->xmin][j-this->ymin])//this means that this point will be of use to us....
            {
                pixeldata = this->parentImage->getPixelData(i,j);

    /*			printf("i:%d j:%d pixelvalue:%d\n",i,j,pixeldata[0]);
                if(pixeldata[0]>255)continue;//pixeldata[0]=255;
                if(pixeldata[1]>255)continue;//pixeldata[1]=255;
                if(pixeldata[2]>255)continue;//pixeldata[2]=255;
    */
                for(int b=0; b<nob; b++)
                    sumPixelData[b] = sumPixelData[b] + pixeldata[b];
    /*			if(pixeldata[0]>255)continue;//pixeldata[0]=255;
                if(pixeldata[1]>255)continue;//pixeldata[1]=255;
                if(pixeldata[2]>255)continue;//pixeldata[2]=255;
    */
                //printf("i:%d j:%d pixelvalue:%d\n",i,j,pixeldata[0]);
                for(int b=0; b<nob; b++)
                {
                    temp_serial_data[b][cnt] = pixeldata[b];
                //	printf("%d ",temp_serial_data[b][cnt]);
                }
                //printf("\n");
                cnt++;

                // Calculating centroid....
                centroid_x += i;
                centroid_y += j;
            }
        }
    }
        //std::cout<<perimeter<<std::endl;
    //printf("sumpixeldata: %d\n",sumPixelData[0]);
    //preparing means
    if(nob > 1)
    {
        this->fVector.meanRed = sumPixelData[0]/this->NOP;
    //	printf("mean1: %d\n",this->fVector.meanRed);
        this->fVector.meanGreen = sumPixelData[1]/this->NOP;
    //	printf("mean2: %d\n",this->fVector.meanGreen);
        this->fVector.meanBlue = sumPixelData[2]/this->NOP;
    //	printf("mean3: %d\n",this->fVector.meanBlue);
    //	printf("nop: %d\n",this->NOP);
    }
    else
        this->fVector.meanPan = sumPixelData[0]/this->NOP;
    //printf("nop: %d\n",this->NOP);
    //printf("means: %d %d %d\n",this->fVector.meanRed,this->fVector.meanGreen,this->fVector.meanBlue);
    //preparind standard deviation values
    if(nob > 1)
    {
        //for(i=0;i<this->NOP;i++)
        for(i=0;i<cnt;i++)
        {
            val=(float) (temp_serial_data[0][i] - this->fVector.meanRed) * (temp_serial_data[0][i] - this->fVector.meanRed);
    //		printf("pixel no : %d, val : %f, temp_serial: %d , mean Red: %d\n",i,val,temp_serial_data[0][i],this->fVector.meanRed);
            ssum_red=ssum_red+val;

            val=(float) (temp_serial_data[1][i] - this->fVector.meanGreen) * (temp_serial_data[1][i] - this->fVector.meanGreen);
            ssum_green=ssum_green+val;

            val=(float) (temp_serial_data[2][i] - this->fVector.meanBlue) * (temp_serial_data[2][i] - this->fVector.meanBlue);
            ssum_blue=ssum_blue+val;

        }

    //	printf("ssum_red: %f\n",ssum_red);
        this->fVector.stdRed = sqrt((float) ssum_red/this->NOP);
        this->fVector.stdRed = floorf(this->fVector.stdRed * 10000) / 10000;
        this->fVector.stdGreen = sqrt((float) ssum_green/this->NOP);
        this->fVector.stdGreen = floorf(this->fVector.stdGreen * 10000) / 10000;
        this->fVector.stdBlue = sqrt((float) ssum_blue/this->NOP);
        this->fVector.stdBlue = floorf(this->fVector.stdBlue * 10000) / 10000;
    }
    else
    {
        //for(i=0;i<this->NOP;i++)
        for(i=0;i<cnt;i++)
        {
            val=(float) (temp_serial_data[0][i]-this->fVector.meanPan) * (temp_serial_data[0][i]-this->fVector.meanPan);
            ssum_pan=ssum_pan+val;
        }
        this->fVector.stdPan = sqrt((float) ssum_pan/this->NOP);
    }
    //	printf("std: %f %f %f\n",this->fVector.stdRed,this->fVector.stdGreen,this->fVector.stdBlue);

    //writing perimeter
    this->fVector.perimeter = perimeter;
    this->fVector.area = this->NOP;
    // Preparing centroids-------------------------------
    this->fVector.xCentroid = centroid_x / this->NOP;
    this->fVector.yCentroid = centroid_y / this->NOP;

    // Calculating roundness-----------------------------
    this->fVector.roundness =(float) (4.0 * 3.14 * this->NOP)/(perimeter * perimeter);
     this->fVector.roundness = floorf(this->fVector.roundness * 10000) / 10000;

    // Calculating compactness---------------------------
    this->fVector.compactness = (float) (perimeter / (3.53 * sqrt((float) this->NOP)));
    this->fVector.compactness = floorf(this->fVector.compactness * 10000) / 10000;

    //printf("xcent:%d ycent:%d round:%f comp:%f\n",this->fVector.xCentroid,this->fVector.yCentroid,this->fVector.roundness,this->fVector.compactness);
    //Modifying the Bounding Box for the calculation of Tamura Features
        //Padding the pixel locations which are not the object with mean rgb values


    int avg_val;
    for(i=0; i<rows; i++)
    {
        for(j=0; j<cols; j++)
        {
            if(i>=this->xmin && i<=this->xmax )
            {
                if( j>=this->ymin && j<=this->ymax)
                {
                    if(this->mask[i-this->xmin][j-this->ymin])
                    {
    //					printf("obj found\n");
                        pixeldata = this->parentImage->getPixelData(i,j);
                    /*	if(pixeldata[0]>255)
                        {
                            pixeldata[0] = this->fVector.meanRed;
                            printf("DDDDDDDDDDDDDD\n");
                        }
                        if(pixeldata[1]>255)
                        {
                            pixeldata[1] = this->fVector.meanGreen;
                            printf("DDDDDDDDDDDDDD\n");
                        }
                        if(pixeldata[2]>255)
                        {
                            pixeldata[2] = this->fVector.meanBlue;
                            printf("DDDDDDDDDDDDDD\n");
                        }
                    */
                        if(nob > 1)
                            avg_val = (pixeldata[0] + pixeldata[1] + pixeldata[2])/3;
                        else
                            avg_val = pixeldata[0];

                        temp_bb_data[i-this->xmin][j-this->ymin] = avg_val;
                    }
                    else
                    {
                        if(nob > 1)
                            avg_val = (this->fVector.meanRed + this->fVector.meanGreen + this->fVector.meanBlue)/3;
                        else
                            avg_val = this->fVector.meanPan;

                        temp_bb_data[i-this->xmin][j-this->ymin] = avg_val;
                    }
                }
            }
        }
    }

    //printf("after duplication\n");
    //Calculating tamura features-----------------------
    int Nx = this->xmax-this->xmin+1, Ny = this->ymax-this->ymin+1;
    int Ng = 256;
    int myflag=0;

    float **E0h,**E0v;
    float **E1h,**E1v;
    float **E2h,**E2v;
    float **E3h,**E3v;
    float **E4h,**E4v;
    float **E5h,**E5v;
    float **theta,**Sbest;

    E0h = (float **)malloc(Nx*sizeof(float *));
    E0v = (float **)malloc(Nx*sizeof(float *));
    E1h = (float **)malloc(Nx*sizeof(float *));
    E1v = (float **)malloc(Nx*sizeof(float *));
    E2h = (float **)malloc(Nx*sizeof(float *));
    E2v = (float **)malloc(Nx*sizeof(float *));
    E3h = (float **)malloc(Nx*sizeof(float *));
    E3v = (float **)malloc(Nx*sizeof(float *));
    E4h = (float **)malloc(Nx*sizeof(float *));
    E4v = (float **)malloc(Nx*sizeof(float *));
    E5h = (float **)malloc(Nx*sizeof(float *));
    E5v = (float **)malloc(Nx*sizeof(float *));

    theta = (float **)malloc(Nx*sizeof(float *));
    Sbest = (float **)malloc(Nx*sizeof(float *));
    for(i=0;i<Nx;i++)
    {
        E0h[i] = (float *)malloc(Ny*sizeof(float));
        E0v[i] = (float *)malloc(Ny*sizeof(float));
        E1h[i] = (float *)malloc(Ny*sizeof(float));
        E1v[i] = (float *)malloc(Ny*sizeof(float));
        E2h[i] = (float *)malloc(Ny*sizeof(float));
        E2v[i] = (float *)malloc(Ny*sizeof(float));
        E3h[i] = (float *)malloc(Ny*sizeof(float));
        E3v[i] = (float *)malloc(Ny*sizeof(float));
        E4h[i] = (float *)malloc(Ny*sizeof(float));
        E4v[i] = (float *)malloc(Ny*sizeof(float));
        E5h[i] = (float *)malloc(Ny*sizeof(float));
        E5v[i] = (float *)malloc(Ny*sizeof(float));

        theta[i] = (float *)malloc(Ny*sizeof(float));
        Sbest[i] = (float *)malloc(Ny*sizeof(float));

    }

    for(i=0;i<Nx;i++)
    {
        for(j=0;j<Ny;j++)
        {
            E0h[i][j]=0.0;E0v[i][j]=0.0;
            E1h[i][j]=0.0;E1v[i][j]=0.0;
            E2h[i][j]=0.0;E2v[i][j]=0.0;
            E3h[i][j]=0.0;E3v[i][j]=0.0;
            E4h[i][j]=0.0;E4v[i][j]=0.0;
            E5h[i][j]=0.0;E5v[i][j]=0.0;

            Sbest[i][j] = 0.0;
            theta[i][j] = 0.0;
        }
    }

    //printf("after array init\n");
    for(i=this->xmin; i<=this->xmax ; i++)
        for(j=this->ymin+1; j<=this->ymax; j++)
            E0h[i-this->xmin][j-this->ymin] =(float) ((temp_bb_data[i-this->xmin][j-this->ymin] - temp_bb_data[i-this->xmin][j-this->ymin-1])/2.0);

    for(i=this->xmin; i<this->xmax ; i++)
        for(j=this->ymin; j<=this->ymax; j++)
            E0v[i-this->xmin][j-this->ymin] =(float) ((temp_bb_data[i-this->xmin][j-this->ymin] - temp_bb_data[i-this->xmin+1][j-this->ymin])/2.0);

    if (Nx<4 || Ny<4)
        myflag = 1;

    if(myflag == 0)
    {
        for(i=this->xmin; i<this->xmax; i++)
        {
            for(j=this->ymin+2; j<this->ymax; j++)
            {
                int sum1 = temp_bb_data[i-this->xmin][j-this->ymin] + temp_bb_data[i-this->xmin][j-this->ymin+1] +
                    temp_bb_data[i-this->xmin+1][j-this->ymin] + temp_bb_data[i-this->xmin+1][j-this->ymin+1];
                int sum2 = temp_bb_data[i-this->xmin][j-this->ymin-2] + temp_bb_data[i-this->xmin][j-this->ymin-1] +
                    temp_bb_data[i-this->xmin+1][j-this->ymin-2] + temp_bb_data[i-this->xmin+1][j-this->ymin-1];

                E1h[i-this->xmin][j-this->ymin] = (float) (sum1 - sum2)/4;
            }
        }

        for(i=this->xmin+1; i<this->xmax-1; i++)
        {
            for(j=this->ymin+1; j<=this->ymax; j++)
            {
                int sum1 = temp_bb_data[i-this->xmin-1][j-this->ymin] + temp_bb_data[i-this->xmin][j-this->ymin] +
                        temp_bb_data[i-this->xmin-1][j-this->ymin-1] + temp_bb_data[i-this->xmin][j-this->ymin-1];
                int sum2 = temp_bb_data[i-this->xmin+1][j-this->ymin-1] + temp_bb_data[i-this->xmin+1][j-this->ymin] +
                        temp_bb_data[i-this->xmin+2][j-this->ymin-1] + temp_bb_data[i-this->xmin+2][j-this->ymin];

                E1v[i-this->xmin][j-this->ymin] =(float) (sum1 - sum2)/4;
            }
        }
    }

    //printf("after loop:4\n");
    if(Nx<8 || Ny<8)
        myflag = 1;

    if(myflag == 0)
    {
        for(i=this->xmin+1; i<this->xmax-1; i++)
        {
            for(j=this->ymin+4; j<this->ymax-2; j++)
            {
                int sum1=0,sum2=0;
                for(int k=i-1; k<=i+2; k++ )
                    for( int l=j; l<=j+3; l++)
                        sum1 += temp_bb_data[k-this->xmin][l-this->ymin];
                for(int k=i-1; k<=i+2; k++ )
                    for( int l=j-4; l<=j-1; l++)
                        sum2 += temp_bb_data[k-this->xmin][l-this->ymin];

                E2h[i-this->xmin][j-this->ymin] =(float) (sum1 - sum2)/16;
            }
        }

        for(i=this->xmin+3; i<this->xmax-4; i++)
        {
            for(j=this->ymin+2; j<this->ymax-1; j++)
            {
                int sum1=0,sum2=0;
                for(int k=i-3; k<=i; k++ )
                    for( int l=j-2; l<=j+1; l++)
                        sum1 += temp_bb_data[k-this->xmin][l-this->ymin];
                for(int k=i+1; k<=i+4; k++ )
                    for( int l=j-2; l<=j+1; l++)
                        sum2 += temp_bb_data[k-this->xmin][l-this->ymin];

                E2v[i-this->xmin][j-this->ymin] = (float) (sum1 - sum2)/16;
            }
        }
    }
    //printf("after loop:8\n");

    if (Nx<16||Ny<16)
        myflag=1;
    if(myflag == 0)
    {
        for(i=this->xmin+3; i<this->xmax-4; i++)
        {
            for(j=this->ymin+8; j<this->ymax-7; j++)
            {
                int sum1=0,sum2=0;
                for(int k=i-3; k<=i+4; k++ )
                    for( int l=j; l<=j+7; l++)
                        sum1 += temp_bb_data[k-this->xmin][l-this->ymin];
                for(int k=i-3; k<=i+4; k++ )
                    for( int l=j-8; l<=j-1; l++)
                        sum2 += temp_bb_data[k-this->xmin][l-this->ymin];

                E3h[i-this->xmin][j-this->ymin] = (float) (sum1 - sum2)/64;
            }
        }

        for(i=this->xmax+7; i<this->xmax-8; i++)
        {
            for(j=this->ymin+4; j<this->ymax-3; j++)
            {
                int sum1=0,sum2=0;
                for(int k=i-7; k<=i; k++ )
                    for( int l=j-4; l<=j+3; l++)
                        sum1 += temp_bb_data[k-this->xmin][l-this->ymin];
                for(int k=i+1; k<=i+8; k++ )
                    for( int l=j-4; l<=j+3; l++)
                        sum2 += temp_bb_data[k-this->xmin][l-this->ymin];

                E3v[i-this->xmin][j-this->ymin] = (float) (sum1 - sum2)/64;
            }
        }
    }
    //printf("after loop:16\n");


    if (Nx<32||Ny<32)
        myflag=1;
    if(myflag == 0)
    {
        for(i=this->xmin+7; i<this->xmax-8; i++)
        {
            for(j=this->ymin+16; j<this->ymax-15; j++)
            {
                int sum1=0,sum2=0;
                for(int k=i-7; k<=i+8; k++ )
                    for( int l=j; l<=j+15; l++)
                        sum1 += temp_bb_data[k-this->xmin][l-this->ymin];
                for(int k=i-7; k<=i+8; k++ )
                    for( int l=j-16; l<=j-1; l++)
                        sum2 += temp_bb_data[k-this->xmin][l-this->ymin];

                E4h[i-this->xmin][j-this->ymin] = (float) (sum1 - sum2)/256;
            }
        }

        for(i=this->xmin+15; i<this->xmax-16; i++)
        {
            for(j=this->ymin+8; j<this->ymax-7; j++)
            {
                int sum1=0,sum2=0;
                for(int k=i-15; k<=i; k++ )
                    for( int l=j-8; l<=j+7; l++)
                        sum1 += temp_bb_data[k-this->xmin][l-this->ymin];
                for(int k=i+1; k<=i+16; k++ )
                    for( int l=j-8; l<=j+7; l++)
                        sum2 += temp_bb_data[k-this->xmin][l-this->ymin];

                E4v[i-this->xmin][j-this->ymin] = (float) (sum1 - sum2)/256;
            }
        }
    }
    //printf("after loop:32\n");

    if (Nx<64||Ny<64)
        myflag=1;

    if(myflag == 0)
    {
        for(i=this->xmin+15; i<this->xmax-16; i++)
        {
            for(j=this->ymin+32; j<this->ymax-31; j++)
            {
                int sum1=0,sum2=0;
                for(int k=i-15; k<=i+16; k++ )
                    for( int l=j; l<=j+31; l++)
                        sum1 += temp_bb_data[k-this->xmin][l-this->ymin];
                for(int k=i-15; k<=i+16; k++ )
                    for( int l=j-32; l<=j-31; l++)
                        sum2 += temp_bb_data[k-this->xmin][l-this->ymin];

                E5h[i-this->xmin][j-this->ymin] = (float) (sum1 - sum2)/1024;
            }
        }

        for(i=this->xmin+31; i<this->xmax-32; i++)
        {
            for(j=this->ymin+16; j<this->ymax-15; j++)
            {
                int sum1=0,sum2=0;
                for(int k=i-31; k<=i; k++ )
                    for( int l=j-16; l<=j+15; l++)
                        sum1 += temp_bb_data[k-this->xmin][l-this->ymin];
                for(int k=i+1; k<=i+32; k++ )
                    for( int l=j-16; l<=j+15; l++)
                        sum2 += temp_bb_data[k-this->xmin][l-this->ymin];

                E5v[i-this->xmin][j-this->ymin] = (float) (sum1 - sum2)/1024;
            }
        }
    }


    int ind=0,temp_ind;
    float Fcoarseness=0.0;
	float maxv = -10000.0;
    for(i=0; i<Nx; i++)
    {
        for(j=0; j<Ny; j++)
        {
            if(E0h[i][j] > maxv){  maxv = E0h[i][j]; ind = 1;  }
            if(E0v[i][j] > maxv){  maxv = E0v[i][j]; ind = 2;  }

            if(E1h[i][j] > maxv){  maxv = E1h[i][j]; ind = 3;  }
            if(E1v[i][j] > maxv){  maxv = E1v[i][j]; ind = 4;  }

            if(E2h[i][j] > maxv){  maxv = E2h[i][j]; ind = 5;  }
            if(E2v[i][j] > maxv){  maxv = E2v[i][j]; ind = 6;  }

            if(E3h[i][j] > maxv){  maxv = E3h[i][j]; ind = 7;  }
            if(E3v[i][j] > maxv){  maxv = E3v[i][j]; ind = 8;  }

            if(E4h[i][j] > maxv){  maxv = E4h[i][j]; ind = 9;   }
            if(E4v[i][j] > maxv){  maxv = E4v[i][j]; ind = 10;  }

            if(E5h[i][j] > maxv){  maxv = E5h[i][j]; ind = 11;  }
            if(E5v[i][j] > maxv){  maxv = E5v[i][j]; ind = 12;  }

            temp_ind = (int) floor((float)(ind+1)/2);
            Sbest[i][j] = pow((float) 2.0,temp_ind);
        }
    }

    for(i=0; i<Nx; i++)
        for(j=0; j<Ny; j++)
            Fcoarseness += Sbest[i][j];

    Fcoarseness = (float) (Fcoarseness/(Nx*Ny));
    this->fVector.coarseness = Fcoarseness;
    this->fVector.coarseness = floorf(this->fVector.coarseness * 10000) / 10000;

    //printf("coar:%f\n",this->fVector.coarseness);
    //cout<<"all is well till the end of coarseness\n";

    /**computing histogram for Tamura's contrast feature**/
    int *counts;
           float *PI;
           PI = (float *)malloc(256*sizeof(float));
           counts = (int *)malloc(256*sizeof(int));
           for(i=0; i<256; i++)
                   counts[i] = 0;
           float averagevalue=0, u4=0, standarddev=0, alpha4=0, Fcontrast=0.0;
           for(i=0; i<Nx; i++)
           {
                   for(j=0; j<Ny; j++)
                   {
                           //if(temp_bb_data[i + this->xmin][j + this->ymin]>255)
                           if(temp_bb_data[i][j]>255)
                                   counts[255]++;
                           //else if(temp_bb_data[i + this->xmin][j + this->ymin]<0)
                           else if(temp_bb_data[i][j]<0)
                                   counts[0]++;
                           else
                           {
                                   //cout<<"here inside hist\n";
                                   //counts[temp_bb_data[i + this->xmin][j + this->ymin]]++;
                                   counts[temp_bb_data[i][j]]++;
                           }
                   }
           }
           /*for(i=0; i<256; i++)
           {
                   if(counts[i] > 0)
                           cout<<"bin not empty: "<< i <<" with count: "<< counts[i]<<endl;
           }
           */
           for(i=0; i<256; i++)
           {
                   PI[i] = (float)1.0*counts[i]/(Nx*Ny);
                   averagevalue += (PI[i]*i);
           //        cout<<"PI value now: "<<PI[i]<<endl;
           //        cout<<"avg value now: "<<averagevalue <<endl;
           }
           //cout << "avg value: "<<averagevalue << " :: ";
           for(i=0; i<256; i++)
           {
                   u4 += PI[i]*pow(fabs(i-averagevalue),4);
                   standarddev += PI[i]*pow(fabs(i-averagevalue),2);
           }



           if(standarddev == 0)
                   Fcontrast = 0.0;
           else{
                   alpha4 = u4/(standarddev*standarddev);
                   Fcontrast = sqrt(fabs(standarddev))/(sqrt(sqrt(fabs(alpha4))));
           }
           //cout<< "u4: "<<u4<<" :: std: " << standarddev <<" "" cont "<<Fcontrast<<endl;

           this->fVector.contrast = Fcontrast;
           this->fVector.contrast = floorf(this->fVector.contrast * 10000) / 10000;


    /***direction feature***/

    int prewittH[3][3], prewittV[3][3];
    int **deltaH,**deltaV;
   // int deltaG;

    deltaH = (int **)malloc(Nx*sizeof(int *));
    deltaV = (int **)malloc(Nx*sizeof(int *));

    for(i=0;i<Nx; i++)
    {
        deltaH[i] = (int *)malloc(Ny*sizeof(int));
        deltaV[i] = (int *)malloc(Ny*sizeof(int));
    }

    prewittH[0][0] = -1;prewittH[0][1] = 0; prewittH[0][2] = 1;
    prewittH[1][0] = -1;prewittH[1][1] = 0; prewittH[1][2] = 1;
    prewittH[2][0] = -1;prewittH[2][1] = 0; prewittH[2][2] = 1;

    prewittV[0][0] =  1;prewittV[0][1] =  1; prewittV[0][1] =  1;
    prewittV[1][0] =  0;prewittV[1][1] =  0; prewittV[1][2] =  0;
    prewittV[2][0] = -1;prewittV[2][1] = -1; prewittV[2][2] = -1;

    for(i=0;i<Nx;i++)
        for(j=0;j<Ny;j++)
        {
            deltaH[i][j]=0;
            deltaV[i][j]=0;
        }

    //---//
    for(i=this->xmin+1;i<this->xmax;i++)
        for(j=this->ymin+1;j<this->ymax;j++)
            for(int m=0; m<3; m++)
                for(int n=0; n<3; n++)
                    deltaH[i-this->xmin][j-this->ymin] +=  prewittH[m][n] * temp_bb_data[i-this->xmin+m-1][j-this->ymin+n-1];

    for(j=this->ymin+1; j<this->ymax; j++)
    {
        deltaH[0][j-this->ymin] = temp_bb_data[0][j-this->ymin+1] - temp_bb_data[0][j-this->ymin];
        deltaH[Nx-1][j-this->ymin] = temp_bb_data[Nx-1][j-this->ymin+1] - temp_bb_data[Nx-1][j-this->ymin];
    }

    for(i=this->xmin; i<=this->xmax; i++)
    {
        deltaH[i-this->xmin][0] = temp_bb_data[i-this->xmin][1] - temp_bb_data[i-this->xmin][0];
        deltaH[i-this->xmin][Ny-1] = temp_bb_data[i-this->xmin][Ny-1] - temp_bb_data[i-this->xmin][Ny-2];
    }

    //---//
    for(i=this->xmin+1;i<this->xmax;i++)
        for(j=this->ymin+1;j<this->ymax;j++)
            for(int m=0; m<3; m++)
                for(int n=0; n<3; n++)
                    deltaV[i-this->xmin][j-this->ymin] += prewittV[m][n]*temp_bb_data[i-this->xmin+m-1][j-this->ymin+n-1];


/*
    for(j=this->ymin; j<=this->ymax; j++)
    {
        deltaV[0][j-this->ymin-1] = temp_bb_data[1][j-this->ymin] - temp_bb_data[0][j-this->ymin];
        deltaV[Nx-1][j-this->ymin-1] = temp_bb_data[Nx-1][j-this->ymin] - temp_bb_data[Nx-2][j-this->ymin];
    }

    for(i=this->xmin+1; i<this->xmax; i++)
    {
        deltaV[i-this->xmin][0] = temp_bb_data[i-this->xmin+1][0] - temp_bb_data[i-this->xmin][0];
        deltaV[i-this->xmin][Ny-1] = temp_bb_data[i-this->xmin+1][Ny-1] - temp_bb_data[i-this->xmin][Ny-1];
    }
*/
    for(i=0; i<Nx; i++)
    {
        for(j=0; j<Ny; j++)
        {
            if(deltaH[i][j]==0 && deltaV[i][j]==0);
            else if(deltaH[i][j]==0)
                theta[i][j] = (float)3.14;
            else
                theta[i][j] = atan((float) deltaV[i][j]/deltaH[i][j]) + (float) (3.14/2);
        }
    }

    float *phai, *phai2;
    phai = (float *)malloc(31416*sizeof(float));

    phai2 = (float *)malloc(31416*sizeof(float));
    for(i=0; i<31416; i++){
        phai[i] = 0.0;
        phai2[i] = 0.0;
    }
    double THRESHOLD=0.01, phaiP = 0.0, Fdirection = 0.0;
    int mv=-1,mi=0;

    for(i=0; i<Nx; i++)
        for(j=0; j<Ny; j++)
            phai[(int)theta[i][j]*10000] =(float) ( phai[(int)theta[i][j]*10000]+1.0);

    for(i=0; i<31416; i++)
        phai[i] =(float)  phai[i]/(Nx*Ny);

    for(i=0; i<31416; i++)
    {
        if(phai[i] >= THRESHOLD)
            phai2[i] = phai[i];
        else
            phai2[i] = 0;
        if(phai2[i]>mv)
        {
            mv = (int) phai2[i];
            mi = i;
        }
    }

    phaiP = mi*0.0001;
    for(i=0; i<31416; i++)
    {
        if(phai2[i] != 0)
            Fdirection += (pow(abs(phai[i] - phaiP),2) * phai2[i]);
    }
    this->fVector.direction = (float) Fdirection;
    this->fVector.direction = floorf(this->fVector.direction * 10000) / 10000;

    //Roughness feature
           float Froughness = Fcoarseness + Fcontrast;
           this->fVector.roughness = Froughness;
           this->fVector.roughness = floorf(this->fVector.roughness * 10000) / 10000;
    //Tamura Feature Extraction ends here------------

    free(temp_bb_data);
    free(sumPixelData);
    free(temp_serial_data);
    free(pixeldata);
    free(E0h);
    free(E0v);
    free(E1h);
    free(E1v);
    free(E2h);
    free(E2v);
    free(E3h);
    free(E3v);
    free(E4h);
    free(E4v);
    free(E5h);
    free(E5v);
    free(Sbest);
    free(theta);
    free(counts);
    free(PI);
    free(deltaH);
    free(deltaV);
    free(phai);
    free(phai2);
    //cout<<"returning after finishing obj \n";
    /*FILE *feat;
    if(objNumber == 0)
        feat = fopen("feature_dump.txt","w");
    else
        feat = fopen("feature_dump.txt","a");
    fprintf(feat,"%d %d %d %f %f %f %d %d %d %d %f %f %f %f\n",this->fVector.meanRed,this->fVector.meanGreen,this->fVector.meanBlue,
                                    this->fVector.stdRed,this->fVector.stdGreen,this->fVector.stdBlue,
                                    this->fVector.area,this->fVector.perimeter,this->fVector.xCentroid,this->fVector.yCentroid,
                                    this->fVector.roundness,this->fVector.compactness,this->fVector.coarseness,this->fVector.direction);

    fclose(feat);
    */
    return ;
}
void fillRegionChanged(int rows, int cols, int info1[], int info2[],int **dt1,int **dt2,int **&dmerge,int val1,int val2,int label,int bi,int bj,vector< float> &meta_obj)
{
    //dt1 is map of time1
    //dt2 is map of time2
    //dmerge is the merged map
    //val1 and val2 is the value pair of the new object
    // bi and bj are the indexes of the first encountered pixel of this object
    queue<int> myq;
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
    int s_o1,s_o2;

    s_o1=info1[val1];
    s_o2=info2[val2];

    meta_obj[0]=(float)label-2;
    meta_obj[1]=(float)val1;
    meta_obj[2]=(float)val2;
    meta_obj[3]=(float)(nop*1.0)/s_o1;
    meta_obj[4]=(float)(nop*1.0)/s_o2;
    meta_obj[5]=(float)nop;

    return;

}
vector< vector< float > > changed(int rows, int cols, int **dt1, int **dt2, int info1[],int info2[], int NO1, int NO2)
{
    int i,j,label;
    int val1,val2;
    int **dmerge;
    dmerge=(int **)malloc(rows*sizeof( int *));

    for(i=0;i<rows;i++)
    {
        dmerge[i]=(int *)malloc(cols*sizeof(int));
    }

    vector< vector< float > >pieceInfo;
    for(i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {
            dmerge[i][j]=-1;
        }
    }

    //preparing merge map
    label=2;
    vector< float >v(6);
    for(i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {
            if(dmerge[i][j]<0)//this means new object will start from this pixel
            {
                val1=(int)dt1[i][j];
                val2=(int)dt2[i][j];

                //this function will fill the newly touched region
                fillRegionChanged(rows, cols, info1, info2, dt1,dt2,dmerge,val1,val2,label,i,j,v);
                pieceInfo.push_back(v);
                label++;

            }
        }
    }

    //the merged map has been prepared with label number of objects in it..
    return pieceInfo;
}

int *changeReporting(int ROWS, int COLS, float LOW, float HIGH, vector< vector < float > > pinfo, int **merge_map)
{

    int NOI = pinfo.size();

    int i,L1,M1,H1,L2,M2,H2;

    int *code;//this is to store what code each object is getting

    code =(int *)malloc(sizeof(int)*NOI);

    float ratio1,ratio2;

    printf("coding begins\n");

    for(i=0;i<NOI;i++)
    {
        ratio1=pinfo[i][3];
        ratio2=pinfo[i][4];
        L1=0; L2=0; M1=0; M2=0; H1=0; H2=0;//initialising variable for deciding

        if(ratio1<=LOW)
            L1=1;
        if(ratio1>LOW && ratio1 <HIGH)
            M1=1;
        if(ratio1>=HIGH)
            H1=1;

        if(ratio2<=LOW)
            L2=1;
        if(ratio2>LOW && ratio2 <HIGH)
            M2=1;
        if(ratio2>=HIGH)
            H2=1;

        if(L1==1 && L2==1)
            code[i]=1;

        if(L1==1 && M2==1)
            code[i]=3;

        if(L1==1 && H2==1)
            code[i]=2;

        if(M1==1 && L2==1)
            code[i]=4;

        if(M1==1 && M2==1)
            code[i]=5;

        if(M1==1 && H2==1)
            code[i]=6;

        if(H1==1 && L2==1)
            code[i]=7;

        if(H1==1 && M2==1)
            code[i]=8;

        if(H1==1 && H2==1)
            code[i]=9;
    }
    printf("coding ends\n");

    return code;
}
