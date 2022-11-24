#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeometryManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

class BancoConstruction : public G4VUserDetectorConstruction
{
public:
	BancoConstruction();
	~BancoConstruction();

	virtual G4VPhysicalVolume *Construct();
public:
    void SetCheckOverlaps(G4bool );

private:
	G4int fNbOfArms = 0;
    G4LogicalVolume** fLogicChamber = nullptr; // pointer to the logical Chamber
    
	G4bool fCheckOverlaps = true; // option to activate checking of volumes overlaps

};

#endif