#ifndef TRANSANALYSIS_H_INCLUDED
#define TRANSANALYSIS_H_INCLUDED
#include"analysis.h"
using namespace std;
class AlignedRel
{
public:
    void LoadAlignedSentence(const string &s);
    int GetChSize();
    int GetEnSize();
    vector<int> GetESpanGivenCSpan(int start,int back);
    vector<int> GetCSpanGivenESpan(int start,int back);
    int CountOfAPreceedB(vector<int> &avec,vector<int>&bvec);
    bool IsReverse(int a,int b,int c);
    void Clear(){C2EMap.clear();E2CMap.clear();}
    int GetSizeOfC(){return sizeC;}
    int GetSizeOfE(){return sizeE;}
public:
    multimap<int,int> C2EMap;
    multimap<int,int> E2CMap;
    int sizeC;
    int sizeE;
};
struct CountStruct
{
  int leftC;
  int rightC;
  int matchedC;
  CountStruct():leftC(0),rightC(0),matchedC(0){};
};

class TransAnalysisClass
{
public:
    int LoadAllTransAlign(const string&chs,const string&eng,const string&align);
    void CountMatchedEdu(CountStruct &resultCount);
    bool IsMatch(RTreeNode *left,RTreeNode *right);
public:
    SynTreeSQ *ChTCTtree;
    SynTreeSQ *EnTCTtree;
    AlignedRel alignInfo;
    vector<RTreeNode *> allChsNodes;
    vector<RTreeNode *> allEngNodes;
};


#endif // TRANSANALYSIS_H_INCLUDED
