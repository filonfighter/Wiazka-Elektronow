#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Electron.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* proton = particleTable->FindParticle("e-");

    fParticleGun->SetParticleDefinition(electron);
    fParticleGun->SetParticleEnergy(200*MeV);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -0.15*m));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    // Rozrzut pozycji na płaszczyźnie wejściowej (zakładając, że fantom jest centrowany w (0,0,0))
    // 0 - wiązka w postaci promienia; 0.5 - obszar wystrzału wielkości fantomu
    gunPos = 0;
    G4double x0 = (G4UniformRand() - 0.5) * gunPos * m; // np. rozrzut w zakresie ±5 cm
    G4double y0 = (G4UniformRand() - 0.5) * gunPos * m; // np. rozrzut w zakresie ±5 cm
    G4double z0 = -0.15 * m; // Stała wartość z przed fantomem

    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    // Jeżeli chcesz dodać mały rozrzut w kierunku
    G4double theta = 0. * deg; // Jeśli nie chcesz rozrzutu kątowego, ustaw theta na 0.
    G4double phi = 2 * CLHEP::pi * G4UniformRand(); // Losowy kąt azymutalny

    G4double dx = std::sin(theta) * std::cos(phi);
    G4double dy = std::sin(theta) * std::sin(phi);
    G4double dz = std::cos(theta);

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dx, dy, dz));
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

