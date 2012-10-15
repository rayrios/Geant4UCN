////////////////////////////////////////////////////////////////////////
//
//  UCNMultiscattering, 7.9.04, peter fierlinger
//
//
////////////////////////////////////////////////////////////////////////

#ifndef UCNSpin_h
#define UCNSpin_h 1


#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4Step.hh"
#include "G4VContinuousDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "UCNUCN.hh"
#include "UCNSpinMessenger.hh"

class UCNSpinMessenger;

// calculation of the UCN spin

class UCNSpin : public G4VContinuousDiscreteProcess
{

public:

        UCNSpin(const G4String& processName = "UCNSpin");

	~UCNSpin();

public: // With description

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
   
	G4double GetMeanFreePath(const G4Track& aTrack,
				 G4double ,
				 G4ForceCondition* condition);

	G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
 				        const G4Step&  aStep);

        G4double GetContinuousStepLimit(const G4Track& ,
                             G4double, G4double , G4double& );

	G4VParticleChange* AlongStepDoIt(const G4Track&  ,
			     const G4Step& );

	void SetSpinTrans(G4String valu);
	void SetLowField(G4String valu);

	//static void SetInitSpin(G4String valu);
	static UCNSpin* GetInstance();
	//static G4ThreeVector GetSpin();
private:
	UCNSpinMessenger * theMessenger;
	static UCNSpin* theInstance;
	//static G4ThreeVector spin;
	int spintrue;
	int lowfield;
	
	// spin angle atracking:
	G4double totalAngle;
 };

////////////////////
// Inline methods
////////////////////

inline
G4bool UCNSpin::IsApplicable(const G4ParticleDefinition& aParticleType)
{
   return ( &aParticleType == UCNUCN::UCN0() );
}

#endif /* UCNSpin_h */
