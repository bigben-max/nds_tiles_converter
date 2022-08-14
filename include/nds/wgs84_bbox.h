#pragma once

#include "nds/wgs84_coordinate.h"

namespace nds {
class Wgs84Bbox {
public:
  Wgs84Bbox(double north, double east, double south, double west)
      : north_(north), east_(east), south_(south), west_(west) {}
  /**
   * Creates a GeoJSON representation of this bounding box as a "Polygon"
   * feature.
   *
   * @return
   */
  std::string toGeoJSON() {
    return "{\"type\": \"Feature\",\r\n" + "      \"properties\": {},\r\n" +
           "      \"geometry\": {\r\n" + "        \"type\": \"Polygon\",\r\n" +
           "        \"coordinates\": [[\r\n" + "          [" +
           std::to_string(west_) + "," + std::to_string(south_) + "],\r\n" +
           "          [" + std::to_string(east_) + "," +
           std::to_string(south_) + "],\r\n" + "          [" +
           std::to_string(east_) + "," + std::to_string(north_) + "],\r\n" +
           "          [" + std::to_string(west_) + "," +
           std::to_string(north_) + "],\r\n" + "          [" +
           std::to_string(west_) + "," + std::to_string(south_) + "]]\r\n" +
           "        ]\r\n" + "      }}";
  }

private:
  double north_;
  double east_;
  double south_;
  double west_;
};

Wgs84Bbox::Wgs84Bbox(/* args */) {}

Wgs84Bbox::~Wgs84Bbox() {}

} // namespace nds
