#include "opensd/geometry_aux.h"
#include <pugixml.hpp>

#include <iostream>

#include "opensd/error.h"
#include "opensd/circuit.h"
#include "opensd/face.h"

namespace opensd {

void read_geometry_xml()
{
  // Display output message
  write_message("Reading geometry XML file...", 5);

  // Check if geometry.xml exists
  std::string filename = settings::path_input + "geometry.xml";
  // if (!file_exists(filename)) {
    // fatal_error("Geometry XML file '" + filename + "' does not exist!");
  // }

  // Parse settings.xml file
  pugi::xml_document doc;
  auto result = doc.load_file(filename.c_str());
  if (!result) {
    fatal_error("Error processing geometry.xml file.");
  }

  // Get root element
  pugi::xml_node root = doc.document_element();

  read_geometry_xml(root);
}

void read_geometry_xml(pugi::xml_node root)
{
  // Read circuits, heatslabs
  read_circuits(root);
  // read_heatslabs(root);

}

void attach_faces() {
  for (auto circuit : model::circuits) {
	for (auto pipe : circuit.pipes) {
      
	  for (auto node : circuit.nodes) {
	    if (pipe.unode_str == node.identifier) {
		  pipe.unode = node;
		  break;
	    }
      }
	  
	  vector<Node> nodes;
      for (int i = 0; i < pipe.ncell-1; ++i) {
        // Node node = Node();
        // node->identifier = identifier + "_node" + std::to_string(i);
        // node->height = unode->height + (dnode->height - unode->height) * (i + 1) / ncell;
        // nodes.push_back(node);
      }
      Node unode;
      double ufrac;
      Node dnode;
      double dfrac;
      double cfarea;
      double delx;
      double delz;
      double roughness;
      double fricopt;

      vector<PFace> faces;
	  
      for (int i = 0; i < pipe.ncell; ++i) {
        if (i == 0 && pipe.ncell == 1) {
            faces.push_back(PFace(i, pipe, unode, ufrac, dnode, dfrac, pipe.diameter, cfarea, delx, delz, fricopt, roughness));
        } else if (i == 0) {
            faces.push_back(PFace(i, pipe, unode, ufrac, nodes[0], -1, pipe.diameter, cfarea, delx, delz, fricopt, roughness));
        } else if (i == pipe.ncell-1) {
            faces.push_back(PFace(i, pipe, nodes[pipe.ncell-2], -1, dnode, dfrac, pipe.diameter, cfarea, delx, delz, fricopt, roughness));
        } else {
            faces.push_back(PFace(i, pipe, nodes[i-1], -1, nodes[i], -1, pipe.diameter, cfarea, delx, delz, fricopt, roughness));
        }
      }
    
    /*   for (int i = 0; i < ncell-1; ++i) {
        nodes[i].ifaces.push_back(&faces[i]);
        nodes[i].ofaces.push_back(&faces[i+1]);
      }
     */

      pipe.unode.ofaces.push_back(&faces[0]);
      // unode.volume += 0.5 * delx * cfarea;
    
      dnode.ifaces.push_back(&faces[pipe.ncell-1]);
      // dnode.volume += 0.5 * delx * cfarea;
	}
  }
}

} // namespace opensd
