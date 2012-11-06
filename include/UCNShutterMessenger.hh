//////////////////////////////////////////////////////////////////////////
// 
//
//    messenger class for UCN shuter, 9.9.04, peter fierlinger
//
//////////////////////////////////////////////////////////////////////////

#ifndef UCNShutterMessenger_h
#define UCNShutterMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class UCNMaterialBoundary;

///////
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class UCNShutterMessenger: public G4UImessenger
{
  public:

   UCNShutterMessenger(UCNMaterialBoundary* );
  ~UCNShutterMessenger();

   void SetNewValue(G4UIcommand* ,G4String );

  private:

   UCNMaterialBoundary* tPropagator;


   G4UIcmdWithAString*        sh6Cmd;
   G4UIcmdWithAString*        sh7Cmd;
   G4UIcmdWithAString*        sh8Cmd;
   G4UIcmdWithAString*        sh9Cmd;
   G4UIcmdWithAString*        loopCmd;
 
   G4UIdirectory*     shutterDir;
};

#endif

