////////////////////////////////////////////////////////////////////////
//
//  UCNShutter, 7.9.04, peter fierlinger
//		21.4.06 marcin kuzniak
//
////////////////////////////////////////////////////////////////////////

#ifndef UCNSaveToFile_h
#define UCNSaveToFile_h 1


#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4ios.hh"
#include <iostream>
#include <fstream>
#include "G4ThreeVector.hh"
#include <cmath>
#include "iomanip"
#include "globals.hh"
#include "UCNSaveToFileMessenger.hh"

class UCNSaveToFileMessenger;


class UCNSaveToFile 
{

public: 

        UCNSaveToFile();

	~UCNSaveToFile();

	G4bool write(double a1=0, double a2=0, double a3=0, double a4=0, double a5=0,
		double a6=0, double a7=0, double a8=0, double a9=0, double a10=0, double a11=0,
		double time=0, double condition=0);
	
	void SetFileName(G4String stri);
	static UCNSaveToFile* GetMe();
	void setVerbose(G4int level);

private:
	UCNSaveToFileMessenger * theMessenger;

        static UCNSaveToFile*  theInstance;
     	G4String fieldfile;
	G4int verboselevel;
};


#endif /* UCNSaveToFile_h */
