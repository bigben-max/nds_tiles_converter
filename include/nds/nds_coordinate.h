#pragma once

/**
 * Implementation of a NDS coordinate, according to the NDS Format
 * Specification, Version 2.5.4, §7.2.1.
 *
 * The NDS coordinate encoding divides the 360° range into 2^32 steps.
 * Consequently, each coordinate is represented by a pair of signed integers,
 * where a coordinate unit corresponds to 360/2^32 = 90/2^30 degrees
 * longitude/latitude (with their respective longitude [-180°,180°] and latitude
 * [-90°,90°] ranges).
 *
 * Note: The integer range is not fully used encoding latitude values due to the
 * half degree range. But this is done in favor of equally sized coordinate
 * units along longitude/latitude.
 *
 * No warranties for correctness, use at own risk.
 *
 */
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
//
#include "nds/wgs84_coordinate.h"

namespace nds {
class NdsCoordinate {
public:
  /**
   * Creates a new {@link NDSCoordinate} instance
   *
   * @param longitude
   * @param latitude
   */
  NdsCoordinate(int longitude, int latitude);
  /**
   * Instantiates a new NDS coordinate from WGS84 coordinates.
   *
   * @param lon
   *                the longitude within [-180, 180]
   * @param lat
   *                the latitude within [-90, 90]
   */
  NdsCoordinate::NdsCoordinate(double lon, double lat) ~NdsCoordinate();

  /**
   * Creates a new NDSCoordinate from a morton code.
   *
   * @param ndsMortonCoordinates
   * @return
   */
  NdsCoordinate(int64_t ndsMortonCoordinates);

  /**
   * Adds an offset specified by two int values to the coordinate.
   * Useful for NDS coordinate decoding using tile offsets.
   *
   * @param deltaLongitude
   * @param deltaLatitude
   * @return NDSCoordinate
   */
  NdsCoordinate add(int deltaLongitude, int deltaLatitude);

  /**
   * Returns the unique morton code for this NDSCoordinate.
   *
   * @see NDS Format Specification, Version 2.5.4, §7.2.1.
   *
   * @return long
   */
  int64_t getMortonCode();

  /**
   *
   * Converts this coordinate to a WGS84 coordinate (using the "usual"
   * longitude/latitude degree ranges)
   *
   * @return
   */
  Wgs84Coordinate toWGS84();
  /**
   * Creates a GeoJSON "Point" feature representation of this coordinate
   *
   * @return
   */
  std::string toGeoJSON();

private:
  bool verify(int lon, int lat);

  int latitude_;
  int longitude_;

  int MAX_LONGITUDE = std::numeric_limits<int>::max();
  int MIN_LONGITUDE = std::numeric_limits<int>::min();
  int MAX_LATITUDE = MAX_LONGITUDE / 2;
  int MIN_LATITUDE = MIN_LONGITUDE / 2;

  int64_t LONGITUDE_RANGE = (int64_t)MAX_LONGITUDE - MIN_LONGITUDE;
  int64_t LATITUDE_RANGE = (int64_t)MAX_LATITUDE - MIN_LATITUDE;
};

} // namespace nds
