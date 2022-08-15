#include "nds/nds_coordinate.h"
//
#include <glog/logging.h>
#include <gtest/gtest.h>

namespace nds {
TEST(NDSTEST, testConstructor) {
  NdsCoordinate c(-170, -50);
  LOG(INFO) << (c);
  LOG(INFO) << (c.toGeoJSON());

  NdsCoordinate c2(-170, -50);
  EXPECT_TRUE(c == c2);
}

TEST(NDSTEST, testNDSCoordinateCorners) {
  EXPECT_TRUE(NdsCoordinate(180.0, 90.0) ==
              NdsCoordinate(kMaxLongitude, kMaxLatitude));
  EXPECT_TRUE(NdsCoordinate(-180.0, -90.0) ==
              NdsCoordinate(kMinLongitude, kMinLatitude));
  EXPECT_TRUE(NdsCoordinate(0.0, 0.0) == NdsCoordinate(0, 0));
}
TEST(NDSTEST, testNDSCoordinateSpecCasesWGSToInt) {
  NdsCoordinate eiffel(2.2945, 48.858222);
  EXPECT_EQ(27374451, eiffel.longitude());
  EXPECT_EQ(582901293, eiffel.latitude());

  NdsCoordinate liberty(-74.044444, 40.689167);
  EXPECT_EQ(-883384626, liberty.longitude());
  EXPECT_EQ(485440670, liberty.latitude()); // NDS 2.5.4 Spec: 485440671

  NdsCoordinate Sugarloaf(-43.157444, -22.948658);
  EXPECT_EQ(-514888363,
            Sugarloaf.longitude()); // NDS 2.5.4 Spec: -514888362
  EXPECT_EQ(-273788155,
            Sugarloaf.latitude()); // NDS 2.5.4 Spec: -273788154

  NdsCoordinate Sydney(151.214189, -33.857529);
  EXPECT_EQ(1804055545, Sydney.longitude());
  EXPECT_EQ(-403936055, Sydney.latitude()); // NDS 2.5.4 Spec: -403936054

  NdsCoordinate dome(0.0, 51.503);
  EXPECT_EQ(0, dome.longitude());
  EXPECT_EQ(614454723, dome.latitude()); // NDS 2.5.4 Spec: 614454724

  NdsCoordinate quito(-78.45, 0.0);
  EXPECT_EQ(-935944957, quito.longitude()); // NDS 2.5.4 Spec: -935944956
  EXPECT_EQ(0, quito.latitude());
}

TEST(NDSTEST, testNDSCoordinateMortonCodeComputationSpecCases) {
  // Eiffel
  NdsCoordinate c(27374451, 582901293);
  EXPECT_EQ(579221254078012839L, c.getMortonCode());

  // Liberty
  c = NdsCoordinate(-883384626, 485440671);
  EXPECT_EQ(5973384896724652798L, c.getMortonCode());

  // Sugarloaf
  c = NdsCoordinate(-514888362, -273788154);
  EXPECT_EQ(8983442095026671932L, c.getMortonCode());

  // Sydney
  c = NdsCoordinate(1804055545, -403936054);
  EXPECT_EQ(4354955230616876489L, c.getMortonCode());

  // Dome
  c = NdsCoordinate(0, 614454724);
  EXPECT_EQ(585611620934393888L, c.getMortonCode());

  // Quito
  c = NdsCoordinate(-935944956, 0);
  EXPECT_EQ(5782627506097029136L, c.getMortonCode());
}

TEST(NDSTEST, testNDSCoordinateMortonCodeComputationCornerCases) {
  NdsCoordinate c(kMaxLongitude, kMaxLatitude);
  // 0001111111111111111111111111111111111111111111111111111111111111
  EXPECT_EQ(2305843009213693951L, c.getMortonCode());
  LOG(INFO) << (c.getMortonCode());

  c = NdsCoordinate(kMinLongitude, kMinLatitude);
  // 0110000000000000000000000000000000000000000000000000000000000000
  EXPECT_EQ(6917529027641081856L, c.getMortonCode());
  LOG(INFO) << (c.getMortonCode());

  c = NdsCoordinate(kMinLongitude, kMaxLatitude);
  // 0100101010101010101010101010101010101010101010101010101010101010
  EXPECT_EQ(5380300354831952554L, c.getMortonCode());
  LOG(INFO) << (c.getMortonCode());

  c = NdsCoordinate(kMaxLongitude, kMinLatitude);
  // 0011010101010101010101010101010101010101010101010101010101010101
  EXPECT_EQ(3843071682022823253L, c.getMortonCode());
  LOG(INFO) << (c.getMortonCode());

  c = NdsCoordinate(0, 0);
  EXPECT_EQ(0L, c.getMortonCode());
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
