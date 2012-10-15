//////////////////////////////////////////////////////////////////////////
// 
//
//    messenger class for UCN data output, 9.9.04, peter fierlinger
//
//////////////////////////////////////////////////////////////////////////

#ifndef UCNSpinMessenger_h
#define UCNSpinMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class UCNSpin;

///////
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class UCNSpinMessenger: public G4UImessenger
{
  public:

   UCNSpinMessenger(UCNSpin* );
  ~UCNSpinMessenger();

   void SetNewValue(G4UIcommand* ,G4String );

  private:


   UCNSpin* tPropagator;


   G4UIcmdWithAString*        sh6Cmd;
   G4UIcmdWithAString*        sh7Cmd;
   G4UIcmdWithAString*        sh8Cmd;
 
   G4UIdirectory*     spinDir;
};

#endif

