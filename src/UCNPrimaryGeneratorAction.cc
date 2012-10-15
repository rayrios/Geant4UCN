//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: UCNPrimaryGeneratorAction.cc,v 1.1.1.1 2004/10/25 12:36:47 kuzniak Exp $
// GEANT4 tag $Name:  $
//
//   changed for UCN, 9.9.04 peter fierlinger

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "UCNPrimaryGeneratorAction.hh"

#include "UCNDetectorConstruction.hh"
#include "UCNPrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4ios.hh"
#include "UCNTimedependentField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
G4String UCNPrimaryGeneratorAction::thePrimaryParticleName = "ucn" ; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNPrimaryGeneratorAction::UCNPrimaryGeneratorAction(
                                            UCNDetectorConstruction* UCNDC)
  : UCNDetector(UCNDC), rndmFlag("off"),
    xvertex(0.),yvertex(-0.),zvertex(0.),
    vertexdefined(false)
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  // create a messenger for this class
  //
  gunMessenger = new UCNPrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="ucn");
  particleGun->SetParticleDefinition(particle);
  
  thePrimaryParticleName = particle->GetParticleName() ;

  zvertex = 0. ; //  -0.5*(F02Detector->GetAbsorberThickness());
  vis = 0;
  filltime = 0;
  particleGun->SetParticlePosition(G4ThreeVector(xvertex,yvertex,zvertex));
  
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNPrimaryGeneratorAction::~UCNPrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UCNPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // this function is called at the begining of event
  // 

  
  thePrimaryParticleName = particleGun->GetParticleDefinition()->
                                                GetParticleName() ;
  G4double x0,y0,z0 ;
  G4double px=0,py=0,pz=0 ;
  G4double n_energy = 0;
  G4double massnew = 1.674927e-27;     //neutronmasse in kg
  G4double p_length = 0;

   // generate and set the initial spin of the particle
  G4ThreeVector spin(0,0,-1);//(0.,0.,G4UniformRand()-.5);
  spin=spin.unit();

  G4FieldManager *fieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  UCNTimedependentField* thefield = (UCNTimedependentField *) fieldManager->GetDetectorField();
  thefield->SetSpin(spin);

  double time = 0.3; // s
  n_energy = G4UniformRand()*250.; // neV
  px = 0.;
  py = 0.;
  pz = 1.;
  x0 = 0.;;
  y0 = 0.;
  z0 = -1200.*mm; // slightly inside deuterium
  G4cout << "UCNPrimaryGenerator: " << n_energy << G4endl;
  
  G4ThreeVector momentum(px,py,pz);
  
  particleGun->SetParticleTime(time);
  particleGun->SetParticlePolarization(spin);
  particleGun->SetParticleMomentumDirection(momentum);
  particleGun->SetParticleEnergy(n_energy*1e-9 *eV);
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);

  
  if (vis ==1){
    visualize(G4ThreeVector(x0,y0,z0), n_energy);
  }
}

///////////////////////////////////////////////////////////////////////
//
//

G4String UCNPrimaryGeneratorAction::GetPrimaryName()
{
   return thePrimaryParticleName ;
}

void UCNPrimaryGeneratorAction::Setzvertex(G4double z)
{
  vertexdefined = true ;
  zvertex = z ;
  G4cout << " Z coordinate of the primary vertex = " << zvertex/mm <<
            " mm." << G4endl;
}
void UCNPrimaryGeneratorAction::Setxvertex(G4double x)
{
  vertexdefined = true ;
  xvertex = x ;
  G4cout << " X coordinate of the primary vertex = " << xvertex/mm <<
            " mm." << G4endl;
}

void UCNPrimaryGeneratorAction::Setyvertex(G4double y)
{
  vertexdefined = true ;
  yvertex = y ;
  G4cout << " Y coordinate of the primary vertex = " << yvertex/mm <<
            " mm." << G4endl;
}


//////////////   make the usual UCN spectrum for the typical eperiment at ILL

G4ThreeVector UCNPrimaryGeneratorAction::Spectrum(double vmax){

 double px1=6.8,px2=6.8,px3=15.;
 double n_energy = 1e10;
 double massnew = 1.674927e-27;     //neutronmasse in kg
 double p_length = 1e10;
 double max_E = vmax;
 while (p_length > max_E || p_length < 0.1){
   // uniform from -px1 to +px1
   px1 = G4UniformRand() * px1;
   double pf1 = G4UniformRand();
   if (pf1 >= 0.5){px1 = -px1;}
   px2 = G4UniformRand() * px2;
   double pf2 = G4UniformRand();
   if (pf2 >= 0.5){px2 = -px2;}
   ////// linear distribution from 0 to 15 m/s linear distributed
   px3 = -sqrt(2 * G4UniformRand() * 112.5);
    // calculate momentum length
   p_length = sqrt(px1 * px1 + px2 * px2 + px3 * px3);
   n_energy = massnew * p_length * p_length / 2 * 6.242e18 * 1e9;
 }

 G4ThreeVector momentum(px1,px2,px3);
 return momentum;
}

void UCNPrimaryGeneratorAction::SetSpec(G4String sp){
  usespectrum = 0; 
  G4cout << "primarygenerator: use a spectrum for UCN production " << sp << G4endl;
if ( sp == "1"){
  usespectrum = 1; 
}
}
void UCNPrimaryGeneratorAction::SetSeed(G4String sp){
  // set a randomseed
  float seed = 0;
  sscanf(sp,"%f", &seed);
  G4cout << "primarygenerator: set a new randomseed : " << seed << G4endl;
  if (seed) CLHEP::HepRandom::setTheSeed((long int)seed);


}
void UCNPrimaryGeneratorAction::SetVis(G4String sp){
  vis = 0;
  if (sp == "1"){ 
  vis = 1;
  G4cout << "primarygenerator: visualize vertices : " << sp << G4endl;
  }

}
void UCNPrimaryGeneratorAction::SetFillTime(G4String sp){
  // set a randomseed
  float seed = 0;
  sscanf(sp,"%f", &seed);
  G4cout << "primarygenerator: set a filltime : " << seed << G4endl;
  filltime = seed;

}
void UCNPrimaryGeneratorAction::visualize(G4ThreeVector position, double energy){
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  double diam = energy / 400. * 7.;  // max energy is about 400, diameter prop to e
  if (pVVisManager) {
       G4Circle circle(position);
       G4Colour colour(1.,0.,0.);
       circle.SetScreenDiameter (diam);
       G4VisAttributes attribs(colour);        // Define a red visualization attribute
       circle.SetVisAttributes(attribs);       // Assign the red attribute to the circle
       pVVisManager -> Draw(circle);

   }
}

