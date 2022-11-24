#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"

class BancoConstruction : public G4VUserDetectorConstruction
{
public:
	BancoConstruction();
	~BancoConstruction();

	virtual G4VPhysicalVolume *Construct();
};

#endif