#include "construction.hh"

BancoConstruction::BancoConstruction()
{
	fNbOfArms = 2;
	fLogicChamber = new G4LogicalVolume*[fNbOfArms];
}

BancoConstruction::~BancoConstruction()
{}

G4VPhysicalVolume *BancoConstruction::Construct()
{
	G4NistManager *nist= G4NistManager::Instance();

	//----- Tracker size ----

  	G4double trackerSpacing = 80*cm; // between both tracker arms  ---> todo modify	
  	G4double trackerWidth =  7.0*cm; // Z width of each detector arm  ---> todo modify
  	G4double trackerX =  3.0*cm; // X of each detector arm  ---> todo modify
  	G4double trackerY =  30.0*cm; // Y of each detector arm  ---> todo modify

  	G4double trackerLength = fNbOfArms*trackerWidth + trackerSpacing;
  	G4double trackerDistanceFromSource = 0*m;// if world not centered around tracker

    G4double worldLength = 1.2 * (trackerDistanceFromSource+trackerLength);

  	//----- Materials ----

	G4Element* O  = nist->FindOrBuildElement("O" );
    G4Element* Si = nist->FindOrBuildElement("Si");
    G4Element* C  = nist->FindOrBuildElement("C");
    G4Element* Al = nist->FindOrBuildElement("Al");
    //G4Element* Cu = nist->FindOrBuildElement("Cu");
    G4Element* H  = nist->FindOrBuildElement("H");
    G4Element* N  = nist->FindOrBuildElement("N");
    G4Element* Br = nist->FindOrBuildElement("Br");
  
  
    G4Material* Alu  = new G4Material( "Alu", 2.70*g/cm3, 1);
    Alu->AddElement( Al, 1. );
  
    //From MFT implementation O2/MFTO2.cxx and MM implementation https://github.com/sPHENIX-Collaboration/coresoftware/blob/master/simulation/g4simulation/g4micromegas/PHG4MicromegasDetector.cc
    G4Material* SiO2 = new G4Material( "SiO2", 2.201*g/cm3, 2);  // From MFTO2
    SiO2->AddElement( Si, 0.532565 );
    SiO2->AddElement(  O, 0.467435 );
  
    G4Material* Rohacell = new G4Material( "Rohacell", 0.075*g/cm3, 3); // From MFTO2
    Rohacell->AddElement( H, 0.0858 );
    Rohacell->AddElement( C, 0.5964 );
    Rohacell->AddElement( O, 0.3178 );
  
    G4Material* Kapton = new G4Material( "Kapton", 1.420*g/cm3, 4); // polyimide film  http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=179
    Kapton->AddElement( C, 0.6911330 );
    Kapton->AddElement( H, 0.0263620 );
    Kapton->AddElement( N, 0.0732700 );
    Kapton->AddElement( O, 0.2092350);
  
    G4Material* FR4 = new G4Material( "FR4", 1.7*g/cm3, 5);
    FR4->AddElement( H,  0.0684428 );
    FR4->AddElement( C,  0.278042  );
    FR4->AddElement( O,  0.405633  );
    FR4->AddElement( Si, 0.180774  );
    FR4->AddElement( Br, 0.0671091 );
  
    G4Material* CarbonFiber = new G4Material( "CarbonFiber", 1.33*g/cm3, 4);
    CarbonFiber->AddElement( C, 0.908508078 );
    CarbonFiber->AddElement( N, 0.010387573 );
    CarbonFiber->AddElement( O, 0.055957585 );
    CarbonFiber->AddElement( H, 0.025146765 );

    //
      // Components per arm
  const G4int fNbOfSubcomponents = 11;
  const G4String Components [fNbOfSubcomponents]{
    "AlStructure1",
    "Flex1",
    "ActiveArea1",
    "Kapton1",
    "CarbonPlate1",
    "Rohacell",
    "CarbonPlate2",
    "Kapton2",
    "ActiveArea2",
    "Flex2",
    "AlStructure2"
  };

  G4Material* material[fNbOfSubcomponents]={
  G4Material::GetMaterial("Alu"),
  G4Material::GetMaterial("FR4"),
  G4Material::GetMaterial("SiO2"),
  G4Material::GetMaterial("Kapton"),
  G4Material::GetMaterial("CarbonFiber"),
  G4Material::GetMaterial("Rohacell"),
  G4Material::GetMaterial("CarbonFiber"),
  G4Material::GetMaterial("Kapton"),
  G4Material::GetMaterial("SiO2"),
  G4Material::GetMaterial("FR4"),
  G4Material::GetMaterial("Alu")
};

G4double CompThick[fNbOfSubcomponents]={
    0.2*mm,
    0.2*mm, // FR4
    0.2*mm, //SiO2
    0.3*mm,	//Kapton
    0.3*mm,	//Carbon
    5*cm,	//Rohacel
    0.3*mm,
    0.3*mm,
    0.2*mm,
    0.2*mm,
    0.2*mm
  };

G4double totalArmlength = 0*mm;
for (int i = 0; i < fNbOfSubcomponents; ++i)
{
	totalArmlength+=CompThick[i];
}
if (trackerWidth<totalArmlength) trackerWidth=totalArmlength;

  const G4Colour compColor[fNbOfSubcomponents]{
    G4Colour::White(),
    G4Colour::Green(),
    G4Colour::Grey(),
    G4Colour::Brown(),
    G4Colour::Black(),
    G4Colour::Brown(),
    G4Colour::Grey(),
    G4Colour::Green(),
    G4Colour::White(),
  };

    ////----- World -----
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength); // not sure what the use is for
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");

	G4Box *solidWorld = new G4Box("solidWorld", worldLength, worldLength, worldLength);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, air,"logicWorld");
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld",0, false, 0, true);

	//----- Tracker -----
	G4double trackerSize   = 0.5*trackerLength;  // Half length of the Tracker
    G4double halfX = 0.5*trackerX;
    G4double halfY = 0.5*trackerY;

	G4ThreeVector positionTracker = G4ThreeVector(0,0,trackerDistanceFromSource);
    G4cout << "tracker dimensions " <<  halfX/cm << ", " << halfY/cm << ", " << trackerSize/cm << G4endl;
    G4Box* trackerS = new G4Box("tracker",halfX, halfY,trackerSize); //todo
 	G4LogicalVolume* fullTracker = new G4LogicalVolume(trackerS, air, "Tracker",0,0,0);
    new G4PVPlacement(0,               // no rotation
                    positionTracker, // at (x,y,z)
                    fullTracker,       // its logical volume
                    "Tracker",       // its name
                    logicWorld,         // its mother  volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps

  // Visualization attributes

  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));

  logicWorld      ->SetVisAttributes(boxVisAtt);
  fullTracker    ->SetVisAttributes(boxVisAtt);

  // Tracker segments
  
  G4cout << "There are " << fNbOfArms << " tracker arms in the tracker region. "
         << G4endl
         << "The arms are " << trackerWidth/cm << " cm "
         << "The distance between arms is " << trackerSpacing/cm << " cm"
         << G4endl;

  G4double firstPosition = -trackerSize + trackerWidth;


  G4double halfWidth = 0.5*trackerWidth;



 //

  for (G4int copyNo=0; copyNo<fNbOfArms; copyNo++) {

    G4double Zposition = firstPosition + copyNo * trackerSpacing;
    G4cout << "arm #" << copyNo << " dimensions " << halfX/cm << ", " << halfY/cm << ", " << halfWidth/cm <<" located at z=" << Zposition/cm << "cm" << G4endl;
    G4Box* armS
      = new G4Box("Chamber_solid", halfX, halfY, halfWidth);  //todo

    fLogicChamber[copyNo] = new G4LogicalVolume(armS,air,"Chamber_LV",0,0,0);
    fLogicChamber[copyNo]->SetVisAttributes(chamberVisAtt);

    new G4PVPlacement(0,                            // no rotation
                      G4ThreeVector(0,0,Zposition), // at (x,y,z)
                      fLogicChamber[copyNo],        // its logical volume
                      "Arm",                 // its name
                      fullTracker,                    // its mother  volume
                      false,                        // no boolean operations
                      copyNo,                       // copy number
                      fCheckOverlaps);              // checking overlaps

    //layers
    for (int comp = 0; comp < fNbOfSubcomponents; ++comp)
    {
      G4cout << "component : " << Components[comp] << G4endl;
      auto componentSolid = new G4Box(Components[comp]+"_"+std::to_string(copyNo)+"_solid", halfX, halfY, CompThick[comp]/2);
      auto componentLogic = new G4LogicalVolume( componentSolid, material[comp], Components[comp]+"_"+std::to_string(copyNo)+"_logic");
      new G4PVPlacement(0,                            // no rotation
                      G4ThreeVector(0,0,CompThick[comp]-CompThick[0]/2), // at (x,y,z)
                      componentLogic,        // its logical volume
                      "Layer"+Components[comp]+"_Arm"+std::to_string(copyNo),                 // its name
                      fLogicChamber[copyNo],                    // its mother  volume
                      false,                        // no boolean operations
                      copyNo,                       // copy number
                      fCheckOverlaps);              // checking overlaps      
      componentLogic->SetVisAttributes(compColor[comp]);
 
    }

  }

	return physWorld;
}

void BancoConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}