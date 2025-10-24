/*buildings.h*/

/**
  * @brief A collection of buildings in the open street map.
  *
  * @note Written by Prof. Joe Hummel
  * @note Northwestern University
  */

#pragma once

#include <vector>

#include "building.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


/**
  * @brief A collection of buildings in the open street map.
  */
class Buildings
{
public:
  vector<Building> osmBuildings;

/**
  * @brief constructor to retrieve all the buildings from the open street map.
  *
  * Given an XML document, reads through the document and 
  * stores all the buildings into the vector data member.
  * 
  * @param xmldoc An XML document object denoting the open street map.
  * @return nothing.
  */
  Buildings(XMLDocument& xmldoc);
  
/**
  * @brief prints all the buildings in summary form.
  *
  * @return nothing
  */
  void print();
  void findAndPrint(Buildings& buildings, Nodes& nodes, int num_of_buildings);
  vector< pair < int, pair <double, double> > > fast_food_search(Buildings& buildings, Nodes& nodes, int num_of_buildings);
};


