#ifndef ANALYSIS_H_INCLUDED
#define ANALYSIS_H_INCLUDED
#include"SynTreeSQ.h"


struct Span
{
   int beg;
   int back;
   Span(int m_beg,int m_back):beg(m_beg),back(m_back){}
};
struct CountStruct
{
  int leftC;
  int rightC;
  int matchedC;
  CountStruct():leftC(0),rightC(0),matchedC(0){};
};

class AnalysisClass
{
public:
  int init(const string&rst_s,vector<string>&src);
  int initAlignConsist(const string&rst_s,vector<string>&srcVec);

  void trimPunct(Span * span);
  int Matched(vector<RTreeNode *>&allNodeVec,vector<Span*>&chartSpan);
  bool IsMatch(Span* lsp, Span* rsp);
  float Matchrate(int matched, int allsize);
  void GetChartPath(vector<string>&src_span, vector<Span*>&chartSpan);
  void ConvertNode2Span(vector<RTreeNode *>&vec,vector<Span*>&node2SpanVec);



private:
  string ExtractSourceSpan(const string&s){
    stringstream ss(s);
    string span;
    ss>>span;ss>>span;ss>>span;
    return span;
  }

public:
  SynTreeSQ *myRtree;
  vector<RTreeNode *> allNodes;
  vector<Span*> spanVecFromHier;
  vector<Span*>spanVecFromNode;


};



#endif // ANALYSIS_H_INCLUDED
