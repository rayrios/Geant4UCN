//////////////////////////////////////////////////////////////////////
//  Material Boundary Process for ultracold neutrons
////////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include "UCNDetector.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4StepPoint.hh"
#include "G4TransportationManager.hh"
#include "G4ParticleDefinition.hh"
#include "UCNSaveToFile.hh"

UCNDetector::UCNDetector(const G4String& processName)
              : G4VDiscreteProcess(processName)
{
	verboselevel = 0;
}
UCNDetector::~UCNDetector(){}

G4VParticleChange* UCNDetector::PostStepDoIt(const G4Track& aTrack, 
const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);

  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
  G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();
  G4String v1 = pPreStepPoint ->GetPhysicalVolume()->GetName();
  G4String v2 = pPostStepPoint ->GetPhysicalVolume()->GetName();
  // make sure to be at a geometrical boundary
  if (pPostStepPoint->GetStepStatus() != fGeomBoundary)
	return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

	
  // make sure to be at the detector surface	
  if ( v2 != "det")return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);	
  if (v1 != "detvol")return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);	
  //G4cout << "v1 " << v1 << ", v2 "<< v2 << G4endl;
  G4double t1 =  aTrack.GetProperTime() *1e-9;
  G4double t2 =  aTrack.GetGlobalTime()*1e-9;
  G4double ekin =  aTrack.GetKineticEnergy()*1e15;
  G4ThreeVector thePoint1 = pPostStepPoint->GetPosition();
  G4double tracklen = aTrack.GetTrackLength();
   
   G4cout << "----------- Hit UCN detector ----------" << G4endl;
  
  if (verboselevel > 0){
   G4cout << "" << G4endl;
   G4cout << " proper time          " << t1 << " s " << G4endl;
   G4cout << " global time          " << t2 << " s " << G4endl;
   G4cout << " track length         " << tracklen << " mm " << G4endl;
   G4cout << " kinetic energy       " << ekin << " neV"     << G4endl;
   G4cout << " position            X=" << thePoint1.getX() << ", Y=" << thePoint1.getY() << G4endl;
   G4cout << " " << G4endl;
  }
  
  // get the savetofile instance and write info to a file
       UCNSaveToFile * stf = UCNSaveToFile::GetMe();
       stf->write(1,2,3,4,5,6,7,8,9,10,11,12,13);     
  
  // kill the particle here 
   aParticleChange.ProposeTrackStatus(fStopAndKill);

  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

 }


G4double UCNDetector::GetMeanFreePath(const G4Track&,
 				         G4double ,
				         G4ForceCondition* condition)
{
  *condition = Forced;
  return DBL_MAX;
}
void UCNDetector::SetVerbose(G4String newval){
  // set a randomseed
  int seed = 0;
  sscanf(newval,"%d", &seed);
  G4cout << "detector: set verbose " << seed << G4endl;
  verboselevel = seed;
}



