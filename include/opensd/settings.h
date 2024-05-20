#ifndef OPENSD_SETTINGS_H
#define OPENSD_SETTINGS_H

//! \file settings.h
//! \brief Settings for OpenSD

#include <string>

#include "pugixml.hpp"

namespace opensd {

//==============================================================================
// Global variable declarations
//==============================================================================

namespace settings {

extern std::string path_input;  //!< directory where main .xml files resides
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
