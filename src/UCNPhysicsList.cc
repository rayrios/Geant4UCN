//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: UCNPhysicsList.cc,v 1.3 2006/11/29 11:40:42 kuzniak Exp $
// GEANT4 tag $Name:  $
// 
//      modified for UCN, 9.9.04 peter fierlinger

#include "G4Timer.hh"
   
#include "UCNPhysicsList.hh"
#include "UCNDetectorConstruction.hh"
#include "UCNPhysicsListMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4EnergyLossTables.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include <iomanip>

#include "G4FastSimulationManagerProcess.hh"


/////////////////////////////////////////////////////////////
//
//

UCNPhysicsList::UCNPhysicsList(UCNDetectorConstruction* p)
:  G4VUserPhysicsList(), MaxChargedStep(DBL_MAX),
   thePhotoElectricEffect(0), theComptonScattering(0), theGammaConversion(0),
   theeminusMultipleScattering(0), theeminusIonisation(0),
   theeminusBremsstrahlung(0),
   theeplusMultipleScattering(0), theeplusIonisation(0),
   theeplusBremsstrahlung(0), theeplusAnnihilation(0),
   theeminusStepCut(0), theeplusStepCut(0)
{
  pDet = p;


  defaultCutValue = 1.000*mm ;

  cutForGamma = defaultCutValue ;
  cutForElectron = defaultCutValue ;

  SetVerboseLevel(1);
  physicsListMessenger = new UCNPhysicsListMessenger(this);
}

/////////////////////////////////////////////////////////////////////////
//
//

UCNPhysicsList::~UCNPhysicsList()
{
  delete physicsListMessenger; 
}

///////////////////////////////////////////////////////////////////////////
//
//

void UCNPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBarions();
}

////////////////////////////////////////////////////////////////////////////
//
//

void UCNPhysicsList::ConstructBosons()
{
}

void UCNPhysicsList::ConstructLeptons()
{
}

void UCNPhysicsList::ConstructMesons()
{
}

#include "UCNUCN.hh"

void UCNPhysicsList::ConstructBarions()
{
//  barions

UCNUCN::UCN0Definition();
 
 }

void UCNPhysicsList::ConstructProcess()
{
  AddUCNTransportation();
  //AddParameterisation();
  //ConstructEM();
  //ConstructGeneral();
  ConstructUCN();
}

#include "UCNTransportation.hh"
void UCNPhysicsList::AddUCNTransportation()
{
  UCNTransportation* theTransportationProcess= new UCNTransportation();

#ifdef G4VERBOSE
    if (verboseLevel >2){
      G4cout << "F02PhysicsList::AddUCNTransportation()  "<< G4endl;
    }
#endif

  // loop over all particles in G4ParticleTable
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (!particle->IsShortLived()) {
      // Add transportation process for all particles other than  "shortlived"
      if ( pmanager == 0) {
	// Error !! no process manager
	G4String particleName = particle->GetParticleName();
	G4Exception("F02PhysicsList::AddUCNTransportation","No process manager",
		    RunMustBeAborted, particleName );
      } else {
	// add transportation with ordering = ( -1, "first", "first" )
	pmanager ->AddProcess(theTransportationProcess);
	pmanager ->SetProcessOrderingToFirst(theTransportationProcess, idxAlongStep);
	pmanager ->SetProcessOrderingToFirst(theTransportationProcess, idxPostStep);
      }
    } else {
      // shortlived particle case
    }
  }
}



void UCNPhysicsList::ConstructEM()
{}

void UCNPhysicsList::ConstructGeneral()
{}

#include "UCNMaterialBoundary.hh"
#include "G4Decay.hh"
#include "G4DecayTable.hh"
#include "UCNSimpleAbsorption.hh"
#include "UCNSimpleLoss.hh"
#include "UCNMultiScattering.hh"
#include "UCNSpin.hh"
#include "UCNDetector.hh"
#include "UCNBetaDecayChannel.hh"

// !!! Uncomment below for geant4.8.1 !!!
#include "G4StepLimiter.hh"

void UCNPhysicsList::ConstructUCN()
{
  // Add UCN processes
  UCNMaterialBoundary* theMaterialBoundary = new UCNMaterialBoundary();
  UCNSimpleAbsorption* theSimpleAbsorption = new UCNSimpleAbsorption();
  UCNSimpleLoss* theSimpleLoss = new UCNSimpleLoss();
  UCNMultiScattering* theMultiScattering = new UCNMultiScattering();
  UCNSpin* theSpin = new UCNSpin();
  UCNDetector* theDetector = new UCNDetector();
  G4Decay* theDecayProcess = new G4Decay();

  theParticleIterator->reset();
  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "ucn")       {  
      
      particle->SetPDGLifeTime(886.7*s);
      particle->SetPDGStable(false);
      
      //create Decay Table
      static G4DecayTable*   table = particle->GetDecayTable();
      if (table!=NULL) delete table;
      table = new G4DecayTable();
      
      // create a decay channel
      G4VDecayChannel* mode =
        new UCNBetaDecayChannel("ucn",1.0);
      table->Insert(mode);

      particle->SetDecayTable(table);
            

      pmanager->AddDiscreteProcess(theDetector);
      pmanager->AddDiscreteProcess(theMaterialBoundary);
      pmanager->AddDiscreteProcess(theSimpleAbsorption);
      pmanager->AddDiscreteProcess(theSimpleLoss);
      pmanager->AddProcess(theMultiScattering);
      //pmanager->AddProcess(theSpin);
      
      pmanager ->SetProcessOrdering(theDetector, idxPostStep);
      //pmanager ->SetProcessOrdering(theSpin, idxPostStep);
      pmanager ->SetProcessOrdering(theMaterialBoundary, idxPostStep);
      pmanager ->SetProcessOrdering(theSimpleAbsorption, idxPostStep);
      pmanager ->SetProcessOrdering(theSimpleLoss, idxPostStep);
      pmanager ->SetProcessOrdering(theMultiScattering, idxPostStep);

      // !!!! Uncomment below for geant 4.8.1 !!!!
      pmanager->AddProcess(new G4StepLimiter,       -1,-1,3);


      if (theDecayProcess->IsApplicable(*particle)) {
	pmanager ->AddProcess(theDecayProcess);
	// set ordering for PostStepDoIt and AtRestDoIt
	pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
	pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
      }

    }
  }
}


/////////////////////////////////////////////////////////////////////////////

void UCNPhysicsList::AddParameterisation()
{}



/////////////////////////////////////////////////////////////////////////////

void UCNPhysicsList::SetCuts()
{
  G4Timer theTimer ;
  theTimer.Start() ;
  if (verboseLevel >0)
  {
    G4cout << "F02PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }  
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
 
   SetCutValue(cutForGamma,"gamma");

   SetCutValue(cutForElectron,"e-");
   SetCutValue(cutForElectron,"e+");

  if (verboseLevel>1)     DumpCutValuesTable();

  theTimer.Stop();
  G4cout.precision(6);
  G4cout << G4endl ;
  G4cout << "total time(SetCuts)=" << theTimer.GetUserElapsed() << " s " <<G4endl;

}

///////////////////////////////////////////////////////////////////////////

void UCNPhysicsList::SetGammaCut(G4double val)
{
  cutForGamma = val;
}

///////////////////////////////////////////////////////////////////////////

void UCNPhysicsList::SetElectronCut(G4double val)
{
  cutForElectron = val;
}

////////////////////////////////////////////////////////////////////////////

void UCNPhysicsList::SetMaxStep(G4double step)
{
  MaxChargedStep = step ;
  G4cout << " MaxChargedStep=" << MaxChargedStep << G4endl;
  G4cout << G4endl;
}

