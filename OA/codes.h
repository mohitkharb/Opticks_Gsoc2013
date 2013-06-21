#include<algorithm>
#include<queue>
#include<vector>

using namespace std;

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
    float roughness;
	float direction;
    int classNumber;
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
	int ***data; //
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
	Image(int rows, int cols, int bands, int ***values)
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
	void createObjects(int NO,vector< vector< int > > objectInfo); //called for segmented image
	int getNOB();	//get the no. of bands
	int *getPixelData(int x, int y);
	int getImageRows();
	int getImageCols();
	void createSegmentedImageInfo(int **merge_map);
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
};


//void readPPMImage(int ***rdata, int ***gdata, int ***bdata,int imgsize[],char *filename);
//void readPPMImage(int ****data, int imgsize[],char *filename);
int ***readPPMImage(int imgsize[],char *filename,int band);
int **prepareNewLabels(char *seg_result_label_file, int rows, int cols);
void fillRegionNewLabels(int **olinfo, int **nlinfo, int ol, int nl, int sx, int sy, int **mymap, int rows, int cols);

vector< vector< int > > prepareObjectInfo(int rows, int cols, int **label_1, int **label_2, int **&dmerge, int &NO);	//merges two images and calculates the MBBs of the objects as well as the merge-map
void fillRegionObjectInfo(int **dt1,int **dt2,int **&dmerge,int val1,int val2,int label,int bi,int bj,vector< int > &meta_obj,int rows, int cols);


vector< vector< float > > changed(int rows, int cols, int **dt1, int**dt2, int info1[], int info2[], int NO1, int NO2);
void fillRegionChanged(int rows, int cols, int info1[],int info2[],  int **dt1, int **dt2, int **&dmerge,int val1,int val2,int label,int bi,int bj,vector<float > &meta_obj);


int *changeReporting(int rows, int cols, float low, float high, vector< vector< float > > pinfo, int **mergemap);