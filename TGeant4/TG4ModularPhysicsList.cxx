// $Id$
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4ModularPhysicsList
// ---------------------------
// See the class description in the header file.

#include "TG4ModularPhysicsList.h"
#include "TG4GeometryServices.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3ControlVector.h"
#include "TG4ProcessControlMap.h"
#include "TG4ExtDecayer.h"
#include "AliDecayer.h"
#include "AliMC.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4BosonConstructor.hh>
#include <G4LeptonConstructor.hh>
#include <G4MesonConstructor.hh>
#include <G4BaryonConstructor.hh>
#include <G4IonConstructor.hh>
#include <G4ShortLivedConstructor.hh>
#include <G4ProcessTable.hh>
#include <G4Decay.hh>

const G4double TG4ModularPhysicsList::fgkDefaultCutValue = 1.0 * mm;

//_____________________________________________________________________________
TG4ModularPhysicsList::TG4ModularPhysicsList()
  : G4VModularPhysicsList() {
//
  defaultCutValue = fgkDefaultCutValue;

  SetVerboseLevel(2);
}

//_____________________________________________________________________________
TG4ModularPhysicsList::TG4ModularPhysicsList(const TG4ModularPhysicsList& right)
{
//
  TG4Globals::Exception("TG4ModularPhysicsList is protected from copying.");
}

//_____________________________________________________________________________
TG4ModularPhysicsList::~TG4ModularPhysicsList() {
//
  //delete fExtDecayer;
       // fExtDecayer is deleted in G4Decay destructor
}

// operators

//_____________________________________________________________________________
TG4ModularPhysicsList& 
TG4ModularPhysicsList::operator=(const TG4ModularPhysicsList &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception("TG4ModularPhysicsList is protected from assigning.");

  return *this;
}

// private methods

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetProcessActivation(
                                      G4ProcessManager* processManager,
                                      G4int  processId, G4bool activation)
{				      
// Sets process activation for the given process.
// ---

  G4String strActivation = "activation";
  if (!activation) strActivation = "inactivation";

  if (verboseLevel>1) {
    G4cout << "Set process " << strActivation << " for " 
           << (*processManager->GetProcessList())[processId]->GetProcessName() 
	   << G4endl;
  }
  
  processManager->SetProcessActivation(processId, activation);	
}  

// protected methods

//_____________________________________________________________________________
void TG4ModularPhysicsList::ConstructParticle()
{
// In this method, static member functions should be called
// for all particles which you want to use.
// This ensures that objects of these particle types will be
// created in the program. 
// ---

  // lock physics manager
  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  g3PhysicsManager->Lock();  
  
  // create particles for registered physics
  G4VModularPhysicsList::ConstructParticle();
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::ConstructProcess()
{
// Constructs all processes.
// ---

  // create processes for registered physics
  G4VModularPhysicsList::ConstructProcess();

  // verbose
  if (verboseLevel>1) DumpAllProcesses();
}


// public methods

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetCuts()
{
// Sets the default cut value for all particle types
// other then e-/e+. 
// The cut value for e-/e+ is high in oredr to supress
// tracking of delta electrons.
// ---

  // SetCutsWithDefault();   
         // "G4VUserPhysicsList::SetCutsWithDefault" method sets 
         // the default cut value for all particle types.

  // default cut value
  G4double cut  = defaultCutValue;
  //G4double ecut = 10.*m; 
  G4double ecut = cut; 

#ifdef G4VERBOSE    
  if (verboseLevel >1){
    G4cout << "TG4ModularPhysicsList::SetCutsWithDefault:";
    G4cout << "CutLength : " << cut/mm << " (mm)" << G4endl;
  }  
#endif

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma 
  SetCutValue(cut, "gamma");
  SetCutValue(ecut, "e-");
  SetCutValue(ecut, "e+");
 
  // set cut values for proton and anti_proton before all other hadrons
  // because some processes for hadrons need cut values for proton/anti_proton 
  SetCutValue(cut, "proton");
  SetCutValue(cut, "anti_proton");
  
  SetCutValueForOthers(cut);

  if (verboseLevel>1) {
    DumpCutValuesTable();
  }
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetProcessActivation()
{
// (In)Activates built processes according
// to the setup in TG4G3PhysicsManager::fControlVector.
// ---

  TG4G3ControlVector* controlVector 
    = TG4G3PhysicsManager::Instance()->GetControlVector();

  G4bool specialControls 
    = TG4GeometryServices::Instance()->IsSpecialControls();  

  if (!specialControls) 
    G4cout << G4endl
           << "### No special controls in user limits are set." << G4endl;

  if (!controlVector) {
    G4String text = "TG4ModularPhysicsList::SetProcessActivation: \n";
    text = text + "    Vector of processes controls is not set.";
    TG4Globals::Warning(text);
    return;
  }    
  
  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ProcessManager* processManager 
      = theParticleIterator->value()->GetProcessManager(); 
      
    G4ProcessVector* processVector = processManager->GetProcessList();
  
    // set processes controls
    for (G4int i=0; i<processVector->length(); i++) {

      TG4G3ControlValue control
	 = controlVector->GetControlValue((*processVector)[i]);
      G4bool activation = processManager->GetProcessActivation(i);

      if (control != kUnset) {
        if (!TG4Globals::Compare(activation, control)) {

          // set new process activation
          G4bool activate;
          if (control == kInActivate) activate = false; 
	  else                        activate = true;
	  
	  SetProcessActivation(processManager, i, activate);         
        }
      }	
      else  {
        // control == kUnset
	if ((*processVector)[i]->GetProcessName().find("specialControl")
	    != G4String::npos) { 

	  SetProcessActivation(processManager, i, specialControls);         
        }  
      }	
    }
  }  
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::PrintAllProcesses() const
{
// Prints all processes.
// ---

  G4cout << "TG4ModularPhysicsList processes: " << G4endl;
  G4cout << "================================ " << G4endl;
 
  G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
  G4ProcessTable::G4ProcNameVector* processNameList 
    = processTable->GetNameList();

  for (G4int i=0; i <processNameList->size(); i++){
    G4cout << "   " << (*processNameList)[i] << G4endl;
  }  
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::DumpAllProcesses() const
{
// Dumps all particles and their processes.
// ---

  G4cout << "TG4ModularPhysicsList particles and processes: " << G4endl;
  G4cout << "============================================== " << G4endl;
 
  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    // print particle name
    G4cout << "Particle: " 
           << theParticleIterator->value()->GetParticleName()
	   << G4endl;

    // dump particle processes
    G4ProcessVector* processVector 
      = theParticleIterator->value()->GetProcessManager()->GetProcessList();
    for (G4int i=0; i<processVector->length(); i++)
      (*processVector)[i]->DumpInfo();
      
    G4cout << G4endl;  
  }  
}

