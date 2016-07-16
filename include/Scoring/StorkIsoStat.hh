#ifndef StorkIsoStat_HH
#define StorkIsoStat_HH

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "G4Isotope.hh"
#include <vector>
#include "StorkParseInput.hh"

using namespace std;

class StorkIsoStat
{
    public:
        StorkIsoStat(const StorkParseInput* USER_INPUT);
        virtual ~StorkIsoStat();
        //Process call statistics
        static void CreateArrays()
        {
            ready=true;
            G4Isotope *isotope;
            const G4IsotopeTable *isoTable = isotope->GetIsotopeTable();
            vector<int> isoNameVec;
            numIso=isotope->GetIsotopeTable()->size();
            int swapInt;

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

            for(int i=0; i<numIso; i++)
            {
                for(int j=i+1; j<numIso; j++)
                {
                    if(isoNameList[i]>isoNameList[j])
                    {
                        swapInt=isoNameList[i];
                        isoNameList[i] = isoNameList[j];
                        isoNameList[j]=swapInt;
                    }
                }

            }

            outNeutData = new int [numIso];
            for(int i=0; i<numIso; i++)
            {
                outNeutData[i]=0;
            }
        }
        static void DeleteArrays()
        {
            if(isoNameList)
                delete [] isoNameList;
            if(outNeutData)
                delete [] outNeutData;
        }
        static void IncIsoCount(int isoNameTemp)
        {
//            cout << isoNameTemp << endl;
            int isotope=0;
            for(; isotope<numIso; isotope++)
            {
                if(isoNameList[isotope]==isoNameTemp)
                {
                    outNeutData[isotope]=outNeutData[isotope]+1;
                    break;
                }
            }
            if(isotope==numIso)
            {
//                cout << isoNameTemp << endl;
            }
        }
        static void ZeroIsoCount()
        {
            std::fill(outNeutData, outNeutData+numIso, 0);
        }
        static void PrintIsoCount()
        {
            int sumIso=0;
            ofstream isoDistFile (fileName.c_str(),std::ofstream::out);
            isoDistFile << "isoNameList" << endl;

            for(int i=0; i<numIso; i++)
            {
                sumIso += outNeutData[i];
            }
            for(int i=0; i<numIso; i++)
            {
                if(outNeutData[i]!=0)
                    isoDistFile << setw(14) << isoNameList[i] <<  setw(14) << double(outNeutData[i])/sumIso << endl;
            }
            isoDistFile.close();
            isoDistFile.clear();
        }

        static bool Ready()
        {
            return ready;
        }

        static int *outNeutData;
        static int numIso,isoName;
        static int *isoNameList;
        static string fileName;
        static bool ready;
    protected:
    private:
};

#endif // StorkIsoStat_HH
