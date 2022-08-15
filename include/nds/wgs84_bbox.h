#pragma once
#include "configor/json.hpp"
//
#include "nds/wgs84_coordinate.h"

namespace nds {
class Wgs84Bbox {
public:
  Wgs84Bbox(double north, double east, double south, double west)
      : north_(north), east_(east), south_(south), west_(west) {}
  
  double north() const { return north_; }
  double east() const { return east_; }
  double south() const { return south_; }
  double west() const { return west_; }

  /**
   * Creates a GeoJSON representation of this bounding box as a "Polygon"
   * feature.
   *
   * @return
   */
  std::string toGeoJSON() {
    configor::json geojson;
    geojson["type"] = "Feature";
    geojson["properties"] = {};
    geojson["geometry"]["type"] = "Polygon";
    geojson["geometry"]["coordinates"] = {{west_, south_},
                                          {east_, south_},
                                          {east_, north_},
                                          {west_, north_},
                                          {west_, south_}};
    return geojson.dump();
  }

private:
  double north_;
  double east_;
  double south_;
  double west_;
};

} // namespace nds
