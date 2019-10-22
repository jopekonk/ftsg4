//---------------------------------------------------------------------
// Create the solids defining a plastic scintillator detector
//---------------------------------------------------------------------
#include "PlasticScintillator.hh"
#include "MyMaterials.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4Material.hh"

//#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

PlasticScintillator::PlasticScintillator() {
  //-------------------------------------------------------------------------
  //Define the dimensions used to construct the plastic scintillator detector
  //-------------------------------------------------------------------------

  // ------------------------
  // Saint-Gobain BC-408 Polyvinyltoluene (PVT)
  // Premium plastic scintillator 25x25x5
  // -------------------------
  // Ketek PM6660 silicon photomultiplier
  // 6 x 6 mm2 - 60 um microcell
  //
  //
  // Dimensions from drawing ISLTFTSA0089
  fPVTFrontR                = (24.0/2.) * mm;
  fPVTBackShoulderR         = (25.0/2.) * mm;
  fPVTBackShoulderThickness = 2.5 * mm;
  fPVTFrontThickness        = 2.5 * mm;

  // -------------------------
  // Ketek PM6660 silicon photomultiplier
  // 6 x 6 mm2 - 60 um microcell
  // -------------------------


  // Get the materials
  fMat = MyMaterials::GetInstance();

  // Default materials

  // Polyvinyltoluene (PVT), density ~ 1.03 g/cm3
  scintillatorMaterial        = fMat->GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  photomultiplierMaterial     = fMat->GetMaterial("Silicon");

  // TODO: create commands for interactive detector definition
  //detectorMessenger = new MyDetectorMessenger(this);

  //Now create the solids
  CreatePlasticScintillatorSolids();
}

PlasticScintillator::~PlasticScintillator() { ; }

void PlasticScintillator::SetPosition(G4ThreeVector thisPos) {
  position = thisPos*mm;
  G4cout << " ----> A PlasticScintillator will be placed at " << position/mm << " mm" << G4endl;
}

void PlasticScintillator::SetRotation(G4RotationMatrix thisRot) { rotation = thisRot; }

//---------------------------------------------------------------------
//---------------------------------------------------------------------
// Create the solids defining the PlasticScintillator -------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void PlasticScintillator::CreatePlasticScintillatorSolids() {


 fPVTFrontR                = (24.0/2.) * mm;
 fPVTBackShoulderR         = (25.0/2.) * mm;
 fPVTBackShoulderThickness = 2.5 * mm;
 fPVTFrontThickness        = 2.5 * mm;
  //------------------------------
  // Plastic scintillator piece
  const G4int numZPlanesScintillator=5;  // no. of G4Polycone planes
  G4double zPlaneScintillator[numZPlanesScintillator]; // positions of the planes
  zPlaneScintillator[0] = 0.0*mm;
  zPlaneScintillator[1] = 0.5*mm;
  zPlaneScintillator[2] = fPVTFrontThickness;
  zPlaneScintillator[3] = fPVTFrontThickness;
  zPlaneScintillator[4] = fPVTFrontThickness+fPVTBackShoulderThickness;
  G4double rInnerScintillator[numZPlanesScintillator]; // interior radii
  rInnerScintillator[0] = 0.0*mm;
  rInnerScintillator[1] = 0.0*mm;
  rInnerScintillator[2] = 0.0*mm;
  rInnerScintillator[3] = 0.0*mm;
  rInnerScintillator[4] = 0.0*mm;
  G4double rOuterScintillator[numZPlanesScintillator]; // exterior radii
  rOuterScintillator[0] = fPVTFrontR-0.5*mm;
  rOuterScintillator[1] = fPVTFrontR;
  rOuterScintillator[2] = fPVTFrontR;
  rOuterScintillator[3] = fPVTBackShoulderR;
  rOuterScintillator[4] = fPVTBackShoulderR;

  solidPlasticScintillator = new G4Polycone("PlasticScintillator", 0.0*degree, 360.0*degree,
                                            numZPlanesScintillator, zPlaneScintillator,
                                            rInnerScintillator, rOuterScintillator);

}

//------------------------------------------------------------------
//------------------------------------------------------------------
void PlasticScintillator::Placement(G4int copyNo, G4VPhysicalVolume* physiMother, G4bool checkOverlaps)
{
  //Create logical and physical instance of a PlasticScintillator
  logicPlasticScintillator = new G4LogicalVolume(solidPlasticScintillator, scintillatorMaterial, "PlasticScintillator", 0, 0, 0);
  physiPlasticScintillator = new G4PVPlacement(&rotation, G4ThreeVector(position.x(),position.y(),position.z()),
				"PlasticScintillator",    //its name
				logicPlasticScintillator, //its logical volume
				physiMother,              //its mother
				false,                    //no boolean operat
				copyNo,                   //copy number
				checkOverlaps);           //overlap check


  // Visualisation attributes for the parts
  G4VisAttributes* visAttScintillator = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
  //visAttActive->SetForceWireframe(true);
  visAttScintillator->SetVisibility(true);

  logicPlasticScintillator->SetVisAttributes(visAttScintillator);

}
