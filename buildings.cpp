/*buildings.cpp*/

/**
  * @brief A collection of buildings in the open street map.
  *
  * @note Written by Prof. Joe Hummel
  * @note Northwestern University
  */

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

#include "buildings.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


/**
  * @brief constructor to retrieve all the buildings from the open street map.
  *
  * Given an XML document, reads through the document and 
  * stores all the buildings into the vector data member.
  * 
  * @param xmldoc An XML document object denoting the open street map.
  * @return nothing.
  */
Buildings::Buildings(XMLDocument& xmldoc)
{
  XMLElement* osm = xmldoc.FirstChildElement("osm");
  assert(osm != nullptr);

  //
  // Parse the XML document element by element, 
  // looking for amenities:
  //
  XMLElement* element = osm->FirstChildElement();

  while (element != nullptr)
  {
    string tag = element->Value();
    
    if (tag != "node" && tag != "way") {
      element = element->NextSiblingElement();
      continue;
    }
    
    const XMLAttribute* attr = element->FindAttribute("id");
    assert(attr != nullptr);
    
    //
    // if this is a building, store info into vector:
    //
    if (osmContainsKeyValue(element, "building", "university"))
    {
      string name = osmGetKeyValue(element, "name");
      
      if (name == "") { // no name, ignore!
        element = element->NextSiblingElement();
        continue;
      }

      string streetAddr = osmGetKeyValue(element, "addr:housenumber")
        + " "
        + osmGetKeyValue(element, "addr:street");

      //
      // create building object, then add the associated
      // node ids to the object:
      //
      // The node/way id serves as the amenity id:
      //
      long long id = attr->Int64Value();

      Building B(id, name, streetAddr);
      
      if (tag == "node") {
        //
        // this node defines the position of the building, so
        // add to vector of node references as our only ref:
        //
        B.add(id);
      }
      else {
        assert(tag == "way");
        
        //
        // the way has a list of nodes that define the perimeter,
        // so collect the node ids as references to the perimeter
        // nodes:
        //
        XMLElement* nd = element->FirstChildElement("nd");
        
        while (nd != nullptr)
        {
          const XMLAttribute* ndref = nd->FindAttribute("ref");
          assert(ndref != nullptr);

          long long id = ndref->Int64Value();

          B.add(id);

          // advance to next node ref:
          nd = nd->NextSiblingElement("nd");
        }
      }//else

      //
      // add the building to the vector:
      //
      this->osmBuildings.push_back(B);
    }//if

    element = element->NextSiblingElement();
  }//while
  
  //
  // we have all the buildings, sort by name:
  //

  sort(osmBuildings.begin(), osmBuildings.end(), 
  [](Building b1, Building b2) -> bool
  {
    if (b1.getName() < b2.getName()) {
      return true;
    }
    else {
      return false;
    }
  }
  );

  
  //
  // done:
  //
  return;
}


/**
  * @brief prints all the buildings in summary form.
  *
  * @return nothing
  */
void Buildings::print_all()
{
  for (Building B : this->osmBuildings) {
    B.print();
  }
  
  return;
}
