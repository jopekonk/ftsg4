#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* Gun)
: G4UImessenger(),fAction(Gun),
  fGunDir(0), fParticleCmd(0), fEnergyCmd(0), fSourcePositionCmd(0)
{
  fGunDir = new G4UIdirectory("/ftsg4/gun/");
  fGunDir->SetGuidance("gun control");

  fParticleCmd = new G4UIcmdWithAnInteger("/ftsg4/gun/particle",this);
  fParticleCmd->SetGuidance("Set particle type 1 == e-, 2 == gamma");
  fParticleCmd->SetParameterName("fParticleType", false);
  fParticleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fEnergyCmd = new G4UIcmdWithADoubleAndUnit("/ftsg4/gun/energy",this);
  fEnergyCmd->SetGuidance("Set particle energy and unit (e.g. 200 keV)");
  fEnergyCmd->SetParameterName("fParticleEnergy",false);
  fEnergyCmd->SetRange("fParticleEnergy>0.0");
  fEnergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSourcePositionCmd = new G4UIcmdWithAnInteger("/ftsg4/gun/position",this);
  fSourcePositionCmd->SetGuidance("Set source position 1 == pos1 (2pi), 2 == pos2 (4pi), 3 == pos3 (Ge)");
  fSourcePositionCmd->SetParameterName("fSourcePosition", false);
  fSourcePositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fParticleCmd;
  delete fEnergyCmd;
  delete fSourcePositionCmd;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{
  if (command == fParticleCmd) { fAction->SetParticleType( fParticleCmd->GetNewIntValue(newValue) ); }
  if (command == fEnergyCmd) { fAction->SetParticleEnergy( fEnergyCmd->GetNewDoubleValue(newValue) ); }
  if (command == fSourcePositionCmd) { fAction->SetSourcePosition( fParticleCmd->GetNewIntValue(newValue) ); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
