#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Accumulable.hh"
#include "G4AccumulableManager.hh"
#include <vector>

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddEdep(G4double edep, G4double z);

private:
    G4Accumulable<G4double> fTotalEdep;
    std::vector<G4double> fEdepHist;
    G4int fNbins;
    G4double fZmin;
    G4double fZmax;
    G4double fDz;
};

#endif

