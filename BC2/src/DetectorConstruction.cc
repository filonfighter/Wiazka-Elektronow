#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    G4Material* galactic = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material* water = nist->FindOrBuildMaterial("G4_WATER");

    G4Box* worldBox = new G4Box("World", 1.0*m, 1.0*m, 1.0*m);
    G4LogicalVolume* worldLogical = new G4LogicalVolume(worldBox, galactic, "World");
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), worldLogical, "World", nullptr, false, 0);

    G4Box* waterBox = new G4Box("WaterPhantom", 0.25*m, 0.25*m, 0.25*m);
    fWaterPhantomLogical = new G4LogicalVolume(waterBox, water, "WaterPhantom");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0.35*m), fWaterPhantomLogical, "WaterPhantom", worldLogical, false, 0);

    return worldPhysical;
}

