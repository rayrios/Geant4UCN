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
// UCN detector construction, 9.9.04, peter fierlinger

#ifndef UCNDetectorConstruction_h
#define UCNDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ios.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class UCNDetectorMessenger;
class UCNFieldSetup;


class UCNDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    UCNDetectorConstruction();
   ~UCNDetectorConstruction();

  public:
     
     void SetWorldMaterial(G4String);
     void SetWorldSizeZ(G4double);
     void SetWorldSizeR(G4double);
     
     G4VPhysicalVolume* Construct();

     void UpdateGeometry();
     
  public:
  
     void PrintCalorParameters(); 
                    
     G4Material* GetWorldMaterial()    {return WorldMaterial;};
     G4double GetWorldSizeZ()          {return WorldSizeZ;}; 
     G4double GetWorldSizeR()          {return WorldSizeR;};
     
     const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;};           

  private:
     
     G4Tubs*            solidWorld;    // pointer to the solid World 
     G4LogicalVolume*   logicWorld;    // pointer to the logical World
     G4VPhysicalVolume* physiWorld;    // pointer to the physical World

     UCNFieldSetup* fEmFieldSetup;     // pointer to the field helper
     UCNDetectorMessenger* detectorMessenger;  // pointer to the Messenger
     
     G4Material*        FoilMaterial;
     G4Material*        WorldMaterial;
     G4Material*        GuideMaterial;
     G4bool             worldchanged;

     G4double           WorldSizeR;
     G4double           WorldSizeZ;
                 
  private:
    
     void DefineMaterials();
     G4VPhysicalVolume* ConstructCalorimeter();     
};

#endif
