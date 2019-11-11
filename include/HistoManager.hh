#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;
class TTree;
class TH1F;
class TH2F;

const G4int kScintillatorHisto = 3;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager {
  public:
    HistoManager();
   ~HistoManager();

    void Initialize();
    void Save();
    void WriteTree();
    void CloseFile();

    void FillScintillatorHistos(G4double e1, G4double e2, G4double e3);
    void FillGeHisto(G4double e1);
    void FillCoincHistos(G4double se1, G4double se2, G4double se3, G4double ge1);

    void FillNtuple(G4double e1, G4double e2, G4double l1, G4double l2);

    void PrintStatistic();

  private:
    TFile*   fRootFile;
    TH1F*    fHistoScintillator[kScintillatorHisto];
    TH1F*    fHistoGe;
    TH1F*    fHistoGeCoinc4pi;
    TH2F*    fHistoScintillator4piCoinc;
    TH1F*    fHistoScintillator4piCoinc2;
    TH1F*    fHistoScintillator4piCoinc3;
    TH1F*    fHistoScintillatorSumE;
    
    TTree*   fNtuple1;
    TTree*   fNtuple2;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
