#pragma once
#include "nds/nds_coordinate.h"
#include "nds/wgs84_bbox.h"
namespace nds {
class NdsBbox {

public:
  NdsBbox(int north, int east, int south, int west)
      : north_(north), east_(east), south_(south), west_(west) {}
  ~NdsBbox() {}

  int north() const { return north_; }
  int east() const { return east_; }
  int south() const { return south_; }
  int west() const { return west_; }

  bool operator==(const NdsBbox &other) {
    return this->north_ == other.north_ && this->east_ == other.east_ &&
           this->south_ == other.south_ && this->west_ == other.west_;
  }
  /**
   * The western hemisphere part of the world map corresponds to NDS Tile
   number
   * 0 on level 0
   */
  static NdsBbox WEST_HEMISPHERE() {
    return NdsBbox(kMaxLatitude, 0, kMinLatitude, kMinLongitude);
  }
  /**
   * The eastern hemisphere part of the world map corresponds to NDS Tile
   number
   * 1 on level 0
   */
  static NdsBbox EAST_HEMISPHERE() {
    return NdsBbox(kMaxLatitude, kMaxLongitude, kMinLatitude, 0);
  }
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
    return Wgs84Bbox(ne.latitude(), ne.longitude(), sw.latitude(),
                     sw.longitude());
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
inline std::ostream &operator<<(std::ostream &out, const NdsBbox &other) {
  out << "north: " << other.north() << " , east: " << other.east()
      << " , south: " << other.south() << " , west: " << other.west();
  return out;
}
} // namespace nds
