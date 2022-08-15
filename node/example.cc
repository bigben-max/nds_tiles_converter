//
#include "nds/nds_tile.h"
#include <glog/logging.h>
int main(int argc, char **argv) {
  nds::NdsTile test(13, nds::Wgs84Coordinate(113.94, 22.5));
  LOG(INFO) << test;
  LOG(INFO) << test.toGeoJSON();
  LOG(INFO) << test.getCenter().toGeoJSON();
  LOG(INFO) << test.getBBox().toGeoJSON();
  LOG(INFO) << test.getBBox().center().toGeoJSON();
  return 0;
}
