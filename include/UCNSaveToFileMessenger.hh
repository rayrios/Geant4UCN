//////////////////////////////////////////////////////////////////////////
// 
//
//    messenger class for UCN data output, 9.9.04, peter fierlinger
//
//////////////////////////////////////////////////////////////////////////

#ifndef UCNSaveToFileMessenger_h
#define UCNSaveToFileMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class UCNSaveToFile;

///////
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class UCNSaveToFileMessenger: public G4UImessenger
{
  public:

   UCNSaveToFileMessenger(UCNSaveToFile* );
  ~UCNSaveToFileMessenger();

   void SetNewValue(G4UIcommand* ,G4String );

  private:

   UCNSaveToFile* tPropagator;


   G4UIcmdWithAString*        sh6Cmd;
 
   G4UIdirectory*     saveToFileDir;
};

#endif

