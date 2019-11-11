#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

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

  fGeDistanceSide = (180.-115.+25.-80.)*mm; // ISLTFTSA0004 drawing, 10 mm from tape
  fScintillatorToTapeDistance = 1.5*mm; // Distance of the scintillator to the tape

  fTapePlasticThickness = 50.0*um;
  fTapeMetalThickness   = 30.0*nm;
  fTapeWidth            = 12.0*mm;
  fTapeLength           = 1.0*m;

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

  //My materials
  fMat = MyMaterials::GetInstance();
  fWorldMaterial = fMat->GetMaterial("Vacuum");
  fGeDetectorFlangeMaterial = fMat->GetMaterial("Aluminium");
  fTapePlasticMaterial = fMat->GetMaterial("G4_MYLAR");
  fTapeMetalMaterial = fMat->GetMaterial("Aluminium");

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
  fSolidWorld = new G4Box("World", 1.0*m, 1.0*m, 1.0*m);
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
				"GeDetFlange",            //its name
				fLogicGeDetectorFlange,   //its logical volume
				fPhysiWorld,              //its mother
				false,                    //no boolean operat
				0,                        //copy number
				true);                    //overlap check

  //
  // Plastic scintillator detectors (beta detectors)
  //

  G4double ScintillatorToTapeDistance = fScintillatorToTapeDistance;

  // Measurement position 1, 2pi detector behind the tape
  PlasticScintillator* scint1 = new PlasticScintillator(1);
  G4RotationMatrix scint1RotMat; scint1RotMat.set(0,0,0);
  G4double scint1theta = 0.*deg;
  G4double scint1phi   = 45.*deg;
  scint1RotMat.rotateY(scint1theta);
  scint1RotMat.rotateZ(scint1phi);
  G4ThreeVector scint1translation(0.,fFTSMeasurementPos1, ScintillatorToTapeDistance);
  scint1->SetRotation(scint1RotMat);
  scint1->SetPosition(scint1translation);
  scint1->Placement(0, fPhysiWorld, true);

  // Measurement position 2, detector behind the tape
  PlasticScintillator* scint2 = new PlasticScintillator(1);
  G4RotationMatrix scint2RotMat; scint2RotMat.set(0,0,0);
  G4double scint2theta = 0.*deg;
  G4double scint2phi   = 45.*deg;
  scint2RotMat.rotateY(scint2theta);
  scint2RotMat.rotateZ(scint2phi);
  G4ThreeVector scint2translation(0.,fFTSMeasurementPos2, ScintillatorToTapeDistance);
  scint2->SetRotation(scint2RotMat);
  scint2->SetPosition(scint2translation);
  scint2->Placement(1, fPhysiWorld, true);

  // Measurement position 2, detector on front side of the tape (implantation side)
  PlasticScintillator* scint3 = new PlasticScintillator(1);
  G4RotationMatrix scint3RotMat; scint3RotMat.set(0,0,0);
  G4double scint3theta = 180.*deg;
  G4double scint3phi   = 45.*deg;
  scint3RotMat.rotateY(scint3theta);
  scint3RotMat.rotateZ(scint3phi);
  G4ThreeVector scint3translation(0.,fFTSMeasurementPos2, -1.*ScintillatorToTapeDistance);
  scint3->SetRotation(scint3RotMat);
  scint3->SetPosition(scint3translation);
  scint3->Placement(2, fPhysiWorld, true);

  //
  // Tape, aluminised mylar
  // compose the geometryof a plastic tape with an aluminium backing
  // Biaxially oriented PETP (BoPET) used in film applications is often called Mylar (trademark).
  //
  // Goodfellow ES301955	Polyethylene terephthalate Metallised Film,
  // Film Polyester, PET, PETP (Polyethylene terephthalate polyester).
  // Thickness:0.05mm, Ohms/Square:2, Surface Finish:Metallised On One Side, Metallised With:30 nm Aluminium
  //
  fSolidTapePlastic = new G4Box("TapePlastic", fTapeWidth/2.0, fTapeLength/2.0, fTapePlasticThickness/2.0);
  fLogicTapePlastic = new G4LogicalVolume(fSolidTapePlastic, fTapePlasticMaterial, "TapePlastic");
  fPhysiTapePlastic = new G4PVPlacement(0, G4ThreeVector(0.0,0.0,fTapePlasticThickness/2.0),
                                        "TapePlastic",
                                        fLogicTapePlastic, fPhysiWorld,false,0,true);
  fSolidTapeMetal = new G4Box("TapeMetal", fTapeWidth/2.0, fTapeLength/2.0, fTapeMetalThickness/2.0);
  fLogicTapeMetal = new G4LogicalVolume(fSolidTapeMetal, fTapeMetalMaterial, "TapeMetal");
  fPhysiTapeMetal = new G4PVPlacement(0, G4ThreeVector(0.0,0.0, fTapePlasticThickness+fTapeMetalThickness/2.0),
                                        "TapeMetal",
                                        fLogicTapeMetal, fPhysiWorld,false,0,true);

  //
  // Visualization attributes
  //
  G4VisAttributes* visAttGeDetFlange = new G4VisAttributes( G4Colour(0.8,0.9,0.9) );
  G4VisAttributes* visAttTapePlastic = new G4VisAttributes( G4Colour(0.2,0.2,0.8) );
  G4VisAttributes* visAttTapeMetal   = new G4VisAttributes( G4Colour(0.8,0.2,0.2) );
  //visAttGeDetFlange->SetVisibility(true);


  fLogicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  fLogicGeDetectorFlange->SetVisAttributes(visAttGeDetFlange);
  fLogicTapePlastic->SetVisAttributes(visAttTapePlastic);
  fLogicTapeMetal->SetVisAttributes(visAttTapeMetal);


  //
  //always return the physical World
  //
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
