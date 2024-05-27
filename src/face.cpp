//! \file face.cpp
#include "opensd/face.h"

#include <iostream>

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
// PFace implementation
//==============================================================================

PFace::PFace(int faceno, Pipe pipe, Node* unode, double ufrac, Node* dnode, double dfrac, 
  double diameter, double cfarea, double delx, double delz, double fricopt, double roughness) {
  this->vflow_gues = 1.E-8;
}

} // namespace opensd
