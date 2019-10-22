#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Polycone.hh"
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
  fFTSMeasurementPos2 = -110.*mm; // y-coordinate of the measurement position 2 (4pi beta counter)
  fFTSMeasurementPos3 = -240.*mm; // y-coordinate of the measurement position 3 (Ge detector)

  fGeDistanceSide = (180.-115.+25.-80.)*mm; // ISLTFTSA0004 drawing

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
fGeDetectorFlangeMaterial = fMat->GetMaterial("Aluminium");

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
  G4ThreeVector translation(-(fGeDistanceSide+1.0*mm),fFTSMeasurementPos3,0.);
  //G4ThreeVector translation(0.,0.,0.);
  ge->SetRotation(rotMat);
  ge->SetPosition(translation);
  ge->Placement(0, fPhysiWorld, true);

  //
  // Ge detector flange on the detector chamber (ISLTFTSA0018 drawing)
  //
  const G4int numZPlanesGeDetFlange=6; // no. of G4Polycone planes
  G4double zPlaneGeDetFlange[numZPlanesGeDetFlange]; // positions of the planes
  zPlaneGeDetFlange[0] = 0.0*mm;
  zPlaneGeDetFlange[1] = 1.0*mm;
  zPlaneGeDetFlange[2] = 1.0*mm;
  zPlaneGeDetFlange[3] = 80.0*mm;
  zPlaneGeDetFlange[4] = 80.0*mm;
  zPlaneGeDetFlange[5] = 92.0*mm;
  G4double rInnerGeDetFlange[numZPlanesGeDetFlange]; // interior radii
  rInnerGeDetFlange[0] = 0.0*mm;
  rInnerGeDetFlange[1] = 0.0*mm;
  rInnerGeDetFlange[2] = (97./2.-2.0)*mm;
  rInnerGeDetFlange[3] = (97./2.-2.0)*mm;
  rInnerGeDetFlange[4] = (97./2.-2.0)*mm;
  rInnerGeDetFlange[5] = (97./2.-2.0)*mm;
  G4double rOuterGeDetFlange[numZPlanesGeDetFlange]; // exterior radii
  rOuterGeDetFlange[0] = (97./2.)*mm;
  rOuterGeDetFlange[1] = (97./2.)*mm;
  rOuterGeDetFlange[2] = (97./2.)*mm;
  rOuterGeDetFlange[3] = (97./2.)*mm;
  rOuterGeDetFlange[4] = (150./2.)*mm;
  rOuterGeDetFlange[5] = (150./2.)*mm;

  fSolidGeDetectorFlange = new G4Polycone("GeDetFlange", 0.0*degree, 360.0*degree,
                                          numZPlanesGeDetFlange, zPlaneGeDetFlange,
                                          rInnerGeDetFlange, rOuterGeDetFlange);

  G4double geDetFlangeTheta = 90.0*deg;
  G4double geDetFlangePhi = 0.0*deg;
  G4RotationMatrix* rotMatGeDetFlange = new G4RotationMatrix();
  rotMatGeDetFlange->rotateY(geDetFlangeTheta);
  rotMatGeDetFlange->rotateZ(geDetFlangePhi);

  fLogicGeDetectorFlange = new G4LogicalVolume(fSolidGeDetectorFlange, fGeDetectorFlangeMaterial, "GeDetFlange", 0, 0, 0);
  fPhysiGeDetectorFlange = new G4PVPlacement(rotMatGeDetFlange, G4ThreeVector(-fGeDistanceSide, fFTSMeasurementPos3, 0.0*mm),
				"GeDetFlange",           //its name
				fLogicGeDetectorFlange,        //its logical volume
				fPhysiWorld,       //its mother
				false,             //no boolean operat
				0,            //copy number
				true);     //overlap check


  //
  // Visualization attributes
  //
  G4VisAttributes* visAttGeDetFlange = new G4VisAttributes( G4Colour(0.8,0.9,0.9) );
  //visAttGeDetFlange->SetVisibility(true);


  fLogicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  fLogicGeDetectorFlange->SetVisAttributes(visAttGeDetFlange);


  //
  //always return the physical World
  //
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
