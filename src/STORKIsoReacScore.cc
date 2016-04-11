#include "STORKIsoReacScore.hh"

double STORKIsoReacScore::binBounds[261];
int*** STORKIsoReacScore::outNeutData;
string STORKIsoReacScore::fileName;
int STORKIsoReacScore::numIso;
int STORKIsoReacScore::procRank;
int STORKIsoReacScore::isoName;
int* STORKIsoReacScore::isoNameList;

STORKIsoReacScore::STORKIsoReacScore(int rankID)
{
    stringstream stream;

    fileName = "/home/SuperKamiGuru/NuclearSim/G4STORKIsoReac/";
    procRank = rankID;
}

STORKIsoReacScore::~STORKIsoReacScore()
{
    //dtor
}
