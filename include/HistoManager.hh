#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;
class TTree;
class TH1F;

const G4int kMaxHisto = 4;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager {
  public:
    HistoManager();
   ~HistoManager();

    void Initialize();
    void Save();
    void WriteTree();
    void CloseFile();

    void FillHisto(G4int id, G4double bin, G4double weight = 1.0);

    void FillNtuple(G4double e1, G4double e2, G4double l1, G4double l2);

    void PrintStatistic();

  private:
    TFile*   fRootFile;
    TH1F*    fHisto[kMaxHisto];
    TTree*   fNtuple1;
    TTree*   fNtuple2;

    G4double fEabs;
    G4double fEgap;
    G4double fLabs;
    G4double fLgap;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
