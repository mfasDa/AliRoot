// $Id$
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4ExtDecayer
// -------------------
// TG4ExtDecayer class implements the G4VExtDecayer abstract class
// with the AliDecayer.
// In case a particle has not defined any decay channel
// and has not pre-assigned decay products,
// the external decayer is called.

#ifndef TG4_EXT_DECAYER_H
#define TG4_EXT_DECAYER_H

#include <G4VExtDecayer.hh>
#include <globals.hh>

class AliDecayer;
class TG4ParticlesManager;

class G4Track;
class G4DecayProducts;

class TClonesArray;

class TG4ExtDecayer : public G4VExtDecayer
{
  public:
    TG4ExtDecayer(AliDecayer* externalDecayer);
    // --> protected
    //TG4ExtDecayer(const TG4ExtDecayer& right);
    virtual ~TG4ExtDecayer();

    virtual G4DecayProducts* ImportDecayProducts(const G4Track& track);
    
    // set methods
    void SetVerboseLevel(G4int verbose);
    
    // get methods
    G4int GetVerboseLevel() const;
    
  protected:  
    TG4ExtDecayer(const TG4ExtDecayer& right);

    // operators
    TG4ExtDecayer& operator=(const TG4ExtDecayer& right);

  private:
    TG4ParticlesManager* fParticlesManager;  //particles manager 
    AliDecayer*          fExternalDecayer;   //the AliDecayer
    TClonesArray*        fDecayProductsArray;//array of decay products
    G4int                fVerboseLevel;      //verbose level         
};

// inline methods

inline void TG4ExtDecayer::SetVerboseLevel(G4int verbose)
{ fVerboseLevel = verbose; }

inline G4int TG4ExtDecayer::GetVerboseLevel() const
{ return fVerboseLevel; }

#endif //TG4_EXT_DECAYER_H
