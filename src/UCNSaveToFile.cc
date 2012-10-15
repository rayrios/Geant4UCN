///////////////////////////////////////////////////////////////////////
//
//   UCN shutter, 7.9.04, peter fierlinger
//
//   shutters to fill and empty ucn storage bottles
///////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include <cmath>
#include "UCNSaveToFile.hh"
#include "Randomize.hh"
#include "UCNSaveToFileMessenger.hh"

UCNSaveToFile* UCNSaveToFile::theInstance = 0;
UCNSaveToFile::UCNSaveToFile()
{

   // create the messenger class instance
 G4cout << "Create the saveToFile class " << G4endl;
      theMessenger = new UCNSaveToFileMessenger(this);

 theInstance = this;
}

UCNSaveToFile::~UCNSaveToFile(){
	delete theMessenger;

}

UCNSaveToFile * UCNSaveToFile::GetMe(){

return theInstance;
}

G4bool UCNSaveToFile::write(double a1, double a2, double a3, double a4, double a5,
		double a6, double a7, double a8 ,double a9, double a10, double a11,
		double time, double condition){

	 
	 G4cout << "write it " << G4endl;
	 char stri[1000] = "";
	 sprintf(stri, "%f %f %f %f %f %f %f %f %f %f %f %f %f", a1,a2,a3,a4,a5,
	 	a6,a7,a8,a9,a10,a11,time,condition);

         //G4cout << "stri " << stri <<G4endl;
	 std::ofstream examplefile (fieldfile, std::ios::app);
	 if (examplefile.is_open())
	 {
	  examplefile << stri << G4endl;
	  examplefile.close();
	 }

	return true ;
}

void UCNSaveToFile::SetFileName(G4String stri)
{

	G4cout << "UCNSaveToFile: set filename for output " << stri << G4endl;
	fieldfile = stri;

}

void UCNSaveToFile::setVerbose(G4int level){
	verboselevel = level;
}


