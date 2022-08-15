#include "nds/nds_coordinate.h"
#include <glog/logging.h>
#include <math.h>
namespace nds {
NdsCoordinate::NdsCoordinate(int longitude, int latitude) {
  verify(longitude, latitude);
  latitude_ = latitude;
  longitude_ = longitude;
}

NdsCoordinate::NdsCoordinate(double lon, double lat) {
  if (lon < -180 || lon > 180) {
    LOG(FATAL) << "The longitude value " << lon
               << " exceeds the valid range of [-180; 180]" << std::endl;
  }
  if (lat < -90 || lat > 90) {
    LOG(FATAL) << "The latitude value " << lat
               << " exceeds the valid range of [-90; 90]" << std::endl;
  }
  latitude_ = (int)std::floor(lat / 180.0 * kLatitudeRange);
  longitude_ = (int)std::floor(lon / 360.0 * kLongitudeRange);
}

bool NdsCoordinate::verify(int lon, int lat) {
  if (lat < kMinLatitude || kMaxLatitude < lat) {
    LOG(FATAL) << ("Latitude value " + std::to_string(lat) +
                   " exceeds allowed range [-2^30; 2^30] [" +
                   std::to_string(kMinLatitude) + "," +
                   std::to_string(kMaxLatitude) + "].");
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
                   ? (double)longitude_ / (double)kMaxLongitude * 180.0
                   : (double)longitude_ / (double)kMinLongitude * -180.0;
  double lat = latitude_ >= 0
                   ? (double)latitude_ / (double)kMaxLatitude * 90.0
                   : (double)latitude_ / (double)kMinLatitude * -90.0;
  return Wgs84Coordinate(lon, lat);
}

std::string NdsCoordinate::toGeoJSON() { return toWGS84().toGeoJSON(); }

} // namespace nds
