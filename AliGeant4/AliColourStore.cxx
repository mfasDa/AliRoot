// $Id$
// Category: visualization
//
// Author: I. Hrivnacova
//
// Class AliColourStore
// --------------------
// See the class description in the header file.

#include "AliColourStore.h"
#include "AliColour.h"
#include "AliGlobals.h"

#include <G4Element.hh>

// static data members

//_____________________________________________________________________________
AliColourStore* AliColourStore::fgInstance = 0;

// lifecycle

//_____________________________________________________________________________
AliColourStore::AliColourStore() {
//
  fColours.push_back(AliColour("White",     1.0, 1.0, 1.0));    
  fColours.push_back(AliColour("Black",     0.0, 0.0, 0.0));     
  fColours.push_back(AliColour("Red",       1.0, 0.0, 0.0));   
  fColours.push_back(AliColour("RoseDark",  1.0, 0.0, 0.5));  
  fColours.push_back(AliColour("Green",     0.0, 1.0, 0.0));     
  fColours.push_back(AliColour("Green2",    0.0, 1.0, 0.5));     
  fColours.push_back(AliColour("GreenClair",0.5, 1.0, 0.0));
  fColours.push_back(AliColour("Yellow",    1.0, 1.0, 0.0));     
  fColours.push_back(AliColour("BlueDark",  0.0, 0.0, 1.0)); 
  fColours.push_back(AliColour("BlueClair", 0.0, 1.0, 1.0)); 
  fColours.push_back(AliColour("BlueClair2",0.0, 0.5, 1.0));
  fColours.push_back(AliColour("Magenta",   1.0, 0.0, 1.0));    
  fColours.push_back(AliColour("Magenta2",  0.5, 0.0, 1.0));   
  fColours.push_back(AliColour("BrownClair",1.0, 0.5, 0.0));
  fColours.push_back(AliColour("Gray",      0.3, 0.3, 0.3));    
  fColours.push_back(AliColour("GrayClair", 0.6, 0.6, 0.6));
}

//_____________________________________________________________________________
AliColourStore::AliColourStore(const AliColourStore& right) {
// 
  AliGlobals::Exception(
    "Attempt to copy AliColourStore singleton.");
}

//_____________________________________________________________________________
AliColourStore::~AliColourStore() {
//
}

// operators

//_____________________________________________________________________________
AliColourStore& AliColourStore::operator=(const AliColourStore& right)
{
  // check assignement to self
  if (this == &right) return *this;

  AliGlobals::Exception(
    "Attempt to assign AliColourStore singleton.");
    
  return *this;  
}    

// static methods
  
//_____________________________________________________________________________
AliColourStore* AliColourStore::Instance() 
{
// Returns the singleton instance.
// Creates the instance if it does not exist.
// ---

  if (fgInstance == 0 )
    fgInstance = new AliColourStore();
  
  return fgInstance;
}

// public methods

//_____________________________________________________________________________
G4Colour AliColourStore::GetColour(G4String name) const
{
// Retrieves the colour by name.
// ---

  ColourConstIterator it;  
  for (it = fColours.begin(); it != fColours.end(); it++) 
    if (name == (*it).GetName()) return (*it).GetColour();
  
  G4String text = "Colour " + name + " is not defined.";
  AliGlobals::Exception(text);
  return 0;
}
    
//_____________________________________________________________________________
G4String AliColourStore::GetColoursList() const
{
// Returns the list of all defined colours names.
// ---

  G4String list = "";
  ColourConstIterator it;

  for (it = fColours.begin(); it != fColours.end(); it++) {
    list += (*it).GetName();
    list += " ";
  }
  
  return list;
} 
       
//_____________________________________________________________________________
G4String AliColourStore::GetColoursListWithCommas() const
{
// Returns the list of all defined colours names
// with commas.
// ---

  G4String list = "";
  G4int i = 0;
  ColourConstIterator it;

  for (it = fColours.begin(); it != fColours.end(); it++) {
    list += (*it).GetName();
    if (i++ < fColours.size()-1) list += ", ";
  }
  
  return list;
} 
