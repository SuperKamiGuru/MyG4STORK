//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
 // Hadronic Process: High Precision low E neutron tracking
 // original by H.P. Wellisch, TRIUMF, 14-Feb-97
 // Builds and has the Cross-section data for one material.

#ifndef StorkNeutronHPInelastic_h
#define StorkNeutronHPInelastic_h 1

// Class Description
// Final state production model for a high precision (based on evaluated data
// libraries) description of neutron inelastic scattering below 20 MeV;
// 36 exclusive final states are consideded.
// To be used in your physics list in case you need this physics.
// In this case you want to register an object of this class with
// the corresponding process.
// Class Description - End

#include "globals.hh"
#include "StorkNeutronHPChannel.hh"
#include "G4HadronicInteraction.hh"
#include "StorkNeutronHPChannelList.hh"

#include "../ModGEANT4/Processes/G4NeutronHP2AInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP2N2AInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP2NAInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP2NDInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP2NInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP2NPInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP2PInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP3AInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP3NAInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP3NInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP3NPInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHP4NInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPAInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPD2AInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPDAInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPDInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPHe3InelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPN2AInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPN2PInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPN3AInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPNAInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPND2AInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPNDInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPNHe3InelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPNInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPNPAInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPNPInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPNT2AInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPNTInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPNXInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPPAInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPPDInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPPInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPPTInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPT2AInelasticFS.hh"
#include "../ModGEANT4/Processes/G4NeutronHPTInelasticFS.hh"

#include "MCNPEnvCheck.hh"

class StorkNeutronHPInelastic : public G4HadronicInteraction
{
  public:

  StorkNeutronHPInelastic(G4double finalStateTemp);

  ~StorkNeutronHPInelastic();

  G4HadFinalState * ApplyYourself(const G4HadProjectile & aTrack, G4Nucleus & aTargetNucleus);

  virtual const std::pair<G4double, G4double> GetFatalEnergyCheckLevels() const;

   public:
      G4int GetVerboseLevel() const;
      void SetVerboseLevel( G4int );

  private:

  G4double * xSec;
  //G4NeutronHPChannelList * theInelastic; // one List per element
      std::vector<StorkNeutronHPChannelList*> theInelastic; // one List per element
  G4String dirName;
  G4int numEle;
  G4double fsTemp;
      void addChannelForNewElement();

  private:

   G4NeutronHP2AInelasticFS the2AFS;
   G4NeutronHP2N2AInelasticFS the2N2AFS;
   G4NeutronHP2NAInelasticFS the2NAFS;
   G4NeutronHP2NDInelasticFS the2NDFS;
   G4NeutronHP2NInelasticFS the2NFS;
   G4NeutronHP2NPInelasticFS the2NPFS;
   G4NeutronHP2PInelasticFS the2PFS;
   G4NeutronHP3AInelasticFS the3AFS;
   G4NeutronHP3NAInelasticFS the3NAFS;
   G4NeutronHP3NInelasticFS the3NFS;
   G4NeutronHP3NPInelasticFS the3NPFS;
   G4NeutronHP4NInelasticFS the4NFS;
   G4NeutronHPAInelasticFS theAFS;
   G4NeutronHPD2AInelasticFS theD2AFS;
   G4NeutronHPDAInelasticFS theDAFS;
   G4NeutronHPDInelasticFS theDFS;
   G4NeutronHPHe3InelasticFS theHe3FS;
   G4NeutronHPN2AInelasticFS theN2AFS;
   G4NeutronHPN2PInelasticFS theN2PFS;
   G4NeutronHPN3AInelasticFS theN3AFS;
   G4NeutronHPNAInelasticFS theNAFS;
   G4NeutronHPND2AInelasticFS theND2AFS;
   G4NeutronHPNDInelasticFS theNDFS;
   G4NeutronHPNHe3InelasticFS theNHe3FS;
   G4NeutronHPNInelasticFS theNFS;
   G4NeutronHPNPAInelasticFS theNPAFS;
   G4NeutronHPNPInelasticFS theNPFS;
   G4NeutronHPNT2AInelasticFS theNT2AFS;
   G4NeutronHPNTInelasticFS theNTFS;
   G4NeutronHPNXInelasticFS theNXFS;
   G4NeutronHPPAInelasticFS thePAFS;
   G4NeutronHPPDInelasticFS thePDFS;
   G4NeutronHPPInelasticFS thePFS;
   G4NeutronHPPTInelasticFS thePTFS;
   G4NeutronHPT2AInelasticFS theT2AFS;
   G4NeutronHPTInelasticFS theTFS;
};

#endif
