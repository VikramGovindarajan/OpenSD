#include "opensd/initialize.h"

#include <iostream>

// #include "opensd/capi.h"
// #include "opensd/constants.h"
#include "opensd/settings.h"
#include "opensd/geometry_aux.h"
#include "opensd/face.h"

int opensd_init(int argc, char* argv[])
{
  using namespace opensd;

  // Read XML input files
  // if (!read_model_xml())
    read_separate_xml_files();
    attach_faces();
    
  return 0;
}

namespace opensd {
    
void read_separate_xml_files()
{
  read_settings_xml();
  read_geometry_xml();

}

void attach_faces() {
  for (auto& circuit : model::circuits) {
	for (auto& pipe : circuit.pipes) {
      
	  for (auto& node : circuit.nodes) {
	    if (pipe.unode_str == node.identifier) {
		  pipe.unode = &node;
		  break;
	    }
      }
	  for (auto& node : circuit.nodes) {
	    if (pipe.dnode_str == node.identifier) {
		  pipe.dnode = &node;
		  break;
	    }
      }
	  
	  vector<Node*> nodes;
      for (int i = 0; i < pipe.ncell-1; ++i) {
        // Node node = Node();
        // node->identifier = identifier + "_node" + std::to_string(i);
        // node->height = unode->height + (dnode->height - unode->height) * (i + 1) / ncell;
        // nodes.push_back(node);
      }
      double ufrac;
      double dfrac;
      double cfarea;
      double delx;
      double delz;
      double roughness;
      double fricopt;

      vector<PFace> faces;
	  
      for (int i = 0; i < pipe.ncell; ++i) {
        if (i == 0 && pipe.ncell == 1) {
            faces.push_back(PFace(i, pipe, pipe.unode, ufrac, pipe.dnode, dfrac, pipe.diameter, cfarea, delx, delz, fricopt, roughness));
        } else if (i == 0) {
            faces.push_back(PFace(i, pipe, pipe.unode, ufrac, nodes[0], -1, pipe.diameter, cfarea, delx, delz, fricopt, roughness));
        } else if (i == pipe.ncell-1) {
            faces.push_back(PFace(i, pipe, nodes[pipe.ncell-2], -1, pipe.dnode, dfrac, pipe.diameter, cfarea, delx, delz, fricopt, roughness));
        } else {
            faces.push_back(PFace(i, pipe, nodes[i-1], -1, nodes[i], -1, pipe.diameter, cfarea, delx, delz, fricopt, roughness));
        }
      }
    
    /*   for (int i = 0; i < ncell-1; ++i) {
        nodes[i].ifaces.push_back(&faces[i]);
        nodes[i].ofaces.push_back(&faces[i+1]);
      }
     */

      pipe.unode->ofaces.push_back(&faces[0]);
      // unode.volume += 0.5 * delx * cfarea;
    
      pipe.dnode->ifaces.push_back(&faces[pipe.ncell-1]);
      // dnode.volume += 0.5 * delx * cfarea;
	}
  }
}

} // namespace opensd
