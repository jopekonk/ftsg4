#include "MyMaterials.hh"

#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

MyMaterials::MyMaterials() {
  CreateMaterials();
}

MyMaterials::~MyMaterials() {}

MyMaterials* MyMaterials::instance = 0;

MyMaterials* MyMaterials::GetInstance() {
  if (instance == 0) {
      instance = new MyMaterials();
    }
  return instance;
}


void MyMaterials::CreateMaterials() {
  G4String name, symbol;
  G4double a, z;
  G4double pressure, density, temperature, fractionmass;
  G4int nel, ncomponents, natoms;

  //---------------------------------
  //Define simple materials


  a=6.941*g/mole;  density=0.534*g/cm3;
  G4Material* Li=new G4Material(name="Lithium", z=3., a, density);

  a=26.98*g/mole;  density=2.7*g/cm3;
  G4Material* Al = new G4Material(name="Aluminium", z=13., a, density);

  a=28.0855*g/mole;  density=2.33*g/cm3;
  G4Material* Si = new G4Material(name="Silicon", z=14., a, density);

  a=63.54*g/mole;  density=8.96*g/cm3;
  G4Material* Cu = new G4Material(name="Copper", z=29., a, density);

  a=72.61*g/mole;  density=5.32*g/cm3;
  G4Material* Ge = new G4Material(name="Germanium", z=32., a, density);

  a = 195.084*g/mole; density=21.45*g/cm3;
  G4Material* Pt = new G4Material(name="Platinum", z=78., a, density);

  a=183.84*g/mole;  density=19.3*g/cm3;
  G4Material* W = new G4Material(name="Tungsten", z=74., a, density);

  a=12.01*g/mole;   density=2.3*g/cm3;
  G4Material* C = new G4Material(name="Carbon", z=12, a, density);

  a=118.710*g/mole;   density=7.31*g/cm3; // white Sn
  G4Material* Sn = new G4Material(name="Tin", z=50, a, density);

  a=207.2*g/mole;   density=11.34*g/cm3;
  G4Material* Pb = new G4Material(name="Lead", z=82, a, density);

  //--------------------------------------------------
  //define some elements to make composite materials
  a = 1.0078*g/mole;
  G4Element* elH  = new G4Element(name="Hydrogen",   symbol="H",  z=1., a);

  a = 4.0026*g/mole;
  G4Element* elHe = new G4Element(name="Helium", symbol="He", z=2., a);

  a = 10.811*g/mole;
  G4Element* elB = new G4Element(name="Boron", symbol="B", z=5., a);

  a = 12.01*g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);

  a = 14.01*g/mole;
  G4Element* elN  = new G4Element(name="Nitrogen",   symbol="N",  z=7., a);

  a = 16.00*g/mole;
  G4Element* elO  = new G4Element(name="Oxygen",     symbol="O",  z=8., a);

  a = 24.305*g/mole;
  G4Element* elMg = new G4Element(name="Magnesium",  symbol="Mg", z=12., a);

  a = 26.98*g/mole;
  G4Element* elAl = new G4Element(name="Aluminium",  symbol="Al", z=13., a);

  a=28.0855*g/mole;
  G4Element* elSi = new G4Element(name="Silicon", symbol="Si", z=14., a);

  a = 35.45*g/mole;
  G4Element* elCl = new G4Element(name="Chlorine",   symbol="Cl", z=17., a);

  a = 39.948*g/mole;
  G4Element* elAr = new G4Element(name="Argon", symbol="Ar", z=18., a);

  a = 40.078*g/mole;
  G4Element* elCa = new G4Element(name="Calcium", symbol="Ca", z=20., a);

  a = 51.9961*g/mole;
  G4Element* elCr = new G4Element(name="Chromium",   symbol="Cr", z=24., a);

  a = 54.938*g/mole;
  G4Element* elMn = new G4Element(name="Manganse",   symbol="Mn", z=25., a);

  a = 58.6934*g/mole;
  G4Element* elNi = new G4Element(name="Nickel",     symbol="Ni", z=28., a);

  a = 55.845*g/mole;
  G4Element* elFe = new G4Element(name="Iron",       symbol="Fe", z=26., a);

  a = 63.54*g/mole;
  G4Element* elCu = new G4Element(name="Copper",     symbol="Cu", z=29., a);

  a = 65.39*g/mole;
  G4Element* elZn = new G4Element(name="Zinc",       symbol="Zn", z=30., a);

  a = 72.61*g/mole;
  G4Element* elGe = new G4Element(name="Germanium",  symbol="Ge", z=32., a);

  a = 112.41*g/mole;
  G4Element* elCd = new G4Element(name="Cadmium",  symbol="Cd", z=48., a);

  a = 127.6*g/mole;
  G4Element* elTe = new G4Element(name="Tellurium",  symbol="Te", z=52., a);

  a =208.98*g/mole;
  G4Element* elBi = new G4Element(name="Bismuth",    symbol="Bi", z=83., a);


  //
  // Composite materials
  //

  G4NistManager* NistManager = G4NistManager::Instance();

  //SiO2
  G4Material* SiO2 = NistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  if(SiO2 == NULL) {
    G4cerr<<"Warning, failed to get SiO2 (G4_SILICON_DIOXIDE) from NistManager"<<G4endl;
    density = 2.2*g/cm3;
    SiO2 = new G4Material("SiO2", density, 2);
    SiO2->AddElement(elSi, 1);
    SiO2->AddElement(elO, 2);
  }

  //B2O3
  G4Material* B2O3 = NistManager->FindOrBuildMaterial("G4_BORON_OXIDE");
  if(B2O3 == NULL) {
    G4cerr<<"Warning, failed to get B2O3 (G4_BORON_OXIDE) from NistManager"<<G4endl;
    density = 1.8*g/cm3;
    B2O3 = new G4Material("B2O3", density, 2);
    B2O3->AddElement(elB, 2);
    B2O3->AddElement(elO, 3);
  }

  //CaO
  G4Material* CaO = NistManager->FindOrBuildMaterial("G4_CALCIUM_OXIDE");
  if(CaO == NULL){
    G4cerr<<"Warning, failed to get CaO (G4_CALCIUM_OXIDE) from NistManager"<<G4endl;
    density = 3.37*g/cm3;
    CaO = new G4Material("CaO", density, 2);
    CaO->AddElement(elCa, 1);
    CaO->AddElement(elO, 1);
  }

  //Al2O3
  G4Material* Al2O3 = NistManager->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  if(Al2O3 == NULL){
    G4cerr<<"Warning, failed to get Al2O3 (G4_ALUMINUM_OXIDE) from NistManager"<<G4endl;
    density = 3.98*g/cm3;
    Al2O3 = new G4Material("Al2O3", density, 2);
    Al2O3->AddElement(elAl, 2);
    Al2O3->AddElement(elO, 3);
  }

  //MgO
  G4Material* MgO = NistManager->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
  if(MgO == NULL){
    G4cerr<<"Warning, failed to get MgO (G4_MAGNESIUM_OXIDE) from NistManager"<<G4endl;
    density = 3.65*g/cm3;
    MgO = new G4Material("MgO", density, 2);
    MgO->AddElement(elMg, 1);
    MgO->AddElement(elO, 1);
  }

  // Mylar
  G4Material* Mylar = NistManager->FindOrBuildMaterial("G4_MYLAR");
  if(Mylar == NULL){
    G4cerr<<"Warning, failed to get Mylar (G4_MYLAR) from NistManager"<<G4endl;
    density = 1.397*g/cm3;
    Mylar= new G4Material("Mylar", density, 3);
    Mylar->AddElement(elC, natoms=10);
    Mylar->AddElement(elH, natoms= 8);
    Mylar->AddElement(elO, natoms= 4);
  }

  // Plastic vinyltoluene (scintillator material)
  G4Material* Vinyltoluene = NistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  if(Vinyltoluene == NULL){
    G4cerr<<"Warning, failed to get Vinyltoluene (G4_PLASTIC_SC_VINYLTOLUENE) from NistManager"<<G4endl;
    density = 1.032*g/cm3;
    Vinyltoluene= new G4Material("Vinyltoluene", density, 2);
    Vinyltoluene->AddElement(elC, natoms=9);
    Vinyltoluene->AddElement(elH, natoms=10);
  }

   ////////////////////////////
  //
  // Gases
  //
  ////////////////////////////

  // Helium as detector gas, STP
  density = 0.178*mg/cm3;
  a = 4.0026*g/mole;
  G4Material* He  = new G4Material(name="He",z=2., a, density );

  // Helium gas at 1mbar (Target chamber volume)
  // Helium gas
  //density = 0.1787*mg/cm3;
  //density = 0.16154e-3*mg/cm3; // calculate by hand:
  pressure = 100.0*pascal; // 100kPa = 1 bar
  temperature = (273.15+25.)*kelvin;
  // calculate density in g/m3: rho = (p*M)/(R*T) = ( 100 Pa*4.0026 g/mole )/( 8.3145 (J/moleK)* 298 K)
  density = 0.16154e-3*mg/cm3;
  G4Material* HeGas = new G4Material(name="HeGas",density,1,kStateGas,temperature,pressure);
  //HeGas->AddElement(elHe,2); // number 2 is number of atoms or fractional mass... hmm... noble gas... JK
  HeGas->AddElement(elHe,1);

  // Neon as detector gas, STP

  density = 0.900*mg/cm3;
  a = 20.179*g/mole;
  G4Material* Neon = new G4Material(name="Ne",z=10., a, density );

  // Argon as detector gas, STP

  density = 1.7836*mg/cm3;       // STP
  G4Material* Argon = new G4Material(name="Argon", density, ncomponents=1);
  Argon->AddElement(elAr, 1);

  // Krypton as detector gas, STP

  density = 3.700*mg/cm3;
  a = 83.80*g/mole;
  G4Material* Kr  = new G4Material(name="Kr",z=36., a, density );

  // Xenon as detector gas, STP

  density = 5.858*mg/cm3;
  a = 131.29*g/mole;
  G4Material* Xe  = new G4Material(name="Xenon",z=54., a, density );

  /////////////////////////
  //
  // Molecular gases

  // Carbon dioxide, STP

  density = 1.977*mg/cm3;
  G4Material* CO2 = new G4Material(name="CO2", density, nel=2,
                                   kStateGas,273.15*kelvin,1.0*atmosphere);
  CO2->AddElement(elC,1);
  CO2->AddElement(elO,2);

  // Carbon dioxide, STP

  density = 1.977*273.*mg/cm3/293.;
  G4Material* CarbonDioxide = new G4Material(name="CO2", density, nel=2);
  CarbonDioxide->AddElement(elC,1);
  CarbonDioxide->AddElement(elO,2);

  // Nitrogen, STP

  density = 1.25053*mg/cm3;       // STP
  G4Material* Nitrogen = new G4Material(name="N2", density, ncomponents=1);
  Nitrogen->AddElement(elN, 2);

  // Oxygen, STP

  density = 1.4289*mg/cm3;       // STP
  G4Material* Oxygen = new G4Material(name="O2", density, ncomponents=1);
  Oxygen->AddElement(elO, 2);

  //---------------------------------
  // AIR
  //1 hPa = 1 mbar
  //1 atm = 1013.25 hPa
  // density = P / R.T where R=286.9 J/(Kg.K) [or is it 287.058]
  // standard (haha - whose?) : T = 20C, P = 1013.25 kPa [1013.25 mbar]

  // Dry Air (average composition with Ar), STP
  density = 1.2928*mg/cm3 ;       // STP
  G4Material* Air = new G4Material(name="Air", density, ncomponents=3);
  Air->AddMaterial( Nitrogen, fractionmass = 0.7557 );
  Air->AddMaterial( Oxygen,   fractionmass = 0.2315 );
  Air->AddMaterial( Argon,    fractionmass = 0.0128 );


  // Galactic VACUUM
  density     = universe_mean_density;
  pressure    = 3.e-18*pascal;
  temperature = 2.73 * kelvin;
  G4Material* Vacuum_galactic
    = new G4Material("Galactic", 1, 0.01*g/mole,density,kStateGas,temperature,pressure);

  // ''Normal'' laboratory vacuum
  pressure = 1.e-6*100.*pascal;  // 1e-6 mbar
  density  = 1.2928*mg/cm3 * pressure / (1.01325 * 100.e3*pascal);
  temperature = (273.15+25.) * kelvin;
  G4Material* Vacuum
    = new G4Material(name="Vacuum", density, ncomponents=1,
				    kStateGas, temperature, pressure);
  Vacuum->AddMaterial(Air, fractionmass=1.);

  //Acrylic  (CH2=C(CH3)CO2CH3)
  density = 1.18*g/cm3;
  G4Material* Acrylic = new G4Material(name="Acrylic", density, 3);
  Acrylic->AddElement(elC, natoms= 5);
  Acrylic->AddElement(elO, natoms= 2);
  Acrylic->AddElement(elH, natoms= 8);
  //Acrylic->AddElement(elC, 0.59984);
  //Acrylic->AddElement(elH, 0.08055);
  //Acrylic->AddElement(elO, 0.31961);
  //---------------------------------
  // Pentane
  //density     = 0.620*g/cm3;  //liquid !
  //density     = 5.9e-6*g/cm3; //gas
  //pressure    = 1.5/760.0*atmosphere;
  //temperature = 310*kelvin;
  //Pentane = new G4Material(name="Pentane", density, ncomponents=2,
  //				    kStateGas, temperature, pressure);
  //Pentane->AddElement(elH, natoms=12);
  //Pentane->AddElement(elC, natoms=5);

  //------------------------------------------------------------------
  // Metal alloys
  //------------------------------------------------------------------

  //Hevimet (BGO shield collimator material)
  //95% W (Tungsten), 3.5% Ni, 0.5% Fe]
  density = 17.0*g/cm3;
  G4Material* Hevimet = new G4Material(name="Hevimet", density, ncomponents=3);
  Hevimet->AddMaterial(W, fractionmass=96.0*perCent);
  Hevimet->AddElement(elNi, fractionmass=3.5*perCent);
  Hevimet->AddElement(elFe, fractionmass=0.5*perCent);

  //DurAl [3-4% Cu, 0.5% Mg, 0.25-1%Mn, remainder Al] : use average val
  density = 2.8*g/cm3;
  G4Material* DurAl = new G4Material(name="DurAl", density, ncomponents=4);
  DurAl->AddElement(elCu, fractionmass= 3.5*perCent);
  DurAl->AddElement(elMg, fractionmass= 0.5*perCent);
  DurAl->AddElement(elMn, fractionmass= 0.6*perCent);
  DurAl->AddElement(elAl, fractionmass=95.4*perCent);

  //------------------------------------------------------------------
  //18-8 (stainless) steel
  density = 8.00*g/cm3;
  G4Material* Steel = new G4Material(name="Steel", density, ncomponents=3);
  Steel->AddElement(elCr, natoms=18);
  Steel->AddElement(elNi, natoms= 8);
  Steel->AddElement(elFe, natoms=74);

  //------------------------------------------------------------------
  //Brass (there are many compositions)
  density =  8.87*g/cm3;
  G4Material* Brass = new G4Material(name="Brass", density, ncomponents=3);
  Brass->AddElement(elCu, fractionmass=70.0*perCent);
  Brass->AddElement(elZn, fractionmass=28.8*perCent);
  Brass->AddElement(elMn, fractionmass= 1.2*perCent);

  // another Steel, was used earlier
  G4Material* Steel_old = new G4Material("Steel_old", 7.9*g/cm3,2);
  Steel_old->AddElement(elFe, 88.*perCent);
  Steel_old->AddElement(elCr, 12.*perCent);

  //------------------------------------------------------------------
  //Epoxy for Si detector support
  density = 1.18*g/cm3;//cast
  density = 1.85*g/cm3;//molded
  G4Material* Epoxy_old = new G4Material(name="Epoxy_old", density, ncomponents=3);
  Epoxy_old->AddElement(elH, natoms=19);
  Epoxy_old->AddElement(elC, natoms=18);
  Epoxy_old->AddElement(elO, natoms= 3);

  //------------------------------------------------------------------
  //Plastic support for the Si detector support
  density = 1.68*g/cm3;//PVC
  G4Material* Pvc = new G4Material(name="PVC", density, ncomponents=3);
  Pvc->AddElement(elH,  natoms=3);
  Pvc->AddElement(elC,  natoms=2);
  Pvc->AddElement(elCl, natoms=1);

  //------------------------------------------------------------------
  // definition of fiber glass (for PCB of the CD Si detector)
  density = 2.5*g/cm3; //2.0 - 2.62
  G4Material* FiberGlass = new G4Material("FiberGlass", density, 5);

  FiberGlass->AddMaterial(SiO2,  0.54);
  FiberGlass->AddMaterial(B2O3,  0.10);
  FiberGlass->AddMaterial(CaO,   0.175);
  FiberGlass->AddMaterial(Al2O3, 0.14);
  FiberGlass->AddMaterial(MgO,   0.045);

	//definition of Epoxy (for PCB)
  density = 1.2*g/cm3;
  G4Material* Epoxy = new G4Material("Epoxy", density, 3);
  Epoxy->AddElement(elH, 24);
  Epoxy->AddElement(elC, 21);
  Epoxy->AddElement(elO, 4);

  //definition of PCB
  density = 1.8*g/cm3; //1.8 - 2.0
  G4Material* PCB = new G4Material("PCB", density, 3);
  PCB->AddMaterial(FiberGlass, 0.70);
  PCB->AddMaterial(Epoxy, 0.25);
  PCB->AddElement(elCu, 0.05);

  //BGO
  density = 7.13*g/cm3;//cast
  G4Material* BGO = new G4Material(name="BGO", density, ncomponents=3);
  BGO->AddElement(elBi, natoms= 4);
  BGO->AddElement(elGe, natoms= 3);
  BGO->AddElement(elO,  natoms=12);

  //------------------------------------------------------------------
  //CdTe (should have some Cl dopent)
  density = 5.85*g/cm3;
  density = 6.20*g/cm3;
  density = 6.02*g/cm3; //depend on source of info !
  G4Material* CdTe = new G4Material(name="CdTe", density, ncomponents=2);
  CdTe->AddElement(elCd, natoms= 1);
  CdTe->AddElement(elTe, natoms= 1);

  //---------------------------------
  //---------------------------------
  // Print all the materials defined.
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  //---------------------------------
}


G4Material* MyMaterials::GetMaterial(const G4String& name) {
  //  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
  G4Material* ma = G4Material::GetMaterial(name);
  G4cout << "Material is selected: " << ma->GetName() << G4endl;
  return ma;
}
