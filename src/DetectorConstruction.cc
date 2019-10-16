#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(), fWorldMaterial(0), fSolidWorld(0),fLogicWorld(0),fPhysiWorld(0), fDetectorMessenger(0)
{

  fFTSMeasurementPos1 = 0.0*mm; // y-coordinate of the measurement position 1 (2pi beta counter)
  fFTSMeasurementPos2 = -100.*mm; // y-coordinate of the measurement position 2 (4pi beta counter)
  fFTSMeasurementPos3 = -200.*mm; // y-coordinate of the measurement position 3 (Ge detector)

  fGeDistanceSide = -50.*mm;

  // materials
  DefineMaterials();

  // create commands for interactive definition of the calorimeter
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() { delete fDetectorMessenger; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct() {
  return ConstructFastTapeStation();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials() {
// use G4-NIST materials data base
G4NistManager* man = G4NistManager::Instance();

//My materials
fMat = MyMaterials::GetInstance();
fWorldMaterial = fMat->GetMaterial("Vacuum");

// print table
G4cout << *(G4Material::GetMaterialTable()) << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructFastTapeStation() {

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //
  // World
  //
  fSolidWorld = new G4Box("World", 1.5*m, 1.5*m, 1.5*m);
  fLogicWorld = new G4LogicalVolume(fSolidWorld, fWorldMaterial, "World");
  fPhysiWorld = new G4PVPlacement(0, G4ThreeVector(), fLogicWorld, "World",0,false,0);

  //
  // Germanium detector
  //
  GenericGeDetector* ge = new GenericGeDetector();

  G4RotationMatrix rotMat;
  rotMat.set(0,0,0);
  G4double theta = -90.*deg;
  G4double phi = 0.*deg;

  rotMat.rotateY(theta);
  rotMat.rotateZ(phi);
  rotMat.invert();

  //G4ThreeVector translation(ge_distance*sin(theta)*cos(phi),
  //                          ge_distance*sin(theta)*sin(phi),
  //                          ge_distance*cos(theta));
  //G4ThreeVector translation(fGeDistanceSide,fFTSMeasurementPos3,0.);
  G4ThreeVector translation(0.,0.,0.);
  ge->SetRotation(rotMat);
  ge->SetPosition(translation);
  ge->Placement(0, fPhysiWorld, true);

  //
  // Visualization attributes
  //
  fLogicWorld->SetVisAttributes(G4VisAttributes::Invisible);


  //
  //always return the physical World
  //
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
