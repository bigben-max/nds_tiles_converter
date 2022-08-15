//
#include <glog/logging.h>
#include <gtest/gtest.h>
//
#include "nds/nds_tile.h"

namespace nds {
constexpr double kEps = 1e-7;
TEST(NDSTEST, testFixedData) {
  // Barcelona area
  NdsTile t(539636700);
  EXPECT_EQ(13, t.level());
  EXPECT_EQ(2765788, t.tileNumber());
  EXPECT_TRUE(NdsCoordinate(24772607, 493486079) == t.getCenter());
  EXPECT_TRUE(NdsBbox(493617151, 24903679, 493355008, 24641536) == t.getBBox());

  LOG(INFO) << (t);
  LOG(INFO) << (t.toGeoJSON());

  t = NdsTile(10, Wgs84Coordinate(30, -34));
  EXPECT_EQ(675564, t.tileNumber());
}
TEST(NDSTEST, testFromCoordinateWorks) {
  NdsTile t(539636700);
  NdsCoordinate c(24772607, 493486079);
  EXPECT_TRUE(t == NdsTile(13, c));

  NdsBbox b = t.getBBox();

  EXPECT_TRUE(t == NdsTile(13, b.northEast()));
  EXPECT_TRUE(t == NdsTile(13, b.northWest()));
  EXPECT_TRUE(t == NdsTile(13, b.southEast()));
  EXPECT_TRUE(t == NdsTile(13, b.southWest()));

  EXPECT_TRUE(NdsTile(134390589) == NdsTile(11, c));
  EXPECT_TRUE(NdsTile(269126903) == NdsTile(12, c));
  EXPECT_TRUE(NdsTile(539636700) == NdsTile(13, c));
  EXPECT_TRUE(NdsTile(1084804976) == NdsTile(14, c));
  EXPECT_TRUE(NdsTile(-2103231037) == NdsTile(15, c));

  LOG(INFO) << (b);
  LOG(INFO) << (b.toGeoJSON());
}
TEST(NDSTEST, packedIDRoundTripWorks) {
  EXPECT_EQ(kMaxLongitude, NdsTile(kMaxLongitude).packedId());
  EXPECT_EQ(kMinLongitude, NdsTile(kMinLongitude).packedId());
  EXPECT_EQ(1 << 16, NdsTile(1 << 16).packedId());
  for (int i = 0; i < 1000; i++) {
    int id = (int)std::round(std::rand() * kMaxLongitude) + (1L << 16);
    EXPECT_EQ(id, NdsTile(id).packedId());

    // id = (int)std::round(std::rand() * kMinLongitude);
    // EXPECT_EQ(id, NdsTile(id).packedId());
  }
}
TEST(NDSTEST, testContainsWorks) {
  NdsTile t(539636700);
  NdsCoordinate c(24772607, 493486079);
  EXPECT_TRUE(t.contains(c));

  NdsBbox b = t.getBBox();

  EXPECT_TRUE(t.contains(b.northEast()));
  EXPECT_TRUE(t.contains(b.northWest()));
  EXPECT_TRUE(t.contains(b.southEast()));
  EXPECT_TRUE(t.contains(b.southWest()));
  EXPECT_TRUE(t.contains(b.center()));
  EXPECT_FALSE(t.contains(b.northEast().add(30, 30)));
  EXPECT_FALSE(t.contains(b.southWest().add(-30, -30)));
}

TEST(NDSTEST, testBoundingBoxWorks) {
  // Tile 0
  NdsTile t(0, 0);
  NdsBbox bb = t.getBBox();
  EXPECT_EQ(kMaxLatitude, bb.north());
  EXPECT_EQ(kMaxLongitude, bb.east());
  EXPECT_EQ(kMinLatitude, bb.south());
  EXPECT_EQ(0, bb.west());
  Wgs84Bbox wgsbox = bb.toWGS84();
  EXPECT_EQ(90.0, wgsbox.north());
  EXPECT_EQ(180.0, wgsbox.east());
  EXPECT_EQ(-90.0, wgsbox.south());
  EXPECT_EQ(0, wgsbox.west());

  // Tile 1
  t = NdsTile(0, 1);
  bb = t.getBBox();
  EXPECT_EQ(kMaxLatitude, bb.north());
  EXPECT_EQ(0, bb.east());
  EXPECT_EQ(kMinLatitude, bb.south());
  EXPECT_EQ(kMinLongitude, bb.west());
  wgsbox = bb.toWGS84();
  EXPECT_NEAR(90.0, wgsbox.north(), kEps);
  EXPECT_NEAR(0.0, wgsbox.east(), kEps);
  EXPECT_NEAR(-90.0, wgsbox.south(), kEps);
  EXPECT_NEAR(-180.0, wgsbox.west(), kEps);

  // Tile 0-00
  t = NdsTile(1, 0);
  bb = t.getBBox();
  EXPECT_EQ(kMaxLatitude, bb.north());
  EXPECT_EQ(kMaxLongitude / 2, bb.east());
  EXPECT_EQ(0, bb.south());
  EXPECT_EQ(0, bb.west());
  wgsbox = bb.toWGS84();
  EXPECT_NEAR(90.0, wgsbox.north(), kEps);
  EXPECT_NEAR(90.0, wgsbox.east(), kEps);
  EXPECT_NEAR(0.0, wgsbox.south(), kEps);
  EXPECT_NEAR(0.0, wgsbox.west(), kEps);

  // Tile 0-01
  t = NdsTile(1, 1);
  bb = t.getBBox();
  EXPECT_EQ(kMaxLatitude, bb.north());
  EXPECT_EQ(kMaxLongitude, bb.east());
  EXPECT_EQ(0, bb.south());
  EXPECT_EQ(kMaxLongitude / 2 + 1, bb.west());
  wgsbox = bb.toWGS84();
  EXPECT_NEAR(90.0, wgsbox.north(), kEps);
  EXPECT_NEAR(180.0, wgsbox.east(), kEps);
  EXPECT_NEAR(0.0, wgsbox.south(), kEps);
  EXPECT_NEAR(90.0, wgsbox.west(), kEps);

  // Tile 0-10
  t = NdsTile(1, 2);
  bb = t.getBBox();
  EXPECT_EQ(0, bb.north());
  EXPECT_EQ(kMaxLongitude / 2, bb.east());
  EXPECT_EQ(kMinLatitude, bb.south());
  EXPECT_EQ(0, bb.west());
  wgsbox = bb.toWGS84();
  EXPECT_NEAR(0.0, wgsbox.north(), kEps);
  EXPECT_NEAR(90.0, wgsbox.east(), kEps);
  EXPECT_NEAR(-90.0, wgsbox.south(), kEps);
  EXPECT_NEAR(0.0, wgsbox.west(), kEps);

  // Tile 0-11
  t = NdsTile(1, 3);
  bb = t.getBBox();
  EXPECT_EQ(0, bb.north());
  EXPECT_EQ(kMaxLongitude, bb.east());
  EXPECT_EQ(kMinLatitude, bb.south());
  EXPECT_EQ(kMaxLongitude / 2 + 1, bb.west());
  wgsbox = bb.toWGS84();
  EXPECT_NEAR(0.0, wgsbox.north(), kEps);
  EXPECT_NEAR(180.0, wgsbox.east(), kEps);
  EXPECT_NEAR(-90.0, wgsbox.south(), kEps);
  EXPECT_NEAR(90.0, wgsbox.west(), kEps);

  // Tile 1-00
  t = NdsTile(1, 4);
  bb = t.getBBox();
  EXPECT_EQ(kMaxLatitude, bb.north());
  EXPECT_EQ(kMinLongitude / 2, bb.east());
  EXPECT_EQ(0, bb.south());
  EXPECT_EQ(kMinLongitude, bb.west());
  wgsbox = bb.toWGS84();
  EXPECT_NEAR(90.0, wgsbox.north(), kEps);
  EXPECT_NEAR(-90.0, wgsbox.east(), kEps);
  EXPECT_NEAR(0.0, wgsbox.south(), kEps);
  EXPECT_NEAR(-180.0, wgsbox.west(), kEps);

  // Tile 1-01
  t = NdsTile(1, 5);
  bb = t.getBBox();
  EXPECT_EQ(kMaxLatitude, bb.north());
  EXPECT_EQ(0, bb.east());
  EXPECT_EQ(0, bb.south());
  EXPECT_EQ(kMinLongitude / 2, bb.west());
  wgsbox = bb.toWGS84();
  EXPECT_NEAR(90.0, wgsbox.north(), kEps);
  EXPECT_NEAR(0.0, wgsbox.east(), kEps);
  EXPECT_NEAR(0.0, wgsbox.south(), kEps);
  EXPECT_NEAR(-90.0, wgsbox.west(), kEps);

  // Tile 1-10
  t = NdsTile(1, 6);
  bb = t.getBBox();
  EXPECT_EQ(0, bb.north());
  EXPECT_EQ(kMinLongitude / 2, bb.east());
  EXPECT_EQ(kMinLatitude, bb.south());
  EXPECT_EQ(kMinLongitude, bb.west());
  wgsbox = bb.toWGS84();
  EXPECT_NEAR(0.0, wgsbox.north(), kEps);
  EXPECT_NEAR(-90.0, wgsbox.east(), kEps);
  EXPECT_NEAR(-90.0, wgsbox.south(), kEps);
  EXPECT_NEAR(-180.0, wgsbox.west(), kEps);

  // Tile 1-11
  t = NdsTile(1, 7);
  bb = t.getBBox();
  EXPECT_EQ(0, bb.north());
  EXPECT_EQ(0, bb.east());
  EXPECT_EQ(kMinLatitude, bb.south());
  EXPECT_EQ(kMinLongitude / 2, bb.west());
  wgsbox = bb.toWGS84();
  EXPECT_NEAR(0.0, wgsbox.north(), kEps);
  EXPECT_NEAR(0.0, wgsbox.east(), kEps);
  EXPECT_NEAR(-90.0, wgsbox.south(), kEps);
  EXPECT_NEAR(-90.0, wgsbox.west(), kEps);
}
TEST(NDSTEST, testCenterWorks) {
  // Tile 0
  NdsTile t(0, 0);
  NdsCoordinate c = t.getCenter();
  EXPECT_EQ(0, c.latitude());
  EXPECT_EQ(kMaxLongitude / 2, c.longitude());
  Wgs84Coordinate wgs = c.toWGS84();
  EXPECT_NEAR(0.0, wgs.latitude(), kEps);
  EXPECT_NEAR(90.0, wgs.longitude(), kEps);

  // Tile 1
  t = NdsTile(0, 1);
  c = t.getCenter();
  EXPECT_EQ(0, c.latitude());
  EXPECT_EQ(kMinLongitude / 2, c.longitude());
  wgs = c.toWGS84();
  EXPECT_NEAR(0.0, wgs.latitude(), kEps);
  EXPECT_NEAR(-90.0, wgs.longitude(), kEps);

  // Tile 1-11
  t = NdsTile(1, 7);
  c = t.getCenter();
  EXPECT_EQ(kMinLatitude / 2, c.latitude());
  EXPECT_EQ(kMinLongitude / 4, c.longitude());
  wgs = c.toWGS84();
  EXPECT_NEAR(-45.0, wgs.latitude(), kEps);
  EXPECT_NEAR(-45.0, wgs.longitude(), kEps);

  // Tile 0-01-01
  t = NdsTile(2, 5);
  c = t.getCenter();
  EXPECT_EQ(std::floor(kMaxLatitude / 4.0), c.latitude());
  EXPECT_EQ(std::floor(kMaxLongitude * 7.0 / 8.0), c.longitude());
  wgs = c.toWGS84();
  EXPECT_NEAR(90.0 / 4.0, wgs.latitude(), kEps);         // 22.5
  EXPECT_NEAR(180.0 * 7.0 / 8.0, wgs.longitude(), kEps); // 112.5

  // Tile 1-11-10
  t = NdsTile(2, 30);
  c = t.getCenter();
  EXPECT_EQ(std::floor(kMinLatitude / 4.0), c.latitude());
  EXPECT_EQ(std::floor(kMinLongitude * 3.0 / 8.0), c.longitude());
  wgs = c.toWGS84();
  EXPECT_NEAR(-90.0 / 4.0, wgs.latitude(), kEps);
  EXPECT_NEAR(-180 * 3.0 / 8.0, wgs.longitude(), kEps);

  // Automated loop for the last and first tile of every level - there's a
  // pattern :-)
  for (int lvl = 3; lvl < 16; lvl++) {
    t = NdsTile(lvl, 0);
    c = t.getCenter();
    double latdiv = (double)(1L << lvl);
    double londiv = (double)(1L << lvl + 1);
    EXPECT_EQ(std::floor(kMaxLatitude / latdiv), c.latitude());
    EXPECT_EQ(std::floor(kMaxLongitude / londiv), c.longitude());
    wgs = c.toWGS84();
    EXPECT_NEAR(90.0 / latdiv, wgs.latitude(), kEps);
    EXPECT_NEAR(180.0 / londiv, wgs.longitude(), kEps);

    t = NdsTile(lvl, (1 << 2 * lvl + 1) - 1);
    c = t.getCenter();
    EXPECT_EQ(std::floor(kMinLatitude / latdiv), c.latitude())
        << "Latitude of Tile " << t.tileNumber() << " on level " << lvl;
    EXPECT_EQ(std::floor(kMinLongitude / londiv), c.longitude())
        << "Latitude of Tile " << t.tileNumber() << " on level " << lvl;
    wgs = c.toWGS84();
    EXPECT_EQ(-90.0 / latdiv, wgs.latitude());
    EXPECT_EQ(-180.0 / londiv, wgs.longitude());
  }
}
} // namespace nds
int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  testing::InitGoogleTest(&argc, argv);
  FLAGS_logtostderr = true;
  FLAGS_colorlogtostderr = true;

  LOG(INFO) << "Run Test ...";
  const int output = RUN_ALL_TESTS();
  return output;
}
