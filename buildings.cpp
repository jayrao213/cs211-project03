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
  * @brief tolower, makes an entire string lowercase
  *
  * @return s, the lowercase string
  */

string toLowerBuildings(string s)
{
  for (char& c : s) c = tolower(c);

  return s;
}


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
void Buildings::print()
{
  for (Building B : this->osmBuildings) {
    B.print();
  }
  
  return;
}

void Buildings::findAndPrint(Buildings& buildings, Nodes& nodes, int num_of_buildings)
{
  //
  // b ENTER => just list all the buildings
  // b building_name ENTER => search for buildings containing that name
  //
  string name;
  getline(cin, name);  // read rest of line in case multiple words in building name
  while (isspace(name[0])) {
    name.erase(0, 1);
  }

  if (name == "") {
    buildings.print();
  }

  else {
    // 
    // find every building that contains this name, use 
    // a case-insensitive search and print a detailed output:
    //

    string check_name = toLowerBuildings(name);
    bool check = false;

    for (int i = 0; i < num_of_buildings; i++){
      string lowercase_building = toLowerBuildings(this->osmBuildings[i].getName());

      if (lowercase_building.find(check_name) != string::npos){
        this->osmBuildings[i].print(nodes);
        check = true;
      }
    }

    if (check == false){
      cout << "No such building" << endl;
    }

  }

  return;

}

vector< pair < int, pair <double, double> > > Buildings::fast_food_search(Buildings& buildings, Nodes& nodes, int num_of_buildings)
{
  vector < pair < int, pair <double, double> > > result;

  string building_name;
  getline(cin, building_name);  // read rest of line in case multiple words in building_name

  while (isspace(building_name[0])){
    building_name.erase(0, 1);
  } 

  string check_name = toLowerBuildings(building_name);

  // Loop through all the buildings to check
  for (int i = 0; i < num_of_buildings; i++){
    string lowercase_building = toLowerBuildings(buildings.osmBuildings[i].getName());

    // Check if input text matches each building
    if (lowercase_building.find(check_name) != string::npos){
      pair <double, double> coordinates = buildings.osmBuildings[i].getLocation(nodes);
      pair < int, pair <double, double> > value = make_pair(i, coordinates);
      result.push_back(value);
    }
  }

  return result;
}
