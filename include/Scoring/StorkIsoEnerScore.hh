#ifndef StorkIsoEnerScore_HH
#define StorkIsoEnerScore_HH

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <sstream>
#include "G4Isotope.hh"
#include "StorkParseInput.hh"

using namespace std;

class StorkIsoEnerScore
{
    public:
        StorkIsoEnerScore(int rankID,const StorkParseInput* USER_INPUT);
        virtual ~StorkIsoEnerScore();
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
            isoUseList = new int [numIso];
            for(int i=0; i<numIso; i++)
            {
                isoNameList[i] = isoNameVec[i];
                isoUseList[i] = 0;
            }

            outNeutData = new int*** [3];
            for(int i=0; i<3; i++)
            {
                outNeutData[i] = new int** [numIso];
                for(int j=0; j<numIso; j++)
                {
                    outNeutData[i][j] = new int* [261];
                    for(int k=0; k<261; k++)
                    {
                        outNeutData[i][j][k] = new int[261];
                    }
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
            for(int k=0; k<3; k++)
            {
                for(int i=0; i<numIso; i++)
                {
                    for(int j=0; j<261; j++)
                    {
                        if(outNeutData[k][i][j])
                            delete [] outNeutData[k][i][j];
                    }
                    if(outNeutData[k][i])
                        delete [] outNeutData[k][i];
                }
                if(outNeutData[k])
                        delete [] outNeutData[k];
            }
            if(outNeutData)
                delete [] outNeutData;
            if(isoNameList)
                delete [] isoNameList;
            if(isoUseList)
                delete [] isoUseList;
        }

        static void ResetScoreTable()
        {
            for(int l=0; l<3; l++)
            {
                for(int k=0; k<numIso; k++)
                {
                    for(int i=0; i<261; i++)
                    {
                        for(int j=0; j<261; j++)
                        {
                            outNeutData[l][k][i][j]=0;
                        }
                    }
                }
            }
        }
        static void SetIso(int ZA)
        {
            isoName=ZA;
        }
        static void ScoreParticle(double EnerIn, double EnerOut, int reacIndx)
        {
            if(reacIndx>2)
                reacIndx=2;

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

            isoUseList[isotope]=1;

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

            if(EnerOut<1.0E-14)
                EnerOut=-100;
            else
                EnerOut=log10(EnerOut);

            for(int i=0; i<261; i++)
            {
                if(EnerOut<=binBounds[i]||i==261)
                {
                    outNeutData[reacIndx][isotope][enerIndx][i]=outNeutData[reacIndx][isotope][enerIndx][i]+1;
                    break;
                }
            }
        }
        static void PrintData()
        {
            stringstream stream;
            string tempFileName;
            string reac[3] = {"Elastic", "Fission","Inelastic"};

            for(int l=0; l<3; l++)
            {
                for(int k=0; k<numIso; k++)
                {
                    if(isoUseList[k]!=0)
                    {
                        stream << fileName << reac[l] << isoNameList[k] << "Slave" << procRank << ".txt";
                        tempFileName = stream.str();
                        stream.str("");
                        stream.clear();
                        ofstream enerDistFile (tempFileName.c_str(),std::ofstream::out);

                        if (!enerDistFile)
                        {
                            cout << "Failed to open file" << endl;
                            return;
                        }

                        enerDistFile.fill(' ');
                        enerDistFile.precision(6);

                        enerDistFile << "MCNP Energy Dist Samples" << endl;
                        enerDistFile << 261 << endl;
                        enerDistFile << 261 << '\n' << endl;
                        for(int i=0; i<261; i++)
                        {
                            enerDistFile << std::setw(14) << binBounds[i];
                            if(((i+1)%6==0)||(i==260))
                                enerDistFile << endl;
                        }
                        enerDistFile << endl;

                        for(int i=0; i<261; i++)
                        {
                            for(int j=0; j<261; j++)
                            {
                                enerDistFile << std::setw(14) << outNeutData[l][k][i][j];
                                if(((j+1)%6==0)||(j==260))
                                    enerDistFile << endl;
                            }
                        }
                        enerDistFile << endl;

                        enerDistFile.close();
                        enerDistFile.clear();
                    }
                }
            }
        }

        static bool Ready()
        {
            return ready;
        }

        static double binBounds[261];
        static int ****outNeutData;
        static string fileName;
        static int numIso, procRank,isoName;
        static int *isoNameList;
        static int *isoUseList;
        static bool ready;

    protected:
    private:
};

#endif // StorkIsoEnerScore_HH
