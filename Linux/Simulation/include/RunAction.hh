#include "G4UserRunAction.hh"
#include "globals.hh"
#include <fstream>
#include <vector>

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddEnergyDeposition(G4double energy, G4double z);
    void ClearEnergyDeposition();

private:
    std::ofstream fOutputFile;
    std::vector<G4double> fEnergyDeposition; // Vector to store energy deposition bins
    G4double fZmin, fZmax, fZstep; // Parameters for binning
    G4int fNbins; // Number of bins
};

