/////////////////////////////////////////////////////////////////////////////
//
//   Messenger class for UCN shutters, 9.9.04, peter fierlinger
//
/////////////////////////////////////////////////////////////////////////////


#include "UCNSaveToFileMessenger.hh"
#include "UCNSaveToFile.hh"
#include "G4UIdirectory.hh"
#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNSaveToFileMessenger::UCNSaveToFileMessenger(UCNSaveToFile* SA)
:tPropagator (SA)
{
  
G4cout << "create messenger for the UCN savetofile class " << G4endl;
  saveToFileDir = new G4UIdirectory("/saveToFile/");
  saveToFileDir->SetGuidance("savetofile parameters");


  sh6Cmd = new G4UIcmdWithAString("/saveToFile/filename",this);
  sh6Cmd->SetGuidance("Set a data file");
  sh6Cmd->SetGuidance("e.g. /saveToFile/filename josef.dat ... saves all output there");
  sh6Cmd->SetParameterName("choice",true);
  sh6Cmd->SetDefaultValue("");
  sh6Cmd->AvailableForStates(G4State_Idle);
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNSaveToFileMessenger::~UCNSaveToFileMessenger()
{

  delete sh6Cmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UCNSaveToFileMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
 if( command == sh6Cmd )    {    // set shutteropen
   tPropagator->SetFileName(newValue);
 }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


