#include "nds/wgs84_coordinate.h"
//
#include "configor/json.hpp"
#include <glog/logging.h>

namespace nds {
Wgs84Coordinate::Wgs84Coordinate(double longitude, double latitude) {
  if (longitude < -180 || longitude > 180) {
    LOG(FATAL) << ("The longitude value " + std::to_string(longitude) +
                   " exceeds the valid range of [-180; 180]");
  }
  if (latitude < -90 || latitude > 90) {
    LOG(FATAL) << ("The latitude value " + std::to_string(latitude) +
                   " exceeds the valid range of [-90;90]");
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
