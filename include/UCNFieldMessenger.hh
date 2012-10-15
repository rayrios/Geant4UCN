//////////////////////////////////////////////////////////////////////////
// 
//
//    field messenger class for UCN
//
//////////////////////////////////////////////////////////////////////////

#ifndef UCNFieldMessenger_h
#define UCNFieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"


class UCNTimedependentField;

///////
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class UCNFieldMessenger: public G4UImessenger
{
  public:

   UCNFieldMessenger(UCNTimedependentField* );
  ~UCNFieldMessenger();

   void SetNewValue(G4UIcommand* ,G4String );

  private:

   UCNTimedependentField* tPropagator;


   G4UIcmdWithAString*        sh6Cmd;
   G4UIcmdWithAString*        sh7Cmd;   
   G4UIcmdWithAString*        sh8Cmd;
   G4UIcmdWithAString*        sh9Cmd;

  G4UIcmdWithAString*        sh11Cmd;
  G4UIcmdWithAString*        sh13Cmd;


   G4UIdirectory*     fieldcubeDir;
};

#endif

