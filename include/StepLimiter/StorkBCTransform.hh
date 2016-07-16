#ifndef StorkBCTRANSFORM_HH
#define StorkBCTRANSFORM_HH
#include "G4ThreeVector.hh"
#include <cmath>


class StorkBCTransform
{
    public:
        StorkBCTransform();
        virtual ~StorkBCTransform();
        virtual void Transform(G4ThreeVector &pos, G4ThreeVector &mom);
    protected:
        G4int TransPos[3][3];
        G4int TransMom[3][3];
    private:
};

#endif // StorkBCTRANSFORM_HH
