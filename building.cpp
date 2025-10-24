/*building.cpp*/

/**
  * @brief defines a building in the open street map.
  *
  * Defines a campus building with an open street map id, a 
  * name (e.g. "Mudd"), a street address (e.g. "2233 Tech Dr"), 
  * and the IDs of the nodes that define the position / outline 
  * of the building.
  *
  * @note Written by Prof. Joe Hummel
  * @note Northwestern University
  */

#include <iostream>
#include <algorithm>
#include <utility>

#include "building.h"

using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
  // vector is default initialized by its constructor
}


//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}


//
// prints information about this building to the console
//
void Building::print()  // summary
{
  //
  // print a simple one line summary of building:
  //
  cout << this->Name << ": "
       << this->StreetAddress
       << endl;
       
  return;
}

void Building::print(Nodes &nodes)  // detailed
{
  //
  // print a more complete, detailed output of building:
  //
  cout << this->Name << endl;
  cout << " OSM ID: " << this->ID << endl;
  cout << " Address: " << this->StreetAddress << endl;
  
  // implement getLocation() function, call here, and output
  // returned latitude and longitude:

  pair<double, double> avg_location = getLocation(nodes);
  cout << " GPS Location: " << avg_location.first << ", " << avg_location.second << endl;

 
  // loop through the sorted nodeids, and for each id, call
  // the find( ) function in the Nodes class to obtain
  // latitude, longitude, and whether it's an entrance or not.
  //
  // If the find( ) function returns false, output "**NODE NOT FOUND**"
  //
  // output format:
  //   id: (latitude, longitude)
  // or
  //   id: (latitude, longitude), is entrance
  // where each line has 2 leading spaces
  //

  cout << " Nodes:" << endl;
    
  vector<long long> sorted = this->getNodeIDs();

  for (long long id : sorted){
    double lat = 0;
    double lon = 0;
    bool isEntrance = false;
    bool check = nodes.find(id, lat, lon, isEntrance);
    if (check){  
      if (isEntrance){
          cout << "  " << id << ": " << "(" << lat << ", " << lon << "), is entrance" << endl;
      }
      else {   
        cout << "  " << id << ": " << "(" << lat << ", " << lon << ")" << endl;
      }
    }
  }
   
  return;
}


//
// getters:
//
long long Building::getID() 
{ return this->ID; }

string Building::getName()
{ return this->Name; }

string Building::getStreetAddress()
{ return this->StreetAddress; }

// returns a sorted copy of the node ids
vector<long long> Building::getNodeIDs()
{ 
  vector<long long> copy = this->NodeIDs;
  
  std::sort(copy.begin(), copy.end());
  
  return copy;
}

pair<double, double> Building::getLocation(Nodes &nodes)
{

    double lat_total = 0;
    double lon_total = 0;
    double length = 0;
  
  for (long long id : NodeIDs){
    double lat = 0;
    double lon = 0;
    bool isEntrance = false;
    bool check = nodes.find(id, lat, lon, isEntrance);
    if (check){  
      lat_total += lat;
      lon_total += lon;
      length += 1;
    }
  }

  double avg_lat = lat_total / length;
  double avg_lon = lon_total / length;

  return make_pair(avg_lat, avg_lon);

}
