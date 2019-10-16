#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "MyMaterials.hh"
#include "GenericGeDetector.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:

     virtual G4VPhysicalVolume* Construct();

  public:

     const G4VPhysicalVolume* GetphysiWorld() {return fPhysiWorld;};

  private:

     MyMaterials*       fMat;
     G4Material*        fWorldMaterial;

     G4double           fFTSMeasurementPos1;
     G4double           fFTSMeasurementPos2;
     G4double           fFTSMeasurementPos3;

     // Distance of the Ge detector from the measurement position (middle of tape) to front face of the detector
     G4double           fGeDistanceSide;

     G4Box*             fSolidWorld;    //pointer to the solid World
     G4LogicalVolume*   fLogicWorld;    //pointer to the logical World
     G4VPhysicalVolume* fPhysiWorld;    //pointer to the physical World

     DetectorMessenger* fDetectorMessenger;  //pointer to the Messenger

  private:

     void DefineMaterials();
     G4VPhysicalVolume* ConstructFastTapeStation();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
