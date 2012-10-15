# $Id: GNUmakefile,v 1.2 2006/11/13 08:57:40 kuzniak Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := SCMagnet
G4TARGET := $(name)
G4EXLIB := true

CPPFLAGS += -I../Geant4UCN/include -L$(G4WORKDIR)/tmp/Linux-g++/Geant4UCN
EXTRALIBS := -lGeant4UCN

ifndef G4INSTALL
  G4INSTALL = ../../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

include $(G4INSTALL)/config/binmake.gmk
