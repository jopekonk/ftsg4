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

void HistoManager::Initialize(G4String outputfile) {

  trandom = new TRandom3();
  trandom->SetSeed();
  srand((unsigned)time(NULL));

  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.

  fRootFile = new TFile(outputfile,"RECREATE");
  if (! fRootFile) {
    G4cout << " HistoManager.cc:: problem creating the ROOT TFile " << G4endl;
    return;
  }

  // Create histograms
  fHistoScintillator[0] = new TH1F("ScintillatorE1", "Energy in scintillator det 1 (2pi, back) (keV)", 2000, 0., 2000.);
  fHistoScintillator[1] = new TH1F("ScintillatorE2", "Energy in scintillator det 2 (4pi, back) (keV)", 2000, 0., 2000.);
  fHistoScintillator[2] = new TH1F("ScintillatorE3", "Energy in scintillator det 3 (4pi, front) (keV)", 2000, 0., 2000.);

  fHistoScintillatorSumE = new TH1F("ScintillatorSumE", "Summed energy in scintillators (keV)", 2000, 0., 2000.);

  fHistoScintillator4piCoinc = new TH2F("ScintillatorCoinc4pi", "Coincidence energy in 4pi scintillators (keV)", 2000, 0., 2000., 2000, 0., 2000.);
  fHistoScintillator4piCoinc2 = new TH1F("ScintillatorCoinc4piE2", "Coincidence energy in scintillator det 2 (4pi, back) (keV)", 2000, 0., 2000.);
  fHistoScintillator4piCoinc3 = new TH1F("ScintillatorCoinc4piE3", "Coincidence energy in scintillator det 3 (4pi, front) (keV)", 2000, 0., 2000.);
  fHistoScintillatorCoincSumE = new TH1F("ScintillatorCoincSumE", "Coincidence summed energy in scintillators 2 and 3 (keV)", 2000, 0., 2000.);

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

  G4cout << "\n HistoManager.cc:: ----> Output file is open in " << outputfile << G4endl;
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

// Apply a gaussian skew to the measured Ge energy
G4double HistoManager::GetGeEnergyResolution(G4double energy) {

  // FWHM: 1.2 keV @ 122 keV and 2.0 keV @ 1.3 MeV
  // For simplicity, assume linear behaviour of the resolution dependence on the gamma-ray energy.
  // Solve for resolution = a+b*x , where x is the gamma-ray energy
  double b = (2.0-1.2)/(1332.5-122.);
  double a = 2.-1332.5*b;

  double resolution = a+b*(energy/keV);
  double sigma = resolution/2.35489652;
  double skew = trandom->Gaus(0.0,sigma);
  G4double result = skew*keV;
  if (energy/keV < resolution) return 0.;  // don't bother with low energy stuff
  else return result;
}

// Apply a gaussian skew to the measured scintillator energy
G4double HistoManager::GetScintillatorEnergyResolution(G4double energy) {



 // No information on the resolution => assume constant resolution
 double resolution = 10.0; // in keV
 double sigma = resolution/2.35489652;
 double skew = trandom->Gaus(0.0,sigma);
 G4double result = skew*keV;
 if (energy/keV < resolution) return 0.;  // don't bother with low energy stuff
 else return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillScintillatorHistos(G4double e1, G4double e2, G4double e3) {

  G4double lowThreshold = 0.1*keV;

  // Apply some finite energy resolution
  G4double rese1 = e1 + GetScintillatorEnergyResolution(e1);
  G4double rese2 = e2 + GetScintillatorEnergyResolution(e2);
  G4double rese3 = e3 + GetScintillatorEnergyResolution(e3);

  if (rese1 > lowThreshold) { fHistoScintillator[0]->Fill(rese1/keV); }
  if (rese2 > lowThreshold) { fHistoScintillator[1]->Fill(rese2/keV); }
  if (rese3 > lowThreshold) { fHistoScintillator[2]->Fill(rese3/keV); }

  G4double sumE = rese1+rese2+rese3;
  if (sumE > lowThreshold) { fHistoScintillatorSumE->Fill(sumE/keV); }

}

void HistoManager::FillGeHisto(G4double e1) {

  G4double lowThreshold = 0.1*keV;
  if (e1 < lowThreshold) return;

  // Apply some finite energy resolution
  G4double resE = e1 + GetGeEnergyResolution(e1);

  if (resE > lowThreshold) { fHistoGe->Fill(resE/keV); }

}


void HistoManager::FillCoincHistos(G4double se1, G4double se2, G4double se3, G4double ge1) {

  G4double lowThreshold = 0.1*keV;

  G4double rese1 = se1 + GetScintillatorEnergyResolution(se1);
  G4double rese2 = se2 + GetScintillatorEnergyResolution(se2);
  G4double rese3 = se3 + GetScintillatorEnergyResolution(se3);

  G4double resge1 = ge1 + GetGeEnergyResolution(ge1);

  // Coincidences in the 4pi scintillators
  if (rese2 > lowThreshold && rese3 > lowThreshold) {
    fHistoScintillator4piCoinc->Fill(rese2/keV,rese3/keV);
    fHistoScintillator4piCoinc2->Fill(rese2/keV);
    fHistoScintillator4piCoinc3->Fill(rese3/keV);
    fHistoScintillatorCoincSumE->Fill(rese2/keV+rese3/keV);
  }

  // Coincidences in the germanium detector with either of the 4pi scintillator
  if (resge1 > lowThreshold && (rese2 > lowThreshold || rese3 > lowThreshold) ) {
    fHistoGeCoinc4pi->Fill(resge1/keV);
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
