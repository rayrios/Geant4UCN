////////////////////////////////////////////////////////////////////////
//
//  UCNMultiscattering, 7.9.04, peter fierlinger
//
//
////////////////////////////////////////////////////////////////////////

#ifndef UCNMultiScattering_h
#define UCNMultiScattering_h 1


#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4Step.hh"
#include "G4VContinuousDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "UCNUCN.hh"

// simple discrete process,
// absorbs UCN due to absorption cross section of the material


class UCNMultiScattering : public G4VContinuousDiscreteProcess
{

public: // Without description

        UCNMultiScattering(const G4String& processName = "UCNMultiScattering");

	~UCNMultiScattering();

public: // With description

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        // Returns true -> 'is applicable' only for an optical photon.

	G4double GetMeanFreePath(const G4Track& aTrack,
				 G4double ,
				 G4ForceCondition* condition);

	G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
 				        const G4Step&  aStep);

	G4ThreeVector scatter(G4ThreeVector initialstate);

        G4double GetContinuousStepLimit(const G4Track& ,
                             G4double, G4double , G4double& );

	G4VParticleChange* AlongStepDoIt(const G4Track&  ,
			     const G4Step& );

	static UCNMultiScattering * GetInstance();	
	//void SetCurrentShutterState(int state);
	void setVerbose(G4int level);

private:
        static G4double next_freepath;
        //static int currentshutterstate;
        static UCNMultiScattering* theInstance;
	G4int verboselevel;
};

////////////////////
// Inline methods
////////////////////

inline
G4bool UCNMultiScattering::IsApplicable(const G4ParticleDefinition& aParticleType)
{
   return ( &aParticleType == UCNUCN::UCN0() );
}

#endif /* UCNMultiScattering_h */
