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


class TransAnalysisClass
{
public:
    int LoadAllTransAlign(const string&chs,const string&eng,const string&align);
    int LoadAllTransAlign(const string&chs,const string&align);
    void CountMatchedEdu(CountStruct &resultCount);
    int IsMatch(RTreeNode *left,RTreeNode *right,multimap<int,int>&alignmap);
    void CountAlignConsistentEdus(CountStruct &resultCount);
    int IsSatisfyAlignConsistent(RTreeNode *left);
    int CountAllNode(vector<RTreeNode*>vec){
      int c(0);
      for(size_t i(0);i<vec.size();++i){
        if(vec[i]->Begin!=vec[i]->Back)
          c +=1;
      }
      return c;}

public:
    SynTreeSQ *ChTCTtree;
    SynTreeSQ *EnTCTtree;
    AlignedRel alignInfo;
    vector<RTreeNode *> allChsNodes;
    vector<RTreeNode *> allEngNodes;
};


#endif // TRANSANALYSIS_H_INCLUDED
