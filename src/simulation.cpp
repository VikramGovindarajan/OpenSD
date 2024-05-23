#include "opensd/simulation.h"

#include <iostream>
#include <ctime>

#include "opensd/settings.h"
#include "opensd/convergence.h"

//==============================================================================
// C API functions
//==============================================================================

// OPENSD_RUN encompasses all the main logic where iterations are performed
// over the time steps 

int opensd_run()
{
    std::clock_t start_time;
    start_time = std::clock();

  // Ensure that a timestep isn't executed in the case that the maximum number of
  // time steps has already been run in a restart statepoint file

  // opensd::settings::alpha_mom

  // Loop through time slots
  for (int i = 0; i < opensd::settings::tim_slot.size(); ++i) {
    opensd::simulation::current_time = opensd::settings::tim_slot[i];
    if (i != 0) {
      opensd::simulation::delt = opensd::settings::tim_slot[i] - opensd::settings::tim_slot[i - 1];
    }
 
    for (int main_iter = 0; main_iter < opensd::settings::no_main_iter; ++main_iter) {
    
      for (int flow_iter = 0; flow_iter < opensd::settings::no_flow_iter; ++flow_iter) {

      }
    
      bool converged = opensd::check_conv(opensd::simulation::current_time, opensd::simulation::delt, false, opensd::settings::alpha_mom, "massmom");
      if (converged) {
        
      } else {
        
      }
 
    }
    
/*     // Your code for post-processing and writing output
    
    if (!trans_sim && false) {
        // Your code for non-transient simulation
    }
 */
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
