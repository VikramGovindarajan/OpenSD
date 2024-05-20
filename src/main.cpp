#include <iostream>
#include "CoolProp.h"
#include "opensd/capi.h"
#include "opensd/constants.h"

int main(int argc, char* argv[])
{
  
  using namespace opensd;
  int err;

  err = opensd_init(argc, argv);

  std::cout << CoolProp::PropsSI("T","P",101325,"Q",0,"Water") << std::endl;

  return 1;
  
}
