//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/**********************************************************************
     Version :  Geant4.10.02.p02
     Syncrotron Radiation Test with GDML Geometry
**********************************************************************/

#include <vector>

#include "G4GDMLParser.hh"
#include "G4ScoringManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "PhysicsList.hh"
#include "SteppingVerbose.hh"
#include "Randomize.hh"
#include "FTFP_BERT.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) {

/*----------------------------------------------------*/

G4cout << G4endl;
    G4cout << "Usage: load_gdml <intput_gdml_file:mandatory>" << G4endl;
    G4cout << G4endl;

    if (argc<2)
    {
        G4cout << "Error! Mandatory input file is not specified!" << G4endl;
        G4cout << G4endl;
        return -1;
    }

   // Detect interactive mode (if only one argument) and define UI session
   //
   G4UIExecutive* ui = 0;
   if ( argc == 2 ) {
     ui = new G4UIExecutive(argc, argv);
   }

    G4GDMLParser parser;

	// ACTIVATING OVERLAP CHECK when read volumes are placed.
    // Can take long time in case of complex geometries
    //parser.SetOverlapCheck(true);

    // Uncomment the following if wish to avoid names stripping
    //parser.SetStripFlag(false);

    parser.Read(argv[1]);

    if (argc>3)
    {
        G4cout << "Error! Too many arguments!" << G4endl;
        G4cout << G4endl;
        return -1;
    }
/*----------------------------------------------------*/

  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    G4int nThreads = G4Threading::G4GetNumberOfCores();
    if (argc==3) nThreads = G4UIcommand::ConvertToInt(argv[2]);
    runManager->SetNumberOfThreads(nThreads);
#else
    G4VSteppingVerbose::SetInstance(new SteppingVerbose);
    G4RunManager* runManager = new G4RunManager;
#endif

  //set mandatory initialization classes
  runManager->SetUserInitialization(new DetectorConstruction(parser));
  runManager->SetUserInitialization(new PhysicsList);

  //set user action classes
  runManager->SetUserInitialization(new ActionInitialization());
  runManager->Initialize();

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  //get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();

  // Process macro or start UI session
   if ( ! ui )   // batch mode  
   {
     G4String command = "/control/execute ";
     G4String fileName = argv[2];
     UI->ApplyCommand(command+fileName);
   }
   else           // interactive mode
   {
     UI->ApplyCommand("/control/execute vis.mac");     
     ui->SessionStart();
     delete ui;
   }

   delete visManager;
   delete runManager;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
