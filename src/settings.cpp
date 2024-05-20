#include "opensd/settings.h"

#include <string>

#include "opensd/error.h"

namespace opensd {
    
//==============================================================================
// Global variables
//==============================================================================

namespace settings {

  std::string path_input;

} // namespace settings

//==============================================================================
// Functions
//==============================================================================
    
void read_settings_xml()
{
  using namespace settings;
  using namespace pugi;
  
  std::string filename = settings::path_input + "settings.xml";
/*   if (!file_exists(filename)) {
    if (run_mode != RunMode::PLOTTING) {
      fatal_error("Could not find any XML input files! In order to run OpenMC, "
                  "you first need a set of input files; at a minimum, this "
                  "includes settings.xml, geometry.xml, and materials.xml or a "
                  "single model XML file. Please consult the user's guide at "
                  "https://docs.openmc.org for further information.");
    } else {
      // The settings.xml file is optional if we just want to make a plot.
      return;
    }
  }
 */

  // Parse settings.xml file
  xml_document doc;
  auto result = doc.load_file(filename.c_str());
  if (!result) {
    fatal_error("Error processing settings.xml file.");
  }




}

} // namespace openmc
