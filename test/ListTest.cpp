#include <filesystem>

#include <music_list.cpp>

#include <gtest/gtest.h>

namespace fs = std::filesystem;

using namespace MusicList;

class ListTest : public ::testing::Test
{
protected:
    fs::path RES_PATH = fs::path("../res");
};

TEST_F(ListTest, ListResFolder)
{
    auto data = runAlbumSearch(RES_PATH);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
