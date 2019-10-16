#ifndef GenericGeDetector_H
#define GenericGeDetector_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"

//class G4Box;
class G4Tubs;
class G4Polycone;
class G4UnionSolid;
//class G4SubtractionSolid;
//class G4IntersectionSolid;
//class G4Polyhedra;
class G4LogicalVolume;
class G4VPhysicalVolume;
class MyDetectorMessenger;
class MyMaterials;

class GenericGeDetector { //: public G4VUserDetectorConstruction {

public:
  GenericGeDetector();
  ~GenericGeDetector();

public:
  void SetPosition( G4ThreeVector );
  void SetRotation( G4RotationMatrix );
  void Placement(G4int, G4VPhysicalVolume*, G4bool);

private:
  //General materials....
  MyMaterials*   fMat;

  G4Material* endCapMaterial;
  G4Material* vacuumMaterial;
  G4Material* geMaterial;
  G4Material* windowMaterial;
  G4Material* contactMaterial;

  G4ThreeVector        position;
  G4RotationMatrix     rotation;

  //Keep track of which detector is used
  G4String  theDetector;
  G4int     nDetectors;

  //-------------------------------------------
  // A tapered generic germanium detector
  G4double            fGeTaperL;
  G4double            fTotalGeL;
  G4double            fGeOuterD;
  G4double            fGeInnerD;
  G4double            fAlCap2Ge;
  G4double            fEndCapThickness;
  G4double            fEndCapTaperL;
  G4double            fEndCapTubeL;
  G4double            fEndCapFrontR;
  G4double            fEndCapBackR;
  G4double            fWindowThickness;

  // Choose entrance window type
  G4int fWindowType;

  //
  G4Polycone*         solidAlCap;
  G4LogicalVolume*    logicAlCap;
  G4Tubs*             solidWindow;
  G4VPhysicalVolume*  physiAlCap;
  G4LogicalVolume*    logicWindow;
  G4VPhysicalVolume*  physiWindow;
  G4Polycone*         solidVacuum;
  G4LogicalVolume*    logicVacuum;
  G4VPhysicalVolume*  physiVacuum;
  G4Polycone*         solidGe;
  G4LogicalVolume*    logicGe;
  G4VPhysicalVolume*  physiGe;

  G4double           fContact_dZ;  //dZ to position Passivated Ge
  G4UnionSolid*      solidPassivated;
  G4LogicalVolume*   logicPassivated;
  G4VPhysicalVolume* physiPassivated;
  G4UnionSolid*      solidContact; //inner contact
  G4LogicalVolume*   logicContact;
  G4VPhysicalVolume* physiContact;
  G4UnionSolid*      solidBoreHole; //inner contact
  G4LogicalVolume*   logicBoreHole;
  G4VPhysicalVolume* physiBoreHole;

private:
   void CreateGenericGeDetectorSolids();
};

#endif
