/*amenities.h*/

/**
  * @brief A collection of amenities in the open street map.
  *
  * @note Written by Jay Rao
  * @note Northwestern University
  */

#pragma once

#include <vector>

#include "amenity.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


/**
  * @brief A collection of amenities in the open street map.
  */
class Amenities
{
public:
  vector<Amenity> osmAmenities;
  vector<string> amenityTypes;

/**
  * @brief constructor to retrieve all the amenities from the open street map.
  *
  * Given an XML document, reads through the document and 
  * stores all the amenities into the vector data member.
  * 
  * @param xmldoc An XML document object denoting the open street map.
  * @return nothing.
  */
  Amenities(XMLDocument& xmldoc);
  
/**
  * @brief prints all the amenities in summary form.
  *
  * @return nothing
  */
  void print_all();

};


