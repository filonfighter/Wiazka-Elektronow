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

#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4RunManager.hh"

#include "G4ios.hh"

DetectorConstructionMessenger::
DetectorConstructionMessenger(
                              DetectorConstruction* mpga)
:fTarget(mpga){
    fMyXtalDirectory = new G4UIdirectory("/xtal/");
    fMyXtalDirectory->SetGuidance("Crystal setup control commands.");
    
    fXtalMaterialCmd = new G4UIcmdWithAString("/xtal/setMaterial",
                                              this);
    fXtalMaterialCmd->SetGuidance("Set crystal material.");
    fXtalMaterialCmd->SetParameterName("xMat",true);
    fXtalMaterialCmd->SetDefaultValue("G4_Si");
    
    fXtalSizeCmd = new G4UIcmdWith3VectorAndUnit("/xtal/setSize",this);
    fXtalSizeCmd->SetGuidance("Set crystal size.");
    fXtalSizeCmd->SetParameterName("xtalSizeX",
                                   "xtalSizeY",
                                   "xtalSizeZ",
                                   true);
    fXtalSizeCmd->SetDefaultValue(G4ThreeVector(6.,2.,6.));
    fXtalSizeCmd->SetDefaultUnit("mm");

    fXtalCylinderCmd = new G4UIcmdWith3VectorAndUnit("/xtal/setCylinder",this);
    fXtalCylinderCmd->SetGuidance("Set cylinder size.");
    fXtalCylinderCmd->SetParameterName("xtalCylinderX",
				       "xtalCylinderY",
				       "xtalCylinderZ",
				       true);
    fXtalCylinderCmd->SetDefaultValue(G4ThreeVector(6.,2.,6.));
    fXtalCylinderCmd->SetDefaultUnit("mm");

    fXtalPositionCmd = new G4UIcmdWith3VectorAndUnit("/xtal/setPosition",this);
    fXtalPositionCmd->SetGuidance("Set the x-axis position of the detestors and the crystal (in both cases the default value is 0).");
    fXtalPositionCmd->SetParameterName("xtalPositionX",
				       "xtalPositionY",
				       "xtalPositionZ",
				       true);
    fXtalPositionCmd->SetDefaultValue(G4ThreeVector(0.,0.,0.));
    fXtalPositionCmd->SetDefaultUnit("m");

    fXtalPhantomCmd = new G4UIcmdWith3VectorAndUnit("/xtal/setPhantom",this);
    fXtalPhantomCmd->SetGuidance("Set phantom size.");
    fXtalPhantomCmd->SetParameterName("xtalPhantomX",
				      "xtalPhantomY",
				      "xtalPhantomZ",
				      true);
    fXtalPhantomCmd->SetDefaultValue(G4ThreeVector(0.3,0.3,0.3));
    fXtalPhantomCmd->SetDefaultUnit("m");

    fXtalPhantomPosCmd = new G4UIcmdWith3VectorAndUnit("/xtal/setPhantomPos",this);
    fXtalPhantomPosCmd->SetGuidance("Set phantom position.");
    fXtalPhantomPosCmd->SetParameterName("xtalPhantomPosX",
					 "xtalPhantomPosY",
					 "xtalPhantomPosZ",
					 true);
    fXtalPhantomPosCmd->SetDefaultValue(G4ThreeVector(0.5,0.,1.));
    fXtalPhantomPosCmd->SetDefaultUnit("m");
    
    fXtalBRCmd = new G4UIcmdWith3VectorAndUnit("/xtal/setBR",this);
    fXtalBRCmd->SetGuidance("Set crystal bending radius.");
    fXtalBRCmd->SetParameterName("xtalBRX",
                                 "xtalBRY",
                                 "xtalBRZ",
                                 true);
    fXtalBRCmd->SetDefaultValue(G4ThreeVector(0.,0.,0.));
    fXtalBRCmd->SetDefaultUnit("mm");
    
    fXtalAngleCmd = new G4UIcmdWith3VectorAndUnit("/xtal/setAngle",this);
    fXtalAngleCmd->SetGuidance("Set crystal angles.");
    fXtalAngleCmd->SetParameterName("xtalAngleX",
                                    "xtalAngleY",
                                    "xtalAngleZ",
                                    true);
    fXtalAngleCmd->SetDefaultValue(G4ThreeVector(0.,0.,0.));
    fXtalAngleCmd->SetDefaultUnit("rad");
    
    fXtalECCmd = new G4UIcmdWithAString("/xtal/setEC",
                                        this);
    fXtalECCmd->SetGuidance("Set crystal EC.");
    fXtalECCmd->SetParameterName("xEC",true);
    fXtalECCmd->SetDefaultValue("data/Si220pl");

    fXtalDetectorCmd = new G4UIcmdWith3VectorAndUnit("/xtal/setDetector",this);
    fXtalDetectorCmd->SetGuidance("Set detector size.");
    fXtalDetectorCmd->SetParameterName("xtalDetectorX",
				       "xtalDetectorY",
				       "xtalDetectorZ",
				       true);
    fXtalDetectorCmd->SetDefaultValue(G4ThreeVector(38.,38.,0.64));
    fXtalDetectorCmd->SetDefaultUnit("mm");

    fXtalDetectorPos1Cmd = new G4UIcmdWith3VectorAndUnit("/xtal/setDetectorPos1",this);
    fXtalDetectorPos1Cmd->SetGuidance("Set detector position in z-axis part 1.");
    fXtalDetectorPos1Cmd->SetParameterName("xtalDetectorPos1X",
					   "xtalDetectorPos1Y",
					   "xtalDetectorPos1Z",
					   true);
    fXtalDetectorPos1Cmd->SetDefaultValue(G4ThreeVector(0.01,0.5,1.));
    fXtalDetectorPos1Cmd->SetDefaultUnit("m");

    fXtalDetectorPos2Cmd = new G4UIcmdWith3VectorAndUnit("/xtal/setDetectorPos2",this);
    fXtalDetectorPos2Cmd->SetGuidance("Set detector position in z-axis part 2.");
    fXtalDetectorPos2Cmd->SetParameterName("xtalDetectorPos2X",
					   "xtalDetectorPos2Y",
					   "xtalDetectorPos2Z",
					   true);
    fXtalDetectorPos2Cmd->SetDefaultValue(G4ThreeVector(1.5,2.,2.5));
    fXtalDetectorPos2Cmd->SetDefaultUnit("m");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstructionMessenger::
~DetectorConstructionMessenger(){
    delete fXtalMaterialCmd;
    delete fXtalSizeCmd;
    delete fXtalCylinderCmd;
    delete fXtalPositionCmd;
    delete fXtalPhantomCmd;
    delete fXtalPhantomPosCmd;
    delete fXtalAngleCmd;
    delete fXtalECCmd;
    delete fXtalBRCmd;
    delete fXtalDetectorCmd;
    delete fXtalDetectorPos1Cmd;
    delete fXtalDetectorPos2Cmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorConstructionMessenger::SetNewValue(
                                                G4UIcommand *command,
                                                G4String newValue){
    if(command==fXtalMaterialCmd ){
        fTarget->SetMaterial(newValue);
    }
    if(command==fXtalSizeCmd ){
        fTarget->SetSizes(fXtalSizeCmd->GetNew3VectorValue(newValue));
    }
    if(command==fXtalCylinderCmd ){
        fTarget->SetCylinder(fXtalCylinderCmd->GetNew3VectorValue(newValue));
    }
    if(command==fXtalPositionCmd ){
        fTarget->SetPosition(fXtalPositionCmd->GetNew3VectorValue(newValue));
    }
    if(command==fXtalPhantomCmd ){
        fTarget->SetPhantom(fXtalPhantomCmd->GetNew3VectorValue(newValue));
    }
    if(command==fXtalPhantomPosCmd ){
        fTarget->SetPhantomPos(fXtalPhantomPosCmd->GetNew3VectorValue(newValue));
    }
    if(command==fXtalBRCmd ){
        fTarget->SetBR(fXtalBRCmd->GetNew3VectorValue(newValue));
    }
    if(command==fXtalAngleCmd ){
        fTarget->SetAngles(fXtalAngleCmd->GetNew3VectorValue(newValue));
    }
    if(command==fXtalECCmd ){
        fTarget->SetEC(newValue);
    }
    if(command==fXtalDetectorCmd ){
        fTarget->SetDetector(fXtalDetectorCmd->GetNew3VectorValue(newValue));
    }
    if(command==fXtalDetectorPos1Cmd ){
        fTarget->SetDetectorPos1(fXtalDetectorPos1Cmd->GetNew3VectorValue(newValue));
    }
    if(command==fXtalDetectorPos2Cmd ){
        fTarget->SetDetectorPos2(fXtalDetectorPos2Cmd->GetNew3VectorValue(newValue));
    }    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4String DetectorConstructionMessenger::GetCurrentValue(
                                                        G4UIcommand * command){
    G4String cv;
    
    if( command==fXtalMaterialCmd ){
        cv = fTarget->GetMaterial();
    }
    if( command==fXtalSizeCmd ){
        cv = fXtalSizeCmd->ConvertToString(fTarget->GetSizes(),"mm");
    }
    if( command==fXtalCylinderCmd ){
        cv = fXtalCylinderCmd->ConvertToString(fTarget->GetCylinder(),"mm");
    }
    if( command==fXtalPositionCmd ){
        cv = fXtalPositionCmd->ConvertToString(fTarget->GetPosition(),"m");
    }
    if( command==fXtalPhantomCmd ){
        cv = fXtalPhantomCmd->ConvertToString(fTarget->GetPhantom(),"m");
    }
    if( command==fXtalPhantomPosCmd ){
        cv = fXtalPhantomPosCmd->ConvertToString(fTarget->GetPhantomPos(),"m");
    }
    if( command==fXtalBRCmd ){
        cv = fXtalBRCmd->ConvertToString(fTarget->GetBR(),"mm");
    }
    if( command==fXtalAngleCmd ){
        cv = fXtalAngleCmd->ConvertToString(fTarget->GetAngles(),"rad");
    }
    if( command==fXtalECCmd ){
        cv = fTarget->GetEC();
    }
    if( command==fXtalDetectorCmd ){
        cv = fXtalDetectorCmd->ConvertToString(fTarget->GetDetector(),"mm");
    }
    if( command==fXtalDetectorPos1Cmd ){
        cv = fXtalDetectorPos1Cmd->ConvertToString(fTarget->GetDetectorPos1(),"m");
    }
    if( command==fXtalDetectorPos2Cmd ){
        cv = fXtalDetectorPos2Cmd->ConvertToString(fTarget->GetDetectorPos2(),"m");
    }

    return cv;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
