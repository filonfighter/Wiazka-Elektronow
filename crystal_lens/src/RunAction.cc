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

#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(): G4UserRunAction(){
    G4RunManager::GetRunManager()->SetPrintProgress(10);
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    //** Set defaults **//
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFirstHistoId(1);
    
    //** Creating ntuple **//
    analysisManager->CreateNtuple("ExExChTree", "Angles and Positions");
    analysisManager->CreateNtupleDColumn("angXin");
    analysisManager->CreateNtupleDColumn("angYin");
    analysisManager->CreateNtupleDColumn("posXin");
    analysisManager->CreateNtupleDColumn("posYin");
    analysisManager->CreateNtupleDColumn("angXout1st");
    analysisManager->CreateNtupleDColumn("angYout1st");
    analysisManager->CreateNtupleDColumn("posXout1st");
    analysisManager->CreateNtupleDColumn("posYout1st");
    analysisManager->CreateNtupleDColumn("angXout2nd");
    analysisManager->CreateNtupleDColumn("angYout2nd");
    analysisManager->CreateNtupleDColumn("posXout2nd");
    analysisManager->CreateNtupleDColumn("posYout2nd");
    analysisManager->CreateNtupleDColumn("angXout3rd");
    analysisManager->CreateNtupleDColumn("angYout3rd");
    analysisManager->CreateNtupleDColumn("posXout3rd");
    analysisManager->CreateNtupleDColumn("posYout3rd");
    analysisManager->CreateNtupleDColumn("angXout4th");
    analysisManager->CreateNtupleDColumn("angYout4th");
    analysisManager->CreateNtupleDColumn("posXout4th");
    analysisManager->CreateNtupleDColumn("posYout4th");
    analysisManager->CreateNtupleDColumn("angXout5th");
    analysisManager->CreateNtupleDColumn("angYout5th");
    analysisManager->CreateNtupleDColumn("posXout5th");
    analysisManager->CreateNtupleDColumn("posYout5th");
    analysisManager->CreateNtupleDColumn("angXout6th");
    analysisManager->CreateNtupleDColumn("angYout6th");
    analysisManager->CreateNtupleDColumn("posXout6th");
    analysisManager->CreateNtupleDColumn("posYout6th");
    analysisManager->FinishNtuple();

    //** Create new ntuple for energy deposition **//
    analysisManager->CreateNtuple("PhantomDeposition", "Energy deposition in the phantom");
    analysisManager->CreateNtupleDColumn("posX");
    analysisManager->CreateNtupleDColumn("posY");
    analysisManager->CreateNtupleDColumn("posZ");
    analysisManager->CreateNtupleDColumn("Edep");
    analysisManager->FinishNtuple();
    
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*){
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4String fileName = "ExExCh.root";
    analysisManager->OpenFile(fileName);
    G4cout << "Using " << analysisManager->GetType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();    
    analysisManager->Write();
    analysisManager->CloseFile();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
