
////////////////////////////////////////////////////////////////////////
// 4.9.04 UCN detector process, peter fierlinger
// 
////////////////////////////////////////////////////////////////////////


#ifndef UCNDetector_h
#define UCNDetector_h 1

/////////////
// Includes
/////////////

#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4Step.hh"
#include "G4VDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "UCNUCN.hh"

class UCNDetector;

class UCNDetector : public G4VDiscreteProcess
{

private:

public: 

         UCNDetector(const G4String& processName = 
"UCNDetector");
	~UCNDetector();

public: 

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);

	G4double GetMeanFreePath(const G4Track& aTrack,
				 G4double ,
				 G4ForceCondition* condition);
 	G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
 				        const G4Step&  aStep);

        void SetVerbose(G4String newval);
					
private:

	int verboselevel;
};

////////////////////
// Inline methods
////////////////////

inline
G4bool UCNDetector::IsApplicable(const G4ParticleDefinition& 
aParticleType)
{
   return ( &aParticleType == UCNUCN::UCN0() );
}

#endif /* UCNDetector_h */
