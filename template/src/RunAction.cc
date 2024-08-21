#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AccumulableManager.hh"
#include <fstream>

RunAction::RunAction()
    : G4UserRunAction(),
      fTotalEdep(0.),
      fNbins(1000),
      fZmin(-0.5 * m),
      fZmax(0.5 * m),
      fDz((fZmax - fZmin) / fNbins) {
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->RegisterAccumulable(fTotalEdep);
    fEdepHist.resize(fNbins, 0.);
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    // Reset accumulables
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();
    std::fill(fEdepHist.begin(), fEdepHist.end(), 0.);
}

void RunAction::EndOfRunAction(const G4Run* run) {
    // Merge accumulables
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    G4double edep = fTotalEdep.GetValue();
    G4double edepPerEvent = edep / nofEvents;

    // Print results
    G4cout << "--------------------End of Run-----------------------" << G4endl;
    G4cout << "Number of events: " << nofEvents << G4endl;
    G4cout << "Total energy deposition: " << G4BestUnit(edep, "Energy") << G4endl;
    G4cout << "Mean energy deposition per event: " << G4BestUnit(edepPerEvent, "Energy") << G4endl;

    // Save histogram to file
    std::ofstream outFile("BraggCurveData.txt");
    outFile << "# Z (mm) \t Energy Deposition (MeV/mm)" << std::endl;
    for (G4int i = 0; i < fNbins; ++i) {
        G4double z = fZmin + (i + 0.5) * fDz;
        outFile << z / mm << "\t" << fEdepHist[i] / MeV / fDz << std::endl;
    }
    outFile.close();
}

void RunAction::AddEdep(G4double edep, G4double z) {
    fTotalEdep += edep;
    G4int bin = static_cast<G4int>((z - fZmin) / fDz);
    if (bin >= 0 && bin < fNbins) {
        fEdepHist[bin] += edep;
    }
}

