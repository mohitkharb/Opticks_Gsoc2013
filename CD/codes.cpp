#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<cmath>
#include<malloc.h>
#include<string.h>
#include"codes.h"
#include<algorithm>
#include<queue>


#define CHECK_EXTREMUM(a,b)            \
  if((diff[a] * diff[b] < 0) ||        \
     ((diff[a] * diff[b] == 0) &&      \
      (diff[a] + diff[b] != 0))) {     \
    extrema[n_extrema] = b;            \
    is_peak[n_extrema] = diff[b] < 0;  \
    n_extrema++;                       \
  }


using namespace std;

/* Functions *****************************************************************/

/* Vector dot product. */
double dot_product(double *a, double *b, int len)
{
  double r = 0.0;
  while(len--) r += *a++ * *b++;
  return r;
}

/* Apply a filter to a matrix.
 * The middle value of the filter is at
 *   ((filter->height+1)/2, (filter->width+1)/2).
 * Not very efficient; could have implemented some special cases (3x3).
 */
Matrix MatrixFilter(Matrix matrix, Matrix filter)
{
  int i,j,k,l;
  int halfy = filter->height/2, halfx = filter->width/2;
  Matrix out = MatrixCreate(matrix->height, matrix->width);
  for(i=0;i<matrix->height;i++) {
    for(j=0;j<matrix->width;j++) {
      Real sum = 0.0;
      for(k=0;k<filter->height;k++) {
    for(l=0;l<filter->width;l++) {
      int ii = (i+k-halfy+matrix->height)%matrix->height,
          ij = (j+l-halfx+matrix->width)%matrix->width;
      sum += filter->data[k][l] * matrix->data[ii][ij];
    }
      }
      out->data[i][j] = sum;
    }
  }
  return out;
}

/* Vector difference */
static void subtract(Real *a, Real *b, int len)
{
  while(len--) *a++ -= *b++;
}

/* Subtract a vector from each row of a matrix */
void MatrixSubtractRow(Matrix matrix, Real *row)
{
  int i;

  for(i=0;i<matrix->height;i++) {
    subtract(matrix->data[i], row, matrix->width);
  }
}

/* Compute a row vector of column sums */
Matrix MatrixColSum(Matrix matrix)
{
  Matrix row;
  int i,j;

  row = MatrixCreate(1, matrix->width);
  MatrixFill(row, 0.0);
  for(i=0;i<matrix->height;i++) {
    for(j=0;j<matrix->width;j++) {
      row->data[0][j] += matrix->data[i][j];
    }
  }
  return row;
}

/* Compute a row vector of column means */
Matrix MatrixColMean(Matrix matrix)
{
  Matrix row;

  row = MatrixColSum(matrix);
  MatrixScale(row, (Real)1/matrix->height);
  return row;
}

/* Compute the maximum and minimum values in a matrix */
void MatrixMinMax(Matrix matrix, Real *minp, Real *maxp)
{
  int y,x;

  *minp = *maxp = matrix->data[0][0];
  for(y=0;y < matrix->height;y++) {
    for(x=0;x < matrix->width;x++) {
      if(matrix->data[y][x] < *minp) *minp = matrix->data[y][x];
      else if(matrix->data[y][x] > *maxp) *maxp = matrix->data[y][x];
    }
  }
}

/* Rescale the values in a matrix to range from new_min to new_max */
void MatrixRescale(Matrix matrix, Real new_min, Real new_max)
{
  Real old_min, old_max;

  MatrixMinMax(matrix, &old_min, &old_max);
  MatrixAddValue(matrix, -old_min);
  if(old_max != old_min)
    MatrixScale(matrix, (new_max - new_min) / (old_max - old_min));
  MatrixAddValue(matrix, new_min);
}

/* Truncate the values in a matrix,
 * i.e. set anything below new_min to new_min, and same for new_max.
 */
void MatrixTruncate(Matrix matrix, Real new_min, Real new_max)
{
  int y,x;

  for(y=0;y<matrix->height;y++) {
    for(x=0;x<matrix->width;x++) {
      if(matrix->data[y][x] < new_min) matrix->data[y][x] = new_min;
      else if(matrix->data[y][x] > new_max) matrix->data[y][x] = new_max;
    }
  }
}

/* Truncate the values in a matrix to [trunc_min, trunc_max] and then
 * rescale to range from new_min to new_max.
 */
void MatrixTruncRescale(Matrix matrix, Real trunc_min, Real trunc_max,
            Real new_min, Real new_max)
{
  int i;
  double factor;

  factor = (new_max - new_min) / (trunc_max - trunc_min);
  for(i=0;i<matrix->height*matrix->width;i++) {
    if(matrix->data[0][i] <= trunc_min) matrix->data[0][i] = new_min;
    else if(matrix->data[0][i] >= trunc_max) matrix->data[0][i] = new_max;
    else
      matrix->data[0][i] = (matrix->data[0][i] - trunc_min) * factor + new_min;
  }
}

/* Apply a unary math function to every element */
void MatrixApplyFunc(Matrix matrix, MathFunc *func)
{
  int y,x;

  for(y=0;y<matrix->height;y++) {
    for(x=0;x<matrix->width;x++) {
      matrix->data[y][x] = func(matrix->data[y][x]);
    }
  }
}

/* Extract the submatrix from (y1, x1) to (y2, x2), inclusive */
Matrix MatrixSub(Matrix matrix, int y1, int x1, int y2, int x2)
{
  Matrix result;
  int range_x, range_y, y, x;

  if((x1 < 0) || (y1 < 0) || (y2 < y1) || (x2 < x1) ||
     (x2 >= matrix->width) || (y2 >= matrix->height)) {
      std::cout<<"matsub error: size"<<std::endl;
    fprintf(stderr,
        "MatrixSub: bad region (%d, %d)-(%d, %d) on %d by %d matrix\n",
        y1, x1, y2, x2, matrix->height, matrix->width);
  }

  range_x = x2 - x1 + 1;
  range_y = y2 - y1 + 1;
  result = MatrixCreate(range_y, range_x);
  for(y=0;y < range_y;y++) {
    for(x=0;x < range_x;x++) {
      result->data[y][x] = matrix->data[y+y1][x+x1];
    }
  }
  return result;
}

/* Sum of all elements */
Real MatrixSum(Matrix matrix)
{
  Real sum;
  int y,x;

  sum = 0;
  for(y=0;y < matrix->height;y++) {
    for(x=0;x < matrix->width;x++) {
      sum += matrix->data[y][x];
    }
  }
  return sum;
}

/* Mean of all elements */
Real MatrixMean(Matrix matrix)
{
  return MatrixSum(matrix) / matrix->width / matrix->height;
}

/* Add a constant value to all elements */
void MatrixAddValue(Matrix matrix, Real value)
{
  int y,x;

  for(y=0;y<matrix->height;y++) {
    for(x=0;x<matrix->width;x++) {
      matrix->data[y][x] += value;
    }
  }
}

/* Copy a matrix */
Matrix MatrixCopy(Matrix matrix)
{
  Matrix result;

  result = MatrixCreate(matrix->height, matrix->width);
  memcpy(result->data[0], matrix->data[0],
     matrix->width*matrix->height*sizeof(Real));
  return result;
}

/* Set all elements to a value */
void MatrixFill(Matrix matrix, Real value)
{
  int y,x;

  for(y=0;y<matrix->height;y++) {
    for(x=0;x<matrix->width;x++) {
      matrix->data[y][x] = value;
    }
  }
}

/* Multiply all elements by a factor */
void MatrixScale(Matrix matrix, Real factor)
{
  int y,x;

  for(y=0;y<matrix->height;y++) {
    for(x=0;x<matrix->width;x++) {
      matrix->data[y][x] *= factor;
    }
  }
}

/* Read a character file as a matrix */
void MatrixReadImage(Matrix matrix, FILE *fp)
{
  unsigned char *buffer;
  int i,bufsize;

  bufsize = matrix->width * matrix->height;
  buffer = Allocate(bufsize, unsigned char);
  if(!buffer) {
    fprintf(stderr, "Cannot allocate %d byte input buffer\n",
        bufsize*sizeof(unsigned char));
    exit(1);
  }
  fread(buffer, sizeof(unsigned char), bufsize, fp);

  for(i=0;i<bufsize;i++) {
    matrix->data[0][i] = (Real)buffer[i];
  }
  free(buffer);
}

/* Write a matrix as a character file by clipping */
void MatrixWriteImage(Matrix matrix, FILE *fp)
{
  unsigned char *buffer;
  int i,bufsize;
  Real v;

  bufsize = matrix->width * matrix->height;
  buffer = Allocate(bufsize, unsigned char);
  if(!buffer) {
    fprintf(stderr, "Cannot allocate %d byte output buffer\n",
        bufsize*sizeof(unsigned char));
    exit(1);
  }
  for(i=0;i<bufsize;i++) {
    v = matrix->data[0][i];
    if(v > 255) v = 255;
    else if(v < 0) v = 0;
    buffer[i] = (unsigned char)v;
  }

  fwrite(buffer, sizeof(unsigned char), bufsize, fp);
  free(buffer);
}

/* Write a matrix as a vector of floating-point numbers, one per line. */
void MatrixWriteVector(Matrix matrix, FILE *fp)
{
  int i;

  for(i=0;i<matrix->height*matrix->width;i++) {
    fprintf(fp, "%lg\n", matrix->data[0][i]);
  }
}

/* Write a matrix as a rectangle of numbers, one row per line */
void MatrixWrite(Matrix matrix, FILE *fp)
{
  int i,j;
  for(i=0;i<matrix->height;i++) {
    for(j=0;j<matrix->width;j++) {
      fprintf(fp, "%g ", matrix->data[i][j]);
    }
    fprintf(fp, "\n");
  }
}

/* Creates a Matrix whose data is given. The array becomes property
 * of the matrix; it will be freed when the matrix is. The array must
 * be height*width elements long.
 */
Matrix MatrixFromArray(int height, int width, double *data)
{
  Matrix result;
  int i;

  result = Allocate(1, struct MatrixStruct);
  result->width = width;
  result->height = height;
  result->data = Allocate(height, Real*);
  result->data[0] = data;
  for(i=1;i<height;i++) {
    result->data[i] = &result->data[0][i*width];
  }
  return result;
}

/* Create a new, uninitialized matrix */
Matrix MatrixCreate(int height, int width)
{
  double *data = Allocate(width*height, Real);
  if(!data) {
    fprintf(stderr, "Cannot allocate %d by %d matrix\n", height, width);
    return NULL;
  }
  return MatrixFromArray(height, width, data);
}

/* Destroy a matrix */
void MatrixFree(Matrix matrix)
{
  free(matrix->data[0]);
  free(matrix->data);
  free(matrix);
}

int Image::getImageRows()
{
    return this->ROWS;
}

int Image::getImageCols()
{
	return this->COLS;
}

unsigned char ***readPPMImage(int imgsize[],char *filename, int band)
{
        int row, col;
        FILE *file;
        char  buffer[1024];
        int   nrows, ncols, ngray;
        unsigned char red, grn, blu;

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

        unsigned char ***data = (unsigned char ***)malloc(sizeof(unsigned char **)*nrows);

        for(int i=0; i<nrows; i++)
        {
            data[i] = (unsigned char **)malloc(sizeof(unsigned char *)*ncols);
            for(int j=0; j<ncols; j++)
                data[i][j] = (unsigned char *)malloc(sizeof(unsigned char)*3);
        }

        /* read the pixel data: 3 bytes per disk pixel */
        /* (not nec. same size as sizeof(pixel) */
                if(band>1){
        for ( row=0; row<nrows; row++ )
            for ( col=0; col<ncols; col++ )
            {
                red = fgetc( file );
                grn = fgetc( file );
                blu = fgetc( file );
                data[row][col][0] = red;
                data[row][col][1] = grn;
                data[row][col][2] = blu;
            }
                }
                else{
                    for ( row=0; row<nrows; row++ )
                            for ( col=0; col<ncols; col++ )
                            {
                                    red = fgetc( file );
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

void fillRegionStitchChunks(int **olinfo,int **nlinfo,int nl,int sx,int sy,int **mymap, int ROWS, int COLS, Image i1)
{
   // std::cout<<"nl : "<<nl<<std::endl;
    queue<int> myq;
    //mymap[sx][sy]=1;
    myq.push(sx*COLS+sy);//pusing inital pixel into the queue
    mymap[sx][sy] = 1;
    nlinfo[sx][sy] = nl;
    //nlinfo[sx][sy]=nl;
    int pos,pos_i,pos_j,i,j;
    int count = 0;
    int spectralDifference = 10;
    while(myq.size()!=0)
    {
        pos=myq.front();
        myq.pop();
        pos_i=pos/COLS;
        pos_j=pos%COLS;
        count = 0;

        //now pushing the untouched pixels in its neighbourhood into the queue..
        i=pos_i-1;j=pos_j; //North

        if( (i>=0 && j < COLS && mymap[i][j]==0 ) &&
                (olinfo[i][j]==olinfo[pos_i][pos_j] ||
                 ( (i + 1)%(ROWS/4) == 0 &&
                   fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanRed  - (float)i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanRed) < spectralDifference &&
                   fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanGreen  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanGreen) < spectralDifference &&
                   fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanBlue  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanBlue) < spectralDifference) ) )
        {
            myq.push(i*COLS+j);
            mymap[i][j]=1;
            nlinfo[i][j]=nl;
            count++;
        }

        i=pos_i+1;j=pos_j; //South
        if((i<ROWS && j < COLS &&  mymap[i][j]==0) &&
                (olinfo[i][j]==olinfo[pos_i][pos_j] ||
                (i%(ROWS/4) == 0 &&
                 fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanRed  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanRed) < spectralDifference &&
                 fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanGreen  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanGreen) < spectralDifference &&
                 fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanBlue  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanBlue) < spectralDifference)))
        {
            myq.push(i*COLS+j);
            mymap[i][j]=1;
            nlinfo[i][j]=nl;
            count++;
        }

        i=pos_i;j=pos_j-1; //West
        if((j>=0 && i <ROWS && mymap[i][j]==0) &&
                (olinfo[i][j]==olinfo[pos_i][pos_j] ||
                 ((j+1)%(COLS/4) == 0 &&
                  fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanRed  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanRed) < spectralDifference &&
                  fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanGreen  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanGreen) < spectralDifference &&
                  fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanBlue  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanBlue) < spectralDifference)))
        {

            myq.push(i*COLS+j);
            mymap[i][j]=1;
            nlinfo[i][j]=nl;
            count++;

        }
        i=pos_i;j=pos_j+1; //East
        if((j<COLS && i<ROWS && mymap[i][j]==0) &&
                (olinfo[i][j]== olinfo[pos_i][pos_j] ||
                 ((j)%(COLS/4) == 0 &&
                  fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanRed  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanRed) < spectralDifference &&
                  fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanGreen  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanGreen) < spectralDifference &&
                  fabs((float)i1.objectArray[olinfo[i][j]].fVector.meanBlue  - i1.objectArray[olinfo[pos_i][pos_j]].fVector.meanBlue) < spectralDifference)))
        {

            myq.push(i*COLS+j);
            mymap[i][j]=1;
            nlinfo[i][j]=nl;
            count++;
        }

    }
    return;
}

int **stitchChunks(char *seg_result_label_file,int rows, int cols, Image i1)
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

    int maxlabel = -1;
    for(i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {
            fscanf(fol,"%d",&olinfo[i][j]);
            if(olinfo[i][j] > maxlabel)
                maxlabel = olinfo[i][j];
            mymap[i][j]=0;
            nlinfo[i][j] = -1;
        }
    }
    std::cout<<"max label in stitchchunks: "<<maxlabel<<std::endl;

    fclose(fol);
    int nl=0;
    int status = 0;
    for(i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {
           // std::cout<<"i: "<<i<<" j: "<<j<<std::endl;
            if(mymap[i][j]==0)//this means untouched so we can start a new label from here
            {
                //ol=olinfo[i][j];
//               fillRegionStitchChunks(olinfo,nlinfo,ol,nl,i,j,mymap,rows,cols);
                fillRegionStitchChunks(olinfo,nlinfo,nl,i,j,mymap,rows,cols,i1);
               nl++;

            }
        }
    }
    for(i=0; i<rows; i++)
    {
        free(olinfo[i]);
        free(mymap[i]);
    }
    free(olinfo);
    free(mymap);
    return(nlinfo);
}



int fillRegionNewLabels(int **olinfo,int **nlinfo,int ol,int nl,int sx,int sy,int **mymap, int ROWS, int COLS)
{
    queue<int> myq;
    //mymap[sx][sy]=1;
    myq.push(sx*COLS+sy);//pusing inital pixel into the queue
    //nlinfo[sx][sy]=nl;
    int pos,pos_i,pos_j,i,j;
    int count = 0;
    while(myq.size()!=0)
    {
        pos=myq.front();
        myq.pop();
        pos_i=pos/COLS;
        pos_j=pos%COLS;
        count = 0;

        //now pushing the untouched pixels in its neighbourhood into the queue..
        i=pos_i-1;j=pos_j;
        //for(j=pos_j-1; j<= pos_j+1; j++){

        //		printf("%d.%d\n",pos_i,pos_j);
        if(i>=0 && j < COLS && olinfo[i][j]==ol && mymap[i][j]==0)
        {
            //				printf("1\n");

            myq.push(i*COLS+j);
            mymap[i][j]=1;
            nlinfo[i][j]=nl;
            count++;
        }


        i=pos_i+1;j=pos_j;
        //for(j=pos_j-1; j<=pos_j+1; j++){
        if(i<ROWS && j < COLS && olinfo[i][j]==ol && mymap[i][j]==0)
        {
            //				printf("2\n");

            myq.push(i*COLS+j);
            mymap[i][j]=1;
            nlinfo[i][j]=nl;
            count++;
        }

        i=pos_i;j=pos_j-1;
        if(j>=0 && i <ROWS && olinfo[i][j]==ol && mymap[i][j]==0)
        {
            //				printf("3\n");

            myq.push(i*COLS+j);
            mymap[i][j]=1;
            nlinfo[i][j]=nl;
            count++;

        }
        i=pos_i;j=pos_j+1;
        if(j<COLS && i<ROWS && olinfo[i][j]==ol && mymap[i][j]==0)
        {
            //				printf("4\n");

            myq.push(i*COLS+j);
            mymap[i][j]=1;
            nlinfo[i][j]=nl;
            count++;
        }

        if(count == 0 && mymap[pos_i][pos_j] == 0)
        {

            if(pos_i+1 < ROWS)
            {
                if(mymap[pos_i+1][pos_j] == 1)
                {
                    //std::cout<<"special"
                    nlinfo[pos_i][pos_j] = nlinfo[pos_i+1][pos_j];
                    mymap[pos_i][pos_j] = 1;
                    return 0;

                }
                else
                {
                    olinfo[pos_i][pos_j] = olinfo[pos_i+1][pos_j];
                    //mymap[pos_i][pos_j] = 0;
                    myq.push(pos_i*COLS + pos_j);
                    ol = olinfo[pos_i][pos_j];


                }
            }
            else if(pos_j + 1 < COLS)
            {
                if(mymap[pos_i][pos_j+1] == 1)
                {
                    //std::cout<<"special"
                    nlinfo[pos_i][pos_j] = nlinfo[pos_i][pos_j+1];
                    mymap[pos_i][pos_j] = 1;
                    return 0;

                }
                else
                {
                    olinfo[pos_i][pos_j] = olinfo[pos_i][pos_j+1];
                    //mymap[pos_i][pos_j] = 0;
                    myq.push(pos_i*COLS + pos_j);
                    ol = olinfo[pos_i][pos_j];

                }
            }
            else
            {
                nlinfo[pos_i][pos_j] = nlinfo[pos_i][pos_j-1];
                mymap[pos_i][pos_j] = 1;
                return 0;
            }

        }
        else
        {
            nlinfo[pos_i][pos_j] = nl;
            mymap[pos_i][pos_j] = 1;
        }

    }
    return 1;
}


int **prepareNewLabels(char *seg_result_label_file,int rows, int cols)
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
    int status = 0;
    for(i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {
           // std::cout<<"i: "<<i<<" j: "<<j<<std::endl;
            if(mymap[i][j]==0)//this means untouched so we can start a new label from here
            {
                ol=olinfo[i][j];
                status = fillRegionNewLabels(olinfo,nlinfo,ol,nl,i,j,mymap,rows,cols);
                if(status == 1)
                {
                    nl++;
                }
            }
        }
    }
    for(i=0; i<rows; i++)
    {
        free(olinfo[i]);
        free(mymap[i]);
    }
    free(olinfo);
    free(mymap);

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
	
	int count=0,i,j,label;

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

unsigned char *Image::getPixelData(int x, int y)
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

void Image::callCreateAttribute(vector< vector< int > >objectInfo, int start, int end)
{
    std::cout<<"I: "<<start<<" J: "<<end<<std::endl;
    for(int i=start; i<=end; i++)
    {
        this->objectArray[i] = Object(objectInfo[i][0],objectInfo[i][1],objectInfo[i][2],objectInfo[i][3],objectInfo[i][4],objectInfo[i][5]);
        this->objectArray[i].setObjectInfo(this);
        this->objectArray[i].createAttribute(i);
        int xmax = objectInfo[i][3];
        int xmin = objectInfo[i][2];
        for(int j=0; j<(xmax - xmin +1); j++)
            free(this->objectArray[i].mask[j]);
        free(this->objectArray[i].mask);
    }
    return;
}

void Image::callCalculateMean(vector< vector< int > >objectInfo, int start, int end)
{
    std::cout<<"I: "<<start<<" J: "<<end<<std::endl;
    for(int i=start; i<=end; i++)
    {
        this->objectArray[i] = Object(objectInfo[i][0],objectInfo[i][1],objectInfo[i][2],objectInfo[i][3],objectInfo[i][4],objectInfo[i][5]);
        this->objectArray[i].setObjectInfo(this);
        this->objectArray[i].calculateMean(i);
        int xmax = objectInfo[i][3];
        int xmin = objectInfo[i][2];
        for(int j=0; j<(xmax - xmin +1); j++)
            free(this->objectArray[i].mask[j]);
        free(this->objectArray[i].mask);
    }
    return;

}


void Image::createObjects(int NO, vector< vector< int > > objectInfo, int flag)
{
//	this->NO = *max_element(*segmentedImageInfo,(*segmentedImageInfo)+(this->ROWS * this->COLS));
    this->NO = NO;
    this->objectArray = (Object *)malloc(sizeof(Object)*this->NO);
    //FILE *objectinfo;
    //objectinfo = fopen("objectinfo_dump_2.txt","w");
//    std::cout<<"createobjects inside"<<std::endl;
    if(flag == 1)
    {
//        std::cout<<"if"<<std::endl;
        int numObjectsPerThread = 25;
        int numThreads = this->NO / numObjectsPerThread + 1;
        QFuture<void> *result = new QFuture<void> [numThreads];
        int i=0, j=numObjectsPerThread - 1;
        if(j>this->NO)
            j = this->NO - 1;
        int threadId = 0;
        while(i < this->NO)
        {
           // result[threadId++] = QtConcurrent::run(std::bind1st(std::mem_fun(&Image::callCreateAttribute), this)( objectInfo,i,j);
           // result[threadId++] = QtConcurrent::run(this,Image::callCreateAttribute(objectInfo,i,j));
            result[threadId++] = QtConcurrent::run(this,&Image::callCreateAttribute,objectInfo,i,j);
                    i=j+1;
            j=i + numObjectsPerThread - 1;
            if(j>=this->NO)
                j=this->NO-1;
        }
        for( i = 0; i<numThreads; i++)
            result[i].waitForFinished();
        //free(result);
    }
    else
    {
//        std::cout<<"else"<<std::endl;
        int numObjectsPerThread = 25;
        int numThreads = this->NO / numObjectsPerThread + 1;
        QFuture<void> *result = new QFuture<void> [numThreads];
        int i=0, j = numObjectsPerThread - 1;
        if(j>this->NO)
            j = this->NO - 1;
        int threadId = 0;
        while(i < this->NO)
        {
//            result[threadId++] = QtConcurrent::run(check,i,j);
            result[threadId++] = QtConcurrent::run(this,&Image::callCalculateMean,objectInfo,i,j);
            //result[threadId++] = QtConcurrent::run(std::bind1st(std::mem_fun(&Image::callCalculateMean), this)( objectInfo,i,j);
            i=j+1;
            j=i + numObjectsPerThread - 1;
            if(j>=this->NO)
                j=this->NO-1;
        }
        for( i = 0; i<numThreads; i++)
            result[i].waitForFinished();
       // free(result);
    }


    return;
}

void Object::calculateMean(int objNumber)
{
    int i,j;

    int nob;
    nob = this->parentImage->getNOB();

    int *sumPixelData;
    unsigned char *pixeldata;
    sumPixelData = (int *)malloc(sizeof(int)*nob);

    for(i=0;i<nob;i++)
        sumPixelData[i] = 0;

    for(i=this->xmin;i<=this->xmax;i++)
    {
        for(j=this->ymin;j<=this->ymax;j++)
        {

            if(this->mask[i-this->xmin][j-this->ymin])//this means that this point will be of use to us....
            {
                pixeldata = this->parentImage->getPixelData(i,j);
                for(int b=0; b<nob; b++)
                    sumPixelData[b] = sumPixelData[b] + pixeldata[b];

            }
        }
    }
    //preparing means
    if(nob > 1)
    {
        this->fVector.meanRed = sumPixelData[0]/this->NOP;
        this->fVector.meanGreen = sumPixelData[1]/this->NOP;
        this->fVector.meanBlue = sumPixelData[2]/this->NOP;
    }
    else
        this->fVector.meanPan = sumPixelData[0]/this->NOP;

    free(sumPixelData);
}


int valid_peak(int *peak, double *histogram, double peak_thres)
{
  double max_valley, sharpness;

  max_valley = histogram[peak[0]];
  if(histogram[peak[2]] > max_valley)
    max_valley = histogram[peak[2]];
  sharpness = histogram[peak[1]] / max_valley;
  max_valley = fabs(sharpness - peak_thres);
/*
  if(max_valley < 0.1) printf("close call\n");
*/
  return(sharpness > peak_thres);
}

double sd_histo(double *histogram, int histo_bins, int start, int stop)
{
  double *interval;
  double sum;
  int i, length, mp;

  length = stop - start;
  if(stop <= start) length += histo_bins;
  interval = Allocate(length, double);

  /* copy the sub-histogram into the interval */
  if(stop <= start) {
    for(i=start;i < histo_bins;i++) interval[i-start] = histogram[i];
    for(i=0;i<stop;i++) interval[histo_bins-start+i] = histogram[i];
  }
  else {
    for(i=start;i<stop;i++) interval[i-start] = histogram[i];
  }

  /* normalize to make a distribution */
  sum = 0.0;
  for(i=0;i<length;i++) sum += interval[i];
  for(i=0;i<length;i++) interval[i] /= sum;

  /* find mean */
  sum = 0.0;
  for(i=0;i<length;i++) {
    sum += (i+1)*interval[i];
  }
  mp = (int)(sum-0.5);

  /* compute variance */
  sum = 0.0;
  for(i=0;i<length;i++) {
    sum += (double)(i - mp) * (i - mp) * interval[i];
  }

  free(interval);
  return sqrt(sum);
}

void Directionality(Matrix image, int histo_bins, int histo_thres,
            double peak_thres,
            int *orientations, double *dir)
{
  double *histogram, *diff;
  int *extrema, n_extrema, *is_peak;
  int num_peaks, i, j, k;
  int **pv;

  /* compute direction histogram */
/*
  histogram = new_dir_histogram(image, histo_bins);
*/
  histogram = dir_histogram(image, histo_bins, histo_thres);

/*
  for(i=0;i<histo_bins;i++) printf("%lf\n", histogram[i]);
*/

  /* find extrema of histogram */
  extrema = Allocate(histo_bins,int);
  is_peak = Allocate(histo_bins,int);
  diff = Allocate(histo_bins,double);
  n_extrema = 0;

  /* compute difference vector */
  for(i=0;i < histo_bins-1;i++)
    diff[i] = histogram[i+1] - histogram[i];
  diff[histo_bins-1] = histogram[0] - histogram[histo_bins-1];
  CHECK_EXTREMUM(histo_bins-1, 0);
  for(i=0;i < histo_bins-1;i++) {
    CHECK_EXTREMUM(i, i+1);
  }
  free(diff);
/*
  for(i=0;i<n_extrema;i++) printf("%d\n", extrema[i]);
*/

  /* extract salient peaks */
  pv = Allocate(n_extrema, int*);
  pv[0] = Allocate(3*n_extrema, int);
  for(i=1;i < n_extrema;i++) pv[i] = &pv[0][i * 3];

  j = 0;
  num_peaks = 0;
  for(i=0;i < n_extrema;i++) {
    if(is_peak[i]) {
      if(j == 0) {
    /* find valley to fill first slot */
        for(k=n_extrema-1;is_peak[k];k--);
    pv[num_peaks][0] = extrema[k];
    j++;
      }
      if(j == 1) {
    pv[num_peaks][1] = extrema[i];
    j++;
      }
    }
    else {
      if(j < 2) {
    pv[num_peaks][0] = extrema[i];
    j = 1;
      }
      else {
    pv[num_peaks][2] = extrema[i];
    if(valid_peak(pv[num_peaks], histogram, peak_thres)) {
      num_peaks++;
    }
    pv[num_peaks][0] = extrema[i];
    j = 1;
      }
    }
  }
  if(j == 2) {
    for(k=0;is_peak[k];k++);
    pv[num_peaks][2] = extrema[k];
  }
  if((j < 2) || !valid_peak(pv[num_peaks], histogram, peak_thres))
    num_peaks--;
  num_peaks++;
/*
  for(i=0;i<num_peaks;i++)
    printf("%d %d %d\n", pv[i][0], pv[i][1], pv[i][2]);
*/
  *orientations = num_peaks;

  /* compute 2nd moment about peaks */
  if(num_peaks == 0) {
    *dir = 1;
    return;
  }
  *dir = 0;
  for(i=0;i<num_peaks;i++) {
    *dir += sd_histo(histogram, histo_bins, pv[i][0], pv[i][2]);
  }
  *dir /= 5; /* normalize by maximum value */
}

double *dir_histogram(Matrix image, int histo_bins, int threshold)
{
  int y,x, bin;
  double delh, delv;
  double delG, theta;
  double sum;
  double *histo;
  double dont_care;

  histo = Allocate(histo_bins, double);
  for(y=0;y<histo_bins;y++) histo[y]=0;
  sum = 0;
  dont_care = 0;
  for(y=1;y<image->height-1;y++) {
    for(x=1;x<image->width-1;x++) {
      delh = (image->data[y-1][x+1] +
          image->data[y][x+1] +
          image->data[y+1][x+1]) -
         (image->data[y-1][x-1] +
          image->data[y][x-1] +
          image->data[y+1][x-1]);
      delv = (image->data[y-1][x-1] +
          image->data[y-1][x] +
          image->data[y-1][x+1]) -
         (image->data[y+1][x-1] +
          image->data[y+1][x] +
          image->data[y+1][x+1]);
      delG = (fabs(delh) + fabs(delv)) / 2;
      if(delG >= threshold) {
    theta = atan2(delv, delh);
	double M_PI = 3.14;
    if(theta < 0) theta += M_PI;
    else if(theta >= M_PI) theta -= M_PI;
    bin = (int)(theta * histo_bins / M_PI + 0.5);
    if(bin == histo_bins) bin = 0;
    if(bin < 0 || bin >= histo_bins) {
      printf("Bin error: %d\n", bin);
      exit(1);
    }
    histo[bin]++;
    sum++;
      }
      else {
    dont_care++;
      }
    }
  }
  /* Equitz correction: add dont_care to every bin */
  sum += dont_care;
  dont_care /= histo_bins;
  if(sum) {
    for(y=0;y<histo_bins;y++) histo[y] = (histo[y] + dont_care) / sum;
  }
  return histo;
}

double *new_dir_histogram(Matrix image, int histo_bins)
{
  int y,x, bin;
  double delh, delv;
  double delG, theta;
  double sum;
  double *histo;

  histo = Allocate(histo_bins, double);
  for(y=0;y<histo_bins;y++) histo[y]=0;
  sum = 0;
  for(y=1;y<image->height-1;y++) {
    for(x=1;x<image->width-1;x++) {
      delh = (image->data[y-1][x+1] +
          image->data[y][x+1] +
          image->data[y+1][x+1]) -
         (image->data[y-1][x-1] +
          image->data[y][x-1] +
          image->data[y+1][x-1]);
      delv = (image->data[y-1][x-1] +
          image->data[y-1][x] +
          image->data[y-1][x+1]) -
         (image->data[y+1][x-1] +
          image->data[y+1][x] +
          image->data[y+1][x+1]);
      delG = (fabs(delh) + fabs(delv))/2;
      if(!delv && !delh) {
    theta = 0.0;
      }
      else {
    theta = atan2(delv, delh);
      }
	  double M_PI=3.14;
      if(theta < 0) theta += M_PI;
      else if(theta >= M_PI) theta -= M_PI;
      bin = (int)(theta * histo_bins / M_PI + 0.5);
      if(bin == histo_bins) bin = 0;
      if(bin < 0 || bin >= histo_bins) {
    printf("Bin error: %d\n", bin);
    exit(1);
      }
      histo[bin] += delG;
      sum += delG;
    }
  }
  if(sum) {
    for(y=0;y<histo_bins;y++) histo[y] /= sum;
  }
  return histo;
}

double Contrast(Matrix image)
{
  double var, norm4, mean, diff;
  int y,x;

  mean = MatrixMean(image);
  var = 0;
  norm4 = 0;
  for(y=0;y<image->height;y++) {
    for(x=0;x<image->width;x++) {
      diff = image->data[y][x] - mean;
      var += diff*diff;
      norm4 += diff*diff*diff*diff;
    }
  }
  var /= (image->width * image->height);
  norm4 /= (image->width * image->height);
  if(norm4 == 0) return 0;
  return var / pow(norm4,0.25) / 128; /* normalize by maximum value */
}

double Coarseness(Matrix image, int max_scale, double weight)
{
  int scale, step;
  Matrix temp, diff, best, scales;
  double crs;
  int y,x;
 // char str[100];

  best = NULL;
  scales=NULL;
  image = MatrixCopy(image);
  int ih,iw;
  ih = image->height;
  iw = image->width;
  for(scale = 1; scale <= max_scale; scale++) {

    temp = image;
    image = nbr_avg(image, scale);
    if(1<<scale >= image->height || 1<<scale >=image->width ) break;
    MatrixFree(temp);
    diff = nbr_diff(image, scale);
    MatrixScale(diff, pow(weight, scale-1));
    if(!best) {
      best = diff;
//     scales = MatrixCreate(diff->width, diff->height);
      scales = MatrixCreate(diff->height, diff->width);
      MatrixFill(scales, 1<<scale);
    }
    else {
      step = 1<<(scale-1);
      temp = best;
      best = MatrixSub(best, step, step, diff->height+step, diff->width+step);
      MatrixFree(temp);
      temp = scales;
      scales = MatrixSub(scales, step, step,
             diff->height+step, diff->width+step);
      MatrixFree(temp);

      for(y=0;y < diff->height;y++) {
    for(x=0;x < diff->width;x++) {
      if(best->data[y][x] <= diff->data[y][x]) {
        best->data[y][x] = diff->data[y][x];
        scales->data[y][x] = 1<<scale;
      }
    }
      }
      MatrixFree(diff);
    }
  }
  MatrixFree(image);
  if(scale == 1)
      return 0.0;
  MatrixFree(best);
  crs = MatrixMean(scales);
  MatrixFree(scales);
  return crs / (1<<max_scale); /* normalize by maximum value */
}

/* Assumes image->width == image->height */
Matrix nbr_diff(Matrix image, int factor)
{

  int step = 1<<factor;
  Matrix result;
  int y,x;
  double h_diff, v_diff;

  result = MatrixCreate(image->height-step, image->width-step);
  for(y=0; y < image->height-step; y++) {
    for(x=0; x < image->width-step; x++) {
      h_diff = image->data[y][x] - image->data[y][x+step];
      v_diff = image->data[y][x] - image->data[y+step][x];
      result->data[y][x] = max(fabs(h_diff), fabs(v_diff));
    }
  }
  return result;
}

/* Assumes image->width == image->height */
Matrix nbr_avg(Matrix image, int factor)
{
  int step;
  Matrix result;
  int y,x;

  step = 1<<(factor-1);
  result = MatrixCreate(image->height-step, image->width-step);
  for(y=0; y < image->height-step; y++) {
    for(x=0; x < image->width-step; x++) {
      result->data[y][x] = (image->data[y][x] +
                image->data[y][x+step] +
                image->data[y+step][x] +
                image->data[y+step][x+step]) / 4;
    }
  }
  return result;
}



//void fillRegionChanged(int rows, int cols, vector< vector< int > > info1, vector< vector< int > > info2,int **dt1,int **dt2,int **&dmerge,int val1,int val2,int label,int bi,int bj,vector< float> &meta_obj)
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

//vector< vector< float > > changed(int rows, int cols, int **dt1, int **dt2, vector< vector< int > > info1,vector< vector< int > > info2, int NO1, int NO2)
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

//	printf("before for loop:\n");
	//preparing merge map
	label=2;
	vector< float >v(6);
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
//			printf("i:%d j:%d\n",i,j);
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

    for(i=0; i<rows; i++)
        free(dmerge[i]);
    free(dmerge);
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
	//int onum;
	printf("coding ends\n");

	return code;
}


void Object::createAttribute(int objNumber)
{
    //cout<<"just inside createatt\n";
        int cnt=0,i,j;
        float ssum_red=0.0,ssum_green=0.0,ssum_blue=0.0,ssum_pan=0.0;
        float val = 0.0;
        float perimeter = 0.0,centroid_x = 0.0,centroid_y = 0.0;

        int rows = this->parentImage->getImageRows();
        int cols = this->parentImage->getImageCols();
        int myi,myj,myflagg,nob;

        nob = this->parentImage->getNOB();

        int **temp_serial_data;
    //    int **temp_bb_data;
        int *sumPixelData;
        unsigned char *pixeldata;

        sumPixelData = (int *)malloc(sizeof(int)*nob);

        temp_serial_data = (int **)malloc(sizeof(int *)*nob);
        for(i=0;i<nob;i++)
            temp_serial_data[i] = (int *)malloc(sizeof(int)*this->NOP);

        for(i=0;i<nob;i++)
            sumPixelData[i] = 0;

    //    temp_bb_data = (int **)malloc(sizeof(int *)*(this->xmax - this->xmin + 1));
    //    for(i=0; i<=this->xmax - this->xmin; i++)
    //        temp_bb_data[i] = (int *)malloc((this->ymax - this->ymin+1)*sizeof(int));


        for(i=this->xmin;i<=this->xmax;i++)
        {
            for(j=this->ymin;j<=this->ymax;j++)
            {

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


                if(this->mask[i-this->xmin][j-this->ymin])//this means that this point will be of use to us....
                {
                    pixeldata = this->parentImage->getPixelData(i,j);

                    for(int b=0; b<nob; b++)
                        sumPixelData[b] = sumPixelData[b] + pixeldata[b];
                    for(int b=0; b<nob; b++)
                        temp_serial_data[b][cnt] = pixeldata[b];
                    cnt++;

                    // Calculating centroid....
                    centroid_x += i;
                    centroid_y += j;
                }
            }
        }
        //preparing means
        if(nob > 1)
        {
            this->fVector.meanRed = sumPixelData[0]/this->NOP;
            this->fVector.meanGreen = sumPixelData[1]/this->NOP;
            this->fVector.meanBlue = sumPixelData[2]/this->NOP;
        }
        else
            this->fVector.meanPan = sumPixelData[0]/this->NOP;
        //preparind standard deviation values
        if(nob > 1)
        {
            for(i=0;i<cnt;i++)
            {
                val=(temp_serial_data[0][i] - this->fVector.meanRed) * (temp_serial_data[0][i] - this->fVector.meanRed);
                ssum_red=ssum_red+val;

                val=(temp_serial_data[1][i] - this->fVector.meanGreen) * (temp_serial_data[1][i] - this->fVector.meanGreen);
                ssum_green=ssum_green+val;

                val=(temp_serial_data[2][i] - this->fVector.meanBlue) * (temp_serial_data[2][i] - this->fVector.meanBlue);
                ssum_blue=ssum_blue+val;

            }

            this->fVector.stdRed = sqrt(1.0*ssum_red/this->NOP);
            this->fVector.stdRed = floorf(this->fVector.stdRed * 10000) / 10000;
            this->fVector.stdGreen = sqrt(1.0*ssum_green/this->NOP);
            this->fVector.stdGreen = floorf(this->fVector.stdGreen * 10000) / 10000;
            this->fVector.stdBlue = sqrt(1.0*ssum_blue/this->NOP);
            this->fVector.stdBlue = floorf(this->fVector.stdBlue * 10000) / 10000;
        }
        else
        {
            for(i=0;i<cnt;i++)
            {
                val=(temp_serial_data[0][i]-this->fVector.meanPan) * (temp_serial_data[0][i]-this->fVector.meanPan);
                ssum_pan=ssum_pan+val;
            }
            this->fVector.stdPan = sqrt(1.0*ssum_pan/this->NOP);
        }

        //writing perimeter
        this->fVector.perimeter = perimeter;
        this->fVector.area = this->NOP;
        // Preparing centroids-------------------------------
        this->fVector.xCentroid = centroid_x / this->NOP;
        this->fVector.yCentroid = centroid_y / this->NOP;


        // Calculating roundness-----------------------------
        this->fVector.roundness = (4.0 * 3.14 * this->NOP)/(perimeter * perimeter);
         this->fVector.roundness = floorf(this->fVector.roundness * 10000) / 10000;

        // Calculating compactness---------------------------
        this->fVector.compactness = perimeter / (3.53 * sqrt((float)this->NOP));
        this->fVector.compactness = floorf(this->fVector.compactness * 10000) / 10000;

        Matrix image;
          int orientations;
          double dir;
    //      char *p,*q;
          int Nx = this->xmax-this->xmin+1, Ny = this->ymax-this->ymin+1;
          image = MatrixCreate(Nx, Ny);

          int index = 0;
          float avg_val;
          for(i=this->xmin; i<=this->xmax; i++)
          {
              for(j=this->ymin; j<=this->ymax; j++)
              {
                  if(this->mask[i-this->xmin][j-this->ymin])
                  {
                      pixeldata = this->parentImage->getPixelData(i,j);

                      if(nob > 1)
                          avg_val = (pixeldata[0]*1.0 + pixeldata[1]*1.0 + pixeldata[2]*1.0)/3;
                      else
                          avg_val = pixeldata[0];

                      image->data[0][index++] = avg_val;
                  }
                  else
                  {
                      if(nob > 1)
                          avg_val = (this->fVector.meanRed + this->fVector.meanGreen + this->fVector.meanBlue)/3;
                      else
                          avg_val = this->fVector.meanPan;

                      image->data[0][index++] = avg_val;
                  }

              }
          }

          this->fVector.coarseness = Coarseness(image, 5, 1);
          this->fVector.contrast = Contrast(image);
          Directionality(image, 16, 12, 2, &orientations, &dir);
          this->fVector.direction = dir;
          this->fVector.roughness = this->fVector.coarseness + this->fVector.contrast;
          free(image);


           for(int i=0; i<nob ; i++)
           {
               free(temp_serial_data[i]);
           }



           free(sumPixelData);
           free(temp_serial_data);
           // free(pixeldata);

    return ;
}
			
