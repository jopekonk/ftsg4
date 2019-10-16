#ifndef MyMaterials_h
#define MyMaterials_h 1

#include "globals.hh"

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;

class G4Material;

class MyMaterials {
 
  public:
  //MyMaterials();
  ~MyMaterials();
  static MyMaterials* GetInstance();

  G4Material* GetMaterial(const G4String&);


  private:
  MyMaterials();

  void CreateMaterials();


  private:

    static MyMaterials* instance;

};

#endif
