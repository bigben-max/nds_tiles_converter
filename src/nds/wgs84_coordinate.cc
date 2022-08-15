#include "nds/wgs84_coordinate.h"
//
#include "configor/json.hpp"

namespace nds {
Wgs84Coordinate::Wgs84Coordinate(double longitude, double latitude) {
  if (longitude < -180 || longitude > 180) {
    // throw new IllegalArgumentException(
    //     "The longitude value " + longitude +
    //     " exceeds the valid range of [-180; 180]");
  }
  if (latitude < -90 || latitude > 90) {
    // throw new IllegalArgumentException("The latitude value " + latitude +
    //                                    " exceeds the valid range of [-90;
    //                                    90]");
  }
  latitude_ = latitude;
  longitude_ = longitude;
}

std::string Wgs84Coordinate::toGeoJSON() {
  configor::json geojson;
  geojson["type"] = "Feature";
  geojson["properties"] = {};
  geojson["geometry"]["type"] = "Point";
  geojson["geometry"]["coordinates"] = {{longitude_, latitude_}};
  return geojson.dump();
}

} // namespace nds
