#include "TransAnalysis.h"
#include "ScoreForBoundary.h"

void test(const char*hierusefile,const char*treefile)
{
  ifstream in(hierusefile);
  ifstream in1(treefile);
  if(!in||!in1){cerr<<"wrong to open\n";exit(1);}
  string l,l1,word; vector<string> srcvec;
  int count(0),tmp(0),matchNum(0),countnode(0),matchline(0);size_t pos,pos1;
  while(getline(in,l)){
    stringstream ss(l);
    ss>>word;ss>>word;ss>>word;
    tmp = boost::lexical_cast<int>(word);
    ss>>word;
    if(tmp == count){
      pos = word.find("[")+1;pos1 = word.find_last_of("]");
      srcvec.push_back(word.substr(pos,pos1-pos));
    } else{
      count += 1;
      getline(in1,l1);
      AnalysisClass obj;
      obj.init(l1,srcvec);
      if(obj.allNodes.size()>1){
        matchline = obj.Matched(obj.allNodes,obj.spanVecFromHier);
        matchNum += matchline;
        countnode += obj.allNodes.size();
      cerr<<"line "<<count-1<<" res: " <<matchline <<" "<< (matchline)/float(obj.allNodes.size())<<endl;}
      vector<string>().swap(srcvec);
    }
    ss.str("");
  }
  getline(in1,l1);
  AnalysisClass obj;
  obj.init(l1,srcvec);
  if(obj.allNodes.size()>1){
        matchline = obj.Matched(obj.allNodes,obj.spanVecFromHier);
        matchNum += matchline;
        countnode += obj.allNodes.size();
        cerr<<"line "<<count-1<<" res: " <<matchline <<" "<< (matchline)/float(obj.allNodes.size())<<endl;}
  vector<string>().swap(srcvec);
  cerr<<"all matched: "<<matchNum<<"    "<<"all nodes: "<<countnode<<endl;
  cerr<<"rate: "<<float(matchNum)/countnode<<endl;
}
void test1(const char*chstctfile,const char*engtctfile,const char*align)
{
    ifstream in1(chstctfile);ifstream in2(engtctfile);
    ifstream in3(align);
    if(!in1 || !in2||!in3){cerr<<"wrong to open\n";exit(1);}
    string l1,l2,l3; int count(0);
    CountStruct cs,tmp;
    while(getline(in1,l1)&&getline(in2,l2)&&getline(in3,l3))
    {
        count++;
        TransAnalysisClass obj;
        if(obj.LoadAllTransAlign(l1,l2,l3)==-1){
            vector<RTreeNode *>().swap(obj.allChsNodes);
            vector<RTreeNode *>().swap(obj.allEngNodes);
            continue;}
        obj.CountMatchedEdu(cs);
        cerr<<count<<" ";
        cerr<<"leftc= "<<cs.leftC-tmp.leftC<<"  rightc= "<<cs.rightC-tmp.rightC<<"  matched= "<<cs.matchedC-tmp.matchedC<<endl;
       // cerr<<cs.leftC<<" "<<cs.rightC<<" "<<cs.matchedC<<endl;

        tmp.leftC=cs.leftC;tmp.rightC=cs.rightC;tmp.matchedC=cs.matchedC;
    }
    cerr<<cs.leftC<<" "<<cs.rightC<<" "<<cs.matchedC<<endl;

}
void test2(const char*chstctfile,const char*engtctfile,const char*align)
{
  ifstream in1(chstctfile);ifstream in2(engtctfile);
    ifstream in3(align);
    if(!in1 || !in2||!in3){cerr<<"wrong to open\n";exit(1);}
    string l1,l2,l3; int count(0);
    CountStruct cs,tmp;
    while(getline(in1,l1)&&getline(in3,l3))
    {
        count++;
        TransAnalysisClass obj;
        if(obj.LoadAllTransAlign(l1,l3)==-1){
            vector<RTreeNode *>().swap(obj.allChsNodes);
            continue;}
        obj.CountAlignConsistentEdus(cs);
        cerr<<count<<" ";
        cerr<<"leftc= "<<cs.leftC-tmp.leftC<<"  matched= "<<cs.matchedC-tmp.matchedC<<endl;
       // cerr<<cs.leftC<<" "<<cs.rightC<<" "<<cs.matchedC<<endl;

        tmp.leftC=cs.leftC;tmp.matchedC=cs.matchedC;
    }
    cerr<<cs.leftC<<" "<<cs.matchedC<<endl;
}

int main(int argc,char *argv[])
{
   /* argv[1]="/home/mtu/hierstruct/hierstruct/bin/Debug/06.detail";
    argv[2]="/home/mtu/hierstruct/hierstruct/bin/Debug/06.treefile";
    test(argv[1],argv[2]);*/
/*    argv[1]="/home/mtu/workdir/corpus/hierdiscourse/ch.tct";
    argv[2]="/home/mtu/workdir/corpus/hierdiscourse/en.tct";
    argv[3]="/home/mtu/workdir/corpus/hierdiscourse/aligned.num.parallel";
    test1(argv[1],argv[2],argv[3]);
*/
    argv[1]="/home/mtu/workdir/corpus/hierdiscourse/ch.tct";
    argv[2]="/home/mtu/hierstruct/hierstruct/bin/Debug/test";
    test3(argv[1],argv[2]);
    return 0;
}
