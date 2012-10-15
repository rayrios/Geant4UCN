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
// $Id: UCNUCN0.cc,v 1.3 2008/09/17 09:04:14 kuzniak Exp $
// GEANT4 tag $Name:  $
//
// 
// ----------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      History: first implementation, based on object model of
//      4th April 1996, G.Cosmo
//                          H.Kurashige 7 July 1996
//      add neutron life time    Oct 17 2000 
// **********************************************************************
//  New impelemenataion as an utility class  M.Asai, 26 July 2004
// ----------------------------------------------------------------------

#include "UCNUCN.hh"
#include "G4ParticleTable.hh"

//#include "UCNUCNBetaDecayChannel.hh"
#include "G4DecayTable.hh"

// ######################################################################
// ###                           NEUTRON                              ###
// ######################################################################
UCNUCN* UCNUCN::theInstance = 0;

UCNUCN* UCNUCN::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "ucn";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4Ions* anInstance =  reinterpret_cast<G4Ions*>(pTable->FindParticle(name));
  if (anInstance ==0)
  {
  // create particle
  //
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding
    anInstance = new G4Ions(
                 name,  0.93956563*GeV, 7.432e-28*GeV,         0.0, 
		    1,              +1,             0,          
		    1,              -1,             0,             
	     "baryon",               0,            +1,        2112,
		false,    886.7*second,          NULL,
		false,       "nucleon",             0,
                  0.0 
              );
    //create Decay Table 
	//    G4DecayTable* table = new G4DecayTable();
	// create a decay channel
	//    G4VDecayChannel* mode = new UCNUCNBetaDecayChannel("neutron",1.00);
	//    table->Insert(mode);
	//    anInstance->SetDecayTable(table);
    
  }
  theInstance = reinterpret_cast<UCNUCN*>(anInstance);
  return theInstance;
}

UCNUCN*  UCNUCN::UCN0Definition()
{
  return Definition();
}

UCNUCN*  UCNUCN::UCN0()
{
  return Definition();
}

