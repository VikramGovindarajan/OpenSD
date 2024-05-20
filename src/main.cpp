#include <iostream>
#include "CoolProp.h"

int main()
{
    std::cout << CoolProp::PropsSI("T","P",101325,"Q",0,"Water") << std::endl;
    read_separate_xml_files();
	return 1;
}

void read_separate_xml_files()
{
  // read_settings_xml();
  // read_model_xml();

}
