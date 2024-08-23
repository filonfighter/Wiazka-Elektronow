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

#include "SensitiveDetector.hh"
#include "SensitiveDetectorHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"

SensitiveDetector::SensitiveDetector(G4String name, G4bool isPhantom, G4ThreeVector phantomPosition):
  G4VSensitiveDetector(name){
    G4String HCname;
    collectionName.insert(HCname="collection");
    fHCID = -1;
    fIsPhantom = isPhantom;
    fPhantomPosition = phantomPosition;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

SensitiveDetector::~SensitiveDetector(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void SensitiveDetector::Initialize(G4HCofThisEvent*HCE){
    fHitsCollection =
        new SensitiveDetectorHitsCollection(SensitiveDetectorName,
                                                        collectionName[0]);
    if(fHCID<0){
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    }
    HCE->AddHitsCollection(fHCID,fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool SensitiveDetector::ProcessHits(G4Step*aStep,
                                                  G4TouchableHistory*){

  // only care about primary particles (aka electrons)
  if(aStep->GetTrack()->GetTrackID()>1) return true;
  
  if(fIsPhantom == false){
    // G4cout<<"A step in a SD that is not a Phantom"<<G4endl;
    //what to do with no phantom
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
    if(!(postStepPoint->GetStepStatus() == fGeomBoundary)) return true;
    
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

    G4TouchableHistory* theTouchable =
        (G4TouchableHistory*)(preStepPoint->GetTouchable());
    G4VPhysicalVolume* thePhysical = theTouchable->GetVolume(0);
    G4int copyNo = thePhysical->GetCopyNo();

    G4ThreeVector worldPos = preStepPoint->GetPosition();
    
    SensitiveDetectorHit* aHit =
        new SensitiveDetectorHit(copyNo);
    aHit->SetLayerID(copyNo);
    aHit->SetWorldPos(worldPos);
    
    fHitsCollection->insert(aHit);
    return true;
  }else{
    //what to do with a phantom
    G4ThreeVector position = aStep->GetTrack()->GetPosition();
    G4double Edep = aStep->GetTotalEnergyDeposit();

    // G4VPhysicalVolume* physVolume = aStep->GetTrack()->GetVolume();
    // G4LogicalVolume* logVolume = physVoulme->GetLogicalVolume();
    // G4ThreeVector phantomPosition = physVolume->GetTranslation();
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(1,0, position.x() / CLHEP::mm);
    analysisManager->FillNtupleDColumn(1,1, position.y() / CLHEP::mm);
    analysisManager->FillNtupleDColumn(1,2, position.z() / CLHEP::mm);
    analysisManager->FillNtupleDColumn(1,3, Edep / CLHEP::MeV);
    analysisManager->AddNtupleRow(1);
    
    return true;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void SensitiveDetector::EndOfEvent(G4HCofThisEvent* /*HCE*/){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
