#ifndef ALIITSSIMULATION_H
#define ALIITSSIMULATION_H
/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
/* $Id$ */

#include <TObject.h>

class AliITSresponse;
class AliITSsegmentation;
class AliITSmodule;
class TRandom;

// This is the base class for ITS detector signal simulations. Data members
// include are a pointer to the detectors specific response and segmentation
// classes. See the detector specific implementations for the propper code.

class AliITSsimulation : public TObject {

 public:
    AliITSsimulation(); // Default constructor
    virtual ~AliITSsimulation() {}// destructor
    // copy constructor. See detector specific implementation.
    AliITSsimulation(const AliITSsimulation &source);
    // Assignment opporator. See detector specific implementation.
    virtual AliITSsimulation& operator=(const AliITSsimulation &source);
    // digitize module using the "slow" detector simulator creating
    // summable digits.
    virtual void SDigitiseModule(AliITSmodule *mod,Int_t module,Int_t event){;}
    // digitize module using the "slow" detector simulator creating digits.
    virtual void DigitiseModule(AliITSmodule *mod,Int_t module,Int_t event) {;}
    // digitizes module using the "fast" detector simulator.
    virtual void CreateFastRecPoints(AliITSmodule *mod,Int_t module,
				     TRandom *rndm) {}

protected:

  AliITSresponse      *fResponse;       //! response
  AliITSsegmentation  *fSegmentation;   //! segmentation

  ClassDef(AliITSsimulation,1)  // Simulation base class 
    
};

#endif
