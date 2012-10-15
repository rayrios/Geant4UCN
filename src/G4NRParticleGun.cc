//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// Original file name:
// $Id: G4NRParticleGun.cc,v 1.1 2008/09/17 08:56:03 kuzniak Exp $
// GEANT4 tag $Name:  $
// adapted by Stefan Heule for non-relativistic particles as ultracold neutrons
//

// G4NRParticleGun
#include "G4NRParticleGun.hh"
#include "G4PrimaryParticle.hh"
#include "G4NRParticleGunMessenger.hh"
#include "G4Event.hh"
#include "G4ios.hh"

G4NRParticleGun::G4NRParticleGun()
{
  SetInitialValues();
}

G4NRParticleGun::G4NRParticleGun(G4int numberofparticles)
{
  SetInitialValues();
  NumberOfParticlesToBeGenerated = numberofparticles;
}

G4NRParticleGun::G4NRParticleGun
    (G4ParticleDefinition * particleDef, G4int numberofparticles)
{
  SetInitialValues();
  NumberOfParticlesToBeGenerated = numberofparticles;
  SetParticleDefinition( particleDef );
}

void G4NRParticleGun::SetInitialValues()
{
  NumberOfParticlesToBeGenerated = 1;
  particle_definition = 0;
  G4ThreeVector zero;
  particle_momentum_direction = (G4ParticleMomentum)zero;
  particle_energy = 0.0;
  particle_position = zero;
  particle_time = 0.0;
  particle_polarization = zero;
  particle_charge = 0.0;
  theMessenger = new G4NRParticleGunMessenger(this);
}

G4NRParticleGun::~G4NRParticleGun()
{
  delete theMessenger;
}

void G4NRParticleGun::SetParticleDefinition
                 (G4ParticleDefinition * aParticleDefinition)
{ 
  if(!aParticleDefinition)
  {
    G4Exception("G4NRParticleGun::SetParticleDefinition()","Event00003",FatalException,
     "Null pointer is given.");
  }
  if(aParticleDefinition->IsShortLived())
  {
    if(!(aParticleDefinition->GetDecayTable()))
    {
      G4cerr << "G4NRParticleGun does not support shooting a short-lived particle without a valid decay table." << G4endl;
      G4cerr << "G4NRParticleGun::SetParticleDefinition for "
             << aParticleDefinition->GetParticleName() << " is ignored." << G4endl;
      return;
    }
  }
  particle_definition = aParticleDefinition; 
  particle_charge = particle_definition->GetPDGCharge();
}
 
void G4NRParticleGun::SetParticleMomentum(G4ParticleMomentum aMomentum)
{
  if(particle_definition==0)
  {
    G4cout <<"Particle Definition not defined yet for G4NRParticleGun"<< G4endl;
    G4cout <<"Zero Mass is assumed"<<G4endl;
    particle_momentum_direction =  aMomentum.unit();
    particle_energy = aMomentum.mag();
  } 
  else 
  {
    G4double mass =  particle_definition->GetPDGMass();
    G4double p = aMomentum.mag();
    particle_momentum_direction =  aMomentum.unit();
    if ((particle_energy>0.0)&&(std::abs(std::sqrt(2*particle_energy*mass)-p)>keV))
    {
      G4cout << "G4NRParticleGun::" << particle_definition->GetParticleName() << G4endl;
      G4cout << "  KineticEnergy and Momentum could be inconsistent" << G4endl;
      G4cout << " (Momentum:" << p/GeV << " GeV/c";
      G4cout << "  Mass:" << mass/GeV << " GeV/c/c)" << G4endl;
      G4cout << "  KineticEnergy is overwritten!! ";
      G4cout << particle_energy/GeV << "->";
      G4cout << (p*p/mass/2)/GeV << "GeV" << G4endl;
    }
    particle_energy = p*p/mass/2;
  }
}

void G4NRParticleGun::GeneratePrimaryVertex(G4Event* evt)
{
  if(particle_definition==0) return;

  // create a new vertex
  G4PrimaryVertex* vertex = 
    new G4PrimaryVertex(particle_position,particle_time);

  // create new primaries and set them to the vertex
  G4double mass =  particle_definition->GetPDGMass();
  //  G4double energy = particle_energy + mass;
  G4double pmom = std::sqrt(2*mass*particle_energy); //non-relativistic momentum
  G4double px = pmom*particle_momentum_direction.x();
  G4double py = pmom*particle_momentum_direction.y();
  G4double pz = pmom*particle_momentum_direction.z();
  for( G4int i=0; i<NumberOfParticlesToBeGenerated; i++ )
  {
    G4PrimaryParticle* particle =
      new G4PrimaryParticle(particle_definition,px,py,pz);
    particle->SetMass( mass );
    particle->SetCharge( particle_charge );
    particle->SetPolarization(particle_polarization.x(),
                               particle_polarization.y(),
                               particle_polarization.z());
    vertex->SetPrimary( particle );
  }

  evt->AddPrimaryVertex( vertex );
}


