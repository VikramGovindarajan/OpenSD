#ifndef MASSMOM_H
#define MASSMOM_H

#include <vector>
#include <Eigen/Dense>
// #include "CoolProp.h"

namespace opensd {

void exec_massmom(double time, double delt, bool trans_sim, double alpha_mom, int main_iter, int flow_iter);

} // namespace opensd

#endif // MASSMOM_H
