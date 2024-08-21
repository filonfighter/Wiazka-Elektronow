#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "EventAction.hh"
#include "G4RunManager.hh"
#include "RunAction.hh"

SteppingAction::SteppingAction(EventAction* eventAction) : G4UserSteppingAction(), fEventAction(eventAction) {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep == 0.) return;

    G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
    G4double z = pos.z();

    RunAction* runAction = const_cast<RunAction*>(static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
    if (runAction) {
        runAction->AddEdep(edep, z);
    }
}

