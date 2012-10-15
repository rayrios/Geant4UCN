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
////////////////////////////////////////////////////////////////////////
// 4.9.04 UCN time dependent field, peter fierlinger
// 
////////////////////////////////////////////////////////////////////////


#ifndef UCNTimedependentField_h
#define UCNTimedependentField_h 1

/////////////
// Includes
/////////////

#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4Step.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "UCNUCN.hh"
#include "UCNUniformGravField.hh"
#include "UCNGravEMField.hh"
#include "UCNFieldMessenger.hh"

/////////////////////
// Class Definition
/////////////////////

class UCNTimedependentField : public G4ElectricField
{

public: 
         UCNTimedependentField();
         UCNTimedependentField(const G4ThreeVector);
	~UCNTimedependentField();

public:

 	// routines that read the field conditions		    
	int readfield();

	inline G4double GetValue(){ return fieldstrength;}
	inline void SetValue(G4double val) { fieldstrength = val;}
	
	// messenger communication
	void SetField(G4String stri);
	void SetFile(G4String stri);
	void DrawField(G4String stri);
	void SetFieldOnTime(G4String stri);
	void SetFieldTime(G4String stri);
	void SetFieldTimeFile(G4String tru);

	// visualization of field points
	void FieldVisualization();
	void readtimedependence();
        G4double GetFieldPercentage(double time) const;
        G4ThreeVector GetGradient(const G4double& posx, const G4double& posy, const G4double& posz, G4double t) const;
	virtual void GetFieldValue(const G4double point[4], G4double *Bfield) const;
    
	// communicate with the field    
	static UCNTimedependentField* GetTimedependentField();
	void SetSpin(G4ThreeVector);
	double fieldstrength;

private:

	UCNFieldMessenger* mfMessenger;  // pointer to the Messenger
	static UCNTimedependentField*  theInstance;
	UCNUniformGravField* grav;
	G4int just_reflected;
        static G4double gtime;
        static G4ThreeVector spin;

	// field array if we use coordinates
      	double FieldValues[10000][4];
       	double TimeRamp[50][2];
       	long fieldlen;      	
	// messenger variables
      	
	int fieldon;
     	G4String fieldfile;
      	int drawfield;
      	double starttime;
      	double fieldtime;
       	G4String fieldtimefile;
     	
};

#endif /* UCNTimedependentField_h */
