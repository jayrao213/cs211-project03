/*amenities.cpp*/

/**
  * @brief A collection of amenities in the open street map.
  *
  * @note Written by Jay Rao
  * @note Northwestern University
  */

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

#include "amenities.h"
#include "buildings.h"
#include "dist.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


/**
  * @brief tolower, makes an entire string lowercase
  *
  * @return s, the lowercase string
  */

string toLowerAmenities(string s)
{
  for (char& c : s) c = tolower(c);

  return s;
}


/**
  * @brief constructor to retrieve all the amenities from the open street map.
  *
  * Given an XML document, reads through the document and 
  * stores all the amenities into the vector data member.
  * 
  * @param xmldoc An XML document object denoting the open street map.
  * @return nothing.
  */
Amenities::Amenities(XMLDocument& xmldoc)
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
    // if this is a amenity, store info into vector:
    //

    string amenityType = osmGetKeyValue(element, "amenity");
      
    if (amenityType == "") { // not amenity, ignore!
        element = element->NextSiblingElement();
        continue;
    }

    amenityTypes.push_back(amenityType);

    string streetAddr = osmGetKeyValue(element, "addr:housenumber")
        + " "
        + osmGetKeyValue(element, "addr:street");

      //
      // create amenity object, then add the associated
      // node ids to the object:
      //
      // The node/way id serves as the amenity id:
      //
    long long id = attr->Int64Value();

    string name = osmGetKeyValue(element, "name");
    
    if (name == "") { // no name, ignore!
        element = element->NextSiblingElement();
        continue;
    }

      Amenity B(id, name, streetAddr, amenityType);
      
      if (tag == "node") {
        //
        // this node defines the position of the amenity, so
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
      // add the amenity to the vector:
      //
      this->osmAmenities.push_back(B);

    element = element->NextSiblingElement();
  }//while
  
  //
  // we have all the amenities, sort by name:
  //

  sort(osmAmenities.begin(), osmAmenities.end(), 
  [](Amenity b1, Amenity b2) -> bool
  {
    if (b1.getName() < b2.getName()) {
      return true;
    }
    else {
      return false;
    }
  }
  );

  sort(amenityTypes.begin(), amenityTypes.end(), 
  [](string a1, string a2) -> bool
  {
    if (a1 < a2) {
      return true;
    }
    else {
      return false;
    }
  }
  );

  vector<string>::iterator last_unique = unique(this->amenityTypes.begin(), this->amenityTypes.end());
  this->amenityTypes.erase(last_unique, this->amenityTypes.end());

 
  //
  // done:
  //
  return;
}


/**
  * @brief prints all the amenities in summary form.
  *
  * @return nothing
  */
void Amenities::print()
{
  for (Amenity B : this->osmAmenities) {
    B.print();
  }
  return;
}

void Amenities::findAndPrint(Amenities& amenities, Nodes& nodes, int num_of_amenities)
{
    string amenity;
    getline(cin, amenity);  // read rest of line in case multiple words in building amenity

    while (isspace(amenity[0])){
      amenity.erase(0, 1);
    } 
    
    if (amenity == "") {
      for (size_t i = 0; i < (amenities.amenityTypes.size() / 5); i++){
        cout << amenities.amenityTypes[i * 5] << " " << amenities.amenityTypes[i * 5 + 1] << " " << amenities.amenityTypes[i * 5 + 2] << " " << amenities.amenityTypes[i * 5 + 3] << " " << amenities.amenityTypes[i * 5 + 4] << endl;
      }
      for (size_t i = (amenities.amenityTypes.size() - (amenities.amenityTypes.size() % 5)); i < amenities.amenityTypes.size(); i++){
        cout << amenities.amenityTypes[i] << " ";
      }
      cout << endl;
    }      


    else {
      // 
      // find every amenity that contains this name, use 
      // a case-insensitive search and print a detailed output:
      //
      
      string check_amenity = toLowerAmenities(amenity);
      bool check = false;

      for (int i = 0; i < num_of_amenities; i++){
        string lowercase_amenity = toLowerAmenities(amenities.osmAmenities[i].getAmenityType());

        if (lowercase_amenity.find(check_amenity) != string::npos){
          amenities.osmAmenities[i].print(nodes);
          check = true;
        }
      }
      if (check == false){
        cout << "No such amenity" << endl;
      }
    }   

}

void Amenities::findNearestFastFood(Amenities& amenities, Buildings& buildings, Nodes& nodes, int num_of_amenities, vector< pair < int, pair <double, double> > > coordinates_list)
{
    for (pair < int, pair <double, double> > coordinates: coordinates_list){
    
    float distance = -1;
    string name = "";
    string address = "";

    // If there is a building match, loop through all the amenities to check if an amenity is fast food, and find the closest fast food option.

    for (int i = 0; i < num_of_amenities; i++){
      if (amenities.osmAmenities[i].getAmenityType() == "fast_food"){
        pair <double, double> new_coordinates = amenities.osmAmenities[i].getLocation(nodes);
        float new_distance = distBetween2Points(coordinates.second.first, coordinates.second.second, new_coordinates.first, new_coordinates.second);
        
        if (distance < 0){
          distance = new_distance;
          name = amenities.osmAmenities[i].getName();
          address = amenities.osmAmenities[i].getStreetAddress();
        }
        else if (new_distance < distance){
          distance = new_distance;
          name = amenities.osmAmenities[i].getName();
          address = amenities.osmAmenities[i].getStreetAddress();
        }
      }
    }
    cout << buildings.osmBuildings[coordinates.first].getName() << endl;
    cout << name << " (fast_food): " << address << endl;
    cout << " Distance: " << distance << " miles" << endl;
  }

  if (coordinates_list.size() < 1){
    cout << "No such building" << endl;
  }
}