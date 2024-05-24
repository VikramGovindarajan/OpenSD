//! \file convergence.cpp

#include "opensd/convergence.h"

#include <iostream>

#include "opensd/circuit.h"

namespace opensd {
    
bool check_conv(double time, double delt, bool trans_sim, double alpha_mom, std::string opt) {
  double eps_mtot = 0.0, eps_ptot = 0.0, eps_htot = 0.0, eps_ttot = 0.0;

  for (auto circuit : model::circuits) {
    
    std::vector<double> eps_mlist;
    for (auto node : circuit.nodes) {
      node.mresidue = node.eqn_cont();
      // if (dynamic_cast<comp::Reservoir*>(&node)) node.mresidue = 0;
      // if (std::abs(node.mflow_in) > 1.E-5 || std::abs(node.mflow_out) > 1.E-5) {
        // eps_mlist.push_back(std::abs(node.mresidue));
      // }
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