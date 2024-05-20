#include "opensd/initialize.h"
#include "opensd/capi.h"
#include "opensd/constants.h"

int opensd_init(int argc, char* argv[])
{
  using namespace opensd;

  // Read XML input files
  // if (!read_model_xml())
    read_separate_xml_files();

  return 0;
}

namespace opensd {
    
void read_separate_xml_files()
{
  // read_settings_xml();
  // read_geometry_xml();

}

} // namespace opensd
