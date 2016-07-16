#include "StorkIsoEnerScore.hh"

double StorkIsoEnerScore::binBounds[261];
int**** StorkIsoEnerScore::outNeutData;
string StorkIsoEnerScore::fileName;
int StorkIsoEnerScore::numIso;
int StorkIsoEnerScore::procRank;
int StorkIsoEnerScore::isoName;
int* StorkIsoEnerScore::isoNameList;
int* StorkIsoEnerScore::isoUseList;
bool StorkIsoEnerScore::ready;

StorkIsoEnerScore::StorkIsoEnerScore(int rankID, const StorkParseInput* USER_INPUT)
{
    ready=false;
    fileName = USER_INPUT->GetScoreIsoEnerDir();
    procRank = rankID;
}

StorkIsoEnerScore::~StorkIsoEnerScore()
{
    //dtor
}
