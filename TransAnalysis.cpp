#include"TransAnalysis.h"

void AlignedRel::LoadAlignedSentence(const string &s)
{
using namespace boost;
    stringstream ss(s);
    int key(0),value(0);
    size_t pos;string word;
    ss>>word;
    sizeC = lexical_cast<int>(word);
    ss>>word;
    sizeE = lexical_cast<int>(word);
    while(ss>>word)
    {
        if((pos=word.find("-"))!=string::npos)
        {
            key=lexical_cast<int>(word.substr(0,pos));
            value=lexical_cast<int>(word.substr(pos+1));
        }
        C2EMap.insert(make_pair(key,value));
        E2CMap.insert(make_pair(value,key));
    }
}
int AlignedRel::GetChSize()
{
    return C2EMap.rbegin()->first+1;
}

int AlignedRel::GetEnSize()
{
    return E2CMap.rbegin()->first+1;
}
vector<int> AlignedRel::GetCSpanGivenESpan(int start,int back)
{
    vector<int> res;
    multimap<int,int>::iterator it,it1;
    for(size_t i(start);i<=back;++i)
    {
        if(E2CMap.find(i)==E2CMap.end())continue;
        it=E2CMap.lower_bound(i);it1=E2CMap.upper_bound(i);
        while(it!=it1)
        {
            res.push_back(it->second);
            it++;
        }
    }
    sort(res.begin(),res.end());
   // copy(res.begin(),res.end(),ostream_iterator<int>(cerr," "));cerr<<endl;
    return res;
}
vector<int> AlignedRel::GetESpanGivenCSpan(int start,int back)
{
    vector<int> res;
    if (back<start){cerr<<"back less than start\n";return res;}
     multimap<int,int>::iterator it,it1;
    for(size_t i(start);i<=back;++i)
    {
        if(C2EMap.find(i)==C2EMap.end())continue;
        it=C2EMap.lower_bound(i);it1=C2EMap.upper_bound(i);
        while(it!=it1)
        {
            res.push_back(it->second);
            it++;
        }
    }
    sort(res.begin(),res.end());
   // copy(res.begin(),res.end(),ostream_iterator<int>(cerr," "));cerr<<endl;
    return res;
}
int AlignedRel::CountOfAPreceedB(vector<int> &avec,vector<int>&bvec)
{
    int countAll(0),countIter;
   // cerr<<avec.size()<<" "<<bvec.size()<<endl;
    for(size_t i(0);i<avec.size();++i)
    {
        countIter=0;

        for(size_t j(0);j<bvec.size();++j)
        {
        //    cerr<<avec[i]<<' '<<bvec[j]<<endl;
            if(avec[i]> bvec[j])continue;
            countIter = bvec.size()-j; break;
        }
        countAll += countIter;
    }
    return countAll;
}
bool AlignedRel::IsReverse(int a,int b,int c)
{
    return a*2 < b*c;
}

int TransAnalysisClass::LoadAllTransAlign(const string&chs,const string&eng,const string&align)
{
  ChTCTtree = new SynTreeSQ;
  EnTCTtree = new SynTreeSQ;
  if(ChTCTtree->ParseTree(chs,0)==-1)return -1;
  if(EnTCTtree->ParseTree(eng,0)==-1)return -1;
  alignInfo.LoadAlignedSentence(align);
  allChsNodes = GetAllNode(&ChTCTtree->root);
  allEngNodes = GetAllNode(&EnTCTtree->root);
  return 0;
}
void TransAnalysisClass::CountMatchedEdu(CountStruct &resultCount)
{
  for(size_t i(0);i<allChsNodes.size();++i){
    for(size_t j(0);j<allEngNodes.size();++j){
      if(IsMatch(allChsNodes[i],allEngNodes[j])){
        resultCount.matchedC +=1;
      }
    }
  }
  resultCount.leftC += allChsNodes.size();
  resultCount.rightC += allEngNodes.size();
}
bool TransAnalysisClass::IsMatch(RTreeNode *left,RTreeNode *right)
{
  int lbeg = left->Begin;int lback = left->Back;
  int rbeg = right->Begin;int rback = right->Back;
  int mat(0);
  for(size_t i(lbeg);i<=lback;++i){
    int lowerbound = this->alignInfo.C2EMap.lower_bound(i)->second;
    int upperbound = this->alignInfo.C2EMap.upper_bound(i)->second;
    if(lowerbound>=rbeg && upperbound <= rback+1){ //条件可以修改更宽一点
      mat +=1;
    }
  }
  float alpha(0.8);//相似度阈值
  if(mat>=(lback-lbeg)*alpha)return true;
  else return false;
}
