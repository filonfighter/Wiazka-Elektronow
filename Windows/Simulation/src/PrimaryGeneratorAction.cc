#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Electron.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh" // Include for random number generation

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* electron = particleTable->FindParticle("e-");

    fParticleGun->SetParticleDefinition(electron);
    fParticleGun->SetParticleEnergy(200 * MeV);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -0.15 * m));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    // Radius of the circular plane
    G4double radius = 0.125 * m; // Adjust the radius as needed

    // Randomly generate a position on the circular plane
    G4double r = radius * std::sqrt(G4UniformRand());
    G4double theta = 2 * M_PI * G4UniformRand();
    G4double gunPosX = r * std::cos(theta);
    G4double gunPosY = r * std::sin(theta);
    G4double gunPosZ = -0.15 * m; // Starting position of the gun

    // Target point 20 cm from the water phantom surface
    G4double targetX = 0.0;
    G4double targetY = 0.0;
    G4double targetZ = 0.15 * m; // 20 cm from the surface

    // Calculate the direction vector from the gun position to the target point
    G4ThreeVector direction = G4ThreeVector(targetX - gunPosX, targetY - gunPosY, targetZ - gunPosZ).unit();

    // Set the particle gun's position and momentum direction
    fParticleGun->SetParticlePosition(G4ThreeVector(gunPosX, gunPosY, gunPosZ));
    fParticleGun->SetParticleMomentumDirection(direction);

    // Generate the primary vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}