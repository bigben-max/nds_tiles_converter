#include "nds/wgs84_coordinate.h"

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
  return "{\"type\": \"Feature\",\r\n\"properties\": {},\r\n\"geometry\": "
         "{\r\n\"type\": \"Point\",\r\n\"coordinates\": [\r\n" +
         std::to_string(longitude_) + "," + std::to_string(latitude_) + "\r\n" +
         "        ]\r\n" + "      }},";
}

} // namespace nds
