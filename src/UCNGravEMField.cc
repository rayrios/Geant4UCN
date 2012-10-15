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
// $Id: UCNGravEMField.cc,v 1.1 2008/09/17 08:57:21 kuzniak Exp $
// GEANT4 tag $Name:  $
//
// --------------------------------------------------------------------

#include "UCNGravEMField.hh"

UCNGravEMField::UCNGravEMField()
{
  for (G4int i=0; i<9; i++)
      fFieldComponents[i] = 0.;	
}

UCNGravEMField::UCNGravEMField(const G4ThreeVector Gfield, const G4ThreeVector Mfield, const G4ThreeVector Efield)
{
      fFieldComponents[0] = Gfield.x();
      fFieldComponents[1] = Gfield.y();
      fFieldComponents[2] = Gfield.z();

      fFieldComponents[3] = Mfield.x();
      fFieldComponents[4] = Mfield.y();
      fFieldComponents[5] = Mfield.z();

      fFieldComponents[6] = Efield.x();
      fFieldComponents[7] = Efield.y();
      fFieldComponents[8] = Efield.z();
}

UCNGravEMField::~UCNGravEMField()
{
}

UCNGravEMField::UCNGravEMField(const UCNGravEMField &r)
  : G4Field(r)
{
   for (G4int i=0; i<9; i++)
      fFieldComponents[i] = r.fFieldComponents[i];

}

UCNGravEMField& 
UCNGravEMField::operator = (const UCNGravEMField &p)
{
  if (&p == this) return *this;
  *this = p; return *this;
}
