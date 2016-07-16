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
#include "G4NeutronHPNBodyPhaseSpaceMCNP.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"

G4ReactionProduct * G4NeutronHPNBodyPhaseSpaceMCNP::Sample(G4double anEnergy, G4double /*massCode*/, G4double )
{
    //we assume that only neutrons use this process
   G4ReactionProduct * result = new G4ReactionProduct;

   G4int nb = theTotalCount;
   G4double ap = theTotalMass, r, s, p, x, y, t, energy, aw, nMass;

     result->SetDefinition(G4Neutron::Neutron());

    while(true)
    {
        r = std::pow(G4UniformRand(),2);
        s = r + std::pow(G4UniformRand(),2);
        if( s <= 1 )
            break;
    }
    x = -r*std::log(s)/s - std::log(G4UniformRand());
    while(true)
    {
      r = std::pow(G4UniformRand(),2);
      s = r + std::pow(G4UniformRand(),2);
      if( s <= 1 )
        break;
    }
    p = G4UniformRand();
    if( nb == 3 )
    {
      y = -r*std::log(s)/s - std::log(p);
    }
    else if( nb==4 )
    {
      p = p*G4UniformRand();
      y = -std::log( p*G4UniformRand() );
    }
    else
    {
      p = p*G4UniformRand()*G4UniformRand();
      p = p*G4UniformRand();
      y = -r*std::log(s)/s - std::log(p);
    }
    t = x/(x+y);
    aw = GetTarget()->GetMass();
    nMass = G4Neutron::Neutron()->GetPDGMass();
    energy = t*((ap-nMass)/ap)*(anEnergy*aw/(aw+nMass)+GetQValue());

   result->SetKineticEnergy(energy);

// now do random direction
   G4double cosTh = 2.*G4UniformRand()-1.;
   G4double phi = twopi*G4UniformRand();
   G4double theta = std::acos(cosTh);
   G4double sinth = std::sin(theta);
   G4double mtot = result->GetTotalMomentum();
   G4ThreeVector tempVector(mtot*sinth*std::cos(phi), mtot*sinth*std::sin(phi), mtot*std::cos(theta) );
   result->SetMomentum(tempVector);
   return result;
}
