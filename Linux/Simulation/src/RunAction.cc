#include "RunAction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include <algorithm>

RunAction::RunAction() : G4UserRunAction(), fZmin(0*m), fZmax(0.7*m), fNbins(1000) {
    fZstep = (fZmax - fZmin) / fNbins;
    fEnergyDeposition.resize(fNbins, 0.0);
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    fOutputFile.open("braggs_curve.txt");
    ClearEnergyDeposition();
}

void RunAction::EndOfRunAction(const G4Run*) {
    fOutputFile << "# Z (mm) \t Energy Deposition (MeV)" << std::endl;
    for (G4int i = 0; i < fNbins; ++i) {
        G4double z = fZmin + i * fZstep + 0.5 * fZstep; // Mid-point of the bin
        fOutputFile << z / mm << "\t" << fEnergyDeposition[i] / MeV / fZstep << "\n";
    }
    fOutputFile.close();
}

void RunAction::AddEnergyDeposition(G4double energy, G4double z) {
    if (z >= fZmin && z < fZmax) {
        G4int binIndex = std::min(fNbins - 1, static_cast<G4int>((z - fZmin) / fZstep));
        fEnergyDeposition[binIndex] += energy;
    }
}

void RunAction::ClearEnergyDeposition() {
    std::fill(fEnergyDeposition.begin(), fEnergyDeposition.end(), 0.0);
}

