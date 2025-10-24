/*main.cpp*/

/**
  * @brief Program for working with open street map of NU Evanston campus.
  *
  * Program to input Nodes (positions), Buildings and Amenities 
  * from an Open Street Map file. User can search for buildings
  * and nearby amenities.
  *
  * @note Written by Jay Rao
  * @note Starter code by Prof. Joe Hummel
  * @note Northwestern University
  */

#include <iostream>
#include <string>

#include "buildings.h"
#include "nodes.h"
#include "amenities.h"
#include "osm.h"
#include "dist.h"

using namespace std;


/**
  * @brief tolower, makes an entire string lowercase
  *
  * @return s, the lowercase string
  */

string toLower(string s)
{
  for (char& c : s) c = tolower(c);

  return s;
}

/**
  * @brief main program
  *
  * @return 0 denoting success
  */
int main()
{
  XMLDocument xmldoc;
  
  cout << "** NU open street map **" << endl;
  cout << endl;
  
  string filename = "nu.osm";

  //
  // 1. load XML-based map file 
  //
  if (!osmLoadMapFile(filename, xmldoc))
  {
    // error message already output by function
    return 0;
  }
  
  //
  // 2. create and read the nodes, which are the various known 
  //    positions on the map:
  //
  Nodes nodes(xmldoc);

  //
  // 3. create and read the university buildings:
  //
  Buildings buildings(xmldoc);
  
  //
  // 4. create and read the amenities:
  //
  Amenities amenities(xmldoc);

  int num_of_nodes = nodes.getNumOsmNodes();
  int num_of_buildings = size(buildings.osmBuildings);
  int num_of_types = size(amenities.amenityTypes);
  int num_of_amenities = size(amenities.osmAmenities);

  //
  // 5. stats
  //
  cout << "# of nodes:     " << nodes.getNumOsmNodes() << endl;
  cout << "# of buildings: " << size(buildings.osmBuildings) << endl;
  cout << "# of amenity types: " << num_of_types << endl;
  cout << "# of amenities:     " << num_of_amenities << endl;

  //
  // 6. Now let the user search for buildings and amenities:
  //
  while (true)
  {
    string cmd;

    cout << endl;
    cout << "Enter cmd (b, a, f) or $ to end>" << endl;

    cin >> cmd;

    if (cmd == "$") {
      break;
    }
    else if (cmd == "b") {
      //
      // b ENTER => just list all the buildings
      // b building_name ENTER => search for buildings containing that name
      //
      string name;
      getline(cin, name);  // read rest of line in case multiple words in building name

      while (isspace(name[0])){
        name.erase(0, 1);
      }
      
      if (name == "") {
        // 
        // just list all the buildings in summary mode:
        //
        buildings.print_all();
      }
      else {
        // 
        // find every building that contains this name, use 
        // a case-insensitive search and print a detailed output:
        //

        string check_name = toLower(name);
        bool check = false;

        for (int i = 0; i < num_of_buildings; i++){
          string lowercase_building = toLower(buildings.osmBuildings[i].getName());

          if (lowercase_building.find(check_name) != string::npos){
            buildings.osmBuildings[i].print(nodes);
            check = true;
          }
        }
        if (check == false){
          cout << "No such building" << endl;
        }
      }
      
    }

    else if (cmd == "a") {
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
        
        string check_amenity = toLower(amenity);
        bool check = false;

        for (int i = 0; i < num_of_amenities; i++){
          string lowercase_amenity = toLower(amenities.osmAmenities[i].getAmenityType());

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

    else if (cmd == "f") {
      string building_name;
      getline(cin, building_name);  // read rest of line in case multiple words in building_name

      while (isspace(building_name[0])){
        building_name.erase(0, 1);
      } 

        string check_name = toLower(building_name);
        bool check = false;

        // Loop through all the buildings to check
        for (int i = 0; i < num_of_buildings; i++){
          string lowercase_building = toLower(buildings.osmBuildings[i].getName());

          // Check if input text matches each building
          if (lowercase_building.find(check_name) != string::npos){
            check = true;
            pair <double, double> coordinates = buildings.osmBuildings[i].getLocation(nodes);
            float distance = -1;
            string name = "";
            string address = "";

            // If there is a building match, loop through all the amenities to check if an amenity is fast food, and find the closest fast food option.

            for (int i = 0; i < num_of_amenities; i++){
              if (amenities.osmAmenities[i].getAmenityType() == "fast_food"){
                pair <double, double> new_coordinates = amenities.osmAmenities[i].getLocation(nodes);
                float new_distance = distBetween2Points(coordinates.first, coordinates.second, new_coordinates.first, new_coordinates.second);
                
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
            cout << buildings.osmBuildings[i].getName() << endl;
            cout << name << " (fast_food): " << address << endl;
            cout << " Distance: " << distance << " miles" << endl;
          }
        }

        if (check == false){
          cout << "No such building" << endl;
        }      
    }

    else {
      cout << "Unknown command, please try again" << endl; 
    }

  }//while


  //
  // done:
  //
  cout << endl;
  cout << "** Done **" << endl;
  
  //cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
  //cout << "# of Nodes created: " << Node::getCreated() << endl;
  //cout << "# of Nodes copied: " << Node::getCopied() << endl;

  return 0;
}
