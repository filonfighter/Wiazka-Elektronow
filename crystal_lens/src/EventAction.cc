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

#include "EventAction.hh"

#include "G4RunManager.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include "SensitiveDetectorHit.hh"

EventAction::EventAction():
fSDHT_ID(-1){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

EventAction::~EventAction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::BeginOfEventAction(const G4Event*){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::EndOfEventAction(const G4Event* evt){
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    
    
    G4ThreeVector ssd[8];
    ssd[0]= G4ThreeVector(0.,0.,0.);
    ssd[1]= G4ThreeVector(0.,0.,0.);
    ssd[2]= G4ThreeVector(0.,0.,0.);
    ssd[3]= G4ThreeVector(0.,0.,0.);
    ssd[4]= G4ThreeVector(0.,0.,0.);
    ssd[5]= G4ThreeVector(0.,0.,0.);
    ssd[6]= G4ThreeVector(0.,0.,0.);
    ssd[7]= G4ThreeVector(0.,0.,0.);

    if(fSDHT_ID == -1) {
        G4String sdName;
        if(SDman->FindSensitiveDetector(sdName="telescope",0)){
	  fSDHT_ID = SDman->GetCollectionID(sdName="telescope/collection");
        }
    }
    // G4cout<<"[DEBUG] fSDHT_ID="<<fSDHT_ID<<G4endl;

    SensitiveDetectorHitsCollection* sdht = 0;
    G4HCofThisEvent *hce = evt->GetHCofThisEvent();
    
    if(hce){
        if(fSDHT_ID != -1){
            G4VHitsCollection* aHCSD = hce->GetHC(fSDHT_ID);
            sdht = (SensitiveDetectorHitsCollection*)(aHCSD);
        }
    }
    
    int bTotalHits = 0;
    if(sdht){
        int n_hit_sd = sdht->entries();
        for(int i2=0;i2<8;i2++){
            for(int i1=0;i1<n_hit_sd;i1++)
            {
                SensitiveDetectorHit* aHit = (*sdht)[i1];
                if(aHit->GetLayerID()==i2) {
                    ssd[i2] = aHit->GetWorldPos();
                    bTotalHits++;
                }
            }
        }
    }

    if(bTotalHits > 2){
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        G4double angXin  = (ssd[1].x() - ssd[0].x()) / (ssd[1].z() - ssd[0].z());
        G4double angYin  = (ssd[1].y() - ssd[0].y()) / (ssd[1].z() - ssd[0].z());
	analysisManager->FillNtupleDColumn(0, 0, angXin * 1.E6 * CLHEP::rad);
        analysisManager->FillNtupleDColumn(0, 1, angYin * 1.E6 * CLHEP::rad);

        double posXin = ssd[1].x() - angXin * ssd[1].z();
        double posYin = ssd[1].y() - angYin * ssd[1].z();
	analysisManager->FillNtupleDColumn(0, 2, posXin / CLHEP::mm);
        analysisManager->FillNtupleDColumn(0, 3, posYin / CLHEP::mm);

        G4double angXout1st = (ssd[2].x() - posXin) / (ssd[2].z());
        G4double angYout1st = (ssd[2].y() - posYin) / (ssd[2].z());
        analysisManager->FillNtupleDColumn(0, 4, angXout1st * 1.E6 * CLHEP::rad);
        analysisManager->FillNtupleDColumn(0, 5, angYout1st * 1.E6 * CLHEP::rad);

	double posXout1st = ssd[2].x() - angXin * ssd[2].z();
        double posYout1st = ssd[2].y() - angYin * ssd[2].z();
	analysisManager->FillNtupleDColumn(0, 6, posXout1st / CLHEP::mm);
        analysisManager->FillNtupleDColumn(0, 7, posYout1st / CLHEP::mm);

	G4double angXout2nd = (ssd[3].x() - posXin) / (ssd[3].z());
        G4double angYout2nd = (ssd[3].y() - posYin) / (ssd[3].z());
        analysisManager->FillNtupleDColumn(0, 8, angXout2nd * 1.E6 * CLHEP::rad);
        analysisManager->FillNtupleDColumn(0, 9, angYout2nd * 1.E6 * CLHEP::rad);

	double posXout2nd = ssd[3].x() - angXin * ssd[3].z();
        double posYout2nd = ssd[3].y() - angYin * ssd[3].z();
	analysisManager->FillNtupleDColumn(0, 10, posXout2nd / CLHEP::mm);
        analysisManager->FillNtupleDColumn(0, 11, posYout2nd / CLHEP::mm);

	G4double angXout3rd = (ssd[4].x() - posXin) / (ssd[4].z());
        G4double angYout3rd = (ssd[4].y() - posYin) / (ssd[4].z());
        analysisManager->FillNtupleDColumn(0, 12, angXout3rd * 1.E6 * CLHEP::rad);
        analysisManager->FillNtupleDColumn(0, 13, angYout3rd * 1.E6 * CLHEP::rad);

	double posXout3rd = ssd[4].x() - angXin * ssd[4].z();
        double posYout3rd = ssd[4].y() - angYin * ssd[4].z();
	analysisManager->FillNtupleDColumn(0, 14, posXout3rd / CLHEP::mm);
        analysisManager->FillNtupleDColumn(0, 15, posYout3rd / CLHEP::mm);

	G4double angXout4th = (ssd[5].x() - posXin) / (ssd[5].z());
        G4double angYout4th = (ssd[5].y() - posYin) / (ssd[5].z());
        analysisManager->FillNtupleDColumn(0, 16, angXout4th * 1.E6 * CLHEP::rad);
        analysisManager->FillNtupleDColumn(0, 17, angYout4th * 1.E6 * CLHEP::rad);

	double posXout4th = ssd[5].x() - angXin * ssd[5].z();
        double posYout4th = ssd[5].y() - angYin * ssd[5].z();
	analysisManager->FillNtupleDColumn(0, 18, posXout4th / CLHEP::mm);
        analysisManager->FillNtupleDColumn(0, 19, posYout4th / CLHEP::mm);

	G4double angXout5th = (ssd[6].x() - posXin) / (ssd[6].z());
        G4double angYout5th = (ssd[6].y() - posYin) / (ssd[6].z());
        analysisManager->FillNtupleDColumn(0, 20, angXout5th * 1.E6 * CLHEP::rad);
        analysisManager->FillNtupleDColumn(0, 21, angYout5th * 1.E6 * CLHEP::rad);

	double posXout5th = ssd[6].x() - angXin * ssd[6].z();
        double posYout5th = ssd[6].y() - angYin * ssd[6].z();
	analysisManager->FillNtupleDColumn(0, 22, posXout5th / CLHEP::mm);
        analysisManager->FillNtupleDColumn(0, 23, posYout5th / CLHEP::mm);

	G4double angXout6th = (ssd[7].x() - posXin) / (ssd[7].z());
        G4double angYout6th = (ssd[7].y() - posYin) / (ssd[7].z());
        analysisManager->FillNtupleDColumn(0, 24, angXout6th * 1.E6 * CLHEP::rad);
        analysisManager->FillNtupleDColumn(0, 25, angYout6th * 1.E6 * CLHEP::rad);

	double posXout6th = ssd[7].x() - angXin * ssd[7].z();
        double posYout6th = ssd[7].y() - angYin * ssd[7].z();
	analysisManager->FillNtupleDColumn(0, 26, posXout6th / CLHEP::mm);
        analysisManager->FillNtupleDColumn(0, 27, posYout6th / CLHEP::mm);
        
        analysisManager->AddNtupleRow();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
