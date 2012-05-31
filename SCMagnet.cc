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
// $Id: UCN_EX3.cc,v 1.2 2006/11/13 08:57:40 kuzniak Exp $
// GEANT4 tag $Name:  $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - TestF02 
//
// --------------------------------------------------------------
// Comments
//     
//   
// --------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "Randomize.hh"
#include "G4UItcsh.hh"

//#ifdef G4VIS_USE
#include "UCNVisManager.hh"
//#endif
#include "G4VisExecutive.hh"

#include "UCNDetectorConstruction.hh"
#include "UCNFieldSetup.hh"
#include "UCNPhysicsList.hh"
#include "UCNPrimaryGeneratorAction.hh"
#include "F02RunAction.hh"
#include "F02EventAction.hh"
#include "F02SteppingAction.hh"
#include "F02SteppingVerbose.hh"
// #include "G4UIXm.hh"
#include "UCNSaveToFile.hh"

int main(int argc,char** argv) 
{

  //choose the Random engine

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  G4long myseed = 715000034;
  CLHEP::HepRandom::setTheSeed(myseed);
  
  //my Verbose output class

  G4VSteppingVerbose::SetInstance(new F02SteppingVerbose);
  
  // Construct the default run manager

  G4RunManager * runManager = new G4RunManager;
 
  // create a class where all results from the calculation are sent to
  UCNSaveToFile * savetofile = new UCNSaveToFile;

  // Construct the helper class to manage the electric field & 
  //  the parameters for the propagation of particles in it.

  UCNFieldSetup* field = new UCNFieldSetup() ;
    
  // Set mandatory initialization classes

  UCNDetectorConstruction* detector;
  detector = new UCNDetectorConstruction;
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new UCNPhysicsList(detector));
  
#ifdef G4VIS_USE

  // visualization manager

  G4VisManager* visManager = new UCNVisManager;
  visManager->Initialize();

#endif 
 
  // Set user action classes

  runManager->SetUserAction(new UCNPrimaryGeneratorAction(detector));

  F02RunAction* runAction = new F02RunAction;

  runManager->SetUserAction(runAction);

  F02EventAction* eventAction = new F02EventAction(runAction);

  runManager->SetUserAction(eventAction);

  F02SteppingAction* steppingAction = new F02SteppingAction();
  runManager->SetUserAction(steppingAction);
  
   G4UIsession* session=0;
  
  if (argc==1)   // Define UI session for interactive mode.
    {
      // G4UIterminal is a (dumb) terminal.
#if defined(G4UI_USE_XM)
      session = new G4UIXm(argc,argv);
#elif defined(G4UI_USE_WIN32)
      session = new G4UIWin32();
#elif defined(G4UI_USE_TCSH)
      session = new G4UIterminal(new G4UItcsh);      
#else
      session = new G4UIterminal();
#endif
    }
  
  // Initialize G4 kernel, physics tables ...

  runManager->Initialize();
    
  // get the pointer to the User Interface manager 

    
  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();  

  if (session)   // Define UI session for interactive mode.
    {
      // G4UIterminal is a (dumb) terminal.
      UI->ApplyCommand("/control/execute SCMagnet.mac");    
#if defined(G4UI_USE_XM) || defined(G4UI_USE_WIN32)
      // Customize the G4UIXm,Win32 menubar with a macro file :
      //UI->ApplyCommand("/control/execute visTutor/gui.mac");
#endif
      session->SessionStart();
      delete session;
    }
  else           // Batch mode
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }
  
     
  // job termination

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete field;
  delete savetofile;
  delete runManager;

  return 0;
}

