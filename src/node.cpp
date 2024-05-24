#include "opensd/node.h"

#include <iostream>

#include "opensd/error.h"
#include "opensd/xml_interface.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
// Node implementation
//==============================================================================

Node::Node(pugi::xml_node flnode_node)
{
  if (check_for_node(flnode_node, "identifier")) {
    identifier = get_node_value(flnode_node, "identifier");
  } else {
    fatal_error("Must specify identifier of flow node in geometry XML file.");
  }
  this->mresidue = 0.;
}

double Node::eqn_cont() {
  double isum_gues = 0.;
  double isum_old = 0.;
/*   for (const auto& iface : ifaces) {
    isum_gues += iface.ther_gues.rhomass() * iface.vflow_gues;
    isum_old += iface.ther_old.rhomass() * iface.vflow_old;
  }

  double osum_gues = 0.;
  double osum_old = 0.;
  for (const auto& oface : ofaces) {
    osum_gues += oface.ther_gues.rhomass() * oface.vflow_gues;
    osum_old += oface.ther_old.rhomass() * oface.vflow_old;
  }

  mflow_in = isum_gues;
  mflow_out = osum_gues;

  double B, D;
  if (ther_old.phase() == 6) {
    B = B1 + volume * ther_old.first_two_phase_deriv(iDmass, iP, iHmass) / ther_old.rhomass();
    D = volume * ther_old.first_two_phase_deriv(iDmass, iHmass, iP);
  } else {
    B = B1 + volume * ther_old.first_partial_deriv(iDmass, iP, iHmass) / ther_old.rhomass();
    D = volume * ther_old.first_partial_deriv(iDmass, iHmass, iP);
  }

  trans3 = trans_sim * ther_old.rhomass() * B * (spres_gues - spres_old) / delt;
  trans4 = trans_sim * D * (senth_gues - senth_old) / delt;

  double y = (trans3 + trans4 + alpha_mom * (osum_gues - isum_gues) + (1. - alpha_mom) * (osum_old - isum_old) - msource);
 */  return 1;
}

} // namespace opensd
