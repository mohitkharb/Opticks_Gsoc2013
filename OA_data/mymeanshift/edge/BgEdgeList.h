/////////////////////////////////////////////////////////////////////////////

class BgEdgeList
{
public:
   int nEdges_;
   BgEdge* edgelist_;
   BgEdge* crtedge_;
   
   BgEdgeList();
   ~BgEdgeList();
   void AddEdge(float*, int);
   void AddEdge(int*, int nPoints);
   void RemoveShortEdges(int);
   void SetBinImage(BgImage*);
   bool SaveEdgeList(char*);
   void SetGradient(float*, float*, float*, int);
   void SetNoMark(void);
   void GetAllEdgePoints(int*, int*, int*);

};
