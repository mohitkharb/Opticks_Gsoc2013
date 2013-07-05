#include<algorithm>
#include<queue>
#include<vector>
#include <QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>
#include <QThread>

using namespace std;

#ifndef Allocate
#define Allocate(n, t) (t*)malloc((n)*sizeof(t))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef sqr
#define sqr(a) ((a)*(a))
#endif

typedef double Real;
typedef struct MatrixStruct {
  Real **data;
  int width, height;
} *Matrix;

#ifdef Complex
/* X.h defines Complex to be zero. Very bad! */
#undef Complex
#endif
typedef struct { Real real, imag; } Complex;

typedef unsigned char uchar;
typedef Real MathFunc(Real x);

/* basic.c *******************************************************************/
Real dot_product(Real *a, Real *b, int len);
Matrix MatrixFilter(Matrix matrix, Matrix filter);
void MatrixSubtractRow(Matrix matrix, Real *row);
Matrix MatrixColSum(Matrix matrix);
Matrix MatrixColMean(Matrix matrix);
void MatrixMinMax(Matrix matrix, Real *minp, Real *maxp);
void MatrixRescale(Matrix matrix, Real new_min, Real new_max);
void MatrixTruncate(Matrix matrix, Real new_min, Real new_max);
void MatrixTruncRescale(Matrix matrix, Real trunc_min, Real trunc_max,
            Real new_min, Real new_max);
void MatrixApplyFunc(Matrix matrix, MathFunc *func);
Matrix MatrixSub(Matrix matrix, int y1, int x1, int y2, int x2);
Real MatrixSum(Matrix matrix);
Real MatrixMean(Matrix matrix);
void MatrixAddValue(Matrix matrix, Real value);
void MatrixFill(Matrix matrix, Real value);
void MatrixScale(Matrix matrix, Real factor);

void MatrixReadImage(Matrix matrix, FILE *fp);
void MatrixWriteImage(Matrix matrix, FILE *fp);
void MatrixWriteVector(Matrix matrix, FILE *fp);
void MatrixWrite(Matrix matrix, FILE *fp);

Matrix MatrixFromArray(int height, int width, double *data);
Matrix MatrixCreate(int height, int width);
void MatrixFree(Matrix matrix);
Matrix MatrixCopy(Matrix matrix);


struct feature
{
	int meanPan;
	int meanRed;
	int meanGreen;
	int meanBlue;
	float stdPan;
	float stdRed;
	float stdGreen;
	float stdBlue;
	int area;
	int perimeter;
	int xCentroid;
	int yCentroid;
	float roundness;
	float compactness;
	float coarseness;
	float contrast;
	float direction;
    float roughness;
};

class Object;
class Image
{
	//private //
        public:
	int ROWS;
	int COLS;
	int NOB; // no of bands
	int NO; //no of objects
    unsigned char ***data; //
	Object *objectArray;


	public:
	int **segmentedImageInfo;
	Image()
	{
		this->ROWS=0;this->COLS=0;this->NOB=0;this->NO=0;
		this->data = NULL;
		this->objectArray = NULL;
		this->segmentedImageInfo = NULL;
	}
    Image(int rows, int cols, int bands, unsigned char ***values)
	{
		this->ROWS = rows;
		this->COLS = cols;
		this->NOB = bands;
		this->data = values;
		this->NO = 0;
		this->objectArray = NULL;
		this->segmentedImageInfo = (int **)malloc(sizeof(int *)*this->ROWS);
		for(int i=0; i<this->ROWS; i++)
			this->segmentedImageInfo[i] = (int *)malloc(sizeof(int)*this->COLS);
	}

	//functions
//	int ** getMask();
    void createObjects(int NO,vector< vector< int > > objectInfo, int flag); //called for segmented image
	int getNOB();	//get the no. of bands
    unsigned char *getPixelData(int x, int y);
	int getImageRows();
    int getImageCols();
    void createSegmentedImageInfo(int **merge_map);
    void getMeanValues();
    void callCreateAttribute(vector< vector< int > >objectInfo, int i, int j);
    void callCalculateMean(vector< vector< int > >objectInfo, int i, int j);

};

class Object
{
        public:
	int id;
	int xmin;
	int xmax;
	int ymin;
	int ymax;
	int NOP;
	bool **mask;
	struct feature fVector;
	Image *parentImage;


	Object()
	{
		this->id=0;this->xmin=0;this->xmax=0;this->ymin=0;this->ymax=0;this->NOP=0;
//		this->fVector = NULL;
		this->mask = NULL;
		this->parentImage = NULL;
	}
	Object(int id, int NOP, int xmin, int xmax, int ymin, int ymax)
	{
		this->id=id;this->xmin=xmin;this->xmax=xmax;this->ymin=ymin;this->ymax=ymax;this->NOP=NOP;
		mask = (bool **)malloc(sizeof(bool*)*(xmax-xmin+1));
		for(int i=0; i<(xmax-xmin+1); i++)
			mask[i] = (bool *)malloc(sizeof(bool)*(ymax-ymin+1));
//		this->fVector = NULL;
		this->parentImage = NULL;

	}
	void setObjectInfo(Image *I);
	void createAttribute(int id);
    void calculateMean(int id);
};

/////////////////////////////////////Matrix Functions///////////////////////////////////
Matrix nbr_avg(Matrix image, int factor);
Matrix nbr_diff(Matrix image, int factor);
double Coarseness(Matrix image, int max_scale, double weight);

/* Contrast */
double Contrast(Matrix image);

/* Directionality */
void Directionality(Matrix image, int histo_bins, int histo_thres,
            double peak_thres,
            int *orientations, double *dir);
double *dir_histogram(Matrix image, int histo_bins, int threshold);
double *new_dir_histogram(Matrix image, int histo_bins);
//////////////////////////////////////////////////////////////////////////////////////////

unsigned char ***readPPMImage(int imgsize[],char *filename,int band);
int **prepareNewLabels(char *seg_result_label_file, int rows, int cols);
int fillRegionNewLabels(int **olinfo, int **nlinfo, int ol, int nl, int sx, int sy, int **mymap, int rows, int cols);

int **stitchChunks(char *seg_result_label_file, int rows, int cols, Image i);
void fillRegionStitchChunks(int **olinfo, int **nlinfo, int nl, int sx, int sy, int **mymap, int rows, int cols, Image i);

vector< vector< int > > prepareObjectInfo(int rows, int cols, int **label_1, int **label_2, int **&dmerge, int &NO);	//merges two images and calculates the MBBs of the objects as well as the merge-map
void fillRegionObjectInfo(int **dt1,int **dt2,int **&dmerge,int val1,int val2,int label,int bi,int bj,vector< int > &meta_obj,int rows, int cols);

//vector< vector< float > > changed(int rows, int cols, int **dt1, int**dt2, vector< vector< int> > info1, vector< vector < int> > info2, int NO1, int NO2);
vector< vector< float > > changed(int rows, int cols, int **dt1, int**dt2, int info1[], int info2[], int NO1, int NO2);
void fillRegionChanged(int rows, int cols, int info1[],int info2[],  int **dt1, int **dt2, int **&dmerge,int val1,int val2,int label,int bi,int bj,vector<float > &meta_obj);
//void fillRegionChanged(int rows, int cols, vector< vector< int > > info1,vector< vector< int > > info2,  int **dt1, int **dt2, int **&dmerge,int val1,int val2,int label,int bi,int bj,vector<float > &meta_obj);

int *changeReporting(int rows, int cols, float low, float high, vector< vector< float > > pinfo, int **mergemap);
