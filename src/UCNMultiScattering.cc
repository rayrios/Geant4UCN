///////////////////////////////////////////////////////////////////////
//
//   UCN multiple scattering, 7.9.04, peter fierlinger
//				21.4.06 marcin kuzniak
//   calculate "elastic scattering" inside materials
///////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include <cmath>
#include "UCNMultiScattering.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4StepPoint.hh"
#include "Randomize.hh"
#include "G4ElementVector.hh"

G4double UCNMultiScattering::next_freepath = 1e300;

UCNMultiScattering* UCNMultiScattering::theInstance = 0;

UCNMultiScattering::UCNMultiScattering(const G4String& processName)
              : G4VContinuousDiscreteProcess(processName)
{

  verboselevel = 0;
  theInstance = this;
  
  if (verboselevel>0) {
    G4cout << GetProcessName() << " is created " << G4endl;
  }
}

UCNMultiScattering::~UCNMultiScattering(){}

G4double UCNMultiScattering::GetContinuousStepLimit(const G4Track& ,
                             G4double,
                             G4double ,
			     G4double& ){
      return next_freepath;
}

G4VParticleChange* UCNMultiScattering::AlongStepDoIt(
			     const G4Track&,
			     const G4Step& 
			    ){
  return &aParticleChange;
}



G4VParticleChange*
UCNMultiScattering::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);
  G4double t1 =  aTrack.GetProperTime();
  G4double t2 =  aTrack.GetGlobalTime();
  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
  G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();
 
  G4ThreeVector thePoint1 = pPostStepPoint->GetPosition();
  G4ThreeVector thePoint = pPreStepPoint->GetPosition();
  G4String v2 = pPostStepPoint ->GetPhysicalVolume()->GetName();

  G4double tracklen = aTrack.GetTrackLength();
  if (verboselevel > 0){
    G4cout << "MULTISCATTER " << t1*1e-9 << "s, " << t2 *1e-9<< "s. " 
       << ", tracklen " << tracklen << ", in volume " << v2 << G4endl;
       
  }
  G4ThreeVector scattered = scatter(aTrack.GetMomentumDirection());
      
  aParticleChange.ProposeMomentumDirection(-scattered);
  return G4VContinuousDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

G4double UCNMultiScattering::GetMeanFreePath(const G4Track& aTrack,
 				         G4double ,
				         G4ForceCondition*)
{
	const G4Material* aMaterial = aTrack.GetMaterial();

	G4MaterialPropertiesTable* aMaterialPropertiesTable;
	
        G4double AttenuationLength = DBL_MAX;
        aMaterialPropertiesTable = aMaterial->GetMaterialPropertiesTable();
        G4double scatcs = 0.;

        if (aMaterialPropertiesTable) { G4MaterialPropertyVector* eff1 =
		aMaterialPropertiesTable->GetProperty("SCATCS");
		if (eff1) {scatcs = eff1->Value(1);}
        } 

        if (scatcs) {
          G4double density = aMaterial->GetTotNbOfAtomsPerVolume();
          G4double crossect = scatcs*barn;
  
          // attenuation length in mm
          AttenuationLength = 1./density/crossect; 
        }

        return AttenuationLength;
}
         

G4ThreeVector UCNMultiScattering::scatter(G4ThreeVector){

	G4ThreeVector final(0.,0.,1.);
	// make a simple uniform distribution in 4 pi
	// apply scattering
	// calculate angle phi, theta
	G4double theta = acos(2*G4UniformRand()-1);
	G4double phi = G4UniformRand() * 2 * pi;
	final.rotateY(theta);
	final.rotateZ(phi);
	final = final.unit();

	return final;
}
               
UCNMultiScattering* UCNMultiScattering::GetInstance(){
	return theInstance;
}

void UCNMultiScattering::setVerbose(G4int level){
	verboselevel = level;
}




