#pragma once
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>

namespace nds {
class Wgs84Coordinate {
public:
  /**
   * Instantiates a new WGS 84 coordinate.
   *
   * @param longitude
   *                      the longitude
   * @param latitude
   *                      the latitude
   *
   */
  Wgs84Coordinate(double longitude, double latitude);

  double getLongitude() const { return longitude_; }
  double getLatitude() const { return latitude_; }
  /**
   * Creates a GeoJSON "Point" feature representation of this coordinate
   *
   * @return
   */
  std::string toGeoJSON();

private:
  double longitude_;
  double latitude_;
};

} // namespace nds
