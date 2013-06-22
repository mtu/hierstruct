#include"TransAnalysis.h"
int GetLargest(multimap<int,int>::iterator left,multimap<int,int>::iterator right)
{
    multimap<int,int>::iterator iter = left;int res = iter->second;
    while(iter!=right){res = iter->second;iter++;}
    return res;
}
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

  //cerr<<allEngNodes.size();
  return 0;
}
int TransAnalysisClass::LoadAllTransAlign(const string&chs,const string&align)
{
  ChTCTtree = new SynTreeSQ;
  if(ChTCTtree->ParseTree(chs,0)==-1)return -1;
  alignInfo.LoadAlignedSentence(align);

  //cerr<<allEngNodes.size();
  return 0;
}
void TransAnalysisClass::CountMatchedEdu(CountStruct &resultCount)
{
  allChsNodes = GetAllNode(&ChTCTtree->root);
  allEngNodes = GetAllNode(&EnTCTtree->root);
  for(size_t i(0);i<allChsNodes.size();++i){
    for(size_t j(0);j<allEngNodes.size();++j){
      if(IsMatch(allChsNodes[i],allEngNodes[j],this->alignInfo.C2EMap)>0 &&
         IsMatch(allEngNodes[j],allChsNodes[i],this->alignInfo.E2CMap)>0){
        resultCount.matchedC +=1;
        break;
      }
    }
  }
//  for(size_t i(0);i<allEngNodes.size();++i){
//    for(size_t j(0);j<allChsNodes.size();++j){
//      if(IsMatch(allEngNodes[i],allChsNodes[j],this->alignInfo.E2CMap)>0){
//        resultCount.matchedC +=1;
//        break;
//      }
//    }
//  }
  resultCount.leftC += CountAllNode(allChsNodes);
  resultCount.rightC += CountAllNode(allEngNodes);

}
int TransAnalysisClass::IsMatch(RTreeNode *left,RTreeNode *right,multimap<int,int>&mmap)
{
  int lbeg = left->Begin;int lback = left->Back;
  int rbeg = right->Begin;int rback = right->Back;
  int mat(0);
  multimap<int,int>::iterator loweriter,upperiter;
  if(lback==lbeg)return -2;//标点符号不算
  for(size_t i(lbeg);i<=lback;++i){
    if(mmap.find(i)==mmap.end())
    {//对空
        mat +=1;
        continue;
    }
    loweriter = mmap.lower_bound(i);
    upperiter = mmap.upper_bound(i);
    int lowerbound = loweriter->second;
    int upperbound = GetLargest(loweriter,upperiter);
    if(lowerbound>=rbeg && upperbound <= rback){ //条件可以修改更宽一点
      mat +=1;
    }
  }
  float alpha(1);//相似度阈值
  if(mat>=(lback-lbeg)*alpha)return 1;
  else return -1;
}
void TransAnalysisClass::CountAlignConsistentEdus(CountStruct&resultCount)
{
  allChsNodes = GetAllEdus(&ChTCTtree->root);
  //allEngNodes = GetAllEdus(&EnTCTtree->root);
  for(size_t i(0);i<allChsNodes.size();++i)
  {
    if(IsSatisfyAlignConsistent(allChsNodes[i])==1){
      resultCount.matchedC += 1;
    }
  }
  resultCount.leftC += CountAllNode(allChsNodes);
}
int TransAnalysisClass::IsSatisfyAlignConsistent(RTreeNode *left)
{
  int lbeg = left->Begin;int lback = left->Back;
  if(lback==lbeg)return -2;//标点符号不算
  int start(9999),end(-1);
  multimap<int,int>::iterator loweriter,upperiter;
  for(size_t i(lbeg);i<=lback;++i){
    if(this->alignInfo.C2EMap.find(i)==this->alignInfo.C2EMap.end()){
    //对空
      continue;
    }
    loweriter = this->alignInfo.C2EMap.lower_bound(i);
    upperiter = this->alignInfo.C2EMap.upper_bound(i);
    int lowerbound = loweriter->second;
    int backbound = GetLargest(loweriter,upperiter);
    if(lowerbound<start)start = lowerbound;
    if(backbound>end)end=backbound;
  }
  if(end<0 || start > end)return 1;

  int rev_lower,rev_upper;
  for(size_t i(start);i<end;++i){
    if(this->alignInfo.E2CMap.find(i)==this->alignInfo.E2CMap.end())continue;
    loweriter = this->alignInfo.E2CMap.lower_bound(i);rev_lower = loweriter->second;
    upperiter = this->alignInfo.E2CMap.upper_bound(i);
    while(loweriter!=upperiter){rev_upper = loweriter->second;loweriter++;}
    if(rev_lower<lbeg||rev_upper>lback){
       return -1;
    }
  }
  return 1;
}

