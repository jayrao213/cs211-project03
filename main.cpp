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

string toLower2(string s)
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
      buildings.findAndPrint(buildings, nodes, num_of_buildings);
    }

    else if (cmd == "a") {
      amenities.findAndPrint(amenities, nodes, num_of_amenities);
    }

    else if (cmd == "f") {
      vector< pair < int, pair <double, double> > > coordinates_list = buildings.fast_food_search(buildings, nodes, num_of_buildings);
      amenities.findNearestFastFood(amenities, buildings, nodes, num_of_amenities, coordinates_list);      
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
