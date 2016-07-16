#ifndef StorkINTERACTSTAT_HH
#define StorkINTERACTSTAT_HH

#include <iostream>
#include <fstream>
#include <iomanip>
#include "StorkParseInput.hh"

using namespace std;

class StorkInteractStat
{
    public:
        StorkInteractStat(const StorkParseInput* USER_INPUT);
        virtual ~StorkInteractStat();
        //Process call statistics
        static void CreateArrays()
        {
            ready=true;
            int Temp[83] = {2,102,18,19,20,21,38,4,5,11,16,17,22,23,24,25,28,29,-1,32,33,34,-1,-1,37,41,42,44,45,103,104,105,106,107,108,
                                -1,111,112,-1,113,115,116,117, 51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,
                                78,79,80,81,82,83,84,85,86,87,88,89,90,91};
            for(int i=0; i<83; i++)
            {
                MTRList[i]=Temp[i];
            }
        }
        static void IncrementReacCount(int MT)
        {
            for(int i=0; i<83; i++)
            {
                if(MTRList[i]==MT)
                {
                    reacCountVec[i]=reacCountVec[i]+1;
                    break;
                }
            }
        }
        static void ZeroReacCount()
        {
            std::fill(reacCountVec, reacCountVec+83, 0);
        }
        static void PrintReacCount()
        {
            ofstream reacDistFile (fileName.c_str(),std::ofstream::out);
            reacDistFile << "MTRList" << endl;
            for(int i=0; i<83; i++)
            {
                reacDistFile << setw(14) << MTRList[i];
                if(((i+1)%3==0)||(i==82))
                {
                    reacDistFile << endl;
                }
            }
            reacDistFile << "MTRCount" << endl;
            for(int i=0; i<83; i++)
            {
                reacDistFile << setw(14) << reacCountVec[i];
                if(((i+1)%3==0)||(i==82))
                {
                    reacDistFile << endl;
                }
            }
            reacDistFile.close();
            reacDistFile.clear();
        }

        static bool Ready()
        {
            return ready;
        }

        static int reacCountVec[83];
        static int MTRList[83];
        static string fileName;
        static bool ready;

    protected:
    private:
};

#endif // StorkINTERACTSTAT_HH
