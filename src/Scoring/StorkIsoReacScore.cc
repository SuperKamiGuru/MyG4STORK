#include "StorkIsoReacScore.hh"

double StorkIsoReacScore::binBounds[261];
int*** StorkIsoReacScore::outNeutData;
string StorkIsoReacScore::fileName;
int StorkIsoReacScore::numIso;
int StorkIsoReacScore::procRank;
int StorkIsoReacScore::isoName;
int* StorkIsoReacScore::isoNameList;
bool StorkIsoReacScore::ready;

StorkIsoReacScore::StorkIsoReacScore(int rankID, const StorkParseInput* USER_INPUT)
{
    ready=false;
    fileName = USER_INPUT->GetScoreIsoEnerDir();
    procRank = rankID;
}

StorkIsoReacScore::~StorkIsoReacScore()
{
    //dtor
}
