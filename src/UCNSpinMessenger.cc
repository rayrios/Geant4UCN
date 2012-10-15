/////////////////////////////////////////////////////////////////////////////
//
//   Messenger class for UCN shutters, 9.9.04, peter fierlinger
//
/////////////////////////////////////////////////////////////////////////////


#include "UCNSpinMessenger.hh"
#include "UCNSpin.hh"
#include "G4UIdirectory.hh"
#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNSpinMessenger::UCNSpinMessenger(UCNSpin* SA)
:tPropagator (SA)
{
  
G4cout << "create messenger for the UCN spin calculation class " << G4endl;
  spinDir = new G4UIdirectory("/spin/");
  spinDir->SetGuidance("spin calculation parameters");


  sh6Cmd = new G4UIcmdWithAString("/spin/direction",this);
  sh6Cmd->SetGuidance("Set a initial spin direction");
  sh6Cmd->SetGuidance("e.g. /spin/direction random / + / - to choose a random initial spin ");
  sh6Cmd->SetParameterName("choice",true);
  sh6Cmd->SetDefaultValue("");
  sh6Cmd->AvailableForStates(G4State_Idle);
  
  sh7Cmd = new G4UIcmdWithAString("/spin/trans",this);
  sh7Cmd->SetGuidance("Set spin transport on or off (0/1)");
  sh7Cmd->SetGuidance("e.g. /spin/trans 1  - to track the angle of the spin precession ");
  sh7Cmd->SetParameterName("choice",true);
  sh7Cmd->SetDefaultValue("");
  sh7Cmd->AvailableForStates(G4State_Idle);
  
  sh8Cmd = new G4UIcmdWithAString("/spin/lowfield",this);
  sh8Cmd->SetGuidance("Set spin transport through loe field regions on or off (0/1)");
  sh8Cmd->SetGuidance("e.g. /spin/lowfield 1  - to track the spin through low fields ");
  sh8Cmd->SetParameterName("choice",true);
  sh8Cmd->SetDefaultValue("");
  sh8Cmd->AvailableForStates(G4State_Idle);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNSpinMessenger::~UCNSpinMessenger()
{
  delete sh6Cmd;
  delete sh7Cmd;
  delete sh8Cmd;  
  delete spinDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UCNSpinMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
 if( command == sh6Cmd )    {    // set init spin
  // tPropagator->SetInitSpin(newValue);
 }
 
 if( command == sh7Cmd )    {    // set spin transport
   tPropagator->SetSpinTrans(newValue);
 }
 if( command == sh8Cmd )    {    // set spin transport
   tPropagator->SetLowField(newValue);
 }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


