#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

#include "TRandom.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TMath.h"

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

    void Initialize(G4String);
    void Save();
    void WriteTree();
    void CloseFile();

    G4double GetGeEnergyResolution(G4double energy);
    G4double GetScintillatorEnergyResolution(G4double energy);

    void FillScintillatorHistos(G4double e1, G4double e2, G4double e3);
    void FillGeHisto(G4double e1);
    void FillCoincHistos(G4double se1, G4double se2, G4double se3, G4double ge1);

    void FillNtuple(G4double e1, G4double e2, G4double l1, G4double l2);

    void PrintStatistic();

  private:

    TRandom  *trandom;

    TFile*   fRootFile;
    TH1F*    fHistoScintillator[kScintillatorHisto];
    TH1F*    fHistoGe;
    TH1F*    fHistoGe4piCoinc;
    TH2F*    fHistoGeScintillator2Coinc;
    TH2F*    fHistoGeScintillator3Coinc;

    TH2F*    fHistoScintillator4piCoinc;
    TH1F*    fHistoScintillator4piCoinc2;
    TH1F*    fHistoScintillator4piCoinc3;
    TH1F*    fHistoScintillator4piSumE;
    TH1F*    fHistoScintillator4piCoincSumE;

    TTree*   fNtuple1;
    TTree*   fNtuple2;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
