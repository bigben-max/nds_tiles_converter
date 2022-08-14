#include "nds/nds_coordinate.h"
#include <math.h>
namespace nds {
NdsCoordinate::NdsCoordinate(int longitude, int latitude) {
  verify(longitude, latitude);
  latitude_ = latitude;
  longitude_ = longitude;
}

NdsCoordinate::NdsCoordinate(double lon, double lat) {
  if (lon < -180 || lon > 180) {
    std::cout << "The longitude value " << lon
              << " exceeds the valid range of [-180; 180]" << std::endl;
  }
  if (lat < -90 || lat > 90) {
    std::cout << "The latitude value " << lat
              << " exceeds the valid range of [-90; 90]" << std::endl;
  }
  latitude_ = (int)std::floor(lat / 180.0 * LATITUDE_RANGE);
  longitude_ = (int)std::floor(lon / 360.0 * LONGITUDE_RANGE);
}

bool NdsCoordinate::verify(int lon, int lat) {
  if (lat < MIN_LATITUDE || MAX_LATITUDE < lat) {
    // throw new IllegalArgumentException(
    //     "Latitude value " + lat + " exceeds allowed range [-2^30; 2^30] [" +
    //     MIN_LATITUDE + "," + MAX_LATITUDE + "].");
    return false;
  }
  return true;
}

NdsCoordinate::NdsCoordinate(int64_t ndsMortonCoordinates) {
  int lat = 0;
  int lon = 0;
  for (int pos = 0; pos < 32; ++pos) {
    if (pos < 31 && (ndsMortonCoordinates & 1L << (pos * 2 + 1)) != 0L) {
      lat |= 1 << pos;
    }
    if ((ndsMortonCoordinates & 1L << (pos * 2)) != 0L) {
      lon |= 1 << pos;
    }
  }
  /*
   * with NDS, the latitude value is considered a 31-bit signed integer.
   * hence, if the 31st bit is 1, this means we have a negative integer,
   * requiring to set the 32st bit to 1 for native java 32bit signed integers.
   */
  if ((lat & 1 << 30) > 0) {
    lat |= 1 << 31;
  }
  verify(lon, lat);
  latitude_ = lat;
  longitude_ = lon;
}

NdsCoordinate NdsCoordinate::add(int deltaLongitude, int deltaLatitude) {
  return NdsCoordinate(longitude_ + deltaLongitude, latitude_ + deltaLatitude);
}

int64_t NdsCoordinate::getMortonCode() {
  int64_t res = 0L;
  for (int pos = 0; pos < 31; pos++) {
    if ((longitude_ & 1 << pos) > 0) {
      res |= 1L << (2 * pos);
    }
    if (pos < 31 && (latitude_ & 1 << pos) > 0) {
      res |= 1L << (2 * pos + 1);
    }
  }
  if (longitude_ < 0) {
    res |= 1L << 62;
  }
  // For 31-bit signed integers the 32st bit needs to be copied to the 31st bit
  // in case of negative numbers.
  if (latitude_ < 0) {
    res |= 1L << 61;
  }
  return res;
}

Wgs84Coordinate NdsCoordinate::toWGS84() {
  double lon = longitude_ >= 0
                   ? (double)longitude_ / (double)MAX_LONGITUDE * 180.0D
                   : (double)longitude_ / (double)MIN_LONGITUDE * -180.0D;
  double lat = latitude_ >= 0
                   ? (double)latitude_ / (double)MAX_LATITUDE * 90.0D
                   : (double)latitude_ / (double)MIN_LATITUDE * -90.0D;
  return Wgs84Coordinate(lon, lat);
}

std::string NdsCoordinate::toGeoJSON() { return toWGS84().toGeoJSON(); }

} // namespace nds
