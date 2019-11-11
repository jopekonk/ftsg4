#include "EventAction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Event.hh"
#include <ctime>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* run, HistoManager* histo)
:G4UserEventAction(), fRunAct(run),fHistoManager(histo), fPrintModulo(0)
{
  fPrintModulo=10000;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction() { ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt) {

  G4int evtNb = evt->GetEventID();
  if (evtNb%fPrintModulo == 0) {
    time_t seconds;
    seconds = time (NULL);

    G4cout << ">>> Event number " << evtNb << " going. "
           << "    Time:"         << ctime (&seconds) << std::flush;
  }

  // initialise variables for each event
  eScint1 =  eScint2 =  eScint3 = eGe = 0.;
}

void EventAction::AddEnergyGe(G4double e, G4int copyNo) {
  eGe += e;
}

void EventAction::AddEnergyScintillator(G4double e, G4int copyNo) {

  switch (copyNo) {
    case 0:
      eScint1 += e;
      break;
    case 1:
      eScint2 += e;
      break;
    case 2:
      eScint3 += e;
      break;
    default:
      break;
  }

  eScintTotal += e;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*) {

  fHistoManager->FillGeHisto(eGe);
  fHistoManager->FillScintillatorHistos(eScint1, eScint2, eScint3);
  fHistoManager->FillCoincHistos(eScint1, eScint2, eScint3, eGe);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
