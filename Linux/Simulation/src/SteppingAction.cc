#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction() : G4UserSteppingAction() {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4VPhysicalVolume* volume = preStepPoint->GetPhysicalVolume();

    // Ensure the energy deposition is only recorded within the water phantom
    if (track->GetDefinition()->GetParticleName() == "proton" &&
        volume->GetName() == "WaterPhantom") {
        
        G4double edep = step->GetTotalEnergyDeposit();
        G4double z = preStepPoint->GetPosition().z();

        // Only add energy deposition if it is within the phantom
        if (edep > 0.0) {
            auto runAction = const_cast<RunAction*>(static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
            runAction->AddEnergyDeposition(edep, z);
        }
    }
}

