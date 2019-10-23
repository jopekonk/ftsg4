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
//#include "G4Sphere.hh"
//#include "G4Trap.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"

#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

PlasticScintillator::PlasticScintillator(G4int shapeSelection) {

  fShapeSelection = shapeSelection; // 0 == round shape, 1 == square shape

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
  // Dimensions from drawing ISLTFTSA0089 (round scintillator)
  fPVTFrontR                = (24.0/2.) * mm;
  fPVTBackShoulderR         = (25.0/2.) * mm;
  fPVTBackShoulderThickness = 2.5 * mm;
  fPVTFrontThickness        = 2.5 * mm;
  // Dimensions from drawing ISLTFTSA0040 (square scintillator)
  fPVTSquareFrontR                = (23.0/2.) * mm;
  fPVTSquareBackShoulderR         = (25.0/2.) * mm;
  fPVTSquareBackShoulderThickness = 3.0 * mm;
  fPVTSquareFrontThickness        = 2.0 * mm;

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

  //-----------------------------------
  // Plastic scintillator piece (round)
  //------------------------------------
  const G4int numZPlanesScintillatorR=5;  // no. of G4Polycone planes
  G4double zPlaneScintillatorR[numZPlanesScintillatorR]; // positions of the planes
  zPlaneScintillatorR[0] = 0.0*mm;
  zPlaneScintillatorR[1] = 0.5*mm;
  zPlaneScintillatorR[2] = fPVTFrontThickness;
  zPlaneScintillatorR[3] = fPVTFrontThickness;
  zPlaneScintillatorR[4] = fPVTFrontThickness+fPVTBackShoulderThickness;
  G4double rInnerScintillatorR[numZPlanesScintillatorR]; // interior radii
  rInnerScintillatorR[0] = 0.0*mm;
  rInnerScintillatorR[1] = 0.0*mm;
  rInnerScintillatorR[2] = 0.0*mm;
  rInnerScintillatorR[3] = 0.0*mm;
  rInnerScintillatorR[4] = 0.0*mm;
  G4double rOuterScintillatorR[numZPlanesScintillatorR]; // exterior radii
  rOuterScintillatorR[0] = fPVTFrontR-0.5*mm;
  rOuterScintillatorR[1] = fPVTFrontR;
  rOuterScintillatorR[2] = fPVTFrontR;
  rOuterScintillatorR[3] = fPVTBackShoulderR;
  rOuterScintillatorR[4] = fPVTBackShoulderR;

  solidPlasticScintillatorRound = new G4Polycone("PlasticScintillator", 0.0*degree, 360.0*degree,
                                            numZPlanesScintillatorR, zPlaneScintillatorR,
                                            rInnerScintillatorR, rOuterScintillatorR);

  //------------------------------------
  // Plastic scintillator piece (square)
  //------------------------------------
  const G4int numZPlanesScintillatorS = 4;  // no. of G4Polyhedra planes
  G4double zPlaneScintillatorS[numZPlanesScintillatorS];
  zPlaneScintillatorS[0] = 0.0*mm;
  zPlaneScintillatorS[1] = fPVTSquareFrontThickness;
  zPlaneScintillatorS[2] = fPVTSquareFrontThickness;
  zPlaneScintillatorS[3] = fPVTSquareFrontThickness+fPVTSquareBackShoulderThickness;
  G4double rInnerScintillatorS[numZPlanesScintillatorS];
  rInnerScintillatorS[0] = 0.0*mm;
  rInnerScintillatorS[1] = 0.0*mm;
  rInnerScintillatorS[2] = 0.0*mm;
  rInnerScintillatorS[3] = 0.0*mm;
  G4double rOuterScintillatorS[numZPlanesScintillatorS];
  rOuterScintillatorS[0] = fPVTSquareFrontR;
  rOuterScintillatorS[1] = fPVTSquareFrontR;
  rOuterScintillatorS[2] = fPVTSquareBackShoulderR;
  rOuterScintillatorS[3] = fPVTSquareBackShoulderR;

  solidPlasticScintillatorSquare = new G4Polyhedra("PlasticScintillator", 0.*deg, 360.*deg,4, // 4 sides !!
                                   numZPlanesScintillatorS,zPlaneScintillatorS,
                                   rInnerScintillatorS,rOuterScintillatorS);

}

//------------------------------------------------------------------
//------------------------------------------------------------------
void PlasticScintillator::Placement(G4int copyNo, G4VPhysicalVolume* physiMother, G4bool checkOverlaps) {

  //Create logical and physical instance of a PlasticScintillator
  if (0 == fShapeSelection) { // ROUND SHAPE
    logicPlasticScintillator = new G4LogicalVolume(solidPlasticScintillatorRound, scintillatorMaterial, "PlasticScintillator", 0, 0, 0);
    physiPlasticScintillator = new G4PVPlacement(&rotation, G4ThreeVector(position.x(),position.y(),position.z()),
  				"PlasticScintillator",    //its name
  				logicPlasticScintillator, //its logical volume
  				physiMother,              //its mother
  				false,                    //no boolean operat
  				copyNo,                   //copy number
  				checkOverlaps);           //overlap check
  } else { // SQUARE SHAPE
    logicPlasticScintillator = new G4LogicalVolume(solidPlasticScintillatorSquare, scintillatorMaterial, "PlasticScintillator", 0, 0, 0);
    physiPlasticScintillator = new G4PVPlacement(&rotation, G4ThreeVector(position.x(),position.y(),position.z()),
      "PlasticScintillator",    //its name
      logicPlasticScintillator, //its logical volume
      physiMother,              //its mother
      false,                    //no boolean operat
      copyNo,                   //copy number
      checkOverlaps);           //overlap check
  }


  // Visualisation attributes for the parts
  G4VisAttributes* visAttScintillator = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
  //visAttActive->SetForceWireframe(true);
  visAttScintillator->SetVisibility(true);

  logicPlasticScintillator->SetVisAttributes(visAttScintillator);

}
