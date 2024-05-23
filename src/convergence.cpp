//! \file convergence.cpp

#include "opensd/convergence.h"
// #include "flow_components.h"

#include <iostream>

#include "opensd/circuit.h"

using namespace std;

namespace opensd {
    
bool check_conv(double time, double delt, bool trans_sim, double alpha_mom, string opt) {
  double eps_mtot = 0.0, eps_ptot = 0.0, eps_htot = 0.0, eps_ttot = 0.0;

  for (const auto& circuit : opensd::model::circuits) {
    std::cout << circuit->identifier_ << " " << circuit->flname_ << std::endl;
    
    eps_mtot = max(eps_mtot, circuit->eps_m);
    eps_ptot = max(eps_ptot, circuit->eps_p);
    if (opt == "all") {
      eps_htot = max(eps_htot, circuit->eps_h);
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