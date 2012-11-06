
////////////////////////////////////////////////////////////////////////
// 4.9.04 UCN material boundary process, peter fierlinger
// 
////////////////////////////////////////////////////////////////////////


#ifndef UCNMaterialBoundary_h
#define UCNMaterialBoundary_h 1

/////////////
// Includes
/////////////

#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4Step.hh"
#include "G4VContinuousDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "UCNUCN.hh"

#define UCN_SHUTTERS_MAX 10
#define UCN_SHUTTERS_STATES_MAX 20
#define UCN_SHUTTERS_DEFAULT_STATE 0
#define UCN_SHUTTERS_DEFAULT_LOOP 0

class UCNMaterialBoundaryMessenger;
class UCNShutterMessenger;

class UCNShutterStates
{
 public:
  int n;
  int loop;
  float times[UCN_SHUTTERS_STATES_MAX];
  int states[UCN_SHUTTERS_STATES_MAX];
  UCNShutterStates() : n(0), loop(UCN_SHUTTERS_DEFAULT_LOOP) {};
  void AddState(int state, float time);
  int GetState(float time);
  void SetLoop(int loop);
};

class UCNMaterialBoundary : public G4VContinuousDiscreteProcess
{

private:

public: 

         UCNMaterialBoundary(const G4String& processName = 
"UCNMaterialBoundary");
	~UCNMaterialBoundary();

public: 

        inline G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
	G4double GetContinuousStepLimit(const G4Track& aTrack,
                             G4double  previousStepSize,
                             G4double  currentMinimumStep,
			     G4double& currentSafety);

	G4double GetMeanFreePath(const G4Track& aTrack,
				 G4double ,
				 G4ForceCondition* condition);

 	G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
 				        const G4Step&  aStep);

	G4VParticleChange* AlongStepDoIt(
                             const G4Track&,
                             const G4Step&);

	int comparepot(double energy, double fermipot);
	int loss(double coefficient, double velnorm, double fermipot);
	int spinflip(double coefficient);
	G4ThreeVector reflect(int modus, double coefficient, G4ThreeVector momdir, G4ThreeVector localnormal);
	double reflectivity(int, double fpot, double enormal);

	double transmit(double fermipot, double energy);
	G4ThreeVector cos_diff(G4ThreeVector localnormal);
        
	
        static UCNMaterialBoundary* GetInstance();
	void SetFermiPotDiff(double state);
	void setVerbose(G4int level);
	void SetShutterClose(G4String newval);
	void SetShutterOpen(G4String newval);
	void SetShutterLoop(G4String newval);
        void SetUseShutters(G4String newval);
	void SetShutterVerbose(G4String newval);


private:
	UCNShutterStates shutter_states[UCN_SHUTTERS_MAX];
	UCNMaterialBoundaryMessenger * theMessenger;
	UCNShutterMessenger * theShutterMessenger;
	G4int just_reflected;
        static UCNMaterialBoundary* theInstance;
        double fermipotdiff_shutter;
	G4int verboselevel;
	int return_it; // shutter 
	int useshutters; // shutter
	double sh_fermipot ;
	double sh_spinflip ;
	double sh_loss ;
	double sh_diffusion ;
	double sh_reflectivity ;
	double sh_abscs ;
	double sh_losscs ;
	double sh_scatcs ;	
        std::ofstream out;
        G4bool atReflection, transmited, stop_gravity;
};

////////////////////
// Inline methods
////////////////////

inline
G4bool UCNMaterialBoundary::IsApplicable(const G4ParticleDefinition& 
aParticleType)
{
   return ( &aParticleType == UCNUCN::UCN0() );
}

#endif /* UCNMaterialBoundary_h */
