////////////////////////////////////////////////////////////////////////
//
//  UCNsimpleloss, 7.9.04, peter fierlinger
//
//
////////////////////////////////////////////////////////////////////////

#ifndef UCNSimpleLoss_h
#define UCNSimpleLoss_h 1


#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4Step.hh"
#include "G4VDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "UCNUCN.hh"

// simple discrete process,
// absorbs UCN due to absorption cross section of the material


class UCNSimpleLoss : public G4VDiscreteProcess
{

public: // Without description

        UCNSimpleLoss(const G4String& processName = "UCNSimpleLoss");

	~UCNSimpleLoss();

public: // With description

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        // Returns true -> 'is applicable' only for an optical photon.

	G4double GetMeanFreePath(const G4Track& aTrack,
				 G4double ,
				 G4ForceCondition* condition);

	G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
 				        const G4Step&  aStep);
	static UCNSimpleLoss* GetInstance();
	//void SetCurrentShutterState(int state);
	void setVerbose(G4int level);

private:
	G4double decaytime;
        static G4double kill_len;
	//static int currentshutterstate;
	static UCNSimpleLoss* theInstance;
	G4int verboselevel;
};

////////////////////
// Inline methods
////////////////////

inline
G4bool UCNSimpleLoss::IsApplicable(const G4ParticleDefinition& aParticleType)
{
   return ( &aParticleType == UCNUCN::UCN0() );
}

#endif /* UCNSimpleLoss_h */
