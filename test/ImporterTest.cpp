#include <filesystem>
#include <fstream>
#include <sstream>
#include <cinttypes>
#include <memory>
#include <vector>

#include <Importer.hpp>

#include <gtest/gtest.h>
#include <json/value.h>
#include <json/writer.h>

namespace fs = std::filesystem;

using namespace MusicList;

using std::vector;
using std::shared_ptr;

class ImporterTest : public ::testing::Test
{
protected:
    const fs::path RES_DIR = fs::path("../res");
    const uint32_t RES_TRACK_NUM = 14U;

    const fs::path MUSIC_DIR = fs::path("/home/bdavidson/Music");
    const uint32_t MUSIC_TRACK_NUM = 1480U;
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

    ASSERT_EQ(MUSIC_TRACK_NUM, tracks.size());
}

TEST_F(ImporterTest, ResDirAlbumCount)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(RES_DIR);

    imptr.generateAlbumsFromTracks();

    const auto& albumMap = imptr.getAlbums();

    ASSERT_EQ(2, albumMap.size());
}

TEST_F(ImporterTest, MusicDirAlbumCount)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(MUSIC_DIR);

    imptr.generateAlbumsFromTracks();

    const auto& albumMap = imptr.getAlbums();

    ASSERT_EQ(128, albumMap.size());
}

TEST_F(ImporterTest, ResDirJSON)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(RES_DIR);

    imptr.generateAlbumsFromTracks();

    const auto& imptrJSON = imptr.toJSON();

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "  ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ostringstream outStr;

    writer->write(imptrJSON, &outStr);
    outStr << std::endl;

    std::ofstream outFile = std::ofstream("./reslist.json", std::ios::out | std::ios::trunc);
    if (outFile.is_open())
    {
        outFile.write(outStr.str().c_str(), outStr.str().length());
        outFile.close();
    }
}

TEST_F(ImporterTest, MusicDirJSON)
{
    Importer imptr = Importer();
    imptr.runTrackSearch(MUSIC_DIR);

    imptr.generateAlbumsFromTracks();

    const auto& imptrJSON = imptr.toJSON();

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "  ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ostringstream outStr;

    writer->write(imptrJSON, &outStr);
    outStr << std::endl;

    std::ofstream outFile = std::ofstream("./musiclist.json", std::ios::out | std::ios::trunc);
    if (outFile.is_open())
    {
        outFile.write(outStr.str().c_str(), outStr.str().length());
        outFile.close();
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
