//! \file simulation.h
//! \brief Variables/functions related to a running simulation

#ifndef OPENSD_SIMULATION_H
#define OPENSD_SIMULATION_H

namespace opensd {

//==============================================================================
// Global variable declarations
//==============================================================================

namespace simulation {

extern "C" double current_time; //!< current time
extern "C" double delt; //!< time step

} // namespace simulation

//==============================================================================
// Functions
//==============================================================================

} // namespace opensd

#endif // OPENSD_SIMULATION_H
