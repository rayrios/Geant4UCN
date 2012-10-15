//////////////////////////////////////////////////////////////////////////
// 
//
//    messenger class for UCN material boindaries , 9.9.04, peter fierlinger
//
//////////////////////////////////////////////////////////////////////////

#ifndef UCNMaterialBoundaryMessenger_h
#define UCNMaterialBoundaryMessenger_h 1

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

class UCNMaterialBoundaryMessenger: public G4UImessenger
{
  public:

   UCNMaterialBoundaryMessenger(UCNMaterialBoundary* );
  ~UCNMaterialBoundaryMessenger();

   void SetNewValue(G4UIcommand* ,G4String );

  private:

   UCNMaterialBoundary* tPropagator;


   G4UIcmdWithAnInteger* setVerboseCmd;
   G4UIdirectory*     materialboundaryDir;
};

#endif

