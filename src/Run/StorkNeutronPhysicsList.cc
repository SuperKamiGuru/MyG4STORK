/*
StorkNeutronPhysicsList.cc

Created by:		Liam Russell
Date:			23-02-2012
Modified:       11-03-2013

Source code for StorkNeutronPhysicsList class.

*/


// Include header file
#include "StorkNeutronPhysicsList.hh"


// Constructor
StorkNeutronPhysicsList::StorkNeutronPhysicsList(const StorkParseInput* infile)
{
    SetVerboseLevel(0);

    csDirName = infile->GetCSDirName();
    fsDirName = infile->GetFSDirName();
    kCalcType = infile->GetKCalcType();
    infile->GetPeriodicBCVector(periodicBCVec);
    infile->GetReflectBCVector(reflectBCVec);

    theHPNeutron=NULL;
    theBertiniNeutron=NULL;
    theNeutrons=NULL;

    // Insure that the slave processes do not output the physics process table
#ifdef G4USE_TOPC
	if(!TOPC_is_master())
	{
		G4HadronicProcessStore *theStore = G4HadronicProcessStore::Instance();
		theStore->SetVerbose(0);
	}
#endif
}


// Destructor
StorkNeutronPhysicsList::~StorkNeutronPhysicsList()
{
    if(theHPNeutron)
        delete theHPNeutron;
    //delete theLEPNeutron;
    if(theBertiniNeutron)
        delete theBertiniNeutron;
    if(theNeutrons)
        delete theNeutrons;
}


// ConstructParticle()
void StorkNeutronPhysicsList::ConstructParticle()
{
	// Construct relevant particles for the physics list
    G4Geantino::GeantinoDefinition();
    G4GenericIon::GenericIonDefinition();
    G4Neutron::NeutronDefinition();
    G4Deuteron::DeuteronDefinition();
    G4Triton::TritonDefinition();
    G4Gamma::GammaDefinition();
    G4Proton::ProtonDefinition();
    G4AntiProton::AntiProtonDefinition();
    G4PionPlus::PionPlusDefinition();
    G4PionZero::PionZeroDefinition();
    G4PionMinus::PionMinusDefinition();
    G4KaonPlus::KaonPlusDefinition();
    G4KaonZero::KaonZeroDefinition();
    G4KaonMinus::KaonMinusDefinition();
    G4KaonZeroShort::KaonZeroShortDefinition();
    G4KaonZeroLong::KaonZeroLongDefinition();
    G4Alpha::AlphaDefinition();
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();
}


// ConstructProcess()
// Construct the HP processes for neutron energies in [0,20) MeV and the
// Bertini processes for neutron energies in [20,inf) MeV.
// Also add transportation.
void StorkNeutronPhysicsList::ConstructProcess()
{
    // Create the Bertini neutron processes ( > 20 MeV)
    theNeutrons = new StorkNeutronProcessBuilder(periodicBCVec, reflectBCVec, fsDirName, kCalcType);

    // Create and register the builders
    theNeutrons->RegisterMe(theHPNeutron = new StorkHPNeutronBuilder(csDirName));
    // Set the minimum and maximum energies for the NeutronHP processes
    theHPNeutron->SetMinEnergy(0.*eV);
    theHPNeutron->SetMaxEnergy(20.*MeV);

    MCNPEnvCheck *runLikeMCNP;
    if(!runLikeMCNP->GetFlag())
    {
        theNeutrons->RegisterMe(theBertiniNeutron = new G4BertiniNeutronBuilder);
        // Set the minimum and maximum energies for the Bertini processes
        theBertiniNeutron->SetMinEnergy(20.0*MeV);
        theBertiniNeutron->SetMaxEnergy(9.9*GeV);
    }

    // Build the models and data
    theNeutrons->Build();

    // Add the transportation process
    AddTransportation();
}


// SetCuts()
void StorkNeutronPhysicsList::SetCuts()
{
    SetCutsWithDefault();
}
