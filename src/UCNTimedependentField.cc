//////////////////////////////////////////////////////////////////////
//  time dependent field for UCN
////////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include "UCNTimedependentField.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4StepPoint.hh"
#include "G4TransportationManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include <iostream>
#include <fstream>
#include "G4ThreeVector.hh"
#include <cmath>
#include "iomanip"
#include "globals.hh"
#include "G4StepPoint.hh"
#include "G4Navigator.hh"
#include "G4PropagatorInField.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"

G4double UCNTimedependentField::gtime = 0;
UCNTimedependentField* UCNTimedependentField::theInstance = 0;
G4ThreeVector UCNTimedependentField::spin;

UCNTimedependentField::UCNTimedependentField()
{
  G4cout << "construct timedependent field " << this << G4endl;
  // create the messenger class
  mfMessenger = new UCNFieldMessenger(this);
  
  fieldon = 0;
  G4String fieldfile = "";
  drawfield = 0;
  starttime = 0;
  fieldtime = 0;
  fieldtimefile = "";
  theInstance = this;  
}

UCNTimedependentField::~UCNTimedependentField()
{
  delete mfMessenger;
}

/// these routines manage pointers for the communication 
/// between the field routines
UCNTimedependentField* UCNTimedependentField::GetTimedependentField(){
	return theInstance;
}

// ------------------------------------------------------------------------
UCNTimedependentField::UCNTimedependentField(const G4ThreeVector FieldVector)
{
    grav = new UCNUniformGravField(FieldVector);
    mfMessenger = new UCNFieldMessenger(this);
    theInstance = this;
}
// ------------------------------------------------------------------------
 void UCNTimedependentField::SetSpin(G4ThreeVector stri)
{
	G4cout << "timedependentfield: setspin " << stri << G4endl;
	spin = stri;
}
// ------------------------------------------------------------------------
void UCNTimedependentField::GetFieldValue (const G4double point[4],
                                            G4double *G) const 
{
  
    G4double fFieldComponents[6];
    
//    if(! timedep == 0){

      G4ThreeVector newgrad = GetGradient(point[0],point[1],point[2],point[3]*1e-9);
      G4double percentage = GetFieldPercentage(point[3]*1e-9);
      grav->GetFieldValue(point, fFieldComponents);
    
//      G4cerr << newgrad.x()/tesla*m << spin << percentage << " " << fFieldComponents[0] << " " << fFieldComponents[1] << " " << fFieldComponents[2] << G4endl;
          
      // force scalingfactor
      G4double coef1 = 1.915e-14;
      
      newgrad *= coef1*percentage;
      
      // add the magneticfield to the fieldcoordinates 
      G[0] = fFieldComponents[0] - newgrad.getX();
      G[1] = fFieldComponents[1] - newgrad.getY();
      G[2] = fFieldComponents[2] - newgrad.getZ();
      //if(newgrad.getZ() != 0)
      //G4cerr << "calc fieldpoint " << G[0] << ", " << G[1] << ", " << G[2] << G4endl;
//    }
/*    else{
      G[0]= fFieldComponents[0] ;
      G[1]= fFieldComponents[1] ;
      G[2]= fFieldComponents[2] ;
      }*/
}

 
 /// these routines are used for the messenger class communication
 /////////////////////////////////////////////////////////////////////////

 
 void UCNTimedependentField::SetField(G4String stri)
{
fieldon = 0;
if (stri != "0") fieldon = 1;
G4cout << "timedependentfield: inhomogeneous magnetic field " << fieldon << G4endl;

}
void UCNTimedependentField::SetFile(G4String stri)
{
fieldfile = stri;
G4cout << "timedependentfield: set values for the fieldcube file = " << stri << G4endl;

}


 void UCNTimedependentField::DrawField(G4String stri){
G4cout << "timedependentfield: set visualization of magfield " << stri << G4endl;
drawfield = 0;
if (stri == "1") drawfield = 1;
else {
 drawfield = 0;
}
}

void UCNTimedependentField::SetFieldOnTime(G4String stri){
G4cout << "timedependentfield: setFieldOnTime " << stri << G4endl;
float txt = 0;
sscanf(stri, "%f", &txt);
starttime = txt;

}

void UCNTimedependentField::SetFieldTime(G4String tru){
G4cout << "timedependentfield: setFieldTimedependence " << tru << G4endl;
int txt = 0;
sscanf(tru, "%d", &txt);
fieldtime = txt;
}

void UCNTimedependentField::SetFieldTimeFile(G4String tru){
G4cout << "timedependentfield: setFieldTimeFile " << tru << G4endl;
fieldtimefile = tru;
  if(fieldtime == 1){
   G4cout << "read the timedependence " << G4endl;
   readtimedependence();
  }
}

// draw the field points
void UCNTimedependentField::FieldVisualization(){


}

void UCNTimedependentField::readtimedependence(){
G4String file;
char stri[1000];
float txt = 0;
float p1 = 0;
int k = 0;
if (fieldtime == 1){
  //G4cout << "read time file " << G4endl;
  file = fieldtimefile;
  std::ifstream examplefile2 (file, std::ios::in);
  if (! examplefile2.is_open())
   { G4cout << "Error opening file";}
  while (! examplefile2.eof())
  {
   examplefile2.getline (stri,100);
   sscanf(stri, "%f %f", &txt, &p1);
   TimeRamp[k][0] = txt;
   TimeRamp[k][1] = p1;
  G4cout << "time behaviour " << TimeRamp[k][0] << ", " 
  << TimeRamp[k][1] << G4endl;
   k++;
  }
  //examplefile2.close;
 }

}

G4double UCNTimedependentField::GetFieldPercentage(double time) const
{
	double t[30];
	double p[30];
	int inside =0;
	int k = 0;
	if (time >= starttime){
  		for (int j = 0; j <30; j++){
    			t[j] = TimeRamp[j][0];
    			p[j] = TimeRamp[j][1];
  		}
  		inside = 1;
	}

	float probability = 1;
	double tim = 0;
	if (inside > 0){
 	k = 0;
 	if (inside == 1) tim -= starttime;

 	while (k<30) {
		if (tim < t[k]){
	   		probability = p[k-1] +  (p[k]-p[k-1])/(t[k]-t[k-1])*(tim-t[k-1]);
           		return probability;
         	}
  		k++;
	}
	}
	return probability;
}

G4ThreeVector UCNTimedependentField::GetGradient(const G4double& x, const G4double& y, const G4double& z, G4double t) const
{

	G4double polarity = 0.;
	//if (fieldon == 0) return 0;
	if (spin.getZ() > 0.){         // force repulsive
		polarity = 1.;
	} else {                      // force attractive
		polarity = -1.;
	}

	int simplified = 1;
	if (simplified == 1){
 		G4ThreeVector simple_grad;

	G4double a = 4.9999*tesla;
 	G4double b = 0.0001*tesla;
 	G4double r2 = x*x/m/m + y/m*y/m;
 	G4double denominator = sqrt(pow(b+a*exp(-9*z*z/m/m),2)+162.*a*a*exp(-18.*z*z/m/m)*r2*z*z/m/m);

 	//G4double Bz = a*exp(-9.*z*z/m/m) + b;
 	//G4double Bxy =  9.*a*exp(-9.*z*z/m/m)*z/m*sqrt(r2);
 	//fieldstrength = sqrt(Bz*Bz + 2.*Bxy*Bxy);

 	G4double gr = 81.*a*a*exp(-18.*z*z/m/m)*sqrt(r2)*z*z/m/m/denominator;
 	G4double gx = gr*cos(atan2(y,x));
 	G4double gy = gr*sin(atan2(y,x));
 	G4double gz = a*z/m*
        	(-18.*b*exp(-9.*z*z/m/m) + a*exp(-18.*z*z/m/m)*(-18. + r2*(81. - 1458.*z*z/m/m)))/denominator;

 		simple_grad.setX(gx);
 		simple_grad.setY(gy);
 		simple_grad.setZ(gz);

 		return simple_grad * polarity;
	}
}


int UCNTimedependentField::readfield(){
  int k = 0;
  char stri[1000];
  float px = 0.0;
  float py = 0.0;
  float pz = 0.0;
  float B = 0.0;
  std::ifstream file2 (fieldfile, std::ios::in);
  if (! file2.is_open())
    { G4cout << "Error opening fieldfile";}

  while (! file2.eof())
  {
   file2.getline (stri,40);
   if (strcmp(stri, "00") == 0) {
     break;}
   sscanf(stri, "%f %f %f %f", &px, &py, &pz, &B);

   px = px * 1000;
   py = py * 1000;
   pz = pz * 1000;

   FieldValues[k][0] = 0;
   FieldValues[k][1] = 0;
   FieldValues[k][2] = 0;
   FieldValues[k][3] = 0;

   if (px) FieldValues[k][0] = (double)px;
   if (py) FieldValues[k][1] = (double)py;
   if (pz) FieldValues[k][2] = (double)pz;
   if (B)  FieldValues[k][3] = (double)B;

   k++;
 } 
  fieldlen = k;
return 0;
}


