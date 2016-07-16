#ifndef StorkREFLECTBCTRANSFORM_HH
#define StorkREFLECTBCTRANSFORM_HH

#include "StorkBCTransform.hh"

class StorkReflectBCTransform: public StorkBCTransform
{
    public:
        StorkReflectBCTransform();
        StorkReflectBCTransform(G4ThreeVector n1);
        virtual ~StorkReflectBCTransform();
        void InitializeTransform(G4ThreeVector n1);
    protected:
    private:
};

#endif // StorkREFLECTBCTRANSFORM_HH
