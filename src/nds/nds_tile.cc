#include "nds/nds_tile.h"
#include "glog/logging.h"
#include <cmath>

namespace nds {

NdsTile::NdsTile(int packedId) {
  level_ = extractLevel(packedId);
  if (level_ < 0) {
    LOG(ERROR) << ("Invalid packed Tile ID " + std::to_string(packedId) +
                   ": No Level bit present.");
  }
  int level_bit = 1L << (16 + level_);
  tileNumber_ = packedId ^ level_bit;
}

NdsTile::NdsTile(int level, int nr) {
  if (level < 0) {
    LOG(FATAL) << ("The Tile level " + std::to_string(level) +
                   " exceeds the range [0, 15].");
  }
  level_ = level;
  if (nr < 0) {
    LOG(FATAL) << ("The Tile id " + std::to_string(level) +
                   " must be positive (Max length is 31 bits).");
  }
  auto max_tilenumber = (1L << 2 * level + 1);
  if (nr > max_tilenumber - 1) {
    LOG(FATAL) << ("Invalid Tile number for level " + std::to_string(level) +
                   ", numbers 0 .. " +
                   std::to_string(std::pow(2, 2 * level + 1) - 1) +
                   " are allowed")
               << ", nr = " << nr << " > max_tilenumber: " << max_tilenumber;
  }
  tileNumber_ = nr;
}

NdsTile::NdsTile(int level, NdsCoordinate coord) {
  /*
   * Getting the NDS tile for a NDS coordinate amount to shifting the morton
   * code of the coordinate by the necessary amount. Each NdsTile can be
   * represented by the level and morton code of the lower left / south west
   * corner.
   */
  *this = NdsTile(level,
                  (int)(coord.getMortonCode() >> 32 + (kMaxLevel - level) * 2));
}

NdsTile::NdsTile(int level, Wgs84Coordinate coord) {
  *this =
      NdsTile(level, NdsCoordinate(coord.longitude(), coord.latitude()));
}

NdsCoordinate NdsTile::getCenter() {
  if (center_ == nullptr) {
    if (level_ == 0) {
      return tileNumber_ == 0 ? NdsCoordinate(kMaxLongitude / 2, 0)
                              : NdsCoordinate(kMinLongitude / 2, 0);
    }
    NdsCoordinate sw(southWestAsMorton());
    // Same computation as for bounding box, but for the next lower level
    int clat =
        (int)(sw.latitude() + std::floor(kLatitudeRange / (1L << level_ + 1))) +
        (sw.latitude() < 0 ? 1 : 0);
    int clon = (int)(sw.longitude() +
                     std::floor(kLongitudeRange / (1L << level_ + 2))) +
               (sw.longitude() < 0 ? 1 : 0);
    center_ = new NdsCoordinate(clon, clat);
  }
  return *center_;
}

NdsBbox NdsTile::getBBox() {
  /*
   * For level 0 there are two tiles.
   */
  if (level_ == 0) {
    return tileNumber_ == 0 ? NdsBbox::EAST_HEMISPHERE()
                            : NdsBbox::WEST_HEMISPHERE();
  }
  int64_t southWestCornerMorton = southWestAsMorton();
  NdsCoordinate sw(southWestCornerMorton);
  int north =
      (int)(sw.latitude() + std::floor(kLatitudeRange / (1L << level_))) +
      (sw.latitude() < 0 ? 1 : 0);
  int east =
      (int)(sw.longitude() + std::floor(kLongitudeRange / (1L << level_ + 1))) +
      (sw.longitude() < 0 ? 1 : 0);
  return NdsBbox(north, east, sw.latitude(), sw.longitude());
}

} // namespace nds
