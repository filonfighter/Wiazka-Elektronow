#include "EventAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "RunAction.hh"

EventAction::EventAction() : G4UserEventAction(), fEdep(0.) {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
    fEdep = 0.;
}

void EventAction::EndOfEventAction(const G4Event* event) {
    RunAction* runAction = const_cast<RunAction*>(static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
    if (runAction) {
        // runAction->AddEdep(fEdep);
        // No need to add energy deposition directly here, handled in SteppingAction
    }
}

void EventAction::AddEdep(G4double edep) {
    fEdep += edep;
}

