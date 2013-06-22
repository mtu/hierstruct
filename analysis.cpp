#include"analysis.h"
using namespace boost;
int AnalysisClass::init(const string&rst_s,vector<string>&src)
{
  myRtree = new SynTreeSQ;
  myRtree->ParseTree(rst_s,0);
  this->allNodes = GetAllEdus(&myRtree->root);
  GetChartPath(src,this->spanVecFromHier);
}


void AnalysisClass::GetChartPath(vector<string>&src, vector<Span*>&chartSpan)
{
    // src:  1..4
  size_t pos,pos1;
  int beg,back;
  for(size_t i(0);i<src.size();++i){

     pos = src[i].find(".");
     pos1 = src[i].find_last_of(".")+1;
     beg = lexical_cast<int>(src[i].substr(0,pos));
     back = lexical_cast<int>(src[i].substr(pos1));
     Span *aspan = new Span(beg,back);
     this->trimPunct(aspan);
     chartSpan.push_back(aspan);
  }
}
void AnalysisClass::ConvertNode2Span(vector<RTreeNode *>&vec,vector<Span*>&node2SpanVec)
{
  for(size_t i(0);i<vec.size();++i){
    Span *aspan=new Span(vec[i]->Begin+1,vec[i]->Back+1);
    this->trimPunct(aspan);
    node2SpanVec.push_back(aspan);
  }
}
bool AnalysisClass::IsMatch(Span*lsp, Span*rsp)
{
  //if(lsp->beg == lsp->back) return false;
  if(lsp->beg == rsp->beg && lsp->back == rsp->back)return true;
  else return false;
}
void AnalysisClass::trimPunct(Span * span)
{
  if(span->beg==0 && this->myRtree->sentenceVec.size()>0)span->beg=1;
  if(span->back>this->myRtree->sentenceVec.size())span->back=this->myRtree->sentenceVec.size();
  while(span->beg < span->back && span->beg <= this->myRtree->sentenceVec.size()){
     if(this->myRtree->sentenceVec[span->beg-1]==",")span->beg+=1;
     else if(this->myRtree->sentenceVec[span->beg-1]=="\"")span->beg+=1;
     else if(this->myRtree->sentenceVec[span->beg-1]=="'")span->beg+=1;
     else if(this->myRtree->sentenceVec[span->beg-1]==";")span->beg+=1;
     else if(this->myRtree->sentenceVec[span->beg-1]=="，")span->beg+=1;
     else break;
  }
  while(span->back > span->beg){
    if(this->myRtree->sentenceVec[span->back-1]==",")span->back-=1;
    else if(this->myRtree->sentenceVec[span->back-1]=="\"")span->back-=1;
    else if(this->myRtree->sentenceVec[span->back-1]=="'")span->back-=1;
    else if(this->myRtree->sentenceVec[span->back-1]==";")span->back-=1;
    else if(this->myRtree->sentenceVec[span->back-1]=="，")span->back-=1;
    else break;
  }
}
int AnalysisClass::Matched(vector<RTreeNode *>&allNodeVec,vector<Span*>&chartSpan)
{
  ConvertNode2Span(allNodeVec,this->spanVecFromNode);
  int matchedNum=0;
  for(size_t i(0);i<this->spanVecFromNode.size();++i){
    if(this->spanVecFromNode[i]->beg==1 && this->spanVecFromNode[i]->back==this->myRtree->sentenceVec.size()){
      continue;
    }
    else if(this->spanVecFromNode[i]->beg == this->spanVecFromNode[i]->back){
      continue;
    }
  //  cerr<<"discourse tree: "<<this->spanVecFromNode[i]->beg<<" "<<this->spanVecFromNode[i]->back<<endl;
    for(size_t j(0);j<chartSpan.size();++j){
  //    cerr<<"use-rule: "<<chartSpan[j]->beg<<" "<<chartSpan[j]->back<<endl;
      if(this->spanVecFromNode[i]->beg >= chartSpan[j]->beg ){
        if(this->IsMatch(this->spanVecFromNode[i],chartSpan[j])){
          matchedNum +=1;break;}
      }else{
        break;
      }
    }// end chartspan
  }//end spanvecfromNode
  return matchedNum;
}



