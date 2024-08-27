#include "PhysicsList.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"  // Use option4 for high precision
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4ProductionCuts.hh"
#include "G4SystemOfUnits.hh"  // Add this line to include units

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics_option4());  // Use high precision EM physics
    RegisterPhysics(new G4HadronElasticPhysics());
    RegisterPhysics(new G4HadronPhysicsQGSP_BERT());
}

PhysicsList::~PhysicsList() {}

void PhysicsList::SetCuts() {
    // Set cut values for secondary particles
    SetCutValue(0.01*mm, "proton");
    SetCutValue(0.01*mm, "e-");
    SetCutValue(0.01*mm, "e+");
    SetCutValue(0.01*mm, "gamma");

    // Apply the cuts
    G4VUserPhysicsList::SetCuts();
}
