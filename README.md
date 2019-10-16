# ftsg4
Geant4 simulation of the new Fast Tape Station (FTS) at ISOLDE

# Required libraries
- Geant4 (tested on 4.10.05.p01)
- ROOT (tested on 6.16)

# Build instructions
- clone repository
- mkdir ftsg4-build
- cd ftsg4-build
- cmake -DGeant4_DIR=/PATH/TO/geant4.10.05.p01-install/lib/Geant4-10.5.1 /PATH/TO/ftsg4/
- make

