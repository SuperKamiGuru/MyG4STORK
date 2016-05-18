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
// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
// 070523 bug fix for G4FPE_DEBUG on by A. Howard ( and T. Koi)
// 080612 bug fix contribution from Benoit Pirard and Laurent Desorgher (Univ. Bern) #5
// 110505 protection for object is created but not initialized
// 110510 delete above protection with more coordinated work to other classes
//
#include "G4NeutronHPAngular.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

void G4NeutronHPAngular::Init(std::istream & aDataFile)
{
//  G4cout << "here we are entering the Angular Init"<<G4endl;
  aDataFile >> theAngularDistributionType >> targetMass;
  aDataFile >> frameFlag;
  if(theAngularDistributionType == 0 )
  {
    theIsoFlag = true;
  }
  else if(theAngularDistributionType==1)
  {
    theIsoFlag = false;
    G4int nEnergy;
    aDataFile >> nEnergy;
    theCoefficients = new G4NeutronHPLegendreStore(nEnergy);
    theCoefficients->InitInterpolation(aDataFile);
    G4double temp, energy;
    G4int tempdep, nLegendre;
    G4int i, ii;
    for (i=0; i<nEnergy; i++)
    {
      aDataFile >> temp >> energy >> tempdep >> nLegendre;
      energy *=eV;
      theCoefficients->Init(i, energy, nLegendre);
      theCoefficients->SetTemperature(i, temp);
      G4double coeff=0;
      for(ii=0; ii<nLegendre; ii++)
      {
        aDataFile >> coeff;
        theCoefficients->SetCoeff(i, ii+1, coeff);
      }
    }
  }
  else if (theAngularDistributionType==2)
  {
    theIsoFlag = false;
    G4int nEnergy;
    aDataFile >> nEnergy;
    theProbArray = new G4NeutronHPPartial(nEnergy, nEnergy);
    theProbArray->InitInterpolation(aDataFile);
    G4double temp, energy;
    G4int tempdep;
    for(G4int i=0; i<nEnergy; i++)
    {
      aDataFile >> temp >> energy >> tempdep;
      energy *= eV;
      theProbArray->SetT(i, temp);
      theProbArray->SetX(i, energy);
      theProbArray->InitData(i, aDataFile);
    }
  }
  else
  {
    theIsoFlag = false;
    G4cout << "unknown distribution found for Angular"<<G4endl;
    throw G4HadronicException(__FILE__, __LINE__, "unknown distribution needs implementation!!!");
  }
}

void G4NeutronHPAngular::SampleAndUpdate(G4ReactionProduct & aHadron)
{

  //********************************************************************
  //EMendoza -> sampling can be isotropic in LAB or in CMS
  /*
  if(theIsoFlag)
  {
//  G4cout << "Angular result "<<aHadron.GetTotalMomentum()<<" ";
// @@@ add code for isotropic emission in CMS.
      G4double costheta = 2.*G4UniformRand()-1;
      G4double theta = std::acos(costheta);
      G4double phi = twopi*G4UniformRand();
      G4double sinth = std::sin(theta);
      G4double en = aHadron.GetTotalMomentum();
      G4ThreeVector temp(en*sinth*std::cos(phi), en*sinth*std::sin(phi), en*std::cos(theta) );
      aHadron.SetMomentum( temp );
      aHadron.Lorentz(aHadron, -1.*theTarget);
  }
  else
  {
  */
  //********************************************************************
    if(frameFlag == 1) // LAB
    {
      G4double en = aHadron.GetTotalMomentum();
      G4ReactionProduct boosted;
      boosted.Lorentz(theNeutron, theTarget);
      G4double kineticEnergy = boosted.GetKineticEnergy();
      G4double cosTh = 0.0;
      //********************************************************************
      //EMendoza --> sampling can be also isotropic
      /*
      if(theAngularDistributionType == 1) cosTh = theCoefficients->SampleMax(kineticEnergy);
      if(theAngularDistributionType == 2) cosTh = theProbArray->Sample(kineticEnergy);
      */
      //********************************************************************
      if(theIsoFlag){cosTh =2.*G4UniformRand()-1;}
      else if(theAngularDistributionType == 1) {cosTh = theCoefficients->SampleMax(kineticEnergy);}
      else if(theAngularDistributionType == 2) {cosTh = theProbArray->Sample(kineticEnergy);}
      else{
        G4cout << "unknown distribution found for Angular"<<G4endl;
        throw G4HadronicException(__FILE__, __LINE__, "unknown distribution needs implementation!!!");
      }
      //********************************************************************
      G4double theta = std::acos(cosTh);
      G4double phi = twopi*G4UniformRand();
      G4double sinth = std::sin(theta);
      G4ThreeVector temp(en*sinth*std::cos(phi), en*sinth*std::sin(phi), en*std::cos(theta) );
      aHadron.SetMomentum( temp );
    }
    else if(frameFlag == 2) // costh in CMS
    {
      G4ReactionProduct boostedN;
      boostedN.Lorentz(theNeutron, theTarget);
      G4double kineticEnergy = boostedN.GetKineticEnergy();

      G4double cosTh = 0.0;
      //********************************************************************
      //EMendoza --> sampling can be also isotropic
      /*
      if(theAngularDistributionType == 1) cosTh = theCoefficients->SampleMax(kineticEnergy);
      if(theAngularDistributionType == 2) cosTh = theProbArray->Sample(kineticEnergy);
      */
      //********************************************************************
      if(theIsoFlag){cosTh =2.*G4UniformRand()-1;}
      else if(theAngularDistributionType == 1) {cosTh = theCoefficients->SampleMax(kineticEnergy);}
      else if(theAngularDistributionType == 2) {cosTh = theProbArray->Sample(kineticEnergy);}
      else{
        G4cout << "unknown distribution found for Angular"<<G4endl;
        throw G4HadronicException(__FILE__, __LINE__, "unknown distribution needs implementation!!!");
      }

        G4double theta = std::acos(cosTh);
      G4double phi = twopi*G4UniformRand();
      G4double sinth = std::sin(theta);
      G4double en = aHadron.GetTotalMomentum();
      G4ThreeVector temp(en*sinth*std::cos(phi), en*sinth*std::sin(phi), en*std::cos(theta) );
      aHadron.SetMomentum( temp );

    }
    else
    {
      throw G4HadronicException(__FILE__, __LINE__, "Tried to sample non isotropic neutron angular");
    }
  aHadron.Lorentz(aHadron, -1.*theTarget);
//  G4cout << aHadron.GetMomentum()<<" ";
//  G4cout << aHadron.GetTotalMomentum()<<G4endl;
}
