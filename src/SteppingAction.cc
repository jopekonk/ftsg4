#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Step.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det, EventAction* evt)
: G4UserSteppingAction(), fDetector(det), fEventAction(evt) { ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() { ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep) {

  G4Track* aTrack = aStep->GetTrack();
  G4int StepNo = aTrack->GetCurrentStepNumber();

  if(StepNo >= 10000) {
    aTrack->SetTrackStatus(fStopAndKill);
    std::cout << "SteppingAction: Step number limit reached (10000). -> Kill track" << std::endl;
    return;
  }

  G4VPhysicalVolume*    volume;

  if( aTrack->GetNextVolume() != 0 ) {
    volume = aTrack->GetVolume();
  } else {
    volumeName = "OutOfWorld";
    aTrack->SetTrackStatus(fStopAndKill);
    return;
  }

  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();
  volumeName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  G4int copyNo = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();

  if ("Ge" == volumeName) {
    fEventAction->AddEnergyGe(edep, copyNo);
  }

  if ("PlasticScintillator" == volumeName) {
    fEventAction->AddEnergyScintillator(edep, copyNo);
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
