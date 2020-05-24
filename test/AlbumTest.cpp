#include <vector>
#include <fstream>

#include <gtest/gtest.h>
#include <json/value.h>
#include <json/writer.h>

#include <Album.hpp>

using std::vector;

using namespace MusicList;

class AlbumTest : public ::testing::Test
{
protected:
    fs::path ALBUM_PATH = fs::path("../res/AM");
    fs::path SINGLE_TRACK = fs::path("../res/AM/02 R U Mine_.opus");
};

TEST_F(AlbumTest, SingleFile)
{
    Track track02 = Track(SINGLE_TRACK);
    Album am = Album();

    track02.readMetadata();
    ASSERT_TRUE(am.addTrack(track02));

    ASSERT_EQ(am.getName(), "AM");
    ASSERT_EQ(am.getArtist(), "Arctic Monkeys");
    ASSERT_EQ(am.getTotalTracks(), 12);
}

TEST_F(AlbumTest, SameFile)
{
    Track track02 = Track(SINGLE_TRACK);
    Album am = Album();

    track02.readMetadata();

    ASSERT_TRUE(am.addTrack(track02));
    ASSERT_FALSE(am.addTrack(track02));
}

TEST_F(AlbumTest, FullAlbum)
{
    Album album = Album();
    for (const auto &item : fs::recursive_directory_iterator(ALBUM_PATH))
    {
        if (fs::is_regular_file(item))
        {
            try
            {
                Track newTrack = Track(item);
                album.addTrack(newTrack);
            }
            catch (const unsupported_format_error &err)
            {
                std::cerr << err.what() << std::endl;
            }
        }
    }
}

TEST_F(AlbumTest, GenerateJSON)
{
    Album album = Album();
    for (const auto &item : fs::recursive_directory_iterator(ALBUM_PATH))
    {
        if (fs::is_regular_file(item))
        {
            try
            {
                Track newTrack = Track(item);
                album.addTrack(newTrack);
            }
            catch (const unsupported_format_error &err)
            {
                std::cerr << err.what() << std::endl;
            }
        }
    }

    Json::Value albumJson = album.toJSON();

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "    ";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ostringstream outStr;

    writer->write(albumJson, &outStr);
    outStr << std::endl;

    std::ofstream outFile = std::ofstream("./album.json", std::ios::out | std::ios::trunc);
    if (outFile.is_open())
    {
        outFile.write(outStr.str().c_str(), outStr.str().length());
        outFile.close();
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
