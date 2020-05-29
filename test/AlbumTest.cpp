#include <vector>
#include <memory>
#include <fstream>

#include <gtest/gtest.h>
#include <json/value.h>
#include <json/writer.h>

#include <Album.hpp>

using std::vector;
using std::shared_ptr;

using namespace MusicList;

class AlbumTest : public ::testing::Test
{
protected:
    fs::path ALBUM_PATH = fs::path("../res/AM");
    fs::path SINGLE_TRACK = fs::path("../res/AM/02 R U Mine_.opus");
    fs::path JETHRO_TULL = fs::path("../res/Original Masters");
};

TEST_F(AlbumTest, SingleFile)
{
    shared_ptr<Track> track02 = std::make_shared<Track>(Track(SINGLE_TRACK));
    Album am = Album();

    am.addTrack(track02);

    ASSERT_EQ(am.getName(), "AM");
    ASSERT_EQ(am.getArtist(), "Arctic Monkeys");
    ASSERT_EQ(am.getTotalTracks(), 12);
}

TEST_F(AlbumTest, SameFile)
{
    shared_ptr<Track> track02 = std::make_shared<Track>(Track(SINGLE_TRACK));
    Album am = Album();

    am.addTrack(track02);
    am.addTrack(track02);

    auto tracks = am.getTrackSet();
    ASSERT_EQ(tracks.size(), 1);
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
                shared_ptr<Track> newTrack = std::make_shared<Track>(Track(item));
                album.addTrack(newTrack);
            }
            catch (const unsupported_format_error &err)
            {
                std::cerr << err.what() << std::endl;
            }
        }
    }
}

TEST_F(AlbumTest, ProblematicAlbum)
{
    Album album = Album();
    for (const auto &item : fs::recursive_directory_iterator(JETHRO_TULL))
    {
        if (fs::is_regular_file(item))
        {
            try
            {
                album.addTrack(std::make_shared<Track>(Track(item)));
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
                shared_ptr<Track> newTrack = std::make_shared<Track>(Track(item));
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
