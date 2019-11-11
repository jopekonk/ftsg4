#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction {

public:
  EventAction(RunAction*, HistoManager*);
  virtual ~EventAction();

  virtual void  BeginOfEventAction(const G4Event*);
  virtual void    EndOfEventAction(const G4Event*);

  void AddEnergyGe(G4double e, G4int copyNo);
  void AddEnergyScintillator(G4double e, G4int copyNo);

private:
   RunAction*    fRunAct;
   HistoManager* fHistoManager;

   G4double  eScint1, eScint2, eScint3;
   G4double  eScintTotal;
   G4double  eGe;

   G4int     fPrintModulo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
