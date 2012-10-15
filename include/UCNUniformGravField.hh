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
// $Id: UCNUniformGravField.hh,v 1.2 2008/09/17 09:18:42 kuzniak Exp $
// GEANT4 tag $Name:  $
//
// 
// class UCNUniformGravField
//
// Class description:
//
// Class for creation of Uniform electric Magnetic Field.

// History:
// - 30.01.97 V.Grichine, Created.
// -------------------------------------------------------------------
//
//  4.9.04 revised for UCN by peter fierlinger
//


#ifndef UCNUNIFORMGRAVFIELD_HH
#define UCNUNIFORMGRAVFIELD_HH

#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4ElectricField.hh"

class UCNUniformGravField : public G4ElectricField
{
  public:  // with description

    UCNUniformGravField(const G4ThreeVector FieldVector );
      // A field with value equal to FieldVector.

    UCNUniformGravField(G4double vField,
                           G4double vTheta,
                           G4double vPhi     ) ;
       
    virtual ~UCNUniformGravField() ;

    UCNUniformGravField(const UCNUniformGravField &p);
    UCNUniformGravField& operator = (const UCNUniformGravField &p);
      // Copy constructor and assignment operator
    
      
    virtual void GetFieldValue(const G4double pos[4], G4double *field) const;
    //virtual void SetFieldValue(G4ThreeVector fieldval);
    static UCNUniformGravField* GetUniformGravField();

  private:
  
    G4double fFieldComponents[6] ;
     
    static G4int index;
    static UCNUniformGravField *theInstance;
};

#endif
