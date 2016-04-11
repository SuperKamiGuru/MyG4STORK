#include "STORKEnergyDistScore.hh"

double STORKEnergyDistScore::binBounds[261];
int*** STORKEnergyDistScore::outNeutData;
string STORKEnergyDistScore::fileName;
int STORKEnergyDistScore::numIso;
int STORKEnergyDistScore::procRank;
int STORKEnergyDistScore::isoName;
int* STORKEnergyDistScore::isoNameList;
int* STORKEnergyDistScore::isoUseList;

STORKEnergyDistScore::STORKEnergyDistScore(int rankID)
{
    stringstream stream;

    fileName = "/home/SuperKamiGuru/NuclearSim/EnergyDist/EnergyDis";
    procRank = rankID;
}

STORKEnergyDistScore::~STORKEnergyDistScore()
{
    //dtor
}
