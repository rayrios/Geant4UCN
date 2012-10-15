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
// $Id: UCNEqGravEMField.cc,v 1.1 2008/09/17 08:57:21 kuzniak Exp $
// GEANT4 tag $Name:  $
//
//
//  This is the standard right-hand side for equation of motion.
//
//  The only case another is required is when using a moving reference
//  frame ... or extending the class to include additional Forces,
//  eg an electric field
//
//  10.11.98   V.Grichine
//
// -------------------------------------------------------------------
//
//  4.9.04  adapted for gravity by peter fierlinger  
//

#include "UCNEqGravEMField.hh"
#include "globals.hh"

void  
UCNEqGravEMField::SetChargeMomentumMass(G4double particleCharge, // e+ units
		                            G4double,
                                            G4double particleMass)
{
  fMass = particleMass;
  fCharge = particleCharge;
  fCof_val = particleCharge*eplus*c_light;
}



void
UCNEqGravEMField::EvaluateRhsGivenB(const G4double y[],
			                const G4double G[],
				              G4double dydx[] ) const
{

  
////////////////////////////////////////////////////////////////////
//   a neutron in a
//   gravitational field


if (fCharge == 0 && fMass > 0){
  G4double momentum_mag_square = sqr(y[3]) + sqr(y[4]) + sqr(y[5]);
  G4double inv_momentum_magnitude = 1.0 / sqrt( momentum_mag_square );

  G4double Energy = sqrt(momentum_mag_square + fMass*fMass);
  G4double cof2 = Energy/c_light;
  G4double cof1 = inv_momentum_magnitude*fMass;
  G4double inverse_velocity = Energy*inv_momentum_magnitude/c_light;

  dydx[0] = y[3]*inv_momentum_magnitude;       //  (d/ds)x = Vx/V
  dydx[1] = y[4]*inv_momentum_magnitude;       //  (d/ds)y = Vy/V
  dydx[2] = y[5]*inv_momentum_magnitude;       //  (d/ds)z = Vz/V

  ///// here changed for a field like gravity in all directions
  dydx[3] = -G[0]*cof1*cof2;
  dydx[4] = -G[1]*cof1*cof2;
  dydx[5] = -G[2]*cof1*cof2;   	//  m*g

  // Lab Time of flight
  dydx[7] = inverse_velocity;
  }
  return;
}
