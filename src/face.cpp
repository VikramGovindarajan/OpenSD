//! \file face.cpp
#include "opensd/face.h"

#include <iostream>

#include "opensd/circuit.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
// Face implementation
//==============================================================================

Face::Face(int faceno, Node* unode, double ufrac, Node* dnode, double dfrac)
    : faceno(faceno), unode(unode), ufrac(ufrac), dnode(dnode), dfrac(dfrac),
      vflow_old{1.0E-8}, mflow(0.0), velocity(0.0), choked(false), presidue(0.0), Gcr(1.0E8), pcr(0.0),
      heat_input_old(0.0), heat_input(0.0), heat_hslab_old{} {
  // if (ufrac != nullptr && typeid(*unode) == typeid(Reservoir)) {
    // uheight = ufrac * unode->height;
  // }
  // if (dfrac != nullptr && typeid(*dnode) == typeid(Reservoir)) {
    // dheight = dfrac * dnode->height;
  // }
}


//==============================================================================
// PFace implementation
//==============================================================================


PFace::PFace(int faceno, Pipe pipe, Node* unode, double ufrac, Node* dnode, double dfrac,
            double diameter, double cfarea, double delx, double delz, double fricopt, double roughness)
  : Face(faceno, unode, ufrac, dnode, dfrac),
    circuit(pipe.circuit), pipe(pipe), diameter(diameter), cfarea(cfarea),
    delx(delx), delz(delz), roughness(roughness), Re(0.0), fricopt(fricopt),
    fricfact_old(64.0), opening(1.0) {
  this->vflow_gues = 1.E-8;
  circuit->faces.push_back(this);
}

double PFace::eqn_mom(double x, double time, double delt, double trans_sim, double alpha_mom) {
/*   delp_fr = fricfact_gues * delx * ther_gues.rhomass() * x * std::abs(x) / (2. * diameter * cfarea * cfarea);
  if (faceno == 0) {
    delp_fr += pipe.Kforward * ther_gues.rhomass() * x * std::abs(x) / (2. * cfarea * cfarea);
  }
  delp_gr = ther_gues.rhomass() * const.grav * delz;

  double Term_old = ((1. - alpha_mom) * ((downstream.tpres_old - upstream.tpres_old) 
                    - vflow_old * vflow_old / (2. * cfarea * cfarea) * (downstream.rhomass_old - upstream.rhomass_old) 
                    + ther_old.rhomass() * const.grav * delz
                    + fricfact_old * delx * ther_old.rhomass() * vflow_old * std::abs(vflow_old) / (2. * diameter * cfarea * cfarea)));
  if (faceno == 0) {
    Term_old += (1. - alpha_mom) * pipe.Kforward_old * ther_old.rhomass() * vflow_old * std::abs(vflow_old) / (2. * cfarea * cfarea);
  }

  double y = (trans_sim * delx * ther_gues.rhomass() * (x - vflow_old) / (delt * cfarea) 
            + alpha_mom * (downstream.tpres_gues - upstream.tpres_gues 
                          - vflow_gues * vflow_gues / (2. * cfarea * cfarea) * (downstream.rhomass_gues - upstream.rhomass_gues) 
                          + delp_gr 
                          + delp_fr) 
            + Term_old);
 */  return 1.;
}


} // namespace opensd