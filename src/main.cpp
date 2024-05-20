#include <iostream>
#include "CoolProp.h"

namespace openmc {
    
void read_separate_xml_files()
{
  // read_settings_xml();
  // read_geometry_xml();

}

} // namespace openmc

int main()
{
    using namespace openmc;
    std::cout << CoolProp::PropsSI("T","P",101325,"Q",0,"Water") << std::endl;
    read_separate_xml_files();
	return 1;
}
