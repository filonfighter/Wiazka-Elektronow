//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "SteppingAction.hh"
#include "G4AnalysisManager.hh"
#include "G4EventManager.hh"

RunAction::RunAction() : G4UserRunAction()
{}

RunAction::~RunAction()
{
    if (fOutputFile.is_open()) {
        fOutputFile.close();
    }
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    // Open the CSV file for writing
    fOutputFile.open("energy_deposition.csv");
    fOutputFile << "Depth (mm),Energy Deposited (MeV)\n";
}

void RunAction::EndOfRunAction(const G4Run*)
{
    // Access the stepping action to retrieve energy deposition data
    SteppingAction* steppingAction = (SteppingAction*)G4EventManager::GetEventManager()->GetUserSteppingAction();

    const std::vector<G4double>& depths = steppingAction->GetDepths();
    const std::vector<G4double>& energies = steppingAction->GetEnergies();

   for (size_t i = 0; i < depths.size(); i++) {
        fOutputFile << depths[i] / mm << "," << energies[i] / MeV << "\n";
    }
}


    // Clear the stepping action data for the next run
    steppingAction->ClearData();
    fOutputFile.close();
}
