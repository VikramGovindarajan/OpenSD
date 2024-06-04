//! \file node.cpp
#include "opensd/node.h"

#include <iostream>
#include <cstdlib>

#include "opensd/error.h"
#include "opensd/xml_interface.h"
#include "opensd/face.h"

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
    tpres_old  = stod(get_node_value(flnode_node, "tpres_old"));
    ttemp_old  = stod(get_node_value(flnode_node, "ttemp_old"));
    tenth_old  = stod(get_node_value(flnode_node, "tenth_old"));
    msource    = stod(get_node_value(flnode_node, "msource"));
  } else {
    fatal_error("Must specify identifier of flow node in geometry XML file.");
  }
  mresidue = 0.;
  mflow_in = 1.E-4;

}


Node::Node(std::string identifier, double volume, double heat_input, double elevation, double tpres_old, double ttemp_old, double tenth_old) //Circuit* circuit, 
  : identifier(identifier), volume(volume), heat_input(heat_input), elevation(elevation), tpres_old(tpres_old), ttemp_old(ttemp_old), tenth_old(tenth_old) {
  // node_ind = circuit.size();
  // if (tpres_old != 0.0)
    // this->tpres_old = tpres_old;
  // if (ttemp_old != 0.0)
    // this->ttemp_old = ttemp_old;
  // if (tenth_old != 0.0)
    // this->tenth_old = tenth_old;

  esource = 0.0;
  mflow_in = 1.0E-4;
  msource = 0.0;
  hresidue = 0.0;
  mresidue = 0.0;
}



double Node::eqn_cont(double alpha_mom) {
  double isum_gues = 0.;
  double isum_old = 0.;
  for (const auto& iface : ifaces) {
    isum_gues += 1000.* iface->vflow_gues; //iface.ther_gues.rhomass()
    // isum_old += 1000. * iface->vflow_old; //iface.ther_old.rhomass()
  }
  double osum_gues = 0.;
  double osum_old = 0.;
  for (const auto& oface : ofaces) {
    osum_gues += 1000. * oface->vflow_gues; //oface.ther_gues.rhomass()
    // osum_old += oface.ther_old.rhomass() * oface.vflow_old;
  }

  mflow_in = isum_gues;
  mflow_out = osum_gues;

/*
  double B, D;
  if (ther_old.phase() == 6) {
    B = B1 + volume * ther_old.first_two_phase_deriv(iDmass, iP, iHmass) / ther_old.rhomass();
    D = volume * ther_old.first_two_phase_deriv(iDmass, iHmass, iP);
  } else {
 */  
    // B = B1 + volume * ther_old.first_partial_deriv(iDmass, iP, iHmass) / ther_old.rhomass();
    // D = volume * ther_old.first_partial_deriv(iDmass, iHmass, iP);
  // }

  double trans3 = 0.; //trans_sim * ther_old.rhomass() * B * (spres_gues - spres_old) / delt;
  double trans4 = 0.; //trans_sim * D * (senth_gues - senth_old) / delt;

  double y = (trans3 + trans4 + alpha_mom * (osum_gues - isum_gues) + (1. - alpha_mom) * (osum_old - isum_old) - msource);
  
  return y;
}

void Node::update_gues() {
  tpres_gues = tpres_old;
  ttemp_gues = ttemp_old;
  spres_gues = spres_old;
  stemp_gues = stemp_old;
  tenth_gues = tenth_old;
  senth_gues = senth_old;
  ther_gues->update(CoolProp::HmassP_INPUTS,senth_gues,spres_gues);
}

void Node::assign_staticvar() {
  spres_old = tpres_old;
  stemp_old = ttemp_old;
  senth_old = tenth_old;
  velocity = 0.;
}

void Node::assign_prop() {
  ther_gues = shared_ptr<CoolProp::AbstractState>(CoolProp::AbstractState::factory("BICUBIC&HEOS", "Water"));
  ther_old = shared_ptr<CoolProp::AbstractState>(CoolProp::AbstractState::factory("BICUBIC&HEOS", "Water"));
}

} // namespace opensd
