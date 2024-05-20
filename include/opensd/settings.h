#ifndef OPENSD_SETTINGS_H
#define OPENSD_SETTINGS_H

//! \file settings.h
//! \brief Settings for OpenSD


namespace opensd {

//==============================================================================
// Global variable declarations
//==============================================================================

namespace settings {

// extern RunMode run_mode;       //!< Run mode (eigenvalue, fixed src, etc.)
// extern SolverType solver_type; //!< Solver Type (Monte Carlo or Random Ray)

} // namespace settings

//==============================================================================
// Functions
//==============================================================================

//! Read settings from XML file
void read_settings_xml();

} // namespace openmc

#endif // OPENSD_SETTINGS_H
