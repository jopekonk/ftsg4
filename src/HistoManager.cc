#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
:fRootFile(0), fNtuple1(0), fNtuple2(0), fEabs(0.), fEgap(0.) ,fLabs(0.), fLgap(0.) {

  // histograms
  for (G4int k=0; k<kMaxHisto; k++) fHisto[k] = 0;

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
  fHisto[0] = new TH1F("Histo0", "Edep in det0 (keV)", 1000, 0., 2000*CLHEP::keV);
  fHisto[1] = new TH1F("Histo1", "Edep in det1 (keV)", 1000, 0., 2000*CLHEP::keV);
  fHisto[2] = new TH1F("Histo2", "Edep in det2 (keV)", 1000, 0., 2000*CLHEP::keV);
  fHisto[3] = new TH1F("Histo3", "Edep in det3 (keV)", 1000, 0., 2000*CLHEP::keV);

  for ( G4int i=0; i<kMaxHisto; ++i ) {
    if (! fHisto[i]) G4cout << "\n HistoManager.cc:: can't create histogram " << i << G4endl;
  }

  // create ntuples
  fNtuple1 = new TTree("Ntuple1", "Edep");
  fNtuple1->Branch("Eabs", &fEabs, "Eabs/D");
  fNtuple1->Branch("Egap", &fEgap, "Egap/D");

  // create 2nd ntuple
  fNtuple2 = new TTree("Ntuple2", "TrackL");
  fNtuple2->Branch("Labs", &fLabs, "Labs/D");
  fNtuple2->Branch("Lgap", &fLgap, "Lgap/D");

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

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight) {
  if (ih >= kMaxHisto) {
    G4cerr << "\n HistoManager.cc::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
  if  (fHisto[ih]) { fHisto[ih]->Fill(xbin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple(G4double e1, G4double e2, G4double l1, G4double l2 ) {
  fEabs = e1;
  fEgap = e2;
  fLabs = l1;
  fLgap = l2;

  if (fNtuple1) fNtuple1->Fill();
  if (fNtuple2) fNtuple2->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic() {
  G4cout << "\n HistoManager.cc:: ----> print histograms statistics\n" << G4endl;
  for ( G4int i=0; i<kMaxHisto; ++i ) {
    TH1F* h1 = fHisto[i];
    const G4String name = h1->GetName();

    G4String unitCategory;
    if (name[0] == 'H' ) unitCategory = "Energy";
    if (name[0] == 'L' ) unitCategory = "Length";

    G4cout << name
           << ": mean = " << G4BestUnit(h1->GetMean(), unitCategory)
           << " rms = " << G4BestUnit(h1->GetRMS(), unitCategory)
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
