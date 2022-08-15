#pragma once
#include "nds/nds_bbox.h"
#include "nds/nds_coordinate.h"

namespace nds {
/**
 * The maximum Tile level within the NDS specification.
 */
constexpr int kMaxLevel = 15;
class NdsTile {
public:
  /**
   * Creates a new {@link NdsTile} instance from a packed Tile id.
   *
   * @param packedId
   * @see NDSSpecification 2.5.4: 7.3.3 Generating Packed Tile IDs
   */
  NdsTile(int packedId);
  /**
   * Creates a new {@link NdsTile} instance for a given id and level.
   *
   * @param level
   *                  Must be in range 0..15
   * @param nr
   *                  An admissible tile number w.r.t to the specified level.
   *
   * @see NDSSpecification 2.5.4: 7.3.3 Generating Packed Tile IDs
   */
  NdsTile(int level, int nr);
  /**
   *
   * Creates a new {@link NdsTile} instance of the specified level, containing
   * the specified coordinate
   *
   * @param level
   * @param coord
   */
  NdsTile(int level, NdsCoordinate coord);
  /**
   * Creates a new {@link NdsTile} instance of the specified level, containing
   * the specified coordinate
   *
   * @param level
   *                  the level
   * @param coord
   *                  the coord
   */
  NdsTile(int level, Wgs84Coordinate coord);
  /**
   * Checks if the current Tile contains a certain coordinate.
   *
   * @param c
   *              the coordinate
   * @return true, if successful
   */
  bool contains(NdsCoordinate c) {
    /*
     * Checks containment via verifying if the coordinates' tile number for the
     * current tile level matches.
     *
     * The tile number is identical to the (2*level+1) most-significant bits of
     * the Morton code of the south-west corner of the tile.
     */
    return tileNumber_ ==
           (int)(c.getMortonCode() >> 32 + (kMaxLevel - level_) * 2);
  }

  /**
   * Returns the packed Tile ID for this tile. Contains the level and (partial)
   * morton code (bitwise)
   *
   * @see NDSFormatSpecification: 7.3.3 Generating Packed Tile IDs
   *
   * @return
   */
  int packedId() { return tileNumber_ + (1L << (16 + level_)); }
  ~NdsTile() {
    if (center_ != nullptr)
      delete center_;
  }
  /**
   * Returns the center of this tile as NdsCoordinate
   *
   * @return NdsCoordinate The center of this tile
   */
  NdsCoordinate getCenter();
  /**
   * Creates a bounding box for the current tile.
   *
   * @see NDSFormatSpecification: 7.3.3 Generating Packed Tile IDs
   *
   * @return
   */
  NdsBbox getBBox();
  /**
   * Computes a GeoJSON representation of the NDS Tile as GeoJSON "Polygon"
   * feature.
   *
   * @return String
   */
  std::string toGeoJSON() { return getBBox().toWGS84().toGeoJSON(); }

  long southWestAsMorton() {
    int shift = 32 + (kMaxLevel - level_) * 2;
    return (long)tileNumber_ << shift;
  }
  int extractLevel(int packedId) {
    for (int lvl = kMaxLevel; lvl > -1; lvl--) {
      int lvl_bit = 1 << 16 + lvl;
      if ((packedId & lvl_bit) > 0) {
        return lvl;
      }
      // The 32nd bit can not be checked by java's native integer, as the sign
      // bit is hidden
      if (packedId < 0 && lvl == kMaxLevel)
        return kMaxLevel;
    }
    return -1;
  }

  int level() const { return level_; }
  int tileNumber() const { return tileNumber_; }
  bool operator==(const NdsTile &other) {
    return this->level_ == other.level_ &&
           this->tileNumber_ == other.tileNumber_;
  }

private:
  /*
   * The tile level
   */
  int level_ = -1;

  /*
   * The tile number.
   *
   * The tile number is identical to the (2*level+1) most-significant bits of
   * the Morton code of the south-west corner of the tile.
   */
  int tileNumber_;

  /*
   * Transient center coordinate
   */
  NdsCoordinate *center_ = nullptr;
};
inline std::ostream &operator<<(std::ostream &out, const NdsTile &other) {
  out << "level: " << other.level() << " , tileNumber: " << other.tileNumber();
  return out;
}
} // namespace nds
