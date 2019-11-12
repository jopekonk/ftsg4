#include "RunActionMessenger.hh"
#include "RunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"


RunActionMessenger::RunActionMessenger(RunAction* aRun):Run(aRun)
{
  MyDetDir = new G4UIdirectory("/ftsg4/output/");
  MyDetDir->SetGuidance("Output options.");

  FilenameCmd = new G4UIcmdWithAString("/ftsg4/output/setFilename",this);
  FilenameCmd->SetGuidance("Set filename for output.");
  FilenameCmd->SetParameterName("fOutputFilename",false);
  FilenameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
RunActionMessenger::~RunActionMessenger()
{
  delete FilenameCmd;
  delete MyDetDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...
void RunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == FilenameCmd ) { Run->SetFileName(newValue); }
}
