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
// $Id: UCNFieldSetup.cc,v 1.2 2006/11/13 08:57:40 kuzniak Exp $
// GEANT4 tag $Name:  $
//
//  
//   User Field class implementation.
//

#include "UCNFieldSetup.hh"
#include "UCNFieldSetupMessenger.hh"

#include "G4UniformElectricField.hh"
#include "G4UniformMagField.hh"
#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4EqMagElectricField.hh"
#include "UCNEqMagElectricField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"

#include "UCNTimedependentField.hh"


//////////////////////////////////////////////////////////////////////////
//
//  Constructors:

UCNFieldSetup::UCNFieldSetup()
  : fChordFinder(0), fStepper(0), fIntgrDriver(0)
{
 fEMfield = new UCNTimedependentField(G4ThreeVector(0.0, 4.8979e-9*6.673e-2*volt/cm, 0.0));
 fEquation = new UCNEqMagElectricField(fEMfield);
 fStepperType = 4 ;      // ClassicalRK4 is default stepper
 fFieldMessenger = new UCNFieldSetupMessenger(this) ;  
 fMinStep     = 0.000010*mm ; // minimal step of 10 microns

 fFieldManager = GetGlobalFieldManager();
 UpdateField();

}

/////////////////////////////////////////////////////////////////////////////////

UCNFieldSetup::UCNFieldSetup(G4ThreeVector pFV)
  : fChordFinder(0), 
    fStepper(0),
    fIntgrDriver(0)
{    
  fEMfield = new UCNTimedependentField(pFV);

  fFieldMessenger = new UCNFieldSetupMessenger(this) ;  
  
  fEquation = new UCNEqMagElectricField(fEMfield); 
  fMinStep     = 0.000010*mm ; // minimal step of 10 microns
  fStepperType = 4 ;        // ClassicalRK4 -- the default stepper

  fFieldManager = GetGlobalFieldManager();
  fFieldManager->GetChordFinder()->SetDeltaChord(1e-7);
  UpdateField();
}

////////////////////////////////////////////////////////////////////////////////

UCNFieldSetup::~UCNFieldSetup()
{
  if(fChordFinder) delete fChordFinder;
  if(fStepper)     delete fStepper;
  if(fEquation)    delete fEquation;   
  if(fEMfield)     delete fEMfield;
  delete fFieldMessenger;
}

/////////////////////////////////////////////////////////////////////////////
//
// Register this field to 'global' Field Manager and 
// Create Stepper and Chord Finder with predefined type, minstep (resp.)
//

void UCNFieldSetup::UpdateField()
{
  SetStepper();
 
  G4cout<<"The minimal step is equal to "<<fMinStep/mm<<" mm"<<G4endl ;

  fFieldManager->SetDetectorField(fEMfield );
  if(fChordFinder) delete fChordFinder;
  if(fIntgrDriver) delete fIntgrDriver;
  // fChordFinder = new G4ChordFinder( fEMfield, fMinStep, fStepper);
  
  fIntgrDriver = new G4MagInt_Driver(fMinStep, 
				     fStepper, 
                                     fStepper->GetNumberOfVariables() );

  fChordFinder = new G4ChordFinder(fIntgrDriver);
 
  fFieldManager->SetChordFinder( fChordFinder );
 }

/////////////////////////////////////////////////////////////////////////////
//
// Set stepper according to the stepper type
//

void UCNFieldSetup::SetStepper()
{
  G4int nvar = 12;

  if(fStepper) delete fStepper;
  
  switch ( fStepperType ) 
  {
    case 0:  
      fStepper = new G4ExplicitEuler( fEquation, nvar ); 
      G4cout<<"G4ExplicitEuler is calledS"<<G4endl;     
      break;
    case 1:  
      fStepper = new G4ImplicitEuler( fEquation, nvar );      
      G4cout<<"G4ImplicitEuler is called"<<G4endl;     
      break;
    case 2:  
      fStepper = new G4SimpleRunge( fEquation, nvar );        
      G4cout<<"G4SimpleRunge is called"<<G4endl;     
      break;
    case 3:  
      fStepper = new G4SimpleHeum( fEquation, nvar );         
      G4cout<<"G4SimpleHeum is called"<<G4endl;     
      break;
    case 4:  
      fStepper = new G4ClassicalRK4( fEquation, nvar );       
      G4cout<<"G4ClassicalRK4 (default) is called"<<G4endl;     
      break;
    case 5:  
      fStepper = new G4CashKarpRKF45( fEquation, nvar );      
      G4cout<<"G4CashKarpRKF45 is called"<<G4endl;     
      break;
    case 6:  
      fStepper = 0; // new G4RKG3_Stepper( fEquation, nvar );       
      G4cout<<"G4RKG3_Stepper is not currently working for Electric Field"<<G4endl;     
      break;
    case 7:  
      fStepper = 0; // new G4HelixExplicitEuler( fEquation ); 
      G4cout<<"G4HelixExplicitEuler is not valid for Electric Field"<<G4endl;     
      break;
    case 8:  
      fStepper = 0; // new G4HelixImplicitEuler( fEquation ); 
      G4cout<<"G4HelixImplicitEuler is not valid for Electric Field"<<G4endl;     
      break;
    case 9:  
      fStepper = 0; // new G4HelixSimpleRunge( fEquation );   
      G4cout<<"G4HelixSimpleRunge is not valid for Electric Field"<<G4endl;     
      break;
    default: fStepper = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
//
// Set the value of the Global Field to fieldValue along Z
//

void UCNFieldSetup::SetFieldValue(G4double fieldValue)
{
  G4ThreeVector fieldVector( 0.0, 0.0, fieldValue );  

  SetFieldValue( fieldVector );
}

///////////////////////////////////////////////////////////////////////////////
//
// Set the value of the Global Field value to fieldVector
//

void UCNFieldSetup::SetFieldValue(G4ThreeVector fieldVector)
{
  // Find the Field Manager for the global field
  G4FieldManager* fieldMgr= GetGlobalFieldManager();
    
   if(fieldVector != G4ThreeVector(0.,0.,0.))
  { 
     if(fEMfield) delete fEMfield;
    fEMfield = new  UCNTimedependentField();
  
    fEquation->SetFieldObj(fEMfield);  // must now point to the new field
 
    // UpdateField();
   
    fieldMgr->SetDetectorField(fEMfield);
  
   
  }
  else 
  {
    // If the new field's value is Zero, then it is best to
    //  insure that it is not used for propagation.
    if(fEMfield) delete fEMfield;
    fEMfield = 0;
    fEquation->SetFieldObj(fEMfield);   // As a double check ...

    G4MagneticField* fEMfield = 0;
    fieldMgr->SetDetectorField(fEMfield);
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//  Utility method

G4FieldManager*  UCNFieldSetup::GetGlobalFieldManager()
{
  return G4TransportationManager::GetTransportationManager()
	 ->GetFieldManager();
}
