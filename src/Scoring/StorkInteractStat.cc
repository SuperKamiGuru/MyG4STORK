#include "StorkInteractStat.hh"
#include "StorkParseInput.hh"

int StorkInteractStat::reacCountVec[83];
int StorkInteractStat::MTRList[83];
string StorkInteractStat::fileName;
bool StorkInteractStat::ready;

StorkInteractStat::StorkInteractStat(const StorkParseInput* USER_INPUT)
{
    fileName = USER_INPUT->GetScoreIsoEnerDir();
    ready=false;
}

StorkInteractStat::~StorkInteractStat()
{

}
