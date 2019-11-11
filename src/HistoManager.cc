#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>
using namespace CLHEP;

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager() :fRootFile(0), fNtuple1(0), fNtuple2(0) {

  // ntuple
  fNtuple1 = 0;
  fNtuple2 = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager() {
  if (fRootFile) delete fRootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Initialize() {

  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.
  G4String fileName = "output.root";
  fRootFile = new TFile(fileName,"RECREATE");
  if (! fRootFile) {
    G4cout << " HistoManager.cc:: problem creating the ROOT TFile " << G4endl;
    return;
  }

  // Create histograms
  fHistoScintillator[0] = new TH1F("ScintillatorE1", "Energy in scintillator det 1 (2pi, back) (keV)", 1000, 0., 2000.);
  fHistoScintillator[1] = new TH1F("ScintillatorE2", "Energy in scintillator det 2 (4pi, back) (keV)", 1000, 0., 2000.);
  fHistoScintillator[2] = new TH1F("ScintillatorE3", "Energy in scintillator det 3 (4pi, front) (keV)", 1000, 0., 2000.);

  fHistoScintillatorSumE = new TH1F("ScintillatorSumE", "Summed energy in scintillators (keV)", 1000, 0., 2000.);

  fHistoScintillator4piCoinc = new TH2F("ScintillatorCoinc4pi", "Coincidence energy in 4pi scintillators (keV)", 1000, 0., 2000., 1000, 0., 2000.);
  fHistoScintillator4piCoinc2 = new TH1F("ScintillatorCoinc4piE2", "Coincidence energy in scintillator det 2 (4pi, back) (keV)", 1000, 0., 2000.);
  fHistoScintillator4piCoinc3 = new TH1F("ScintillatorCoinc4piE3", "Coincidence energy in scintillator det 3 (4pi, front) (keV)", 1000, 0., 2000.);

  fHistoGe = new TH1F("GermaniumE", "Energy in germanium det (keV)", 2000, 0., 2000.);
  fHistoGeCoinc4pi = new TH1F("GermaniumCoincE4pi", "Coincidence energy in germanium (keV)", 2000, 0., 2000.);

  // create ntuples
  //fNtuple1 = new TTree("Ntuple1", "Edep");
  //fNtuple1->Branch("Eabs", &fEabs, "Eabs/D");
  //fNtuple1->Branch("Egap", &fEgap, "Egap/D");

  // create 2nd ntuple
  //fNtuple2 = new TTree("Ntuple2", "TrackL");
  //fNtuple2->Branch("Labs", &fLabs, "Labs/D");
  //fNtuple2->Branch("Lgap", &fLgap, "Lgap/D");

  G4cout << "\n HistoManager.cc:: ----> Output file is open in " << fileName << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save() {
  if (! fRootFile) return;

  fRootFile->Write();       // Writing the histograms to the file
  fRootFile->Close();       // and closing the tree (and the file)

  G4cout << "\n HistoManager.cc:: ----> Histograms and ntuples are saved and file closed.\n" << G4endl;
}

void HistoManager::WriteTree() {
  if (! fRootFile) return;
  fRootFile->Write();
  G4cout << "\n HistoManager.cc:: ----> Histograms and ntuples are written to file (but file is not closed).\n" << G4endl;
}

void HistoManager::CloseFile() {
  if (! fRootFile) return;
  fRootFile->Close();
  G4cout << "\n HistoManager.cc:: ----> Output file is closed.\n" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillScintillatorHistos(G4double e1, G4double e2, G4double e3) {

  G4double lowThreshold = 0.0001*keV;

  if (e1 > lowThreshold) { fHistoScintillator[0]->Fill(e1/keV); }
  if (e2 > lowThreshold) { fHistoScintillator[1]->Fill(e2/keV); }
  if (e3 > lowThreshold) { fHistoScintillator[2]->Fill(e3/keV); }

  G4double sumE = e1+e2+e3;
  if (sumE > lowThreshold) { fHistoScintillatorSumE->Fill(sumE/keV); }

}

void HistoManager::FillGeHisto(G4double e1) {

  G4double lowThreshold = 0.0001*keV;
  if (e1 > lowThreshold) { fHistoGe->Fill(e1/keV); }

}


void HistoManager::FillCoincHistos(G4double se1, G4double se2, G4double se3, G4double ge1) {

  G4double lowThreshold = 0.0001*keV;

  // Coincidences in the 4pi scintillators
  if (se2 > lowThreshold && se3 > lowThreshold) {
    fHistoScintillator4piCoinc->Fill(se2/keV,se3/keV);
    fHistoScintillator4piCoinc2->Fill(se2/keV);
    fHistoScintillator4piCoinc3->Fill(se3/keV);
  }

  // Coincidences in the germanium detector with either of the 4pi scintillator
  if (ge1 > lowThreshold && (se2 > lowThreshold || se3 > lowThreshold) ) {
    fHistoGeCoinc4pi->Fill(ge1/keV);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple(G4double e1, G4double e2, G4double l1, G4double l2 ) {
  if (fNtuple1) fNtuple1->Fill();
  if (fNtuple2) fNtuple2->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic() {
  G4cout << "\n HistoManager.cc:: ----> print histograms statistics\n" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
