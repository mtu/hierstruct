#include"SynTreeSQ.h"
using namespace boost;
using namespace RelationTree;

vector<vector<string> > disVec;
vector<vector<string> > eduVec;

void LoadResource(const char*disfile,const char*edufile)
{
    ifstream in(disfile);
    ifstream in1(edufile);
    if(!in||!in1){cerr<<"wrong to open\n";exit(1);}
    string l,l1;
    vector<string> vectmp; vector<string>().swap(vectmp);
    while(getline(in,l)){
      trim(l);
      if(l.find("( Root",0)!=string::npos){
        if(!vectmp.empty())disVec.push_back(vectmp);
        vector<string>().swap(vectmp);
      }
      vectmp.push_back(l);
    }
    if(!vectmp.empty())
    disVec.push_back(vectmp);vector<string>().swap(vectmp);
    while(getline(in1,l)){
      trim(l);
      if(l==""){
        if(!vectmp.empty())eduVec.push_back(vectmp);
        vector<string>().swap(vectmp);
      }
      else
      vectmp.push_back(l);
    }
    if(!vectmp.empty())
    eduVec.push_back(vectmp);vector<string>().swap(vectmp);
    if(eduVec.size()!=disVec.size()){cerr<<"size doesnot match\n";
      cerr<<eduVec.size()<<" "<<disVec.size()<<endl;
    exit(1);}
}
string GetParaStr(const string&src,const string&symbol)
{
    size_t pos = src.find(symbol)+symbol.length();
    size_t pos1 = src.find(")",pos);
    string s;
    if (pos != string::npos && pos1 !=string::npos)
    s=src.substr(pos,pos1-pos);
    else{s="";}
    trim(s);return s;
}
TreeNode GenerateRSTTree(vector<string>&unitDisVec,vector<string>&unitEduVec)
{
    TreeNode root("",NULL);
    if(unitDisVec[0].find("( Root")==string::npos)return root;
    int beg,back;string word;
    size_t prepos(0),curpos(0);
    stringstream ss(unitDisVec[0].substr(12,unitDisVec[0].length()-13));
    ss>>beg,ss>>back;
    if(back!=unitEduVec.size()){cerr<<"back:"<<back<<" unitEudVecsize:"<<unitEduVec.size()<<endl;exit(1);}

    root.Begin=beg;root.Back=back;
    TreeNode *currNode = &root;
    for(size_t i(1);i<unitDisVec.size();++i){
        size_t tmppos=unitDisVec[i].find("//");
        if(tmppos!=string::npos)
          unitDisVec[i]=unitDisVec[i].substr(0,tmppos);
        if(unitDisVec[i]==")"){
            currNode = currNode->parent;continue;}
        if(unitDisVec[i].find("(leaf")!=string::npos){
          word = GetParaStr(unitDisVec[i],"(leaf");
          beg=lexical_cast<int>(word);back=beg;
        }else if(unitDisVec[i].find("(span")<15){
          word = GetParaStr(unitDisVec[i],"(span");
          stringstream ss(word);
          ss>>beg;ss>>back;
        }else{cerr<<"wrong to find leaf or span\n";exit(1);}
        //new a treenode as current node's child
        TreeNode *p = new TreeNode("",currNode);
        p->Begin=beg;p->Back=back;
        if(unitDisVec[i].find("rel2par")!=string::npos){
          word = GetParaStr(unitDisVec[i],"rel2par");trim(word);
        }else{cerr<<"wrong to find rel2par\n";exit(1);}
        //assign at current value
        if(currNode->value=="" && word.compare("span")!=0){
          currNode->value = word;
        }
        //push back child
        currNode->children.push_back(p);
        //check children
        //if(currNode->children.size()>2){cerr<<"children size > 2\n"<<i<<endl;exit(1);}
        //check whether return
        if(unitDisVec[i].substr(0,2)=="( "){
            currNode = p;}

//        if(tmppos!=string::npos){
//          if(unitDisVec[i].substr(tmppos-2,2)==" )")
//            currNode = currNode->parent;
//        }
        if(unitDisVec[i].substr(unitDisVec[i].length()-2)==" )"){
            currNode = currNode->parent;}
    }
    return root;
}
void PrintSingle(TreeNode *node,vector<string>& unitEduVec)
{
  if(node->children.empty()){cout<<" [0: "<<unitEduVec[node->Begin-1]<<" ]";return;}
  cout<<" ["<<node->value;
  for(size_t i(0);i<node->children.size();++i){
    PrintSingle(node->children[i],unitEduVec);
  }
  cout<<" ]";
}
void RecurseNode_1(TreeNode *node,vector<TreeNode *>&vec)
{
  if(node->children.empty()){return;}
  for(size_t i(0);i<node->children.size();++i){
    vec.push_back(node->children[i]);
    RecurseNode_1(node->children[i],vec);
  }
}
vector<TreeNode*> GetAllNodes(TreeNode *root)
{
  vector<TreeNode *> vec;
  vec.push_back(root);
  RecurseNode_1(root,vec);
  //cerr<<vec.size()<<endl;
  return vec;
}
int CutintoSentenceLevel(TreeNode *node,vector<string>& unitEduVec)
{
  vector<TreeNode *> vec = GetAllNodes(node);
  int beg(0),index(-1);
  for(size_t i(0);i<unitEduVec.size();++i){
    if(unitEduVec[i].substr(unitEduVec[i].length()-1)=="."){
       //cerr<<"target: "<<beg<<" "<<i<<endl;
       for(size_t k(beg);k<=i;++k){
         cout<<unitEduVec[k]<<" ";
       }
       cout<<endl;
       /*for(size_t j(0);j<vec.size();++j){
       //  cerr<<vec[j]->Begin<<" "<<vec[j]->Back<<endl;
         if(vec[j]->Begin == beg+1 && vec[j]->Back == i+1){
           index=j;break;
         }
       }
       if(index>-1){
          PrintSingle(vec[index],unitEduVec);
          cout<<endl;
        }else{
          //cout<<"wrong to find the right index\n"<<beg<<" "<<i;return -1;
          cout<<"[UNK";
          for(size_t k(beg);k<=i;++k){
            cout<<" [0: "<<unitEduVec[k]<<" ]";
          }
          cout<<" ]"<<endl;
        }*/
        beg = i+1;index=-1;
    }
  }
  return 0;
}
void ConvertRST2TST(const char*disfile,const char*edufile)
{
  LoadResource(disfile,edufile);

  for(size_t i(0);i<disVec.size();++i){
    TreeNode root = GenerateRSTTree(disVec[i],eduVec[i]);
//    PrintSingle(&root,eduVec[i]);
//    cout<<endl;
    if(CutintoSentenceLevel(&root,eduVec[i])==-1){
      cout<<" dis index:"<<i<<endl;
    }
  }

}

//int main(int argc,char *argv[])
//{
////    argv[2]="/home/mtu/hierstruct/hierstruct/bin/Debug/edutok.test";
////    argv[1]="/home/mtu/hierstruct/hierstruct/bin/Debug/tmpdis.test";
//    if(argc==1){cerr<<argv[0]<<" <tmpdis> <edutok> \n";exit(1);}
//    ConvertRST2TST(argv[1],argv[2]);
//    return 0;
//}

