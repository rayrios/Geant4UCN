/////////////////////////////////////////////////////////////////////////////
//
//   Messenger class for UCN field
//
/////////////////////////////////////////////////////////////////////////////


#include "UCNFieldMessenger.hh"
#include "UCNTimedependentField.hh"
#include "G4UIdirectory.hh"
#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNFieldMessenger::UCNFieldMessenger(UCNTimedependentField* SA)
:tPropagator (SA)
{
  
G4cout << "create messenger for field " << G4endl;

  fieldcubeDir = new G4UIdirectory("/fieldcube/");
  fieldcubeDir->SetGuidance("inhomogeneous magnetic field");

  sh6Cmd = new G4UIcmdWithAString("/fieldcube/enabled",this);
  sh6Cmd->SetGuidance("Set fieldcube enabled true or false");
  sh6Cmd->SetGuidance("e.g. /fieldcube/enabled true");
  sh6Cmd->SetParameterName("choice",true);
  sh6Cmd->SetDefaultValue("false");
  sh6Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  

  sh9Cmd = new G4UIcmdWithAString("/fieldcube/starttime",this);
  sh9Cmd->SetGuidance("Set fieldcube enabled true or false");
  sh9Cmd->SetGuidance("e.g. /fieldcube/enabled true");
  sh9Cmd->SetParameterName("choice",true);
  sh9Cmd->SetDefaultValue("0");
  sh9Cmd->AvailableForStates(G4State_Idle);
    
  sh7Cmd = new G4UIcmdWithAString("/fieldcube/file",this);
  sh7Cmd->SetGuidance("Set fieldcube file");
  sh7Cmd->SetGuidance("e.g. /fieldcube/file magfield.dat");
  sh7Cmd->SetParameterName("choice",true);
  sh7Cmd->SetDefaultValue("magfield.dat");
  sh7Cmd->AvailableForStates(G4State_Idle);


    sh8Cmd = new G4UIcmdWithAString("/fieldcube/drawfield",this);
  sh8Cmd->SetGuidance("draw fieldcube");
  sh8Cmd->SetGuidance("/fieldcube/drawfield 1 draws some fieldgradients on the result picture");
  sh8Cmd->SetParameterName("choice",true);
  sh8Cmd->SetDefaultValue("0");
  sh8Cmd->AvailableForStates(G4State_Idle);

  ///// start ramp for magnetic field
    sh11Cmd = new G4UIcmdWithAString("/fieldcube/timedependence",this);
  sh11Cmd->SetGuidance("Set fieldcube timedependence 1 (true) or 0 (false)");
  sh11Cmd->SetGuidance("e.g. /fieldcube/timedepence 1");
  sh11Cmd->SetParameterName("choice",true);
  sh11Cmd->SetDefaultValue("0");
  sh11Cmd->AvailableForStates(G4State_Idle);
 
  ///// start ramp file for magnetic field
    sh13Cmd = new G4UIcmdWithAString("/fieldcube/timefile",this);
  sh13Cmd->SetGuidance("Set fieldcube timefile time.dat");
  sh13Cmd->SetGuidance("e.g. /fieldcube/timefile time.dat");
  sh13Cmd->SetParameterName("choice",true);
  sh13Cmd->SetDefaultValue("0");
  sh13Cmd->AvailableForStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

UCNFieldMessenger::~UCNFieldMessenger()
{

  delete sh6Cmd;
  delete sh7Cmd;  
  delete sh8Cmd;
  delete sh9Cmd;
  delete sh11Cmd;
  delete sh13Cmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UCNFieldMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
 if( command == sh6Cmd )    {    // set magnetic field on
   tPropagator->SetField(newValue);
 }
 if( command == sh7Cmd )    {    // set magnetic field on
   tPropagator->SetFile(newValue);
 }
 if( command == sh8Cmd )    {    // set magnetic field on
   tPropagator->DrawField(newValue);
 }
 if( command == sh9Cmd )    {    // set magnetic field on
   tPropagator->SetFieldOnTime(newValue);
 }
 if( command == sh11Cmd )    {    // set magnetic field on
   tPropagator->SetFieldTime(newValue);
 }
 if( command == sh13Cmd )    {    // set magnetic field on
   tPropagator->SetFieldTimeFile(newValue);
 }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


