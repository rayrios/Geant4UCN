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
// $Id: UCNPrimaryGeneratorMessenger.cc,v 1.1.1.1 2004/10/25 12:36:47 kuzniak Exp $
// GEANT4 tag $Name:  $
//
//   changed for UCN 9.9.04 peter fierlinger

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "UCNPrimaryGeneratorMessenger.hh"

#include "UCNPrimaryGeneratorAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNPrimaryGeneratorMessenger::UCNPrimaryGeneratorMessenger(UCNPrimaryGeneratorAction* UCNGun)
  : UCNAction(UCNGun)
{ 
  RndmCmd = new G4UIcmdWithAString("/gun/random",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance("  Choice : on, off(default)");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("off");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  setxvertexCmd = new G4UIcmdWithADoubleAndUnit("/gun/xvertex",this);
  setxvertexCmd->SetGuidance(" Set x coord. of the primary vertex.");
  setxvertexCmd->SetParameterName("xv",true);
  setxvertexCmd->SetDefaultValue(0.0*mm) ; 
  
  setyvertexCmd = new G4UIcmdWithADoubleAndUnit("/gun/yvertex",this);
  setyvertexCmd->SetGuidance(" Set y coord. of the primary vertex.");
  setyvertexCmd->SetParameterName("yv",true);
  setyvertexCmd->SetDefaultValue(0.0*mm) ; 
  
  setzvertexCmd = new G4UIcmdWithADoubleAndUnit("/gun/zvertex",this);
  setzvertexCmd->SetGuidance(" Set z coord. of the primary vertex.");
  setzvertexCmd->SetParameterName("zv",true);
  setzvertexCmd->SetDefaultValue(0.0*mm) ; 
  
  setspecCmd = new G4UIcmdWithAString("/gun/usespectrum",this);
  setspecCmd->SetGuidance(" set the spectrum on or off (0/1)");
  setspecCmd->SetParameterName("choice",true);
  setspecCmd->SetDefaultValue("0") ; 
  setspecCmd->AvailableForStates(G4State_Idle);

  setseedCmd = new G4UIcmdWithAString("/gun/randomseed",this);
  setseedCmd->SetGuidance(" Set randomseed 3598487837");
  setseedCmd->SetParameterName("choice",true);
  setseedCmd->SetDefaultValue("0") ; 
  setseedCmd->AvailableForStates(G4State_Idle);

  setvisCmd = new G4UIcmdWithAString("/gun/visual",this);
  setvisCmd->SetGuidance(" Set visualisation 1 (0/1)");
  setvisCmd->SetParameterName("choice",true);
  setvisCmd->SetDefaultValue("0") ; 
  setvisCmd->AvailableForStates(G4State_Idle);

  setfillCmd = new G4UIcmdWithAString("/gun/filltime",this);
  setfillCmd->SetGuidance(" Set filltime 5.5... produces ucn for 5.5 seconds");
  setfillCmd->SetParameterName("choice",true);
  setfillCmd->SetDefaultValue("0") ; 
  setfillCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNPrimaryGeneratorMessenger::~UCNPrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete setxvertexCmd;
  delete setyvertexCmd;
  delete setzvertexCmd;
  delete setspecCmd;
  delete setseedCmd;
  delete setvisCmd;
  delete setfillCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UCNPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{ 
  if( command == RndmCmd )
   { UCNAction->SetRndmFlag(newValue);}
  if( command == setxvertexCmd)
   { UCNAction->Setxvertex(setxvertexCmd->GetNewDoubleValue(newValue));}
  if( command == setyvertexCmd)
   { UCNAction->Setyvertex(setyvertexCmd->GetNewDoubleValue(newValue));}
  if( command == setzvertexCmd)
   { UCNAction->Setzvertex(setzvertexCmd->GetNewDoubleValue(newValue));}
  if( command == setspecCmd)
   { UCNAction->SetSpec(newValue);}
  if( command == setseedCmd)
   { UCNAction->SetSeed(newValue);}
  if( command == setvisCmd)
   { UCNAction->SetVis(newValue);}
  if( command == setfillCmd)
   { UCNAction->SetFillTime(newValue);}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

