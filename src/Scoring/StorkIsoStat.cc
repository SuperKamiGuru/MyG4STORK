#include "StorkIsoStat.hh"

int* StorkIsoStat::outNeutData;
int StorkIsoStat::numIso;
int StorkIsoStat::isoName;
int* StorkIsoStat::isoNameList;
string StorkIsoStat::fileName;
bool StorkIsoStat::ready;

StorkIsoStat::StorkIsoStat(const StorkParseInput* USER_INPUT)
{
    ready=false;
    fileName = USER_INPUT->GetScoreIsoEnerDir();
}

StorkIsoStat::~StorkIsoStat()
{

}
