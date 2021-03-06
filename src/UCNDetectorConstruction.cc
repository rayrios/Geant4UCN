//
//   UCN detector construction, 9.9.04, peter fierlinger 
// 

#include "UCNDetectorConstruction.hh"
#include "UCNDetectorMessenger.hh"

#include "UCNFieldSetup.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"
#include "G4ios.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4Torus.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
/////////////////////////////////////////////////////////////////////////////
//
//

UCNDetectorConstruction::UCNDetectorConstruction()
 : solidWorld(0), logicWorld(0), physiWorld(0),
   fEmFieldSetup(0),
   worldchanged(false), WorldMaterial(0)
{
  // default parameter values of the calorimeter

  WorldSizeR = 50.*cm;
  WorldSizeZ = 3000.*mm;
  
  // create commands for interactive definition of the calorimeter  

  detectorMessenger = new UCNDetectorMessenger(this);
  
  DefineMaterials();

  fEmFieldSetup = new UCNFieldSetup() ;
}

//////////////////////////////////////////////////////////////////////////
//
//

UCNDetectorConstruction::~UCNDetectorConstruction()
{ 
  delete detectorMessenger;
  delete fEmFieldSetup ;
}

//////////////////////////////////////////////////////////////////////////
//
//

G4VPhysicalVolume* UCNDetectorConstruction::Construct()
{
  return ConstructCalorimeter();
}

//////////////////////////////////////////////////////////////////////////////
//
//

void UCNDetectorConstruction::DefineMaterials()
{ 
#include "UCNDetectorMaterials.icc"
}

/////////////////////////////////////////////////////////////////////////
//
//
  
G4VPhysicalVolume* UCNDetectorConstruction::ConstructCalorimeter()
{      
  // Cleanup old geometry

  if (physiWorld)
  {
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
  }

  G4RotationMatrix* yRot90deg = new G4RotationMatrix();
  yRot90deg->rotateY(halfpi);

  // World
  
  solidWorld = new G4Tubs("World",				//its name
                   0.,WorldSizeR,WorldSizeZ/2.,0.,twopi);       //its size
                         
  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                   WorldMaterial,	//its material
                                   "World");		//its name
                                   
  physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 "World",		//its name
                                 logicWorld,		//its logical volume
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number                             
  
  G4UserLimits * theWorldUserLimits=new G4UserLimits(1*cm);
  logicWorld->SetUserLimits(theWorldUserLimits);

  // --- UCN Guide

  G4double rMin = 6*cm;
  G4double rMax = 7*cm;
  G4double hLength = 1225.5*mm; 

  G4Tubs *solidGuide = new G4Tubs("SolidGuide", rMin, rMax, hLength, 0., twopi);
  G4LogicalVolume *logicGuide = new  G4LogicalVolume(solidGuide, GuideMaterial, "GuideLV");
  G4VPhysicalVolume *physiGuide = new G4PVPlacement(0, G4ThreeVector(), "GuidePV", logicGuide, physiWorld, false, 0);

  // --- Aluminum window

  G4double foilThickness = 100*micrometer;

  G4Tubs *solidFoil = new G4Tubs("SolidFoil", 0, rMax, foilThickness/2., 0., twopi);
  G4LogicalVolume *logicFoil = new  G4LogicalVolume(solidFoil, FoilMaterial, "FoilLV");
  G4VPhysicalVolume *physiFoil = new G4PVPlacement(0, G4ThreeVector(), "FoilPV", logicFoil, physiWorld, false, 0);
  
    
  //--------- Visualization attributes -------------------------------

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4VisAttributes * greenAndWire
    = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  G4VisAttributes * cyanAndWire
    = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  G4VisAttributes * cyan2AndWire
    = new G4VisAttributes(G4Colour(0.0,0.5,1.0));
  G4VisAttributes * cyanAndSolid
    = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  G4VisAttributes * redAndSolid
    = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  G4VisAttributes * blueAndSolid
    = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes * blueAndWire
    = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes * yellowAndSolid
    = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes * yellowAndWire
    = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes * orangeAndWire
    = new G4VisAttributes(G4Colour(1.0,0.75,0.0));
  G4VisAttributes * redAndWire
    = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  G4VisAttributes * grayAndWire
    = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
  G4VisAttributes * grayAndSolid
    = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
  G4VisAttributes * whiteAndSolid
    = new G4VisAttributes(G4Colour(1.,1.,1.));

  redAndSolid->SetVisibility(true);
  blueAndSolid->SetVisibility(true);
  yellowAndSolid->SetVisibility(true);

  greenAndWire->SetVisibility(true);
  cyanAndWire->SetVisibility(true);
  orangeAndWire->SetVisibility(true);
  redAndWire->SetVisibility(true);
  grayAndWire->SetVisibility(true);

  redAndSolid->SetForceSolid(true);
  blueAndSolid->SetForceSolid(true);
  cyanAndSolid->SetForceSolid(true);
  yellowAndSolid->SetForceSolid(true);
  grayAndSolid->SetForceSolid(true);
  whiteAndSolid->SetForceSolid(true);

  yellowAndWire->SetForceWireframe(true);
  greenAndWire->SetForceWireframe(true);
  cyanAndWire->SetForceWireframe(true);
  grayAndWire->SetForceWireframe(true);
  redAndWire->SetForceWireframe(true);


  logicGuide->SetVisAttributes(grayAndSolid);
  logicFoil->SetVisAttributes(redAndSolid);

//-------------------------------Shutters--------------------------------------------------------------------


// shutter 1 
// 
      G4double s5radius = 27.999; // mm
      if (s5radius > 0){
      G4double t1innerradius =0*cm;// mm
      G4double t1thickness = 20.; //mm
      G4Tubs *solidt2 = new G4Tubs("s1",t1innerradius,rMin-.00001*cm,t1thickness/2.,0.,twopi);
      G4LogicalVolume *logict2 = new G4LogicalVolume(solidt2, GuideMaterial,"s1");
      G4double phi, x, y, z;
      phi = 0.;        x = 0.0*mm;       y = 0.0*mm;   z = -95.*cm;
      G4RotationMatrix rot; rot.rotateX(phi);
      G4VPhysicalVolume* physis2;
                        physis2 = new G4PVPlacement(
                        G4Transform3D(rot,G4ThreeVector(x,y,z)),
                        "Shutter1", logict2, physiWorld, false, 0);
      logict2->SetVisAttributes(redAndSolid);
      }

      G4double s6radius = 70.;  
      if (s6radius > 0){


      G4double t1innerradius = rMin; // mm
      G4double t1sthickness = 20.; //mm
      G4Tubs *solidt2s = new G4Tubs("s1surround",0,rMin-.00001*cm,t1sthickness/2.,0.,twopi);
      G4LogicalVolume *logict2s = new G4LogicalVolume(solidt2s, GuideMaterial,"s1surround");
      G4double phi, x, y, z;
      phi = 0.;        x = 0.0*mm;       y = 0.0*mm;   z = -95.*cm;
      G4RotationMatrix rot; rot.rotateX(phi);
      G4VPhysicalVolume* physit4;
                        physit4 = new G4PVPlacement(
                        G4Transform3D(rot,G4ThreeVector(x,y,z)),
                        "Sh_surrounding1", logict2s, physiWorld, false, 0);
      logict2s->SetVisAttributes(redAndSolid);
	
    }
  
// shutter 2 
// 
      G4double s3radius = 27.999; // mm
      if (s3radius > 0){
      G4double t1innerradius =0*cm;// mm
      G4double t1thickness = 20.; //mm
      G4Tubs *solidt2 = new G4Tubs("s2",t1innerradius,rMin-.00001*cm,t1thickness/2.,0.,twopi);
      G4LogicalVolume *logict2 = new G4LogicalVolume(solidt2, GuideMaterial,"s2");
      G4double phi, x, y, z;
      phi = 0.;        x = 0.0*mm;       y = 0.0*mm;   z = 95.*cm;
      G4RotationMatrix rot; rot.rotateX(phi);
      G4VPhysicalVolume* physis2;
                        physis2 = new G4PVPlacement(
                        G4Transform3D(rot,G4ThreeVector(x,y,z)),
                        "Shutter2", logict2, physiWorld, false, 0);
      logict2->SetVisAttributes(redAndSolid);
      }

      G4double t1sthickness1;
      G4double s4radius = 70.;  
    if (s4radius > 0){


      G4double t1innerradius = rMin; // mm
      G4double t1sthickness = 20.; //mm
      t1sthickness1=t1sthickness;
      G4Tubs *solidt2s = new G4Tubs("s2surround",0,rMin-.00001*cm,t1sthickness1/2.,0.,twopi);
      G4LogicalVolume *logict2s = new G4LogicalVolume(solidt2s, GuideMaterial,"s2surround");
      G4double phi, x, y, z;
      phi = 0.;        x = 0.0*mm;       y = 0.0*mm;   z = 95.*cm;
      G4RotationMatrix rot; rot.rotateX(phi);
      G4VPhysicalVolume* physit4;
                        physit4 = new G4PVPlacement(
                        G4Transform3D(rot,G4ThreeVector(x,y,z)),
                        "Sh_surrounding2", logict2s, physiWorld, false, 0);
      logict2s->SetVisAttributes(redAndSolid);
	
    }
//-------------------------------------------------------------------------------------------------------------------------
  return physiWorld;
}

////////////////////////////////////////////////////////////////////////////
//
//

void UCNDetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name   
  G4Material* pttoMaterial;
  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
   { pttoMaterial = (*theMaterialTable)[J];     
     if(pttoMaterial->GetName() == materialChoice)
        {
	  WorldMaterial = pttoMaterial;
          logicWorld->SetMaterial(pttoMaterial); 
        }             
   }
}

////////////////////////////////////////////////////////////////////////////
//
//

void UCNDetectorConstruction::SetWorldSizeZ(G4double val)
{
  worldchanged=true;
  WorldSizeZ = val;
}  

///////////////////////////////////////////////////////////////////////////
//
//

void UCNDetectorConstruction::SetWorldSizeR(G4double val)
{
  worldchanged=true;
  WorldSizeR = val;
}  


////////////////////////////////////////////////////////////////////////////
//
//
  
void UCNDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
}

//
//
////////////////////////////////////////////////////////////////////////////
