//---------------------------------------------------------------------
// Create the solids defining a simple generic germanium detector
//---------------------------------------------------------------------
#include "GenericGeDetector.hh"
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

GenericGeDetector::GenericGeDetector() {
  //--------------------------------------------------------------
  //Define the dimensions used to construct the germanium detector
  //--------------------------------------------------------------

  // ------------------------
  // Canberra / Mirion GX4020 XtRa
  //-------------------------
  // NOTE: Most of these dimensions are approximate as they are not publicly available from the manufacturer !!!!!
  // Aluminium end cap diameter: 76 mm
  // Relative efficiency: ~ 40 % (0.4 * 1.2e-3 at 25 cm distance from source at 1.33 MeV)
  // Peak-to-Compton ratio 56
  // FWHM: 1.2 keV @ 122 keV / 2.0 keV @ 1.3 MeV
  // Carbon composite window? 0.6 mm (XtRa - extended range coaxial)
  // Also available Be and aluminium windows

  fAlCap2Ge           =   5.0 * mm; // Distance from the front face of the Al end cap to Ge crystal face
  fGeTaperL           =   5.0 * mm; // Length of the tapered front part of the Ge
  fTotalGeL           =  40.0 * mm; // Full length of the Ge crystal
  fGeInnerD           =  64.0 * mm; // Diameter of the tapered front part of the Ge
  fGeOuterD           =  66.0 * mm; // Diameter of the rest of the Ge
  fEndCapFrontR       =  70.0/2.0 * mm; // Radius of the tapered front part of the Al end cap
  fEndCapBackR        =  76.0/2.0 * mm; // Radius of the rest of the Al end cap
  fEndCapTaperL       =   5.0 * mm; // Length of the tapered front part of the Al end cap
  fEndCapTubeL        = 200.0 * mm; // Length of the straight part of the Al end cap
  fEndCapThickness    =   1.2 * mm; // Thickness of the Al end cap


  // Choose entrance window type and specify thickness
  // 1     --> aluminium (normal)
  // 2     --> carbon (thin window for low energies)
  // Other --> vacuum (no window)
  //
  //fWindowType = 1;
  //fWindowThickness = fEndCapThickness; // Thickness of the entrance window (Al)
  fWindowType = 2;
  fWindowThickness = 0.6*mm; // Thickness of the entrance window (carbon)


  // Get the materials
  fMat = MyMaterials::GetInstance();

  //Default materials
  geMaterial         = fMat->GetMaterial("Germanium");
  endCapMaterial     = fMat->GetMaterial("DurAl");
  vacuumMaterial     = fMat->GetMaterial("Vacuum");
  contactMaterial    = fMat->GetMaterial("Lithium");

  switch(fWindowType){
   case(1):
     windowMaterial     = fMat->GetMaterial("DurAl");
     break;
   case(2):
     windowMaterial     = fMat->GetMaterial("Carbon");
     break;
   default:
     windowMaterial     = fMat->GetMaterial("Vacuum");
     break;
  }

  // TODO: create commands for interactive detector definition
  //detectorMessenger = new MyDetectorMessenger(this);

  //Now create the solids
  CreateGenericGeDetectorSolids();
}

GenericGeDetector::~GenericGeDetector() { ; }

void GenericGeDetector::SetPosition(G4ThreeVector thisPos) {
  position = thisPos*mm;
  G4cout << " ----> A GenericGeDetector will be placed at " << position/mm << " mm" << G4endl;
}

void GenericGeDetector::SetRotation(G4RotationMatrix thisRot) { rotation = thisRot; }

//---------------------------------------------------------------------
//---------------------------------------------------------------------
// Create the solids defining the GenericGeDetector -------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GenericGeDetector::CreateGenericGeDetectorSolids() {
  //-------------------------------------------
  G4double endCapFrontR    = fEndCapFrontR;
  G4double endCapBackR     = fEndCapBackR;
  G4double endCapTaperL    = fEndCapTaperL;
  G4double endCapTubeL     = fEndCapTubeL;
  //------------------------------
  // Al container tube
  const G4int numZPlanesAl=3;      // no. of G4Polycone planes
  G4double zPlaneAl[numZPlanesAl]; // positions of the planes
  zPlaneAl[0] = 0.0*mm;
  zPlaneAl[1] = endCapTaperL;
  zPlaneAl[2] = endCapTaperL + endCapTubeL;
  G4double rInnerAl[numZPlanesAl]; // interior radii
  rInnerAl[0] = 0.0*mm;
  rInnerAl[1] = 0.0*mm;
  rInnerAl[2] = 0.0*mm;
  G4double rOuterAl[numZPlanesAl]; // exterior radii
  rOuterAl[0] = endCapFrontR;
  rOuterAl[1] = endCapBackR;  //=> tan(theta) = 10/100
  rOuterAl[2] = endCapBackR;

  solidAlCap = new G4Polycone("capAl", 0.0*degree, 360.0*degree, numZPlanesAl, zPlaneAl, rInnerAl, rOuterAl);

  // Entrance window for the Al end cap
  G4double windowRadius = endCapFrontR - fEndCapThickness;
  G4double windowThickness = fWindowThickness;
  solidWindow = new G4Tubs( "window", 0.0*mm, windowRadius, windowThickness/2.0, 0.0*degree, 360.0*degree);

  //------------------------------
  // Vacuum tube inside container according to the Al end cap thickness
  G4double AlThick = fEndCapThickness;

  const G4int numZPlanesVac=4;       // no. of G4Polycone planes
  G4double zPlaneVac[numZPlanesVac]; // positions of the planes
  zPlaneVac[0] = 0.0*mm;
  zPlaneVac[1] = AlThick;
  zPlaneVac[2] = endCapTaperL;
  zPlaneVac[3] = endCapTaperL + endCapTubeL;
  G4double rInnerVac[numZPlanesVac]; // interior radii
  rInnerVac[0] = 0.0*mm;
  rInnerVac[1] = 0.0*mm;
  rInnerVac[2] = 0.0*mm;
  rInnerVac[3] = 0.0*mm;
  G4double rOuterVac[numZPlanesVac]; // exterior radii
  rOuterVac[0] = endCapFrontR - AlThick;
  rOuterVac[1] = endCapFrontR - AlThick;
  rOuterVac[2] = endCapBackR - AlThick;
  rOuterVac[3] = endCapBackR - AlThick;
  solidVacuum = new G4Polycone("vacuum", 0.0*degree, 360.0*degree, numZPlanesVac, zPlaneVac, rInnerVac, rOuterVac);

  //-------------------------------------------------------------------------------------
  // Germanium crystal
  G4double delta   = 2.0 * mm;  //nice rounded edges

  const G4int numZPlanesGe=4;      // no. of G4Polycone planes
  G4double zPlaneGe[numZPlanesGe]; // positions of the planes
  zPlaneGe[0] = 0.0 * mm;
  zPlaneGe[1] = delta;
  zPlaneGe[2] = fGeTaperL;
  zPlaneGe[3] = fTotalGeL;
  G4double rInnerGe[numZPlanesGe]; // interior radii
  rInnerGe[0] = 0.0*mm;
  rInnerGe[1] = 0.0*mm;
  rInnerGe[2] = 0.0*mm;
  rInnerGe[3] = 0.0*mm;
  G4double rOuterGe[numZPlanesGe]; // exterior radii
  rOuterGe[0] = fGeInnerD/2.0 - delta;     //a little rounding...
  rOuterGe[1] = fGeInnerD/2.0 + 0.1*delta; //tan(theta) = 0.1
  rOuterGe[2] = fGeOuterD/2.0;
  rOuterGe[3] = fGeOuterD/2.0;

  solidGe = new G4Polycone("germanium", 0.0*degree, 360.0*degree, numZPlanesGe, zPlaneGe, rInnerGe, rOuterGe);

  //------------------------------------------------------------------
  // Inner bore hole for the contact + passivated Ge
  G4double GeDepth      =  25.0*mm;
  G4double contactThick =   0.5*mm;
  G4double passiveThick =   0.3*mm;
  G4double holeR        =   5.0*mm;
  G4double contactR     = holeR     + contactThick;
  G4double passiveR     = contactR  + passiveThick;
  G4double holeL        = fTotalGeL - GeDepth;
  G4double tubeL        = holeL - holeR;

  //save this for placement
  fContact_dZ = fTotalGeL - tubeL/2.;

  //the same translation works for all the following.
  //G4ThreeVector transHole( 0.001*mm, 0.001*mm, -tubeL/2.-0.001*mm); //if offsets are 0. it does not display !!
  G4ThreeVector transHole( 0.0*mm, 0.0*mm, -tubeL/2.);

  //now add a passivated layer
  G4Sphere* passivatedSphere = new G4Sphere("passSphere", 0.0*mm, passiveR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
  G4Tubs*   passivatedTube   = new G4Tubs(  "passTube",   0.0*mm, passiveR, tubeL/2., 0.*degree, 360.*degree);
  solidPassivated = new G4UnionSolid("passivatedGe",passivatedTube,passivatedSphere,0,transHole);

  //and the Li contact
  G4Sphere* contactSphere = new G4Sphere("sphere1", 0.0*mm, contactR,           0.*deg, 360.*deg, 0.*deg, 180.*deg);
  G4Tubs*   contactTube   = new G4Tubs(  "tube1",   0.0*mm, contactR, tubeL/2., 0.*deg, 360.*deg);
  solidContact = new G4UnionSolid("liContact",contactTube,contactSphere,0,transHole);

  //bore out a hole
  G4Sphere* boreSphere = new G4Sphere("boreSphere",0.0*mm, holeR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
  G4Tubs*   boreTube   = new G4Tubs(  "boreTube",  0.0*mm, holeR, tubeL/2., 0.*degree, 360.*degree);
  solidBoreHole = new G4UnionSolid("boreHole",boreTube,boreSphere,0,transHole);

}

//------------------------------------------------------------------
//------------------------------------------------------------------
void GenericGeDetector::Placement(G4int copyNo, G4VPhysicalVolume* physiMother, G4bool checkOverlaps)
{
  //Create physical instance of a GenericGeDetector

  logicAlCap = new G4LogicalVolume(solidAlCap, endCapMaterial, "capAl", 0, 0, 0);
  //logicAlCap = new G4LogicalVolume(solidAlCapWithHole, endCapMaterial, "capAl", 0, 0, 0);
  physiAlCap = new G4PVPlacement(&rotation, G4ThreeVector(position.x(),position.y(),position.z()),
				"CapAl",           //its name
				logicAlCap,        //its logical volume
				physiMother,       //its mother
				false,             //no boolean operat
				copyNo,            //copy number
				checkOverlaps);     //overlap check

  logicVacuum = new G4LogicalVolume(solidVacuum, vacuumMaterial, "vacuum", 0, 0, 0);
  physiVacuum = new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm),
				"Vacuum",        //its name
				logicVacuum,     //its logical volume
				physiAlCap,      //its mother
				false,           //no boolean operat
				copyNo,          //copy number
				checkOverlaps);  //overlap check

  logicWindow = new G4LogicalVolume(solidWindow, windowMaterial, "window", 0, 0, 0);
  physiWindow = new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm, fWindowThickness/2.0),
    "Window",        //its name
    logicWindow,     //its logical volume
    physiVacuum,      //its mother
    false,           //no boolean operat
    copyNo,          //copy number
    checkOverlaps);  //overlap check

  logicGe = new G4LogicalVolume(solidGe, geMaterial, "germanium", 0, 0, 0);
  G4double geZ = fAlCap2Ge + fEndCapThickness;
  physiGe = new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm, geZ),
				"Ge",            //its name
				logicGe,         //its logical volume
				physiVacuum,     //its mother
				false,           //no boolean operat
				copyNo,          //copy number
				checkOverlaps);  //overlap check

  logicPassivated = new G4LogicalVolume(solidPassivated, geMaterial, "passiveGe", 0, 0, 0);
  physiPassivated = new G4PVPlacement(0,
				G4ThreeVector(0.0*mm,0.0*mm,fContact_dZ),
				"PassiveGe",      //its name
				logicPassivated,  //its logical volume
				physiGe,          //its mother
				false,            //no boolean operat
				copyNo,           //copy number
				checkOverlaps);   //overlap check

  logicContact = new G4LogicalVolume(solidContact, contactMaterial, "lithium", 0, 0, 0);
  physiContact = new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,0.0*mm),
				"LiContact",         //its name
				logicContact,        //its logical volume
				physiPassivated,     //its mother
				false,               //no boolean operat
				copyNo,              //copy number
				checkOverlaps);      //overlap check

  logicBoreHole = new G4LogicalVolume(solidBoreHole, vacuumMaterial, "hole", 0, 0, 0);
  physiBoreHole = new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,0.0*mm),
				"BoreHole",        //its name
				logicBoreHole,     //its logical volume
				physiContact,      //its mother
				false,             //no boolean operat
				copyNo,            //copy number
				checkOverlaps);    //overlap check

  // Visualisation attributes for the parts
  // The active Ge layer will appear in yellow colour.
  G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
  //visAttActive->SetForceWireframe(true);
  visAttActive->SetVisibility(true);

  G4VisAttributes* visAttContact = new G4VisAttributes( G4Colour(1.0,0.0,1.0) );
  visAttContact->SetVisibility(true);
  //visAttHole->SetVisibility(false);

  G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.1,0.1,0.1) );
  visAttGeVac->SetForceWireframe(true);
  //visAttGeVac->SetVisibility(false);

  G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
  visAttPassive->SetVisibility(true);

  G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,1.0,1.0) );
  //visAttAlCap->SetForceWireframe(true);
  visAttAlCap->SetVisibility(true);

  G4VisAttributes* visAttWindow;
  switch(fWindowType){
   case(1):
     visAttWindow = visAttAlCap;
     break;
   case(2):
     visAttWindow = new G4VisAttributes(G4Colour(0.4,0.4,0.4) );
     break;
   default:
     visAttWindow = new G4VisAttributes(G4Colour(0.1,0.1,0.1) );
     visAttWindow->SetVisibility(false);
     break;
  }

  logicGe->SetVisAttributes(visAttActive);
  logicBoreHole->SetVisAttributes(visAttPassive);
  logicPassivated->SetVisAttributes(visAttPassive);
  logicContact->SetVisAttributes(visAttContact);
  logicVacuum->SetVisAttributes(visAttGeVac);
  logicAlCap->SetVisAttributes(visAttAlCap);
  logicWindow->SetVisAttributes(visAttWindow);
}
