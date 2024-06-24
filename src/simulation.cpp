//! \file simulation.cpp
#include "opensd/simulation.h"

#include <iostream>
#include <ctime>
#include <cstdlib> // for std::exit
#include <cmath> // for rounding function
#include <iomanip> // for setting precision in output

#include "opensd/settings.h"
#include "opensd/convergence.h"
#include "opensd/flow_solver.h"

//==============================================================================
// C API functions
//==============================================================================

// OPENSD_RUN encompasses all the main logic where iterations are performed
// over the time steps 

int opensd_run()
{
  using namespace opensd;

  std::clock_t start_time;
  start_time = std::clock();

  // Ensure that a timestep isn't executed in the case that the maximum number of
  // time steps has already been run in a restart statepoint file

  // opensd::settings::alpha_mom

  // Loop through time slots
  for (int i = 0; i < settings::tim_slot.size(); ++i) {
    simulation::current_time = settings::tim_slot[i];
    if (i != 0) {
      simulation::delt = settings::tim_slot[i] - settings::tim_slot[i-1];
    }
    
    if (settings::verbosity >= 1) std::cout << "time=" << std::setprecision(5) << simulation::current_time << " ";
    
    // action_setup.update(time, delt);
    bool trans_sim = false;
    bool converged;
    double eps_m, eps_p, eps_h, eps_t;
    for (int main_iter = 0; main_iter < settings::no_main_iter; ++main_iter) {
      
      for (int flow_iter = 0; flow_iter < settings::no_flow_iter; ++flow_iter) {
        exec_massmom(simulation::current_time, simulation::delt, trans_sim, settings::alpha_mom, main_iter, flow_iter);
        std::tuple<bool, std::tuple<double, double>> result = check_conv(simulation::current_time, simulation::delt, trans_sim, settings::alpha_mom, "massmom");
        converged = std::get<0>(result);
        std::tie(eps_m, eps_p) = std::get<1>(result);
        if (converged) {
          if (settings::verbosity >= 3 || (settings::verbosity >= 2 && !trans_sim)) {
            std::cout << "massmom converged in " << flow_iter + 1 << " iter. " << eps_m << " " << eps_p << std::endl;
            std::exit(0);
          }
          break;
        } else {
          if (settings::verbosity >= 3 || (settings::verbosity >= 2 && !trans_sim)) {
            std::cout << "massmom iteration " << flow_iter + 1 << " " << eps_m << " " << eps_p << std::endl;
          }
        }
      }
      
      if (!converged) {
        std::cerr << "massmom not converged. stopping " << eps_m << " " << eps_p << std::endl;
        std::exit(EXIT_FAILURE);
      }

      // vof_solver.update_int(time);

      // if (solver_settings.temp_solve) {
        // HT_solver.exec_energy(time, delt, trans_sim, alpha_heat, main_iter);
        // if (!trans_sim) {
          // slug_flow_solver.exec_ss(time, delt, trans_sim, alpha_mom);
        // }
        // flow_solver.exec_energy(time, delt, trans_sim, alpha_ener, main_iter);
        // vof_solver.exec_energy(time, delt, trans_sim, alpha_ener);

        // bool converged;
        // double eps_h, eps_t, eps_hvof;
        // std::tie(converged, std::tie(eps_m, eps_p, eps_h, eps_t, eps_hvof)) = convergence.check_conv(
            // time, delt, trans_sim, alpha_mom, alpha_ener, "all", alpha_heat);

        if (converged && main_iter > 1) {
          // if (trans_sim) {
            // slug_flow_solver.exec_trans(time, delt, trans_sim, alpha_mom, alpha_heat, main_iter);
          // }
          // if (solver_settings.temp_solve) {
            // if (verbosity >= 1 || (verbosity >= 0 && !trans_sim)) {
              // std::cout << "main converged in " << main_iter + 1 << " iter. " << eps_m << " " << eps_p << " " << eps_h << " " << eps_t << " " << eps_hvof << std::endl;
            // }
          // } else {
            if (settings::verbosity >= 1 || (settings::verbosity >= 0 && !trans_sim)) {
              std::cout << "main converged in " << main_iter + 1 << " iter. " << eps_m << " " << eps_p << std::endl;
            }
          // }
          // break;
        } else {
          if (settings::verbosity >= 2 || (settings::verbosity >= 1 && !trans_sim)) {
            std::cout << "main iteration " << main_iter + 1 << " " << eps_m << " " << eps_p << " " << eps_h << " " << eps_t << std::endl;
          }
        }
      // }
    }

    if (!converged) {
      std::cerr << "main not converged. stopping " << eps_m << " " << eps_p << " " << eps_h << " " << eps_t << " " << std::endl;
      std::exit(EXIT_FAILURE);
    }

    // for (auto& lmass : HTcomp.LumpedMass::_registry) {
      // lmass.update(time, delt);
    // }

    // post.update_calcs(time, delt);

    // if (flag_write) {
      // post.write_output(time, delt);
    // }
  }
  
  std::cout << "Execution time = " << (std::clock() - start_time) / (double)CLOCKS_PER_SEC << std::endl;
    
  return 0;

}

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

namespace simulation {

double current_time; //!< current time
double delt {1.E8}; //!< time step

} // namespace simulation

//==============================================================================
// Non-member functions
//==============================================================================

} // namespace opensd
