//! \file convergence.h

#ifndef CONVERGENCE_H
#define CONVERGENCE_H

#include <tuple>
#include <string>

namespace opensd {
    
bool check_conv(double time, double delt, bool trans_sim, double alpha_mom, std::string opt = "all");

}

#endif // CONVERGENCE_H
