#ifndef StorkIsoReacScore_HH
#define StorkIsoReacScore_HH

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <sstream>
#include "G4Isotope.hh"
#include "StorkParseInput.hh"

using namespace std;

class StorkIsoReacScore
{
    public:
        StorkIsoReacScore(int rankID,const StorkParseInput* USER_INPUT);
        virtual ~StorkIsoReacScore();
        static void InitData()
        {
            ready=true;
            G4Isotope *isotope;
            const G4IsotopeTable *isoTable = isotope->GetIsotopeTable();
            vector<int> isoNameVec;
            numIso=isotope->GetIsotopeTable()->size();

            for(int i=0; i<numIso; i++)
            {
                isoName = isoTable[0][i]->GetZ()*1000+isoTable[0][i]->GetN();
                int j=0;
                for(; j<int(isoNameVec.size()); j++)
                {
                    if(isoNameVec[j]==isoName)
                    {
                        break;
                    }
                }
                if(j==int(isoNameVec.size()))
                {
                    isoNameVec.push_back(isoName);
                }
            }
            numIso=isoNameVec.size();

            isoNameList = new int[numIso];
            for(int i=0; i<numIso; i++)
            {
                isoNameList[i] = isoNameVec[i];
            }

            outNeutData = new int** [numIso];
            for(int i=0; i<numIso; i++)
            {
                outNeutData[i] = new int* [261];
                for(int j=0; j<261; j++)
                {
                    outNeutData[i][j] = new int[4];
                }
            }

            double energyStep;
            binBounds[0]=log10(1.0E-11);
            energyStep = log10(20.0/1.0E-11)/(261-1);

            for(int i=1; i<261; i++)
            {
                binBounds[i]=binBounds[i-1]+energyStep;
            }
        }

        static void DeleteData()
        {
            for(int i=0; i<numIso; i++)
            {
                for(int j=0; j<261; j++)
                {
                    if(outNeutData[i][j])
                        delete [] outNeutData[i][j];
                }
                if(outNeutData[i])
                    delete [] outNeutData[i];
            }
            if(outNeutData)
                delete [] outNeutData;
            if(isoNameList)
                delete [] isoNameList;
        }
        static void ResetScoreTable()
        {
            for(int k=0; k<numIso; k++)
            {
                for(int i=0; i<261; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        outNeutData[k][i][j]=0;
                    }
                }
            }
        }
        static void SetIso(int ZA)
        {
            isoName=ZA;
        }
        static void ScoreParticle(double EnerIn, int reacIndx)
        {
            int isotope=0;
            for(; isotope<numIso; isotope++)
            {
                if(isoNameList[isotope]==isoName)
                {
                    break;
                }
            }
            if(isotope==numIso)
            {
                cout << "isotope not found" << endl;
            }

            int enerIndx=0;
            if(EnerIn<1.0E-14)
                EnerIn=-100;
            else
                EnerIn=log10(EnerIn);

            for(int i=0; i<261; i++)
            {
                if(EnerIn<=binBounds[i]||i==261)
                {
                    enerIndx=i;
                    break;
                }
            }

            outNeutData[isotope][enerIndx][reacIndx]=outNeutData[isotope][enerIndx][reacIndx]+1;
        }
        static void PrintData()
        {
            stringstream stream;
            string tempFileName;

            for(int k=0; k<numIso; k++)
            {
                system(("mkdir -p -m=666 "+fileName).c_str());
                stream << fileName << "IsoReac" << isoNameList[k] << "Slave" << procRank << ".txt";
                tempFileName = stream.str();
                stream.str("");
                stream.clear();
                ofstream isoReacFile (tempFileName.c_str(),std::ofstream::out | std::ios::trunc );

                if (!isoReacFile)
                {
                    cout << "Failed to open file" << tempFileName << endl;
                    return;
                }

                isoReacFile.fill(' ');
                isoReacFile.precision(6);

                isoReacFile << "MCNP Energy Dist Samples" << endl;
                isoReacFile << 261 << '\n' << 4 << '\n' << endl;
                for(int i=0; i<261; i++)
                {
                    isoReacFile << std::setw(14) << binBounds[i];
                    if(((i+1)%6==0)||(i==260))
                        isoReacFile << endl;
                }
                isoReacFile << endl;

                for(int i=0; i<261; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        isoReacFile << std::setw(14) << outNeutData[k][i][j];
                    }
                    isoReacFile << endl;
                }
                isoReacFile << endl;

                isoReacFile.close();
            }
        }

        static bool Ready()
        {
            return ready;
        }

        static double binBounds[261];
        static int ***outNeutData;
        static string fileName;
        static int numIso, procRank,isoName;
        static int *isoNameList;
        static bool ready;


    protected:
    private:
};

#endif // StorkIsoReacScore_HH
