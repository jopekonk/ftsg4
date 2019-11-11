//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fDetector(DC)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  fParticleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
  //particleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  G4double position_x, position_y;
  G4ThreeVector direction;
  G4double theta_e, cos_theta_e, phi_e;
  double particleEnergy;

  phi_e = RandFlat::shoot( -M_PI , M_PI);
  cos_theta_e = RandFlat::shoot( -1.0 , 1.0);
  theta_e = acos(cos_theta_e);
  direction.setRThetaPhi( 1.0, theta_e , phi_e );

  //fFTSMeasurementPos1 = 0.0*mm; // y-coordinate of the measurement position 1 (2pi beta counter)
  //fFTSMeasurementPos2 = -110.*mm; // y-coordinate of the measurement position 2 (4pi beta counter)
  //fFTSMeasurementPos3 = -240.*mm; // y-coordinate of the measurement position 3 (Ge detector)

  // Gaussian distribution of the source position in xy plane, ''beam spot''
  //G4double pos_phi = RandFlat::shoot(0., 2*M_PI);
  //double beamradius = 3.; // beam width in FWHM
  //double sigma = beamradius / 2.35489652; //
  //double radius = RandGauss::shoot(0.,sigma);
  //if (beamradius < 0.) beamradius = -beamradius;
  //position_x = radius*cos(pos_phi)*mm;
  //position_y = -110.*mm + radius*sin(pos_phi)*mm;

  // spot source
  position_x = 0.0*mm;
  position_y = -110.*mm;

  // Energy
  //particleEnergy = RandFlat::shoot(0.,100.);
  particleEnergy = 1000.0;

  //this function is called at the begining of event
  fParticleGun->SetParticlePosition(G4ThreeVector(position_x,position_y,0.));
  fParticleGun->SetParticleMomentumDirection(direction);
  fParticleGun->SetParticleEnergy(particleEnergy*keV);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
