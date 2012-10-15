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
// $Id: F02SteppingAction.cc,v 1.1.1.1 2004/10/25 12:36:47 kuzniak Exp $
// GEANT4 tag $Name:  $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "F02SteppingAction.hh"
#include "F02SteppingMessenger.hh"
#include "G4ios.hh"
#include <iomanip>
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "UCNTimedependentField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

F02SteppingAction::F02SteppingAction()
  : steppingMessenger(0)
{
  steppingMessenger = new F02SteppingMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

F02SteppingAction::~F02SteppingAction()
{
  delete steppingMessenger ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void F02SteppingAction::UserSteppingAction(const G4Step* theStep)
{ 

    G4Track * theTrack = theStep->GetTrack();
  // check if it is alive
  if(theTrack->GetTrackStatus()!=fAlive) { return; }
  // check if it is primary
  if(theTrack->GetParentID()!=0) { return; }

/*  G4FieldManager *fieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  UCNTimedependentField* thefield = (UCNTimedependentField *) fieldManager->GetDetectorField();

  G4ThreeVector pos = theTrack->GetPosition();

                if (pos.z() > 0 && pos.z() <= 100) {
                        G4double gz =  -0.015 ;
                        thefield->SetValue( 1.5 - gz * pos.z() );
                }
                if (pos.z() < 0 && pos.z() >= -100) {
                        G4double gz = 0.015;
                        thefield->SetValue( gz * pos.z() + 1.5 );
                }
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

