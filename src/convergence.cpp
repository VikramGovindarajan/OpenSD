//! \file convergence.cpp

#include "opensd/convergence.h"

#include <iostream>

#include "opensd/initialize.h"

namespace opensd {
    
bool check_conv(double time, double delt, bool trans_sim, double alpha_mom, std::string opt) {
  double eps_mtot = 0.0, eps_ptot = 0.0, eps_htot = 0.0, eps_ttot = 0.0;

  for (auto& circuit : model::circuits) {
    
    vector<double> eps_mlist;
    for (auto& node : circuit.nodes) {
      node.mresidue = node.eqn_cont(alpha_mom);
      // if (dynamic_cast<comp::Reservoir*>(&node)) node.mresidue = 0;
      if (std::abs(node.mflow_in) > 1.E-5 || std::abs(node.mflow_out) > 1.E-5) {
        eps_mlist.push_back(std::abs(node.mresidue));
      }
    }

    circuit.eps_p = 0.0;
    std::vector<double> e_mass;
    for (auto& face : circuit.faces) {
      face.presidue = face.eqn_mom(face.vflow_gues, time, delt, trans_sim, alpha_mom);
      // circuit.eps_p += std::abs(face.presidue) / face.tpres_gues;
      // face.mflow = face.vflow_gues * face.ther_gues.rhomass();
      // if (std::abs(face.mflow) > 1.0E-5) {
        // e_mass.push_back(std::abs(face.mflow));
      // }
      // face.mflow = face.vflow_gues * face.ther_gues.rhomass();
    }
    for (auto& pipe : circuit.pipes) {
      // pipe.update_mflow();
    }

    
    
    eps_mtot = std::max(eps_mtot, circuit.eps_m);
    eps_ptot = std::max(eps_ptot, circuit.eps_p);
    if (opt == "all") {
      eps_htot = std::max(eps_htot, circuit.eps_h);
    }
  }

  bool condition = false;
  // tuple<double, double, double, double, double> ret_args(eps_mtot, eps_ptot, eps_htot, eps_ttot);

  if (condition) {
    return true;
  } else {
    return false;
  }
}

}