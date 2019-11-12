#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

public:
  PrimaryGeneratorAction(DetectorConstruction*);
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

  void SetParticleType(G4int);
  void SetParticleEnergy(G4double);
  void SetSourcePosition(G4int);

  void SetSourcePositionY(G4int);

private:
  G4ParticleGun*           fParticleGun;
  DetectorConstruction*    fDetector;

  PrimaryGeneratorMessenger* fGunMessenger;

  G4ParticleTable* particleTable;

  G4int    fParticleType;
  G4double fParticleEnergy;
  G4int    fSourcePosition;
  G4double fSourcePositionY;
  G4double fFTSMeasurementPos1; // y-coordinate of the measurement position 1 (2pi beta counter)
  G4double fFTSMeasurementPos2; // y-coordinate of the measurement position 2 (4pi beta counter)
  G4double fFTSMeasurementPos3; // y-coordinate of the measurement position 3 (Ge detector)

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
