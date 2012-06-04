///////////////////////////////////////////////////////////////////////
//
//   UCN simple absorption, 7.9.04, peter fierlinger
//				21.4.06 marcin kuzniak
//   energy-independent loss cross section inside a material
///////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include <cmath>
#include "UCNSimpleLoss.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4StepPoint.hh"
#include "Randomize.hh"
#include "G4ElementVector.hh"

UCNSimpleLoss* UCNSimpleLoss::theInstance = 0;

UCNSimpleLoss::UCNSimpleLoss(const G4String& processName)
              : G4VDiscreteProcess(processName)
{

     theInstance = this;
	if (verboselevel>0) {
           G4cout << GetProcessName() << " is created " << G4endl;
        }
}

UCNSimpleLoss::~UCNSimpleLoss(){}

G4VParticleChange*
UCNSimpleLoss::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);
  G4double t1 =  aTrack.GetProperTime();
  G4double t2 =  aTrack.GetGlobalTime();
  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
  G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();
  G4ThreeVector thePoint1 = pPostStepPoint->GetPosition();
  G4ThreeVector thePoint = pPreStepPoint->GetPosition();
  G4double tracklen = aTrack.GetTrackLength();
  G4String v2 = pPostStepPoint ->GetPhysicalVolume()->GetName();

  if (verboseLevel>0) 
       G4cout << "SIMPLELOSS " << t1*1e-9 << "s, " << t2*1e-9 << "s. " 
       << ", tracklen " << tracklen << ", in volume " << v2 << G4endl;
     
  aParticleChange.ProposeTrackStatus(fStopAndKill);
  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  
}

G4double UCNSimpleLoss::GetMeanFreePath(const G4Track& aTrack,
 				         G4double ,
				         G4ForceCondition*)
{
	
	const G4Material* aMaterial = aTrack.GetMaterial();
	G4MaterialPropertiesTable* aMaterialPropertiesTable;
        G4double AttenuationLength = DBL_MAX;
        aMaterialPropertiesTable = aMaterial->GetMaterialPropertiesTable();
	G4double losscs = 0.;
        if (aMaterialPropertiesTable) { G4MaterialPropertyVector* eff1 =
		aMaterialPropertiesTable->GetProperty("LOSSCS");
		if (eff1) {losscs = eff1->Value(1);}
        } 

   if (losscs) {
    // calculate an absorption length for this cross section
    G4ThreeVector momdir = aTrack.GetMomentumDirection();
    G4double density = aMaterial->GetTotNbOfAtomsPerVolume();

    // calculate cross section for a constant loss 
    G4double crossect = losscs*barn;
  
    // attenuation length in mm
    AttenuationLength = 1./density/crossect; 
   }
   return AttenuationLength;
}
                        
UCNSimpleLoss* UCNSimpleLoss::GetInstance(){
	return theInstance;	
}

void UCNSimpleLoss::setVerbose(G4int level){
  verboselevel = level;
}

