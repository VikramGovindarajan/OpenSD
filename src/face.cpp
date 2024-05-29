//! \file face.cpp
#include "opensd/face.h"

#include <iostream>
#include <cmath>
// #include <cstdlib>

#include "opensd/circuit.h"
#include "opensd/constants.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
// Face implementation
//==============================================================================

Face::Face(int faceno, Node* unode, double ufrac, Node* dnode, double dfrac)
    : faceno(faceno), unode(unode), ufrac(ufrac), dnode(dnode), dfrac(dfrac),
      vflow_old{1.0E-8}, vflow_gues{1.0E-8}, mflow(0.0), velocity(0.0), choked(false), 
	  presidue(0.0), Gcr(1.0E8), pcr(0.0),heat_input_old(0.0), heat_input(0.0),
	  heat_hslab_old{} {
  // if (ufrac != nullptr && typeid(*unode) == typeid(Reservoir)) {
    // uheight = ufrac * unode->height;
  // }
  // if (dfrac != nullptr && typeid(*dnode) == typeid(Reservoir)) {
    // dheight = dfrac * dnode->height;
  // }
}

void Face::assign_statevar() {
  tpres_old = 0.5 * (unode->tpres_old + dnode->tpres_old);
  spres_old = 0.5 * (unode->spres_old + dnode->spres_old);
  ttemp_old = 0.5 * (unode->ttemp_old + dnode->ttemp_old);
  stemp_old = 0.5 * (unode->stemp_old + dnode->stemp_old);
}

void Face::update_gues() {
  vflow_gues = vflow_old;
  tpres_gues = tpres_old;
  spres_gues = spres_old;
  ttemp_gues = ttemp_old;
  stemp_gues = stemp_old;
}


//==============================================================================
// PFace implementation
//==============================================================================


PFace::PFace(int faceno, Pipe& pipe, Node* unode, double ufrac, Node* dnode, double dfrac,
            double diameter, double cfarea, double delx, double delz, double fricopt, double roughness)
  : Face(faceno, unode, ufrac, dnode, dfrac),
    circuit(pipe.circuit), pipe(pipe), diameter(diameter), cfarea(cfarea),
    delx(delx), delz(delz), roughness(roughness), Re(0.0), fricopt(fricopt),
    fricfact_old(64.0), fricfact_gues(64.0), opening(1.0) {
  circuit->faces.push_back(*this);
}

double PFace::eqn_mom(double x, double time, double delt, bool trans_sim, double alpha_mom) {
  Re = 1000.*x*diameter/(cfarea*0.001);
  fricfact_gues = 0.316/std::pow(Re,0.25);
  double delp_fr = fricfact_gues * delx * 1000. * x * std::abs(x) / (2. * diameter * cfarea * cfarea); //ther_gues.rhomass()
/*  if (faceno == 0) {
    delp_fr += pipe.Kforward * ther_gues.rhomass() * x * std::abs(x) / (2. * cfarea * cfarea);
  }
*/
  double delp_gr = 1000. * grav * delz; //ther_gues.rhomass()
  // double term_old = ((1. - alpha_mom) * ((downstream.tpres_old - upstream.tpres_old) 
                    // - vflow_old * vflow_old / (2. * cfarea * cfarea) * (downstream.rhomass_old - upstream.rhomass_old) 
                    // + ther_old.rhomass() * const.grav * delz
                    // + fricfact_old * delx * ther_old.rhomass() * vflow_old * std::abs(vflow_old) / (2. * diameter * cfarea * cfarea)));
  double term_old = 0.;
/*
  if (faceno == 0) {
    Term_old += (1. - alpha_mom) * pipe.Kforward_old * ther_old.rhomass() * vflow_old * std::abs(vflow_old) / (2. * cfarea * cfarea);
  }
*/

  double y = (trans_sim * delx * 1000. * (x - vflow_old) / (delt * cfarea)  //ther_gues.rhomass()
            + alpha_mom * (dnode->tpres_gues - unode->tpres_gues //downstream.tpres_gues - upstream.tpres_gues 
                          - vflow_gues * vflow_gues / (2. * cfarea * cfarea) * 0. //(downstream.rhomass_gues - upstream.rhomass_gues) 
                          + delp_gr 
                          + delp_fr) 
            + term_old);
  return y;
}


void PFace::update_abcoef(double time, double delt, double trans_sim, double alpha_mom) {
  if (!choked) {
    double A = 0;
    // if (dnode.ther_gues.phase() == 6) {
      // A = pow(vflow_gues, 2) / (2 * pow(cfarea, 2)) * dnode.spres_gues / dnode.tpres_gues * dnode.ther_gues.first_two_phase_deriv(1, 2, 3); // replace iDmass, iP, iHmass with appropriate values
    // } else {
      // A = pow(vflow_gues, 2) / (2 * pow(cfarea, 2)) * dnode.spres_gues / dnode.tpres_gues * dnode.ther_gues.first_partial_deriv(1, 2, 3); // replace iDmass, iP, iHmass with appropriate values
    // }

    double dr = 0.;
    dr = (trans_sim * 1000. * delx / (cfarea * delt) //dnode.ther_gues.rhomass()
                 + alpha_mom * (2 * (fricfact_gues * delx / diameter) * 1000. * fabs(vflow_gues) / (2 * pow(cfarea, 2)) //dnode.ther_gues.rhomass()
                                + 0.0 * 2.0 * vflow_gues * (1000. - 1000.) / (2 * pow(cfarea, 2)))); //unode.ther_gues.rhomass() - dnode.ther_gues.rhomass()

    if (faceno == 0) {
      dr += alpha_mom * 2.0 * pipe.Kforward * 1000. * fabs(vflow_gues) / (2 * pow(cfarea, 2)); //dnode.ther_gues.rhomass()
    }

    // double aplus = ((alpha_mom * (1.0 - A * 0.0)
              // + (spres_gues / tpres_gues * delx * 0.5 * dnode.ther_gues.drho_dp_consth() *
                 // (trans_sim * (vflow_gues - vflow_old) / (cfarea * delt) + 0.0 * alpha_mom * 9.81 * delz / delx + alpha_mom * (fricfact_gues / diameter) * vflow_gues * fabs(vflow_gues) / (2 * pow(cfarea, 2)))))
             // / dr);

    // if (faceno == 0) {
      // aplus += (spres_gues / tpres_gues * 0.5 * dnode.ther_gues.drho_dp_consth() *
                // alpha_mom * pipe.Kforward * vflow_gues * fabs(vflow_gues) / (2 * pow(cfarea, 2))) / dr;
    // }

    // double B;
    // if (unode.ther_gues.phase() == 6) {
      // B = pow(vflow_gues, 2) / (2 * pow(cfarea, 2)) * unode.spres_gues / unode.tpres_gues * unode.ther_gues.first_two_phase_deriv(1, 2, 3); // replace iDmass, iP, iHmass with appropriate values
    // } else {
      // B = pow(vflow_gues, 2) / (2 * pow(cfarea, 2)) * unode.spres_gues / unode.tpres_gues * unode.ther_gues.first_partial_deriv(1, 2, 3); // replace iDmass, iP, iHmass with appropriate values
    // }

    // aminus = ((alpha_mom * (1.0 - B * 0.0)
               // - (spres_gues / tpres_gues * delx * 0.5 * dnode.ther_gues.drho_dp_consth() *
                  // (trans_sim * (vflow_gues - vflow_old) / (cfarea * delt) + 0.0 * alpha_mom * 9.81 * delz / delx + alpha_mom * fricfact_gues * vflow_gues * fabs(vflow_gues) / (2 * diameter * pow(cfarea, 2)))))
              // / dr);

    // if (faceno == 0) {
      // aminus -= (spres_gues / tpres_gues * 0.5 * dnode.ther_gues.drho_dp_consth() *
                 // alpha_mom * pipe.Kforward * vflow_gues * fabs(vflow_gues) / (2 * pow(cfarea, 2))) / dr;
    // }

    // bplus = bminus = spres_gues / tpres_gues * 0.5 * dnode.ther_gues.drho_dp_consth();

    // if (aplus < 0.0 || aminus < 0.0) {
      // if ((true && trans_sim) || (!trans_sim)) { // replace true with appropriate condition if `show_warn` is a variable
        // std::cout << "warning. acoef negative." << pipe.identifier << faceno << aplus << aminus << unode.ther_gues.rhomass() << dnode.ther_gues.rhomass() << A << B << dr << vflow_gues << std::endl;
      // }
    // }

    // if (bplus < 0) {
      // std::cout << spres_gues << tpres_gues << dnode.ther_gues.drho_dp_consth() << std::endl;
      // std::exit(EXIT_FAILURE);
    // }
  } else {
/*     double delta = 0.1;
    double y1 = Gcr / rhocr;
    auto& flstate = circuit.flstate;
    flstate.update(1, 1, 1 + delta); // replace HmassP_INPUTS, upstream.tenth_gues, upstream.tpres_gues with appropriate values
    double Gcr2, pcr2, rhocr2;
    std::tie(Gcr2, pcr2, rhocr2) = GcrHEM(flstate); // replace with appropriate function call
    double y2 = Gcr2 / rhocr2;
    aminus = (y2 - y1) / delta * 0.1;
    bminus = (rhocr2 - rhocr) / delta * 0.1;
 */  }
}


} // namespace opensd
