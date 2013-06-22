#ifndef SCOREFORBOUNDARY_H_INCLUDED
#define SCOREFORBOUNDARY_H_INCLUDED
#include"Common.h"
#include"SynTreeSQ.h"

class ScoreForBoundary
{
public:
    int InitForClass(const string& tctstr);
    int AddBoundaryLabel(vector<string>& originRules,vector<int>& labelFirst,vector<int>& labelLast);
    int EduInd2Str(vector<RTreeNode *>& allEdus,vector<string>&edustrs,vector<string>& eduextstrs);
    int CountForEachEduVector(vector<string>&edus,const string&src_first,const string& src_last,
                                            const string&tgt_first,const string&tgt_last,vector<int>&res,int mode=0);

public:
    SynTreeSQ *mytree;

};
void test3(const char*chtct,const char*extractfile);

#endif // SCOREFORBOUNDARY_H_INCLUDED
