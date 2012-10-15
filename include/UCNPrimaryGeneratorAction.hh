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
// $Id: UCNPrimaryGeneratorAction.hh,v 1.1.1.1 2004/10/25 12:36:46 kuzniak Exp $
// GEANT4 tag $Name:  $
//
//   changed for UCN 9.9.04 peter fierlinger

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef UCNPrimaryGeneratorAction_h
#define UCNPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4Event;
class UCNDetectorConstruction;
class UCNPrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class UCNPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    UCNPrimaryGeneratorAction(UCNDetectorConstruction*);    
   ~UCNPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);
    void SetRndmFlag(G4String val) { rndmFlag = val;}
    void Setxvertex(G4double x) ;
    void Setyvertex(G4double y) ;
    void Setzvertex(G4double z) ;
    void SetSpec(G4String sp) ;
    void SetSeed(G4String sp) ;
    void SetVis(G4String sp) ;
    G4ThreeVector Spectrum(double vmax);
    
    static G4String GetPrimaryName() ;                
    
    void visualize(G4ThreeVector position, double energy); // visualize
    void SetFillTime(G4String sp);


    
  private:
    G4ParticleGun*                particleGun;	// pointer a to G4 service class
    UCNDetectorConstruction*      UCNDetector;  // pointer to the geometry
    
    UCNPrimaryGeneratorMessenger* gunMessenger; // messenger of this class
    G4String                      rndmFlag;	// flag for a random impact point       

    static G4String thePrimaryParticleName ;
    G4int usespectrum;
    G4int vis;
    G4double xvertex,yvertex,zvertex;
    G4bool vertexdefined ;
    G4double filltime;
};

#endif
