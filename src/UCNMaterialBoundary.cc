
//////////////////////////////////////////////////////////////////////
//  Material Boundary Process for ultracold neutrons
//  9.9.04 peter fierlinger
////////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include "UCNMaterialBoundary.hh"
#include "UCNMaterialBoundaryMessenger.hh"
#include "UCNShutterMessenger.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4StepPoint.hh"
#include "G4TransportationManager.hh"
#include "G4ParticleDefinition.hh"

#define neV (1.0e-9*eV)

UCNMaterialBoundary* UCNMaterialBoundary::theInstance = 0;

UCNMaterialBoundary::UCNMaterialBoundary(const G4String& processName)
  : G4VContinuousDiscreteProcess(processName)
  , out("killing.dat",std::ios::out)
{
 theMessenger = new UCNMaterialBoundaryMessenger(this);
 theShutterMessenger = new UCNShutterMessenger(this);
 theInstance = this;
 just_reflected = 0;
 fermipotdiff_shutter = 1e300;
 return_it = 0;
 sh_fermipot = 0;sh_spinflip = 0;sh_loss = 0;sh_diffusion = 0;sh_reflectivity = 0;
 sh_abscs = 0;sh_losscs = 0;sh_scatcs = 0;
 return_it = 0;
 useshutters = 0;
 st=0;
}

UCNMaterialBoundary::~UCNMaterialBoundary(){
   delete theMessenger;
   delete theShutterMessenger;
} 

G4VParticleChange* UCNMaterialBoundary::PostStepDoIt(const G4Track& aTrack, 
const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);
  verboselevel=0;//6;
  verboseLevel=0;
  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
  G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();
  G4String volnam1 = pPreStepPoint->GetPhysicalVolume()->GetName();
  G4String volnam2 = pPostStepPoint->GetPhysicalVolume()->GetName();
  if (verboselevel > 5){
    G4cout << "MATERIALBOUNDARYPROCESS: vol1 " << volnam1 << ", vol2 " << volnam2 << " " << aTrack.GetKineticEnergy()/neV<< G4endl;
  }

  // make sure to be at a geometrical boundary
  if (pPostStepPoint->GetStepStatus() != fGeomBoundary) {
    just_reflected = 0;
    return G4VContinuousDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  if (verboselevel > 3){
    G4cout << "MATERIALBOUNDARYPROCESS: vol1 " << volnam1 << ", vol2 " << volnam2 <<  G4endl;
    G4cout << aTrack.GetMomentumDirection() << G4endl;
    G4cout << "pos " << pPreStepPoint->GetPosition() << " " << pPostStepPoint->GetPosition() <<G4endl;
  }
  
  G4Material* Material1 = pPreStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial();
  G4Material* Material2 = pPostStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial();

  // check, if we are at a shutter volume an if it is open or closed
  // if we use shutters,
  if (useshutters == 1){
    int nr = 0, nr2 = 0;
    sscanf(volnam2, "Shutter%d", &nr);
    sscanf(volnam1, "Shutter%d", &nr2);
    G4double t2 =  aTrack.GetGlobalTime();
    if (nr){
      for (int l = 0; l < st; l++){
	if (states[l][0] == nr){ 
	  // if the shutternr is correct, check details   
	  // G4cout << "x shutter nr " << nr << ", time " << t2*1e-9 << G4endl;
	  if (states[l][2] <= t2 && states[l+1][2] > t2){
            state = (int)states[l][1];   // time (proper time)!
	    if (verboseLevel> 3) {
	      G4cout << "ucnshutter: set the state for shutter " << nr << " to " << state << G4endl;
	    }
	    if (state == 1) {
	      // the shutter is open.
	      if (verboseLevel> 1) {
		G4cout << "enter a shutter, the shutter is open  " << G4endl;
	      }
	      /////// change the material properties and remember the values until leaving
	      G4MaterialPropertiesTable * T;
	      T = Material2->GetMaterialPropertiesTable();
	      
	      if (T) { 
		G4MaterialPropertyVector* eff1 =T->GetProperty("FERMIPOT");
		if (eff1) {sh_fermipot = eff1->Value(1);}
		G4MaterialPropertyVector* eff2 =T->GetProperty("REFLECTIVITY");
		if (eff2) {sh_reflectivity = eff2->Value(1);}
		G4MaterialPropertyVector* eff3 =T->GetProperty("DIFFUSION");
		if (eff3) {sh_diffusion = eff3->Value(1);}
		G4MaterialPropertyVector* eff4 =T->GetProperty("SPINFLIP");
		if (eff4) {sh_spinflip = eff4->Value(1);}
		G4MaterialPropertyVector* eff5 =T->GetProperty("LOSS");
		if (eff5) {sh_loss = eff5->Value(1);}
		G4MaterialPropertyVector* eff6 =T->GetProperty("LOSSCS");
		if (eff6) {sh_losscs = eff6->Value(1);}
		G4MaterialPropertyVector* eff7 =T->GetProperty("ABSCS");
		if (eff7) {sh_abscs = eff7->Value(1);}
		G4MaterialPropertyVector* eff8 =T->GetProperty("SCATCS");
		if (eff8) {sh_scatcs = eff8->Value(1);}
		T->RemoveProperty("FERMIPOT");
		T->RemoveProperty("REFLECTIVITY");
		T->RemoveProperty("DIFFUSION");
		T->RemoveProperty("SPINFLIP");
		T->RemoveProperty("LOSS");
		T->RemoveProperty("LOSSCS");
		T->RemoveProperty("ABSCS");
		T->RemoveProperty("SCATCS");
		return_it = 1;
	      }	
	    }	  	  
	    if (state == 0){
	      // shutter is closed, dont change properties
	      if (verboseLevel> 1) {
		G4cout << "the shutter is closed  " << G4endl;
	      }
	      return_it = 0;
	    }
	  }	
	}
      }
    }  
    else if (nr2){
      if (verboseLevel> 3) {
	G4cout << "UCNSHUTTER: leave a shutter volume " << G4endl;
      }
      if (return_it ==1){	
	const G4int NUM = 2;
	G4double PP[NUM] =  { 1.0, 1.0};
	G4MaterialPropertiesTable * T;
	T = Material1->GetMaterialPropertiesTable();
	G4double FE_POT[NUM] =         {sh_fermipot,sh_fermipot};      // neV
	G4double FE_SPINFLIP[NUM] =      {sh_spinflip,sh_spinflip};   // rel. per wall collision
	G4double FE_ETA[NUM] =          {sh_loss,sh_loss};        // loss coefficient W/V
	G4double FE_DIFFUS[NUM] =       {sh_diffusion,sh_diffusion};    // diffuse scattering probability
	G4double FE_REFLECTIVITY[NUM] =     {sh_reflectivity,sh_reflectivity};   // reflectivity, not used parameter
	G4double FE_ABSCS[NUM] =       {sh_abscs,sh_abscs};    // 1/v loss cross section at room temperature for Be
	G4double FE_LOSSCS[NUM] =       {sh_losscs,sh_losscs};    // loss cross section at room temperature for Be
	G4double FE_SCATCS[NUM] =       {sh_scatcs,sh_scatcs};    // (incoherent) "elastic" scattering cs
	T->AddProperty("REFLECTIVITY", PP, FE_REFLECTIVITY,      NUM);
	T->AddProperty("DIFFUSION",    PP, FE_DIFFUS,            NUM);
	T->AddProperty("FERMIPOT",     PP, FE_POT,               NUM);
	T->AddProperty("SPINFLIP",     PP, FE_SPINFLIP,          NUM);
	T->AddProperty("LOSS",         PP, FE_ETA              , NUM);
	T->AddProperty("LOSSCS",       PP, FE_LOSSCS           , NUM);
	T->AddProperty("ABSCS",        PP, FE_ABSCS            , NUM);
	T->AddProperty("SCATCS",       PP, FE_SCATCS           , NUM);
	Material1->SetMaterialPropertiesTable(T);
	
		
	fermipotdiff_shutter = sh_fermipot;
		
	sh_fermipot = 0;
	sh_spinflip = 0;
	sh_loss = 0;
	sh_diffusion = 0;
	sh_reflectivity = 0;
	sh_abscs = 0;
	sh_losscs = 0;
	sh_scatcs = 0;	
	return_it = 0;
      }
    } 
    }

  if (Material1 == Material2) return G4VContinuousDiscreteProcess::PostStepDoIt(aTrack, aStep);
  
  // get the normal to the surface
  G4ThreeVector theGlobalPoint = pPostStepPoint->GetPosition();
  G4ThreeVector thePrePoint = pPreStepPoint->GetPosition();
  
  G4Navigator* theNavigator =
    G4TransportationManager::GetTransportationManager()->
    GetNavigatorForTracking();
  
  G4ThreeVector theLocalPoint = theNavigator->
    GetGlobalToLocalTransform().
    TransformPoint(theGlobalPoint);
  
  G4ThreeVector theLocalNormal;	// Normal points back into volume
  
  G4bool valid;
  theLocalNormal = -theNavigator->GetLocalExitNormal(&valid);
  
  if (!valid) {
    G4cout << "local normal: " << theLocalNormal <<G4endl;
  } 
  
  G4ThreeVector theGlobalNormal = theNavigator->GetLocalToGlobalTransform().
    TransformAxis(theLocalNormal);
  
  G4double vel = aTrack.GetVelocity();
  G4ThreeVector momdir = aTrack.GetMomentumDirection();
  G4ThreeVector mom = aTrack.GetMomentum();
  G4double tfakt = momdir.dot(theGlobalNormal);
  G4double momnorm = mom.dot(theGlobalNormal);
  G4double velnorm = vel * tfakt;
  G4double enormal = momnorm*momnorm/2./neutron_mass_c2/neV; 
  G4double energy = aTrack.GetKineticEnergy()/neV; 
   
  G4MaterialPropertiesTable* aMaterialPropertiesTable;
  aMaterialPropertiesTable = Material2->GetMaterialPropertiesTable();

  G4double fermipot = 0.;
  G4double pdiffus = 0.;
  G4double pspinflip = 0.;
  G4double pupscatter = 0.;

  // properties in the new volume
  if (aMaterialPropertiesTable) { 
    G4MaterialPropertyVector* eff1 = aMaterialPropertiesTable->GetProperty("FERMIPOT");
    if (eff1) {fermipot = eff1->Value(1);}
  }

  if (aMaterialPropertiesTable) {
    G4MaterialPropertyVector* eff2 = aMaterialPropertiesTable->GetProperty("DIFFUSION");
    if (eff2) {pdiffus = eff2->Value(1);}
  }

  if (aMaterialPropertiesTable) {
    G4MaterialPropertyVector* eff3 = aMaterialPropertiesTable->GetProperty("SPINFLIP");
    if (eff3) {pspinflip = eff3->Value(1);}
  }
  
  if (aMaterialPropertiesTable) {
    G4MaterialPropertyVector* eff4 = aMaterialPropertiesTable->GetProperty("LOSS");
    if (eff4) { pupscatter = eff4->Value(1);}
  }

  // properties of the old volume
  G4MaterialPropertiesTable* aMaterialPropertiesTable2;
  aMaterialPropertiesTable2 = Material1->GetMaterialPropertiesTable();
  G4double fermipot_previous = 0.;
  if (aMaterialPropertiesTable2) { 
    G4MaterialPropertyVector* prev1 = aMaterialPropertiesTable2->GetProperty("FERMIPOT");
    if (prev1) {fermipot_previous = prev1->Value(1);}	
  }
  G4double pdiffus_previous = 0.;
  if (aMaterialPropertiesTable2) { 
    G4MaterialPropertyVector* prev2 = aMaterialPropertiesTable2->GetProperty("DIFFUSION");
    if (prev2) {pdiffus_previous = prev2->Value(1);}
  }
  G4double spinflip_previous = 0.;
  if (aMaterialPropertiesTable2) { 
    G4MaterialPropertyVector* prev3 = aMaterialPropertiesTable2->GetProperty("SPINFLIP");
    if (prev3) {spinflip_previous = prev3->Value(1);}
  }
  G4double loss_previous = 0.;
  if (aMaterialPropertiesTable2) { 
    G4MaterialPropertyVector* prev4 = aMaterialPropertiesTable2->GetProperty("LOSS");
    if (prev4) {loss_previous = prev4->Value(1);}
  }
  //// there is the possibility to use the inverted material properties,
  //   eg. for tubes in a vacuum, you dont have to specify inner and outer diameter,
  //   make it easier to build volumes.
  /*int inverted = 0;
  if (inverted == 1){
   G4double a1 = fermipot;
   G4double a2 = pdiffus;
   G4double a3 = pspinflip;
   G4double a4 = pupscatter;
   fermipot = fermipot_previous;
   pdiffus = pdiffus_previous;
   pspinflip = spinflip_previous;
   pupscatter = loss_previous;
   fermipot_previous = a1;
   pdiffus_previous  = a2;
   spinflip_previous = a3;
   loss_previous     = a4;
   }*/

  G4double fermipot_diff = fermipot - fermipot_previous;
  if (verboselevel > 2){ 
    G4cout << "MATERIALBOUNDARY: new fermipot " << fermipot << ", old " << fermipot_previous << G4endl;
    G4cout << "position " << thePrePoint << ", post " << theGlobalPoint << G4endl;
    G4cout << "energy " << energy << G4endl;
  }
  
  if (fermipotdiff_shutter < 1e300){
      if (verboselevel > 2){ 
	G4cout << "override by shutter " << fermipotdiff_shutter << G4endl;
      }
      fermipot_diff = fermipot - fermipot_previous + fermipotdiff_shutter;
      fermipotdiff_shutter = 1e300;
      //if(fermipot_diff==0) return G4VContinuousDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }
    

  ///// below critical velocity
    if (verboselevel > 5)G4cout << "enrm " << enormal << " " << fermipot_diff << " " << comparepot(enormal, fermipot_diff)<<G4endl;
    if (comparepot(enormal, fermipot_diff) == 0){             // reflect from surface
      if (verboselevel >1) G4cout << "MATERIALBOUNDARY: reflect " << G4endl;
      /////// losses
      if (loss(pupscatter, velnorm, fermipot_diff) == 1){             // loss on reflection
	if (verboselevel >1) G4cout << "MATERIALBOUNDARY: loss on surface " << G4endl;
	// kill it.
	aParticleChange.ProposeTrackStatus( fStopAndKill ) ;
      }
      /////// spinflips
      if (spinflip(pspinflip) == 1) {
	if (verboselevel >1) G4cout << "MATERIALBOUNDARY: spinflip " << G4endl;
	G4ThreeVector spin = aTrack.GetPolarization();
	spin *= -1;
	aParticleChange.ProposePolarization(spin);
	
      }
      ////// reflect it
      G4ThreeVector ref = reflect(0, pdiffus, momdir, theGlobalNormal);
      ref = ref.unit();
      aParticleChange.ProposeMomentumDirection(ref);
      just_reflected = 1;
    }
    
    /////// above critical velocity
    else {                                      // transmit material
      if (just_reflected == 0){
	// if it is faster than the crticial velocity, there is a probability to be still
	// reflected. this formula is (only) valid for low loss materials
	G4double refl2 = reflectivity(0, fermipot_diff,enormal);
	if (verboselevel > 5)G4cout << "jr " << just_reflected << " " << G4endl; 
	if (verboselevel >1) G4cout << "MATERIALBOUNDARY: reflectivity " << refl2 << G4endl;
	if (G4UniformRand() < refl2){ 
	  ////// reflect it
	  G4ThreeVector ref = reflect(0, pdiffus, momdir, theGlobalNormal);
	  aParticleChange.ProposeMomentumDirection(ref);
	  // set the just_reflected variable 1
	  just_reflected = 1;
	} else {
	  // --- transmission because it is faster than the critical velocity
	 
	  // --- kinetic energy in the new media
	  G4double enew = transmit(fermipot_diff, energy);
	  
	  // --- change of the normal momentum component
	  //     p = sqrt(2*m*Ekin)
	  G4double m = -sqrt(momnorm*momnorm - neutron_mass_c2*2.*fermipot_diff*neV);
	  
	  // --- momentum direction in new media
	  G4ThreeVector ref = mom - (momnorm-m)*theGlobalNormal;
	  aParticleChange.ProposeMomentumDirection(ref.unit());
	  aParticleChange.ProposeEnergy(enew*neV);

	  if (verboselevel >1){ 
	    G4cout << "energy " << energy << ", enrm " << enormal << ", fpdiff " << fermipot_diff << ", enew " << enew << G4endl;
	    G4cout << "MATERIALBOUNDARY: transmitt and set the new energy " << aParticleChange.GetEnergy()/neV << " ekin " << G4endl;
	  }
	  
	  if (verboselevel > 5)G4cout<<volnam2<<G4endl; 
	}
      }
      else if (just_reflected == 1){
	if (verboselevel >1) G4cout << "justreflected " << G4endl;
	just_reflected = 0 ;
      }
    }
    return G4VContinuousDiscreteProcess::PostStepDoIt(aTrack, aStep);
}


G4double UCNMaterialBoundary::GetMeanFreePath(const G4Track&,
 				         G4double ,
				         G4ForceCondition* condition)
{
  SetGPILSelection(CandidateForSelection);
  *condition = Forced;
  return DBL_MAX;
}

G4VParticleChange* UCNMaterialBoundary::AlongStepDoIt(
                             const G4Track& aTrack,
                             const G4Step&)
{
  aParticleChange.Initialize(aTrack);
  return &aParticleChange;
}

G4double UCNMaterialBoundary::GetContinuousStepLimit(const G4Track &  aTrack,
                                                                G4double  previousStepSize,
                                                                G4double currentMinimumStep,
                                                                G4double& )
{
  if ((previousStepSize == 0 || just_reflected==1) && aTrack.GetCurrentStepNumber()-1) {
    return .1*micrometer;
  }
  return std::min(currentMinimumStep, 10*cm);
}

int UCNMaterialBoundary::comparepot(G4double energy, G4double fermipot_diff)
{
  return (energy > fermipot_diff);    
}


int UCNMaterialBoundary::loss(G4double coefficient, G4double velnorm, G4double
fermipot){
  
  /// the surface roughness is not taken into account here, 
  // one could use e.g. ultracold neutrons, r.golub, p.35,
  // where mu is increased by roughness parameters sigma and
  // omega, which are related to the height and the distance of
  // "disturbances" on the surface 
  
  G4double v_bound = sqrt(2.*fermipot*neV/neutron_mass_c2*c_squared);
  G4double v_ratio = velnorm/v_bound;
  G4double loss_von_E = (2*coefficient*v_ratio)/(sqrt(1-(v_ratio*v_ratio)));

  return (G4UniformRand() <= fabs(loss_von_E));
}


int UCNMaterialBoundary::spinflip(G4double coefficient){
  G4double rnd_loss = G4UniformRand();
  if (rnd_loss <= coefficient){
   //G4cout << "MATERIALBOUNDARY: spinflip " << rnd_loss << G4endl;
   return 1;
  }
  else{
   return 0;
  }
}

G4double UCNMaterialBoundary::reflectivity(int, G4double fpot, G4double enormal){
  
  G4double r = (sqrt(enormal) - sqrt(enormal - fpot)) /
  (sqrt(enormal) + sqrt(enormal - fpot));
  
  return r*r;
} 
G4ThreeVector UCNMaterialBoundary::reflect(int, G4double coefficient, 
					   G4ThreeVector momdir, G4ThreeVector localnormal){
  // reflect specular
  G4double tfakt = momdir.dot(localnormal);
  G4ThreeVector reflected = momdir - 2 * localnormal * tfakt;
  if (verboselevel > 5)G4cout << "specular reflected << " << reflected << G4endl;

  // reflect diffuse 
  if (reflected==momdir || G4UniformRand() < coefficient) {
    G4ThreeVector diffus = cos_diff(localnormal);
    if (verboselevel > 5)G4cout << "diffus localnormal " << localnormal << ", " << diffus << G4endl;
    return diffus;
  }
  return reflected;
}

G4double UCNMaterialBoundary::transmit(G4double fermipot, G4double energy){
  
  return energy - fermipot;

}

G4ThreeVector UCNMaterialBoundary::cos_diff(G4ThreeVector localnormal)
{
  G4ThreeVector momentum;
  // cosine distribution - Lambert's law
  momentum.setRThetaPhi(1., acos(sqrt(G4UniformRand())), 2.*pi*G4UniformRand());
  momentum.rotateUz(localnormal);
  if(momentum*localnormal<0) {momentum*=-1;G4cout << "!" << G4endl;}
  return momentum;
}

UCNMaterialBoundary* UCNMaterialBoundary::GetInstance()
{
	return theInstance;
}

void UCNMaterialBoundary::SetFermiPotDiff(G4double state)
{
	fermipotdiff_shutter = state;
}

void UCNMaterialBoundary::setVerbose(G4int level)
{
	verboselevel = level ;
}

void UCNMaterialBoundary::SetShutterClose(G4String newval){
	G4cout << "ucnshutter: setshutterclose " << newval << ", st " << st << G4endl;
	int nr = 0;
	float ti = 0.;
        sscanf(newval, "%d %f", &nr, &ti); // shutternr time
        //G4cout <<" also " << nr << "; " << ti << G4endl;
	states[st][0] = nr;  //  0 shutternumber, 1 state, 2 time
	states[st][1] = 0.;  //  0 shutternumber, 1 state, 2 time
	states[st][2] = ti*second;  //  0 shutternumber, 1 state, 2 time
	st++;
	
}
void UCNMaterialBoundary::SetShutterOpen(G4String newval){
	G4cout << "ucnshutter: setshutteropen " << newval << ", st " << st << G4endl;
	int nr = 0;
	float ti = 0.;
        sscanf(newval, "%d %f", &nr, &ti); // shutternr time
        //G4cout <<" also " << nr << "; " << ti << G4endl;
	states[st][0] = nr;  //  0 shutternumber, 1 state, 2 time
	states[st][1] = 1.;  //  0 shutternumber, 1 state, 2 time
	states[st][2] = ti*second;  //  0 shutternumber, 1 state, 2 time
	st++;
}
void UCNMaterialBoundary::SetUseShutters(G4String newval){
if (newval == "1"){
 useshutters = 1;
}

}
void UCNMaterialBoundary::SetShutterVerbose(G4String newval){
if (newval == "1"){
 verboselevel = 1;
}

}
