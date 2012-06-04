///////////////////////////////////////////////////////////////////////
//
//   UCN simple absorption, 7.9.04, peter fierlinger
//				21.4.06 marcin kuzniak
//    1/v energydependent absorption cross section inside materials
///////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include <cmath>
#include "UCNSimpleAbsorption.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4StepPoint.hh"
#include "Randomize.hh"
#include "G4ElementVector.hh"
#include "G4ReactionProduct.hh"
#include "G4Nucleus.hh"
#include "G4NucleiProperties.hh"
#include "G4Element.hh"

UCNSimpleAbsorption* UCNSimpleAbsorption::theInstance = 0;

UCNSimpleAbsorption::UCNSimpleAbsorption(const G4String& processName)
              : G4VDiscreteProcess(processName)
{
	theInstance = this;
	verboseLevel=0;
	if (verboseLevel>0) {
           G4cout << GetProcessName() << " is created " << G4endl;
        }
}

UCNSimpleAbsorption::~UCNSimpleAbsorption(){}

G4VParticleChange*
UCNSimpleAbsorption::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
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
    G4cout << "SIMPLEABSORPTION " << t1/s << "s, " << t2/s << "s. " 
	   << ", tracklen " << tracklen << ", in volume " << v2 << G4endl;
  
  aParticleChange.ProposeTrackStatus(fStopAndKill);
  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  

 
}

G4double UCNSimpleAbsorption::GetMeanFreePath(const G4Track& aTrack,
 				         G4double ,
				         G4ForceCondition*)
{
  const G4Material* aMaterial = aTrack.GetMaterial();
  G4MaterialPropertiesTable* aMaterialPropertiesTable;
  G4double AttenuationLength = DBL_MAX;
  aMaterialPropertiesTable = aMaterial->GetMaterialPropertiesTable();
  G4double losscs = 0.0;

  if (aMaterialPropertiesTable) { 
    G4MaterialPropertyVector* eff1 = aMaterialPropertiesTable->GetProperty("ABSCS");
    if (eff1) losscs = eff1->Value(1);
  }
 
  G4double theA = aMaterial->GetElement(0)->GetN();
  G4double theZ = aMaterial->GetElement(0)->GetZ();


  if (losscs) {
    // calculate an absorption length for this cross section

    //thermal boost

    // prepare neutron
    //G4double eKinetic = aTrack.GetKineticEnergy();
    //G4cout << eKinetic*1e9/eV << " "; 
    /*G4ReactionProduct theNeutron( const_cast<G4ParticleDefinition *>(aTrack.GetDefinition()) );
    theNeutron.SetMomentum( aTrack.GetDynamicParticle()->Get4Momentum().vect() );
    theNeutron.SetKineticEnergy( eKinetic );
    G4ThreeVector neuVelo = (1./aTrack.GetDefinition()->GetPDGMass())*theNeutron.GetMomentum();

    // prepare properly biased thermal nucleus
    G4Nucleus aNuc;
    G4double eps = 0.0001;
    G4double eleMass;
    //eleMass = ( G4NucleiPropertiesTable::GetNuclearMass(static_cast<G4int>(theZ+eps), static_cast<G4int>(theA+eps))
               ) / G4Neutron::Neutron()->GetPDGMass();

    //G4ReactionProduct aThermalNuc = aNuc.GetBiasedThermalNucleus(eleMass, neuVelo, aMaterial->GetTemperature());

    // boost to rest system and return
    //G4ReactionProduct boosted;
    //boosted.Lorentz(theNeutron, aThermalNuc);
    //G4cout << boosted.GetKineticEnergy()*1e9/eV << G4endl;

    G4ThreeVector momdir = aTrack.GetMomentumDirection();*/
    G4double density = aMaterial->GetTotNbOfAtomsPerVolume();

    // calculate cross section for a constant loss 
    G4double vel = aTrack.GetVelocity();
    //G4double vel = sqrt(2*boosted.GetKineticEnergy()/neutron_mass_c2*c_squared);
    
    //G4cout << aTrack.GetVelocity()/meter*second << " " << vel/meter*second << G4endl;
   
    // input data is normally taken from the website:
    // http://rrdjazz.nist.gov/resources/n-lengths/list.html
    // and coresponds to 2200 m/s fast neutrons 
    G4double crossect = losscs*barn*2200.*meter/second/vel;   
    
    // in principle, if one asks for the materialproperty incoherent cross section,
    // one could put the formula for inelastic up scattering here and add the cross section
    // to the absorption
    
    //    sigma inelastic = ... ignatovic, p. 174.
    
    
    // attenuation length in mm
    AttenuationLength = 1./density/crossect;

    if (verboseLevel>0) {
      G4cout << "SIMPLEABSORPTION: attlen " << AttenuationLength << " xs " << crossect << G4endl;
    }

  }
  return AttenuationLength;
}
                        
UCNSimpleAbsorption* UCNSimpleAbsorption::GetInstance(){
	return theInstance;
}

void UCNSimpleAbsorption::setVerbose(G4int level){
  verboseLevel = level;
}

