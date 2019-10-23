#ifndef PlasticScintillator_H
#define PlasticScintillator_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"

//class G4Box;
class G4Tubs;
class G4Polycone;
class G4Polyhedra;

class G4LogicalVolume;
class G4VPhysicalVolume;
class MyDetectorMessenger;
class MyMaterials;

class PlasticScintillator { //: public G4VUserDetectorConstruction {

public:
  PlasticScintillator(G4int);
  ~PlasticScintillator();

public:
  void SetPosition( G4ThreeVector );
  void SetRotation( G4RotationMatrix );
  void Placement(G4int, G4VPhysicalVolume*, G4bool);

private:
  //General materials....
  MyMaterials*   fMat;

  G4Material* scintillatorMaterial;
  G4Material* photomultiplierMaterial;

  G4ThreeVector        position;
  G4RotationMatrix     rotation;

  //Keep track of which detector is used
  G4String  theDetector;
  G4int     nDetectors;

  // Selection of round or square scintillator shape
  G4int fShapeSelection;

  //-------------------------------------------
  // Plastic scintillator detector dimensions (round)
  G4double            fPVTFrontR;
  G4double            fPVTBackShoulderR;
  G4double            fPVTBackShoulderThickness;
  G4double            fPVTFrontThickness;

  //-------------------------------------------
  // Plastic scintillator detector dimensions (square)
  G4double            fPVTSquareFrontR;
  G4double            fPVTSquareBackShoulderR;
  G4double            fPVTSquareBackShoulderThickness;
  G4double            fPVTSquareFrontThickness;

  //
  G4Polycone*         solidPlasticScintillatorRound;
  G4Polyhedra*        solidPlasticScintillatorSquare;
  G4LogicalVolume*    logicPlasticScintillator;
  G4VPhysicalVolume*  physiPlasticScintillator;

private:
   void CreatePlasticScintillatorSolids();
};

#endif
