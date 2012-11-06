/////////////////////////////////////////////////////////////////////////////
//
//   Messenger class for UCN shutters, 9.9.04, peter fierlinger
//
/////////////////////////////////////////////////////////////////////////////


#include "UCNShutterMessenger.hh"
#include "UCNMaterialBoundary.hh"
#include "G4UIdirectory.hh"
#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNShutterMessenger::UCNShutterMessenger(UCNMaterialBoundary* SA)
:tPropagator (SA)
{
  
G4cout << "create messenger for the UCN shutters " << G4endl;
  shutterDir = new G4UIdirectory("/shutter/");
  shutterDir->SetGuidance("shutter parameters");


  sh6Cmd = new G4UIcmdWithAString("/shutter/open",this);
  sh6Cmd->SetGuidance("Set shutter open number time");
  sh6Cmd->SetGuidance("e.g. /shutter/open 1 4 ... opens shutter 1 at time 4 s");
  sh6Cmd->SetParameterName("choice",true);
  sh6Cmd->SetDefaultValue("");
  sh6Cmd->AvailableForStates(G4State_Idle);
  
  

  sh9Cmd = new G4UIcmdWithAString("/shutter/close",this);
  sh9Cmd->SetGuidance("Set shutter close number time");
  sh9Cmd->SetGuidance("e.g. /shutter/close 2 1 closes shutter 2 at time 1");
  sh9Cmd->SetParameterName("choice",true);
  sh9Cmd->SetDefaultValue("");
  sh9Cmd->AvailableForStates(G4State_Idle);

  sh7Cmd = new G4UIcmdWithAString("/shutter/use",this);
  sh7Cmd->SetGuidance("Choose, if shutters should be used");
  sh7Cmd->SetGuidance("e.g. /shutter/use 1");
  sh7Cmd->SetParameterName("choice",true);
  sh7Cmd->SetDefaultValue("");
  sh7Cmd->AvailableForStates(G4State_Idle);

  loopCmd = new G4UIcmdWithAString("/shutter/loop",this);
  loopCmd->SetGuidance("Choose, if shutters should be used");
  loopCmd->SetGuidance("e.g. /shutter/loop nr (0|1)");
  loopCmd->SetParameterName("choice",true);
  loopCmd->SetDefaultValue("");
  loopCmd->AvailableForStates(G4State_Idle);

  sh8Cmd = new G4UIcmdWithAString("/shutter/setverbose",this);
  sh8Cmd->SetGuidance("set shutter verbose level");
  sh8Cmd->SetGuidance("e.g. /shutter/verbose 3");
  sh8Cmd->SetParameterName("choice",true);
  sh8Cmd->SetDefaultValue("");
  sh8Cmd->AvailableForStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNShutterMessenger::~UCNShutterMessenger()
{

  delete sh6Cmd;
  delete sh7Cmd;
  delete sh8Cmd;
  delete sh9Cmd;
  delete shutterDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UCNShutterMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
 if( command == sh6Cmd )    {    // set shutteropen
   tPropagator->SetShutterOpen(newValue);
 }
 if( command == sh9Cmd )    {    // set shutterclose
   tPropagator->SetShutterClose(newValue);
 }
 if( command == sh7Cmd )    {    // set shutterclose
   tPropagator->SetUseShutters(newValue);
 }
 if( command == sh8Cmd )    {    // set shutterclose
   tPropagator->SetShutterVerbose(newValue);
 }
 if (command == loopCmd) {
   // newValue "%d %d" as shutter number and bool
   tPropagator->SetShutterLoop(newValue);
 }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


