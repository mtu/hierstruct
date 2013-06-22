#include"ScoreForBoundary.h"

int ScoreForBoundary::InitForClass(const string&tctstr)
{
  this->mytree = new SynTreeSQ;
  if(mytree->ParseTree(tctstr,0)==-1)
    return -1;
  return 1;
}
int ScoreForBoundary::EduInd2Str(vector<RTreeNode *>& allEdus,vector<string> &edustrs,vector<string>& eduextstrs)
{
  string tmp,lasttmp,nexttmp; vector<string>().swap(edustrs);
  bool IsPunc=false;
  for(size_t i(0);i<allEdus.size();++i)
  {
      if(allEdus[i]->Begin==allEdus[i]->Back && (i==0||i==allEdus.size()-1))continue;//如果是标点符号就不算
      else if(allEdus[i]->Begin==allEdus[i]->Back){
          IsPunc=true;
          tmp = edustrs.back()+" "+this->mytree->sentenceVec[allEdus[i]->Begin];boost::trim(tmp);
          eduextstrs.push_back(tmp); tmp=" ";
          continue;}
      for(size_t j(allEdus[i]->Begin);j<=allEdus[i]->Back;++j){
        tmp += this->mytree->sentenceVec[j]+" ";
      }
      boost::trim(tmp);
      edustrs.push_back(tmp);
      if(IsPunc){
         IsPunc = false;
         tmp = this->mytree->sentenceVec[allEdus[i-1]->Begin]+" "+tmp; boost::trim(tmp);
         eduextstrs.push_back(tmp);
         }
       tmp="";
  }

  copy(edustrs.begin(),edustrs.end(),ostream_iterator<string,char>(cout,"\n"));
  return 0;
}
int ScoreForBoundary::CountForEachEduVector(vector<string>&edus,const string&src_first,const string& src_last,
                                            const string&tgt_first,const string&tgt_last,vector<int>&res,int mode)
{
    using namespace boost;

    for(size_t j(0);j<edus.size();++j){
        if(mode!=1 && src_first.compare("")>0){ //mode=1:禁止查第一个词是否匹配
        if(starts_with(src_first,edus[j]) || starts_with(edus[j],src_first)){
             //第一个边界词重合
            if(!starts_with(tgt_first,"[X]")) //目标端的第一个词不能是非终结符
                res[0] += 1;
        }}
        if(mode!=2 && src_last.compare("")>0){//mode=2:禁止查最后一个词是否匹配
        if(ends_with(edus[j],src_last)||ends_with(src_last,edus[j])){
            //最后一个词重合
            if(!ends_with(tgt_last,"[X]"))//目标端的最后一个词不能是非终结符
              res[1] += 1;
        }}
    }
}
int ScoreForBoundary::AddBoundaryLabel(vector<string>& originRules,vector<int>& labelFirst,vector<int>& labelLast)
{
  using namespace boost;
  vector<RTreeNode *> allEdus = GetAllEdus(&this->mytree->root);
  //labelVec.resize(originRules.size(),"");
  vector<string> edustrs,eduextstrs;
  this->EduInd2Str(allEdus,edustrs,eduextstrs);
  int first(0),last(0); vector<int> resForOneRule(2,0);
  int modefirst(0);int modelast(0);
  for(size_t i(0);i<originRules.size();++i)
  {
      modefirst=0;modelast=0;
      resForOneRule[0]=0,resForOneRule[1]=0;
      size_t pos = originRules[i].find("[X] |||");
      string src = originRules[i].substr(0,pos); trim(src); pos+=8;
      size_t pos1 = originRules[i].find("[X] |||",pos);
      string tgt = originRules[i].substr(pos,pos1-pos);trim(tgt);
      string src_first(src),src_last(src),tgt_first(tgt),tgt_last(tgt);
      if(src.find("[X][X]")!=string::npos)
      {src_first = src.substr(0,src.find("[X][X]"));trim(src_first);
      src_last = src.substr(src.rfind("[X][X]")+6);trim(src_last);
      tgt_first = tgt.substr(0,tgt.find("[X][X]"));trim(tgt_first);
      tgt_last = tgt.substr(tgt.rfind("[X][X]")+6);trim(tgt_last);}

        //如果有边界词，打标签
        if(src_first.find(" ")==string::npos)modefirst=1; //如果只有标点符号则不进行额外vector的判断
        if(src_last.find(" ")==string::npos)modelast=2;
        CountForEachEduVector(edustrs,src_first,src_last,tgt_first,tgt_last,resForOneRule);
        if(resForOneRule[0]==0 && resForOneRule[1]==0 ){
          if(modefirst!=1 && modelast !=2)
            CountForEachEduVector(eduextstrs,src_first,src_last,tgt_first,tgt_last,resForOneRule);
          else if(modefirst==1 && modelast!=2)
            CountForEachEduVector(eduextstrs,src_first,src_last,tgt_first,tgt_last,resForOneRule,1);
          else if(modefirst != 1&& modelast==2)
            CountForEachEduVector(eduextstrs,src_first,src_last,tgt_first,tgt_last,resForOneRule,2);
        }
        else if(resForOneRule[0]==0 &&resForOneRule[1]!=0 && modefirst !=1){
          CountForEachEduVector(eduextstrs,src_first,src_last,tgt_first,tgt_last,resForOneRule,2);
        }
        else if(resForOneRule[0]!=0 && resForOneRule[1]==0 && modelast !=2){
          CountForEachEduVector(eduextstrs,src_first,src_last,tgt_first,tgt_last,resForOneRule,1);
        }
        labelFirst[i]=resForOneRule[0];labelLast[i]=resForOneRule[1];
    }

}
void test3(const char*chtct,const char*extractfile)
{
    ifstream in(chtct);ifstream in1(extractfile);
  string l,l1;
  vector<string> rules;
  while(getline(in,l)){
    vector<string>().swap(rules);
    while(getline(in1,l1)){
      if(l1.find("ABLANCKLINE")!=string::npos)break;
      else rules.push_back(l1);
    }
    if (rules.empty()){cout<<endl;continue;}
    ScoreForBoundary obj;
    if(obj.InitForClass(l)==-1){
      for(size_t i(0);i<rules.size();++i){cout<<"0 0"<<endl;}
    }
    else{
      vector<int> a(rules.size(),0),b(rules.size(),0);
      obj.AddBoundaryLabel(rules,a,b);
      for(size_t i(0);i<rules.size();++i){cout<<rules[i]<<" "<<a[i]<<" "<<b[i]<<endl;}
    }
    cout<<endl;
  }
}
