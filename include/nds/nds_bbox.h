#pragma once
#include "nds/nds_coordinate.h"
#include "nds/wgs84_bbox.h"
namespace nds {
class NdsBbox {

public:
  NdsBbox(/* args */);
  ~NdsBbox();

  //   /**
  //    * The western hemisphere part of the world map corresponds to NDS Tile
  //    number
  //    * 0 on level 0
  //    */
  //   static NdsBbox NdsBbox(NdsCoordinate.MAX_LATITUDE, 0,
  //                          NdsCoordinate.MIN_LATITUDE,
  //                          NdsCoordinate.MIN_LONGITUDE);
  //   /**
  //    * The eastern hemisphere part of the world map corresponds to NDS Tile
  //    number
  //    * 1 on level 0
  //    */
  // public
  //   static NdsBbox EAST_HEMISPHERE =
  //       new NdsBbox(NdsCoordinate.MAX_LATITUDE, NdsCoordinate.MAX_LONGITUDE,
  //                   NdsCoordinate.MIN_LATITUDE, 0);
  /**
   *
   * Gets the south west corner of the bounding box
   *
   * @return
   */
  NdsCoordinate southWest() { return NdsCoordinate(west_, south_); }
  /**
   * Gets the south east corner of the bounding box
   *
   * @return NdsCoordinate
   */
  NdsCoordinate southEast() { return NdsCoordinate(east_, south_); }

  /**
   * Gets the north west corner of the bounding box
   *
   * @return NdsCoordinate
   */
  NdsCoordinate northWest() { return NdsCoordinate(west_, north_); }

  /**
   * Gets the north east corner of the bounding box
   *
   * @return NdsCoordinate
   */
  NdsCoordinate northEast() { return NdsCoordinate(east_, north_); }

  /**
   * Returns the center of the bounding box
   *
   * @return NdsCoordinate
   */
  NdsCoordinate center() {
    return NdsCoordinate((east_ + west_) / 2, (north_ + south_) / 2);
  }

  /**
   *
   * Converts this bounding box to a WGS84-coordinate based bounding box.
   *
   * @return Wgs84Bbox
   */
  Wgs84Bbox toWGS84() {
    Wgs84Coordinate ne = northEast().toWGS84();
    Wgs84Coordinate sw = southWest().toWGS84();
    return Wgs84Bbox(ne.getLatitude(), ne.getLongitude(), sw.getLatitude(),
                     sw.getLongitude());
  }

  /**
   * Creates a GeoJSON representation of this bounding box as a "Polygon"
   * feature.
   *
   * @return String
   */
  std::string toGeoJSON() { return toWGS84().toGeoJSON(); }

private:
  int north_;
  int east_;
  int south_;
  int west_;
};

} // namespace nds
