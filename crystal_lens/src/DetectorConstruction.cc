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

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4CrystalExtension.hh"
#include "G4ExtendedMaterial.hh"
#include "G4LogicalCrystalVolume.hh"

#include "G4ChannelingMaterialData.hh"
#include "G4ChannelingOptrMultiParticleChangeCrossSection.hh"

#include "SensitiveDetector.hh"

#include "G4SDManager.hh"

#include "G4PSDoseDeposit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction():
fECfileName("Si220pl"),
fMaterialName("G4_Si"),
fSizes(G4ThreeVector(2.2*CLHEP::mm,            // crystal's size in x-axis
                     15.0*CLHEP::mm,           // crystal's size in y-axis
                     12.0 * CLHEP::mm)),       // crystal's size in z-axis
fCylinder(G4ThreeVector(2061.55*CLHEP::mm,     // cylinder's radius
			2000.0*CLHEP::mm,      // cylinder's shift in x-axis
			497.99*CLHEP::mm)),    // cylinder's shift in z-axis
fPosition(G4ThreeVector(0.*CLHEP::m,           // detector's shift in x-axis
			0.*CLHEP::m,           // crystal's shift in x-axis
			0.*CLHEP::m)),
fPhantom(G4ThreeVector(0.3*CLHEP::m,           // phantom's size in x-axis
		       0.3*CLHEP::m,           // phantom's size in y-axis
		       0.3*CLHEP::m)),         // phantom's size in z-axis
fPhantomPos(G4ThreeVector(0.5*CLHEP::m,        // phantom's placement in x-axis
			  0.*CLHEP::m,         // phantom's placement in y-axis
			  1.0*CLHEP::m)),      // phantom's placement in z-axis
fBR(G4ThreeVector(4000.* CLHEP::mm,0.,0.)),    // crystal's bending radius
fAngles(G4ThreeVector(0.,-0.0015,0.)),         // incoming angle
fDetector(G4ThreeVector(38.0*CLHEP::mm,        // detector's size in x-axis
		        38.0*CLHEP::mm,        // detector's size in y-axis
			0.64*CLHEP::mm)),      // detector's size in z-axis
fDetectorPos1(G4ThreeVector(0.01*CLHEP::m,     // 1st detector behind the crystal placement in z-axis
			    0.5*CLHEP::m,      // 2nd detector behind the crystal placement in z-axis
			    1.0*CLHEP::m)),    // 3rd detector behind the crystal placement in z-axis
fDetectorPos2(G4ThreeVector(1.5*CLHEP::m,      // 4th detector behind the crystal placement in z-axis
			    2.0*CLHEP::m,      // 5th detector behind the crystal placement in z-axis
			    2.5*CLHEP::m)){    // 6th detector behind the crystal placement in z-axis
    fMessenger = new DetectorConstructionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

DetectorConstruction::~DetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void DetectorConstruction::DefineMaterials(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct(){
    
    //** World **//
    G4Material* worldMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
    
    G4double worldSizeXY =  2. * CLHEP::meter;
    G4double worldSizeZ  = 22. * CLHEP::meter;
    
    G4Box* worldSolid = new G4Box("world.solid",
                                  worldSizeXY/2.,
                                  worldSizeXY/2.,
                                  worldSizeZ/2.);
    
    G4LogicalVolume* worldLogic = new G4LogicalVolume(worldSolid,
                                                      worldMaterial,
                                                      "world.logic");
    
    G4PVPlacement* worldPhysical = new G4PVPlacement(0,
                                                     G4ThreeVector(),
                                                     worldLogic,
                                                     "world.physic",
                                                     0,
                                                     false,
                                                     0);
    
    //** Detectors instantiation **//
    G4ThreeVector fDetectorSizes(G4ThreeVector(fDetector.x(),
                                               fDetector.y(),
                                               fDetector.z()));
    
    G4double fDetectorDistance[8] = {
        -9.998 * CLHEP::m,
        -0.320 * CLHEP::m,
        fDetectorPos1.x(),
	fDetectorPos1.y(),
	fDetectorPos1.z(),
	fDetectorPos2.x(),
	fDetectorPos2.y(),
	fDetectorPos2.z(),
    };
    
    
    G4Box* ssdSolid = new G4Box("ssd.solid",
                                fDetectorSizes.x()/2.,
                                fDetectorSizes.y()/2.,
                                fDetectorSizes.z()/2.);
    
    G4Material* detectorMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
    G4LogicalVolume* ssdLogic =
      new G4LogicalVolume(ssdSolid,
                          detectorMaterial,
                          "ssd.logic");
    
    for(size_t i1=0;i1<8;i1++){
        new G4PVPlacement(0,
                          G4ThreeVector(fPosition.x(),
                                        0.,
                                        fDetectorDistance[i1]),
                          ssdLogic,
                          "ssd.physic",
                          worldLogic,
                          false,
                          i1);
        
    }
    
    //** Crystal solid parameters - Start **//
    // G4Box* crystalSolid = new G4Box("crystal.solid",
    // 				    fSizes.x()/2.,
    // 				    fSizes.y()/2.,
    // 				    fSizes.z()/2.);

    //** Crystal solid parameters - End **//

    
    //** Crystal solid parameters with profiled exit - Start**//
    G4Box* crystalSolidT = new G4Box("crystal.solid",
				     fSizes.x()/2.,
				     fSizes.y()/2.,
				     fSizes.z()/2.);

    G4VSolid* crystalCut = new G4Tubs("crystal.cut",
				      0.*CLHEP::mm,
				      fCylinder.x(),
				      30. * CLHEP::mm,
				      0.,
				      2*CLHEP::pi);
  
    G4ThreeVector translateVector(fCylinder.y(),0.*CLHEP::mm,fCylinder.z()); //cylinders length 4mm - the good focusing

    G4ThreeVector rotationVector(CLHEP::halfpi,0.,0.); //cylinder - the good focusing

    G4RotationMatrix* rotCut = new G4RotationMatrix;
    if(fAngles.x()!=0. || rotationVector.x()!=0.){
      rotCut->rotateX(rotationVector.x());
    }
    if(fAngles.y()!=0. || rotationVector.y()!=0.){
      rotCut->rotateY(rotationVector.y());
    }
    if(fAngles.z()!=0. || rotationVector.z()!=0.){
      rotCut->rotateZ(rotationVector.z());
    }

    // G4LogicalVolume* tubsLogic = new G4LogicalVolume(crystalSolidT,
    // 						     worldMaterial,
    // 						     "world.logicTubs");
    // G4LogicalVolume* cutLogic = new G4LogicalVolume(crystalCut,
    // 						    worldMaterial,
    //                                              "world.logicCut");

    G4VSolid* crystalSolid = new G4SubtractionSolid("crystal.solid",
						crystalSolidT,
						crystalCut,
						rotCut,
						translateVector);

    //** Crystal solid parameters with profiled exit - Start**//

    
    
    //** Crystal Definition Start **//
    G4Material* mat =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
    G4ExtendedMaterial* Crystal =
      new G4ExtendedMaterial("crystal.material",mat);
    
    Crystal->RegisterExtension(std::unique_ptr<G4CrystalExtension>(
            new G4CrystalExtension(Crystal)));
    G4CrystalExtension* crystalExtension =
      (G4CrystalExtension*)Crystal->RetrieveExtension("crystal");
    crystalExtension->SetUnitCell(
        new G4CrystalUnitCell(5.43 * CLHEP::angstrom,
                              5.43 * CLHEP::angstrom,
                              5.43 * CLHEP::angstrom,
                              CLHEP::halfpi,
                              CLHEP::halfpi,
                              CLHEP::halfpi,
                              227));
    
    Crystal->RegisterExtension(std::unique_ptr<G4ChannelingMaterialData>(
            new G4ChannelingMaterialData("channeling")));
    G4ChannelingMaterialData* crystalChannelingData =
      (G4ChannelingMaterialData*)Crystal->RetrieveExtension("channeling");
    crystalChannelingData->SetFilename(fECfileName);

    if(fBR!=G4ThreeVector()){
      crystalChannelingData->SetBR(fBR.x());
    }
    
    G4LogicalCrystalVolume* crystalLogic =
      new G4LogicalCrystalVolume(crystalSolid,
                                 Crystal,
                                 "crystal.logic");
    crystalLogic->SetVerbose(1);
    
    G4RotationMatrix* rot = new G4RotationMatrix;
    if(fAngles.x()!=0.){
      rot->rotateX(fAngles.x());
    }
    if(fAngles.y()!=0.){
      rot->rotateY(fAngles.y());
    }
    if(fAngles.z()!=0.){
      rot->rotateZ(fAngles.z());
    }
    
    new G4PVPlacement(rot,
		      G4ThreeVector(fPosition.y(), 0., 0.),
                      crystalLogic,
                      "crystal.physic",
                      worldLogic,
                      false,
                      0);

    // new G4PVPlacement(rot,
    // 		      G4ThreeVector(),
    // 		      tubsLogic,
    // 		      "tubs.physic",
    // 		      worldLogic,
    // 		      false,
    // 		      0);

    // new G4PVPlacement(rotCut,
    // 		      translateVector,
    // 		      cutLogic,
    // 		      "cut.physic",
    // 		      worldLogic,
    // 		      false,
    // 		      0);

    //** Crystal Definition - End **//

    
    //** Water Phantom - Start **//

    G4Material* phantomMaterial =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
    
    G4Box* phantomSolid = new G4Box("phantom.solid",
                                    fPhantom.x()/2.,
				    fPhantom.y()/2.,
				    fPhantom.z()/2.);

    G4LogicalVolume* phantomLogic = new G4LogicalVolume(phantomSolid,
							phantomMaterial,
							"phantom.logic");

    new G4PVPlacement(0,
		      G4ThreeVector(fPhantomPos.x(),
				    fPhantomPos.y(),
				    fPhantomPos.z()),
                      phantomLogic,
                      "phantom.physic",
                      worldLogic,
                      false,
                      0);


    //** Water Phantom - End **//
    
    
#ifndef G4MULTITHREADED
    G4ChannelingOptrMultiParticleChangeCrossSection* testMany =
    new G4ChannelingOptrMultiParticleChangeCrossSection();
    testMany->AttachTo(crystalLogic);
    G4cout << " Attaching biasing operator " << testMany->GetName()
    << " to logical volume " << crystalLogic->GetName()
    << G4endl;
    
    G4VSensitiveDetector* telescope = new SensitiveDetector("/telescope");
    G4SDManager::GetSDMpointer()->AddNewDetector(telescope);
    for(unsigned int i1=0;i1<3;i1++){
        ssdLogic->SetSensitiveDetector(telescope);
    }
#endif

    // Visualization
    G4VisAttributes* crisVis = new G4VisAttributes(G4Colour::Magenta());
    crystalLogic->SetVisAttributes(crisVis);

    G4VisAttributes* worldVis = new G4VisAttributes(G4Colour::White());
    worldLogic->SetVisAttributes(worldVis);

    G4VisAttributes* phantomVis = new G4VisAttributes(G4Colour::Blue());
    phantomLogic->SetVisAttributes(phantomVis);

    // G4VisAttributes* crisCut = new G4VisAttributes(G4Colour::Green());
    // //crisCut->SetForceSolid(true);
    // cutLogic->SetVisAttributes(crisCut);

    // G4VisAttributes* crisTubs = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.6));
    // crisTubs->SetForceSolid(true);
    // tubsLogic->SetVisAttributes(crisTubs);
    
    return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef G4MULTITHREADED
void DetectorConstruction::ConstructSDandField(){
    G4LogicalVolume* crystalLogic =
      G4LogicalVolumeStore::GetInstance()->GetVolume("crystal.logic");
    G4ChannelingOptrMultiParticleChangeCrossSection* testMany =
    new G4ChannelingOptrMultiParticleChangeCrossSection();
    testMany->AttachTo(crystalLogic);
    G4cout << " Attaching biasing operator " << testMany->GetName()
    << " to logical volume " << crystalLogic->GetName()
    << G4endl;
    
    G4LogicalVolume* ssdLogic =
      G4LogicalVolumeStore::GetInstance()->GetVolume("ssd.logic");
    G4VSensitiveDetector* telescope = new SensitiveDetector("/telescope");
    G4SDManager::GetSDMpointer()->AddNewDetector(telescope);
    for(unsigned int i1=0;i1<8;i1++){
        ssdLogic->SetSensitiveDetector(telescope);

    G4LogicalVolume* phantomLogic =
      G4LogicalVolumeStore::GetInstance()->GetVolume("phantom.logic");
    G4VSensitiveDetector* dose = new SensitiveDetector("/dose",true);
    phantomLogic->SetSensitiveDetector(dose);
    
    }
}
#else
void DetectorConstruction::ConstructSDandField(){;}
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
