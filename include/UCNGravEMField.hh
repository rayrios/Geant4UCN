//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: UCNGravEMField.hh,v 1.1 2008/09/17 08:57:21 kuzniak Exp $
// GEANT4 tag $Name:  $
//
//
// class UCNGravEMField
//
// Class description:
//
// A full Electromagnetic field, containing both electric and magnetic fields.
// It is an abstract class, and a derived type of this field must be
// created by the user to describe his/her field configuration.

// We have established a convention for the electromagnetic field components:
// In the GetValue method, the return values of Bfield will have 
// the following meaning
//  - Components 3, 4 and 5 are the Magnetic Field (x, y, z respectively);
//  - Components 6, 7 and 8 are the Electric field (x, y, z respectively).
// 
// Note 1: one or the other field could optional, depending on the Equation
// Note 2: such a convention is required between any field and its 
// corresponding equation of motion.
//
// History:
// - Created:  J. Apostolakis, November 12th, 1998
// - Modified: 
//   V. Grichine     8 Nov 2001: Extended "Point" arg to [4] array to add time
//   G. Cosmo        2 Apr 2003: Reorgansised, moved inline methods to .cc
//   J. Apostolakis  5 Nov 2003: Derive directly from G4Field 
//   J. Apostolakis 31 Aug 2004: Information on convention for components
// -------------------------------------------------------------------

#ifndef G4GRAVEM_FIELD_DEF
#define G4GRAVEM_FIELD_DEF

#include "G4Field.hh"
#include "G4ThreeVector.hh"

class UCNGravEMField : public G4Field
{
  public:  // with description

     UCNGravEMField();
     UCNGravEMField(const G4ThreeVector Gfield, const G4ThreeVector Mfield, const G4ThreeVector Efield);
     virtual ~UCNGravEMField();

     UCNGravEMField(const UCNGravEMField &r);
     UCNGravEMField& operator = (const UCNGravEMField &p);
       // Copy constructor & assignment operators.

     virtual void  GetFieldValue(const G4double Point[4],
                                       G4double *Field ) const = 0;
       //  Return as Field[3], [4], [5] the magnetic field x, y & z components
       //   and   as Field[6], [7], [8] the electric field x, y & z components
       //   and   as Field[0], [1], [2] the gravitational field x, y & z components

     virtual void GetEMFieldValue(const G4double Point[4],
                                       G4double *Field ) const = 0;     

     virtual G4double GetValue() = 0;
     

     G4bool   DoesFieldChangeEnergy() const { return true; }
     // Since an electric or gravitational field can change track energy

     void SetSpin(G4ThreeVector val) {fSpin = val;};

       //  For field with an electric component this should be true
       //  For pure magnetic field this should be false
       //    Alternative: default safe implementation { return true; }

     G4double fFieldComponents[9] ;
     G4ThreeVector fSpin;
};

#endif /* G4ELECTROMAGNETIC_FIELD_DEF */
