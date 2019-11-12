//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fDetector(DC),
  fGunMessenger(0)
{
  G4int n_particle = 1;
  fParticleType = 1;
  fParticleEnergy = 500.0;
  fSourcePosition = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  particleTable = G4ParticleTable::GetParticleTable();

  //fParticleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
  //fParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));

  //create a messenger for this class
  fGunMessenger = new PrimaryGeneratorMessenger(this);

  fFTSMeasurementPos1 = 0.0*mm;   // y-coordinate of the measurement position 1 (2pi beta counter)
  fFTSMeasurementPos2 = -110.*mm; // y-coordinate of the measurement position 2 (4pi beta counter)
  fFTSMeasurementPos3 = -240.*mm; // y-coordinate of the measurement position 3 (Ge detector)

  SetSourcePositionY(fSourcePosition);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; delete fGunMessenger; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetParticleType(G4int newValue) {
  fParticleType = newValue;
  if (fParticleType == 1) G4cout << "Particle set to electron" << G4endl;
  if (fParticleType == 2) G4cout << "Particle set to gamma" << G4endl;
}
void PrimaryGeneratorAction::SetParticleEnergy(G4double newValue) {
  fParticleEnergy = newValue;
  G4cout << "The energy of electron/gamma set to " << fParticleEnergy/keV << " keV" << G4endl;
}

void PrimaryGeneratorAction::SetSourcePosition(G4int newValue) {
  fSourcePosition = newValue;
  SetSourcePositionY(fSourcePosition);
  G4cout << "Source position set to " << fSourcePosition << " (y=" << fSourcePositionY << ")" << G4endl;
}

void PrimaryGeneratorAction::SetSourcePositionY(G4int pos) {
  switch(pos){
    case 1:
       fSourcePositionY = fFTSMeasurementPos1;
       break;
    case 2:
       fSourcePositionY = fFTSMeasurementPos2;
       break;
    case 3:
       fSourcePositionY = fFTSMeasurementPos3;
       break;
    default:
       fSourcePositionY = fFTSMeasurementPos1;
  }
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  switch(fParticleType) {
    case 1:
        fParticleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
        break;
    case 2:
        fParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
        break;
    default:
        fParticleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
  }

  G4double position_x, position_y, position_z;
  G4ThreeVector direction;
  G4double theta_e, cos_theta_e, phi_e;

  phi_e = RandFlat::shoot( -M_PI , M_PI);
  cos_theta_e = RandFlat::shoot( -1.0 , 1.0);
  theta_e = acos(cos_theta_e);
  direction.setRThetaPhi( 1.0, theta_e , phi_e );

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
  position_y = fSourcePositionY;
  position_z = 0.0*mm;

  //this function is called at the begining of event
  fParticleGun->SetParticlePosition(G4ThreeVector(position_x,position_y,position_z));
  fParticleGun->SetParticleMomentumDirection(direction);
  fParticleGun->SetParticleEnergy(fParticleEnergy);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
