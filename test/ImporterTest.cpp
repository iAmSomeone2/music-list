#include <filesystem>
#include <cinttypes>
#include <memory>
#include <vector>

#include <Importer.hpp>

#include <gtest/gtest.h>

namespace fs = std::filesystem;

using namespace MusicList;

using std::vector;
using std::shared_ptr;

class ImporterTest : public ::testing::Test
{
protected:
    const fs::path RES_DIR = fs::path("../res");
    const uint32_t RES_TRACK_NUM = 2U;

    const fs::path MUSIC_DIR = fs::path("/home/bdavidson/Music");
    const uint32_t MUSIC_TRACK_NUM = 2767U;
};

TEST_F(ImporterTest, ResDirTrackCount)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(RES_DIR);
    auto tracks = imptr.getTracks();

    ASSERT_EQ(RES_TRACK_NUM, tracks.size());
}

TEST_F(ImporterTest, MusicDirTrackCount)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(MUSIC_DIR);
    auto tracks = imptr.getTracks();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
